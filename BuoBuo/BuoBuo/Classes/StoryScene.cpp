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
#define BG_WAVE 199
#define BG_SKY 200
#define UI_TIMEOUT 201

StoryScene::StoryScene()
{
	setIsTouchEnabled(true);
    setIsAccelerometerEnabled(true);
	
	min = "02";
	sec = "00";
	
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
	
	// background wave
	CCSprite *backgroundWave = CCSprite::spriteWithFile("backgroundWave.png");
	CCParallaxNode *parallaxNode = CCParallaxNode::node();
	parallaxNode->addChild(backgroundWave, 0, ccp(1.0f, 0), CCPoint(winsize.width / 2, winsize.height / 2));
	addChild(parallaxNode, 0, BG_WAVE);
	
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
	string timeout = min;
	timeout.insert(timeout.length(), ":");
	timeout.insert(timeout.length(), sec);
	
    CCLabelTTF *timeLabel = CCLabelTTF::labelWithString(timeout.c_str(), "arial", 20);
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
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(-screenSize.width/2/PTM_RATIO, 0), 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // right
    groundBox.SetAsBox(0, screenSize.height/2/PTM_RATIO, b2Vec2(screenSize.width/2/PTM_RATIO, 0), 0);
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
	nodeShapeDef.density = 3.0f;
	nodeShapeDef.friction = 1.0f;
	nodeShapeDef.restitution = 0.3f;
	
	b2Body *body = world->CreateBody(&nodeBodyDef);
	body->CreateFixture(&nodeShapeDef);
	
	return true;
}

void StoryScene::startTimeoutAnimation(ccTime dt)
{
	cout<<dt<<endl;
	stringstream ss (stringstream::in | stringstream::out);
	ss.str("");
	
	// float to string
	if (atoi(sec) == 0) {
		int minTime = atoi(min);
		minTime--;
		
		ss << "0" << minTime;
		min = ss.str().c_str();
		sec = "59";
	}
	else {	
		int secTime = atoi(sec);
		secTime--;

		if (secTime < 10) ss << "0" << secTime;
		else ss << secTime;
		sec = ss.str().c_str();
	}

	
	string timeout = min;
	timeout.insert(timeout.length(), ":");
	timeout.insert(timeout.length(), sec);
	
	CCLabelTTF *label = (CCLabelTTF *)this->getChildByTag(UI_TIMEOUT);
	label->setString(timeout.c_str());
}

void StoryScene::startBackgroundAnimation()
{	
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	// animate background
	CCNode *parallaxNode = this->getChildByTag(BG_WAVE);
	float duration = 10.f;
	float xRatio = 1.f;
	
	CCAction *moveAction = CCMoveBy::actionWithDuration(duration, ccp( -1 * winsize.width / xRatio, 0));
	parallaxNode->runAction(moveAction);
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
	
	int velocityIterations = 8;
	int positionIterations = 1;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);

	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			CCSprite *myActor = (CCSprite *)b->GetUserData();
			myActor->setPosition(ccp(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
//			myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}	
	}
}

void StoryScene::didAccelerate(cocos2d::CCAcceleration *pAcceleration)
{
	cout<<"called didAccelerate-------------------------"<<endl;
	cout<<"Acceleration X : "<<pAcceleration->x * 10.f<<endl;
	cout<<"Acceleration Y : "<<pAcceleration->y * 10.f<<endl;
	cout<<"Acceleration Z : "<<pAcceleration->z * 10.f<<endl;
    
	b2Vec2 gravity(0.f, pAcceleration->y * 5.f);
	world->SetGravity(gravity);
}
