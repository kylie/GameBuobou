//
//  FirstStage.cpp
//  BuoBuo
//
//  Created by Kylie Kim on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#include <iostream>
#include "FirstStage.h"

using namespace cocos2d;

FirstStage::FirstStage()
{
	// pysics world start
	b2Vec2 gravity;
	gravity.Set(-20.0f, 0.0f);
	
	// Do we want to let bodies sleep?
	bool doSleep = false;
	
	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);    
	world->SetContinuousPhysics(true);
	// physics world end
	
	schedule(schedule_selector(FirstStage::tick));
}

FirstStage::~FirstStage()
{
	
}

bool FirstStage::preloadResources()
{
	CCSprite *backgroundWave = CCSprite::spriteWithFile("backgroundWave.png");
	backgroundWave->setPosition(CCPoint(0, 0));
	addChild(backgroundWave);
	
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *buobuo = CCSprite::spriteWithFile("");
	buobuo->setPosition(CCPoint(20, winsize.height / 2));
	addChild(buobuo);
	
	return true;
}

void FirstStage::createPhysicsWorld()
{
	
}

void FirstStage::tick(cocos2d::ccTime dt)
{
	cout<<dt<<endl;
}

void FirstStage::onExit()
{
	
}

void FirstStage::onEnter()
{
	
}