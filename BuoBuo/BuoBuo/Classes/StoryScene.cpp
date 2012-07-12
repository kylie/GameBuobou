//
//  StoryScene.cpp
//  BuoBuo
//
//  Created by JihyunChoi on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#include <iostream>
#include "StoryScene.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32

StoryScene::StoryScene()
{
	// add accelerometer delegate
	cocos2d::CCAccelerometer *accelerometer = cocos2d::CCAccelerometer::sharedAccelerometer();
	accelerometer->setDelegate(this);

	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	
	// background wave
	CCSprite *backgroundWave = CCSprite::spriteWithFile("backgroundWave.png");
	backgroundWave->setPosition(CCPoint(winsize.width / 2, winsize.height / 2));
	addChild(backgroundWave);
	
	// create main character
	CCSprite *buobuo = CCSprite::spriteWithFile("buobuo_R.png");
	buobuo->setPosition(CCPoint(100, winsize.height / 2));
	addChild(buobuo);
	
	// menu item
    CCMenuItemImage* item = CCMenuItemImage::itemFromNormalImage("back.png", "back.png", this, menu_selector(StoryScene::menuCallbackMain));
    CCMenu* menu = CCMenu::menuWithItem(item);
    menu->setPosition(ccp(30, 300));
    addChild(menu);
    
	// title label
    CCLabelTTF *label = CCLabelTTF::labelWithString("BuoBuo game demo", "Marker Felt", 32);
	addChild(label, 0);
	label->setColor( ccc3(0,0,255) );
	label->setPosition( CCPointMake( winsize.width/2, winsize.height-50) );
	
	// create physics world
	createPhysicsWorld();
	
	// add physics body to sprite
	addPhysicsBodyToSprite(buobuo);
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
	
	
	//
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
	
	
	schedule(schedule_selector(StoryScene::tick));
	
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
	nodeShapeDef.density = 10.0f;
	nodeShapeDef.friction = 0.3f;
	nodeShapeDef.restitution = 0.3f;
	
	b2Body *body = world->CreateBody(&nodeBodyDef);
	body->CreateFixture(&nodeShapeDef);
	
	return true;
}

void StoryScene::menuCallbackMain(CCObject* sender)
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCScene *pScene = MainScene::scene();
	pDirector->replaceScene(pScene);
}

void StoryScene::tick(cocos2d::ccTime dt)
{	
	cout<<"tick"<<endl;
	
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
