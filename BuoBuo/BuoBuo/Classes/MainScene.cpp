//
//  MainScene.cpp
//  BuoBuo
//
//  Created by JihyunChoi on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#include <iostream>
#include "MainScene.h"
#include "StoryScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32

MainScene::MainScene()
{
	setIsTouchEnabled( true );
	setIsAccelerometerEnabled( true );
    
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	//UXLOG(L"Screen width %0.2f screen height %0.2f",screenSize.width,screenSize.height);
    
	//Set up sprite  
    CCSprite *sprite = CCSprite::spriteWithFile("main.png");
    sprite->setPosition(CCPoint(screenSize.width/2, screenSize.height/2));
    addChild(sprite);
    
    //Set up menu
    //(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector)
    CCMenuItemImage* item1 = CCMenuItemImage::itemFromNormalImage("main_menu_story.png", "main_menu_story.png", this, menu_selector(MainScene::menuCallbackStory));
    CCMenuItemImage* item2 = CCMenuItemImage::itemFromNormalImage("main_menu_play.png", "main_menu_story.png", this, menu_selector(MainScene::menuCallbackPlay) );

    
    CCMenu* menu = CCMenu::menuWithItems(item1, item2, NULL);
    menu->setPosition(ccp(100, 100));
    
    
    addChild(menu);
    item1->setPosition(ccp(0, 50));
    item2->setPosition(ccp(0, 0));
    

	
	CCLabelTTF *label = CCLabelTTF::labelWithString("BuoBuo main", "Marker Felt", 32);
	addChild(label, 0);
	label->setColor( ccc3(0,0,255) );
	label->setPosition( CCPointMake( screenSize.width/2, screenSize.height-50) );
}

MainScene::~MainScene()
{
}

CCScene* MainScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();
    
    // add layer as a child to scene
    CCLayer* layer = new MainScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void MainScene::menuCallbackStory(CCObject* sender)
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCScene *pScene = StoryScene::scene();
	pDirector->replaceScene(pScene);
}

void MainScene::menuCallbackPlay(CCObject* sender)
{
}