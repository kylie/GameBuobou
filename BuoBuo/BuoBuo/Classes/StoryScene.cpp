//
//  StoryScene.cpp
//  BuoBuo
//
//  Created by JihyunChoi on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "StoryScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32
#define BUOBUO_POSITION_X 150

enum {
    kTagSpriteBuo = 0,
    kTagSpriteEnemy1 = 1,
    kTagSpriteEnemy2 = 2,
    kTagSpriteEnemy3 = 3
};

#define BG_WAVE1 198
#define BG_WAVE2 199
#define BG_SKY 200
#define UI_TIMEOUT 201
#define TAG_ENEMY 202
#define TAG_LIGHTHOUSE 203

#define BG_Z_ORDER 0
#define CHARACTER_Z_ORDER 1
#define UI_Z_ORDER 2

StoryScene::StoryScene()
{
    setIsTouchEnabled(true);
    setIsAccelerometerEnabled(true);

	timeout = 10; // play time : 2 min
	life = 3; // life : 3
	enemyArr = CCArray::arrayWithCapacity(100);
	bgArr = CCArray::arrayWithCapacity(0);
	
	// create physics world
	createPhysicsWorld();
	
	// create object stuff
	createBackgroundObjects();
	createCharacterObjects();
	createInterfaceObjects();
//	createEnemyObjects();
}

StoryScene::~StoryScene()
{
    
}

CCScene* StoryScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();
    
    // add layer as a child to scene
    CCLayer* layer = new StoryScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void StoryScene::onEnter()
{
	this->cocos2d::CCLayer::onEnter();
	preStartGameScene();
}

void StoryScene::onEnterTransitionDidFinish()
{
	this->cocos2d::CCLayer::onEnterTransitionDidFinish();
}

void StoryScene::onExit()
{
	this->cocos2d::CCLayer::onExit();
}

bool StoryScene::createBackgroundObjects()
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	CCSprite *sky = CCSprite::spriteWithFile("sky.png");
	sky->setPosition(CCPoint(winsize.width / 2, winsize.height - (sky->getTextureRect().size.height / 2)));
	addChild(sky, BG_Z_ORDER);
	
	CCSprite *sea = CCSprite::spriteWithFile("sea.png");
	sea->setPosition(CCPoint(winsize.width / 2, (sea->getTextureRect().size.height / 2)));
	addChild(sea, BG_Z_ORDER);
	
	// background
	CCParallaxNode *parallaxNode = CCParallaxNode::node();
	
	for (int i =0; i < 4; i++) {
		stringstream ss (stringstream::in | stringstream::out);
		ss << "cloud" << i + 1 <<".png";
		CCSprite *cloud = CCSprite::spriteWithFile(ss.str().c_str());
		cloud->setAnchorPoint(CCPointZero);
		parallaxNode->addChild(cloud, BG_Z_ORDER, ccp(0.05f, 0), CCPoint((i * cloud->getContentSize().width) + 100, 
															   sea->getContentSize().height - 20));
	}
	
	for (int i = 0; i < 16; i++) {
		stringstream ss (stringstream::in | stringstream::out);
		ss << "wave" << i + 1 <<".png";
		CCSprite *wave = CCSprite::spriteWithFile(ss.str().c_str());
		wave->setAnchorPoint(CCPointZero);
		parallaxNode->addChild(wave, BG_Z_ORDER, ccp(1.0f, 0), CCPoint(i * wave->getContentSize().width, 0));
	}
	
	addChild(parallaxNode, BG_Z_ORDER, BG_WAVE1);
	
	return true;
}

