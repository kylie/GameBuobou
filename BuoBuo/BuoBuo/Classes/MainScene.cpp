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
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	//UXLOG(L"Screen width %0.2f screen height %0.2f",screenSize.width,screenSize.height);
    
	//Set up sprite  
    CCSprite *sprite = CCSprite::spriteWithFile("main.png");
    sprite->setPosition(CCPoint(round(screenSize.width/2), round(screenSize.height/2)));
    addChild(sprite);
    
    CCSprite *title = CCSprite::spriteWithFile("title.png");
    title->setPosition(CCPoint(round(screenSize.width/2)+10, round(screenSize.height+180)));
    addChild(title);
    CCMoveTo *move = CCMoveTo::actionWithDuration(3, CCPoint(round(screenSize.width/2)+10, round(screenSize.height-80)));
//    title->setPosition(CCPoint(round(screenSize.width/2)+10, round(screenSize.height-80)));
    title->runAction(move);
   
    
    //Set up menu
    //(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector)
    CCMenuItemImage* item1 = CCMenuItemImage::itemFromNormalImage("main_menu_story.png", "main_menu_story_selected.png", this, menu_selector(MainScene::menuCallbackStory));
    CCMenuItemImage* item2 = CCMenuItemImage::itemFromNormalImage("main_menu_play.png", "main_menu_play_selected.png", this, menu_selector(MainScene::menuCallbackPlay) );
    
    CCMenu* menu = CCMenu::menuWithItems(item1, item2, NULL);
    menu->setPosition(ccp(150, 30));
    
    addChild(menu);
    item1->setPosition(ccp(0, 0));
    item2->setPosition(ccp(200, 0));
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
