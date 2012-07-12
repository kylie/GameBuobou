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
#define BUOBUO_POSITION_X 100

enum {
    kTagSpriteBuo = 0,
    kTagSpriteEnemy1 = 1,
    kTagSpriteEnemy2 = 2,
    kTagSpriteEnemy3 = 3
};
#define BG_WAVE 199
#define BG_SKY 200
#define UI_TIMEOUT 201

StoryScene::StoryScene()
{
    time = 0;
    setIsTouchEnabled(true);
    setIsAccelerometerEnabled(true);

	timeout = 120; // play time : 2 min
	
	// create physics world
	createPhysicsWorld();
	
	// create object stuff
	createBackgroundObjects();
	createChracterObjects();
	createInterfaceObjects();
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
	addChild(sky, 0);
	
	CCSprite *sea = CCSprite::spriteWithFile("sea.png");
	sea->setPosition(CCPoint(winsize.width / 2, (sea->getTextureRect().size.height / 2)));
	addChild(sea, 0);
	
	// backgroud cloud
	CCSprite *cloud = CCSprite::spriteWithFile("cloud1.png");
	CCSprite *wave1 = CCSprite::spriteWithFile("wave1.png");
//	CCSprite *wave2 = CCSprite::spriteWithFile("wave2.png");

	CCParallaxNode *parallaxNode = CCParallaxNode::node();
	parallaxNode->addChild(cloud, 0, ccp(0.2f, 0), CCPoint(winsize.width / 2, winsize.height - (cloud->getTextureRect().size.height / 2)));
	parallaxNode->addChild(wave1, 0, ccp(5.f, 0), CCPoint(winsize.width / 2, (wave1->getTextureRect().size.height / 2)));
//	parallaxNode->addChild(wave2, 0, ccp(3.f, 0), CCPoint((winsize.width / 2) + winsize.width, 
//															(wave2->getTextureRect().size.height / 2)));
	addChild(parallaxNode, 0, BG_WAVE);
	
	// background wave
//	CCSprite *wave1 = CCSprite::spriteWithFile("wave1.png");
//	CCSprite *wave2 = CCSprite::spriteWithFile("wave2.png");
//	CCParallaxNode *parallaxNode = CCParallaxNode::node();
//	parallaxNode->addChild(wave1, 0, ccp(speed, 0), CCPoint(winsize.width / 2, (wave1->getTextureRect().size.height / 2)));
//	parallaxNode->addChild(wave2, 0, ccp(speed, 0), CCPoint((winsize.width / 2) + winsize.width, 
//														   (wave2->getTextureRect().size.height / 2)));
//	addChild(parallaxNode, 0, BG_WAVE);
	
	return true;
}

bool StoryScene::createChracterObjects()
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	// create main character
	CCSprite *buobuo = CCSprite::spriteWithFile("buobuo_R.png");
	buobuo->setPosition(CCPoint(150, winsize.height / 2));
	addChild(buobuo);
	
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
	timeLabel->setColor( ccc3(255, 255, 255) );
	timeLabel->setPosition( CCPointMake( winsize.width / 2, winsize.height - 25) );
	addChild(timeLabel, 0, UI_TIMEOUT);
	
	return true;
}

bool StoryScene::preStartGameScene()
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	// stage label
    CCLabelTTF *stageLabel = CCLabelTTF::labelWithString("Stage 1", "arial", 32);
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
	
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(sprite->getContentSize().width * sprite->getScale() / 2 / PTM_RATIO, 
						  sprite->getContentSize().height  * sprite->getScale() / 2 / PTM_RATIO);
	
	b2FixtureDef nodeShapeDef;
	nodeShapeDef.shape = &polygonShape;
	nodeShapeDef.density = 0.0f;
	nodeShapeDef.friction = 1.0f;
	nodeShapeDef.restitution = 0.0f;
	
	b2Body *body = world->CreateBody(&nodeBodyDef);
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
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	// animate background
	CCParallaxNode *parallaxNode = (CCParallaxNode *) this->getChildByTag(BG_WAVE);
	float duration = 5.f;
	
	if (parallaxNode->getPosition().x <= -(winsize.width * 0.5)) {
		parallaxNode->setPosition(winsize.width / 2 + winsize.width, parallaxNode->getPosition().y);
	}
	
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
	
    interval = (CCRANDOM_0_1()*10);
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
            else if (myActor->getTag() == kTagSpriteEnemy1)
            {
                b2Vec2 gravity(-5.f, 0.f);
                b->SetLinearVelocity(gravity);
                myActor->setPosition(ccp(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
                if (myActor->getPosition().x <= -(myActor->boundingBox().size.width/2))
                {
                    enemy1->release();
                    world->DestroyBody(b);
                    
//                    this->destroyEnemy(enemy1);
//                    world->DestroyBody(b);
//                    enemy1->release();
//                    myActor->release();
                }
            }
//     
       
//			myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}	
	}
    
    if(time == 100 || time == 500 || time == 900)
        this->addEnemy();
}