bool StoryScene::createCharacterObjects()
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	// create main character
	CCSprite *buobuo = CCSprite::spriteWithFile("buobuo1.png");
	buobuo->setPosition(CCPoint(BUOBUO_POSITION_X, winsize.height / 2));
    buobuo->setTag(kTagSpriteBuo);
	addChild(buobuo, CHARACTER_Z_ORDER);
	
	// create lighthouse
	CCSprite *lighthouse = CCSprite::spriteWithFile("lighthouse1.png");
	lighthouse->setPosition(CCPoint(winsize.width + 800, winsize.height / 2));
	addChild(lighthouse, CHARACTER_Z_ORDER, TAG_LIGHTHOUSE);
	
	// add physics body to sprite
	addPhysicsBodyToSprite(buobuo);
	
	return true;
}

bool StoryScene::createInterfaceObjects()
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	// menu item
    CCMenuItemImage* item = CCMenuItemImage::itemFromNormalImage("back.png", "back.png", this, menu_selector(StoryScene::menuCallbackMain));
    CCMenu* menu = CCMenu::menuWithItem(item);
    menu->setPosition(ccp(30, 300));
    addChild(menu);
	
	// time label
    CCLabelTTF *timeLabel = CCLabelTTF::labelWithString("02:00", "arial", 20);
	timeLabel->setColor( ccc3(0, 0, 100) );
	timeLabel->setPosition( CCPointMake( winsize.width / 2, winsize.height - 25) );
	addChild(timeLabel, UI_Z_ORDER, UI_TIMEOUT);
	
	return true;
}

bool StoryScene::preStartGameScene()
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	// stage label
    CCLabelTTF *stageLabel = CCLabelTTF::labelWithString("Stage 1", "Comica_BD", 32);
	addChild(stageLabel, 0);
	stageLabel->setColor( ccc3(255, 255, 255) );
	stageLabel->setPosition( CCPointMake( winsize.width / 2, winsize.height / 2) );
	
	CCFadeIn *fadeInAction = CCFadeIn::actionWithDuration(0.5);
	CCDelayTime *delayAction = CCDelayTime::actionWithDuration(2);
	CCFadeOut *fadeOutAction = CCFadeOut::actionWithDuration(0.5);
	CCFiniteTimeAction *startAction = CCCallFuncND::actionWithTarget(this, callfuncND_selector(StoryScene::startGameScene), NULL);
	
	CCFiniteTimeAction *seq = CCSequence::actions(fadeInAction, delayAction, fadeOutAction, startAction, NULL);
	stageLabel->runAction(seq);
	
	return true;
}

bool StoryScene::startGameScene()
{
	// add accelerometer delegate
	cocos2d::CCAccelerometer *accelerometer = cocos2d::CCAccelerometer::sharedAccelerometer();
	accelerometer->setDelegate(this);
	
	// set schedular
	schedule(schedule_selector(StoryScene::tick));
	
	// start animation stuff
	schedule(schedule_selector(StoryScene::startTimeoutAnimation), 1);
	startBackgroundAnimation();
//	startEnemyAnimation();
	
	return true;
}

