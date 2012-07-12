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
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    
    CCMenuItemImage* item = CCMenuItemImage::itemFromNormalImage("back.png", "back.png", this, menu_selector(StoryScene::menuCallbackMain));
    
    
    CCMenu* menu = CCMenu::menuWithItem(item);
    menu->setPosition(ccp(30, 300));
    
    
    addChild(menu);

    
    CCLabelTTF *label = CCLabelTTF::labelWithString("BuoBuo game demo", "Marker Felt", 32);
	addChild(label, 0);
	label->setColor( ccc3(0,0,255) );
	label->setPosition( CCPointMake( screenSize.width/2, screenSize.height-50) );
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

void StoryScene::menuCallbackMain(CCObject* sender)
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCScene *pScene = MainScene::scene();
	pDirector->replaceScene(pScene);
}