void StoryScene::didAccelerate(cocos2d::CCAcceleration *pAcceleration)
{
	cout<<"called didAccelerate-------------------------"<<endl;
	cout<<"Acceleration X : "<<pAcceleration->x * 10.f<<endl;
	cout<<"Acceleration Y : "<<pAcceleration->y * 10.f<<endl;
	cout<<"Acceleration Z : "<<pAcceleration->z * 10.f<<endl;
	
    if ( buobuo == NULL ) {
        return;
    }
    
    
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			CCSprite *myActor = (CCSprite *)b->GetUserData();
            if (myActor->getTag() == kTagSpriteBuo)
            { 
                b2Vec2 gravity(0.f, pAcceleration->y * 500.f);
                b->ApplyForce(gravity, b->GetWorldCenter());
            }
            else if (myActor->getTag() == kTagSpriteEnemy1)
            {
//                b2Vec2 gravity(pAcceleration->x * -500.f, 0.f);
//                b->SetLinearVelocity(gravity);
//               b->ApplyForce(gravity, b->GetWorldCenter());
            }
                

//			myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}	
	}
    b2Vec2 g(0,0);
	world->SetGravity(g);
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
                buobuo = CCSprite::spriteWithFile("buobuo_R.png");
                buobuo->setPosition(CCPoint(BUOBUO_POSITION_X, winsize.height / 2));
                buobuo->setTag(kTagSpriteBuo);
                addPhysicsBodyToSprite(buobuo);
                addChild(buobuo);
                
                
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
//                world->DestroyBody(b);
//                this->scene()->removeChild(enemy1, true);
                
                
            }
        }
    }
}

void StoryScene::addEnemy()
{
    CCPoint p;
    
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
    
//    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
//		if (b->GetUserData() != NULL) {
//			CCSprite *myActor = (CCSprite *)b->GetUserData();
//            if (myActor->getTag() == kTagSpriteEnemy1)
//            {
//                world->DestroyBody(b);
//                enemy1->release();
//            }
//        }
//    }

    
    enemy1 = CCSprite::spriteWithFile("enemy1.png");
	enemy1->setPosition(CCPoint(500, winsize.height / 3));
    enemy1->setTag(kTagSpriteEnemy1);
    addChild(enemy1);

//    bDef.type = b2_kinematicBody;
    b2BodyDef nodeBodyDef;
	nodeBodyDef.type = b2_dynamicBody;
	nodeBodyDef.position.Set(enemy1->getPosition().x / PTM_RATIO, enemy1->getPosition().y / PTM_RATIO);
	nodeBodyDef.userData = enemy1;
	
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(enemy1->getContentSize().width * enemy1->getScale() / 2 / PTM_RATIO, 
						  enemy1->getContentSize().height  * enemy1->getScale() / 2 / PTM_RATIO);
	
	b2FixtureDef nodeShapeDef;
	nodeShapeDef.shape = &polygonShape;
	nodeShapeDef.density = 5.0f;
	nodeShapeDef.friction = 0.3f;
	nodeShapeDef.restitution = 0.0f;

    b2Body *body = world->CreateBody(&nodeBodyDef);
	body->CreateFixture(&nodeShapeDef);
   
    
    cout<<"Enemy++++++++++++++++++++++++++++"<<endl;
}