bool StoryScene::createPhysicsWorld()
{
	// pysics world start
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	
	// Do we want to let bodies sleep?
	bool doSleep = false;
	
	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);    
	world->SetContinuousPhysics(true);
	// physics world end
	
	// create screen wall
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(screenSize.width/2/PTM_RATIO, 
                               screenSize.height/2/PTM_RATIO); // bottom-left corner
	
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world->CreateBody(&groundBodyDef);
	
	// Define the ground box shape.
	b2PolygonShape groundBox;		
	
    // bottom
    groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, -screenSize.height/2/PTM_RATIO), 0);
 	groundBody->CreateFixture(&groundBox, 0);
	
    // top
    groundBox.SetAsBox(screenSize.width/2/PTM_RATIO, 0, b2Vec2(0, screenSize.height/2/PTM_RATIO), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // left
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(-(screenSize.width+500)/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // right
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(( screenSize.width+500)/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
	
	return true;
}

bool StoryScene::addPhysicsBodyToSprite(cocos2d::CCSprite *sprite)
{
	//
	b2BodyDef nodeBodyDef;
	nodeBodyDef.type = b2_dynamicBody;
	nodeBodyDef.position.Set(sprite->getPosition().x / PTM_RATIO, sprite->getPosition().y / PTM_RATIO);
	nodeBodyDef.userData = sprite;
	b2Body *body = world->CreateBody(&nodeBodyDef);
    
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(sprite->getContentSize().width * sprite->getScale() / 2 / PTM_RATIO, 
						  sprite->getContentSize().height  * sprite->getScale() / 2 / PTM_RATIO);
	
	b2FixtureDef nodeShapeDef;
	nodeShapeDef.shape = &polygonShape;
	nodeShapeDef.density = 1.0f;
	nodeShapeDef.friction = 0.3f;
	nodeShapeDef.restitution = 0.3f;
	
	
	body->CreateFixture(&nodeShapeDef);
        
	
	return true;
}

void StoryScene::startTimeoutAnimation(ccTime dt)
{
	// check time limit
	if (timeout <= 0) {
		isGameOver = true;
		return;
	}
	
	if (timeout == 10) {
		startEndingScene();
	}
	
	cout<<timeout--<<endl;
	stringstream ss (stringstream::in | stringstream::out);

	int minTime = timeout / 60;
	ss << "0" << minTime;
	
	int secTime = timeout - (minTime * 60);
	if (secTime < 10) ss << ":" << "0" << secTime;
	else ss << ":" << secTime;
	
	CCLabelTTF *label = (CCLabelTTF *)this->getChildByTag(UI_TIMEOUT);
	label->setString(ss.str().c_str());
}

void StoryScene::startBackgroundAnimation()
{	
	if (isGameOver) {
		return;
	}
	
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();

	CCParallaxNode *parallaxNode = (CCParallaxNode *)this->getChildByTag(BG_WAVE1);
	float duration = 10.f;
	
	CCMoveBy *moveAction = CCMoveBy::actionWithDuration(duration, ccp( -1 * winsize.width, 0));
	CCFiniteTimeAction *callback = CCCallFuncND::actionWithTarget(this, 
																  callfuncND_selector(StoryScene::startBackgroundAnimation), NULL);
	CCFiniteTimeAction *seq = CCSequence::actions(moveAction, callback, NULL);
	parallaxNode->runAction(seq);
}

void StoryScene::menuCallbackMain(CCObject* sender)
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCScene *pScene = MainScene::scene();
	pDirector->replaceScene(pScene);
}

void StoryScene::tick(cocos2d::ccTime dt)
{	
//	cout<<"tick"<<endl;
	
   
    time += 1;

     cout<< "time : " << time <<endl;
	int velocityIterations = 8;
	int positionIterations = 1;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);

	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			CCSprite *myActor = (CCSprite *)b->GetUserData();
            if (myActor->getTag() == kTagSpriteBuo)
            {
                myActor->setPosition(ccp(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
                if(myActor->getPosition().x <= -(myActor->boundingBox().size.width/2))
                {
                    if (isGameOver == false)
                        this->buobuoDied();
                    else if (isGameOver == true)
                        this->gameOver();
                }
            }
            else if (myActor->getTag() == kTagSpriteEnemy1 || myActor->getTag() == kTagSpriteEnemy2 || myActor->getTag() == kTagSpriteEnemy3) {
//                    enemy1->release();
//                    world->DestroyBody(b);
//                    world->DestroyBody(b);
//                    enemy1->release();
//                    myActor->release();
                }
            }
//     
       
//			myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}	
    
//    if(time == 100 || time == 500 || time == 900)
    if (time >= 100 && time%300 == 0) {
        int i = (CCRANDOM_0_1()*2 + 1);
        this->addEnemy(i);
    }
}

void StoryScene::didAccelerate(cocos2d::CCAcceleration *pAcceleration)
{
	cout<<"called didAccelerate-------------------------"<<endl;
	cout<<"Acceleration X : "<<pAcceleration->x * 10.f<<endl;
	cout<<"Acceleration Y : "<<pAcceleration->y * 10.f<<endl;
	cout<<"Acceleration Z : "<<pAcceleration->z * 10.f<<endl;
	
	b2Vec2 gravity(pAcceleration->x * 10.f, pAcceleration->y * 30.f);
	world->SetGravity(gravity);

    
    
//    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
//		if (b->GetUserData() != NULL) {
//			CCSprite *myActor = (CCSprite *)b->GetUserData();
//            if (myActor->getTag() == kTagSpriteBuo)
//            { 
//                b2Vec2 gravity(0.f, pAcceleration->y * 30.f);
//				world->SetGravity(gravity);
//                b->ApplyForce(gravity, b->GetWorldCenter());
//            }
//		}
//	}
//            else if (myActor->getTag() == kTagSpriteEnemy1)
//            {
////                b2Vec2 gravity(pAcceleration->x * -500.f, 0.f);
////                b->SetLinearVelocity(gravity);
////               b->ApplyForce(gravity, b->GetWorldCenter());
//            }
//                
//
////			myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
//		}	
//	}
////    b2Vec2 g(0,0);
////	world->SetGravity(g);
}


void StoryScene::gameOver()
{
    
}

void StoryScene::buobuoDied()
{
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			CCSprite *myActor = (CCSprite *)b->GetUserData();
            if (myActor->getTag() == kTagSpriteBuo)
            {
                world->DestroyBody(b);
                this->scene()->removeChild(buobuo, true);
                

//                b2Vec2 position(100 * PTM_RATIO, 0);
                
//                b->SetTransform(position, 0);
//                myActor->setPosition(ccp(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
//                CCFiniteTimeAction* action1 = CCFadeOut::actionWithDuration(0.5);
//                CCFiniteTimeAction* action2 = CCFadeIn::actionWithDuration(0.5);
//                CCFiniteTimeAction* sequence = CCSequence::actions(action1, action2, action1, action2);
//                myActor->runAction(sequence);
                CCSize winsize = CCDirector::sharedDirector()->getWinSize();
                buobuo = CCSprite::spriteWithFile("buobuo1.png");
                buobuo->setPosition(CCPoint(BUOBUO_POSITION_X, winsize.height / 2));
                buobuo->setTag(kTagSpriteBuo);
                addPhysicsBodyToSprite(buobuo);
                addChild(buobuo);

                
                // add physics body to sprite
                addPhysicsBodyToSprite(buobuo);
                
                
            }
        }
    }
}

void StoryScene::destroyEnemy(CCSprite *enemy)
{
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			CCSprite *myActor = (CCSprite *)b->GetUserData();
            if (myActor->getTag() == kTagSpriteEnemy1)
            {
                world->DestroyBody(b);
                this->scene()->removeChild(enemy1, true);
            }
            else if (myActor->getTag() == kTagSpriteEnemy2)
            {
                world->DestroyBody(b);
                this->scene()->removeChild(enemy2, true);
            }
            else if (myActor->getTag() == kTagSpriteEnemy3)
            {
                world->DestroyBody(b);
                this->scene()->removeChild(enemy3, true);
            }
        }
    }
}

void StoryScene::addEnemy(int n)
{
    CCPoint p;
    
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	

    float py = (CCRANDOM_0_1() * (winsize.height-50));
    
    b2BodyDef nodeBodyDef;
	nodeBodyDef.type = b2_dynamicBody;
    
    if (n == 1)
    {
        if (enemy1 != NULL)
            this->destroyEnemy(enemy1);
        enemy1 = CCSprite::spriteWithFile("enemy1.png");
        enemy1->setPosition(CCPoint(500, py));
        enemy1->setTag(kTagSpriteEnemy1);
        addChild(enemy1);        
        
        nodeBodyDef.position.Set(enemy1->getPosition().x / PTM_RATIO, enemy1->getPosition().y / PTM_RATIO);
        nodeBodyDef.userData = enemy1;
        b2Body *body = world->CreateBody(&nodeBodyDef);
        
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(enemy1->getContentSize().width * enemy1->getScale() / 2 / PTM_RATIO, 
                              enemy1->getContentSize().height  * enemy1->getScale() / 2 / PTM_RATIO);
        b2FixtureDef nodeShapeDef;
        nodeShapeDef.shape = &polygonShape;
        nodeShapeDef.density = 5.0f;
        nodeShapeDef.friction = 0.3f;
        nodeShapeDef.restitution = 0.0f;
        
        body->CreateFixture(&nodeShapeDef);
    }
    if (n == 2)
    {
        if (enemy2 != NULL)
            this->destroyEnemy(enemy2);
        enemy2 = CCSprite::spriteWithFile("enemy2.png");
        enemy2->setPosition(CCPoint(500, py));
        enemy2->setTag(kTagSpriteEnemy2);
        addChild(enemy2);     
        
        nodeBodyDef.position.Set(enemy2->getPosition().x / PTM_RATIO, enemy2->getPosition().y / PTM_RATIO);
        nodeBodyDef.userData = enemy2;
        b2Body *body = world->CreateBody(&nodeBodyDef);
        
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(enemy1->getContentSize().width * enemy2->getScale() / 2 / PTM_RATIO, 
                              enemy1->getContentSize().height  * enemy2->getScale() / 2 / PTM_RATIO);
        b2FixtureDef nodeShapeDef;
        nodeShapeDef.shape = &polygonShape;
        nodeShapeDef.density = 5.0f;
        nodeShapeDef.friction = 0.3f;
        nodeShapeDef.restitution = 0.0f;
        
        body->CreateFixture(&nodeShapeDef);
    }
    if (n == 3)
    {
        if (enemy3 != NULL)
            this->destroyEnemy(enemy3);
        enemy3 = CCSprite::spriteWithFile("enemy3.png");
        enemy3->setPosition(CCPoint(500, py));
        enemy3->setTag(kTagSpriteEnemy3);
        addChild(enemy3);     
        
        nodeBodyDef.position.Set(enemy3->getPosition().x / PTM_RATIO, enemy3->getPosition().y / PTM_RATIO);
        nodeBodyDef.userData = enemy3;
        b2Body *body = world->CreateBody(&nodeBodyDef);
        
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(enemy3->getContentSize().width * enemy3->getScale() / 2 / PTM_RATIO, 
                              enemy3->getContentSize().height  * enemy3->getScale() / 2 / PTM_RATIO);
        b2FixtureDef nodeShapeDef;
        nodeShapeDef.shape = &polygonShape;
        nodeShapeDef.density = 5.0f;
        nodeShapeDef.friction = 0.3f;
        nodeShapeDef.restitution = 0.0f;
        
        body->CreateFixture(&nodeShapeDef);
    }
    

//    bDef.type = b2_kinematicBody;
    cout<<"Enemy++++++++++++++++++++++++++++" << n << " added"<<endl;
}

void StoryScene::startEndingScene()
{
		CCSprite *lightHouse = (CCSprite *)getChildByTag(TAG_LIGHTHOUSE);
	if (isGameOver) {
		CCSprite *lightHouse2 = CCSprite::spriteWithFile("lighthouse2.png");
		lightHouse2->setAnchorPoint(CCPointZero);
		lightHouse2->setPosition(CCPointZero);
		lightHouse->addChild(lightHouse2);
		return;
	}
	
	setIsAccelerometerEnabled(false);
	
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	float duration = 10.f;
	CCMoveBy *moveAction = CCMoveBy::actionWithDuration(duration, ccp(( -1 * winsize.width), 0));
	CCFiniteTimeAction *startAction = CCCallFuncND::actionWithTarget(this, callfuncND_selector(StoryScene::startEndingScene), NULL);
	CCFiniteTimeAction *seq = CCSequence::actions(moveAction, startAction, NULL);
	lightHouse->runAction(seq);
}
