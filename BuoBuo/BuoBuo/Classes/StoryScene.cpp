//
//  GameScene.cpp
//  BuoBuo
//
//  Created by JihyunChoi on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#include <iostream>
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32

GameScene::GameScene()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    
    CCLabelTTF *label = CCLabelTTF::labelWithString("BuoBuo game demo", "Marker Felt", 32);
	addChild(label, 0);
	label->setColor( ccc3(0,0,255) );
	label->setPosition( CCPointMake( screenSize.width/2, screenSize.height-50) );
}

GameScene::~GameScene()
{
    
}

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::node();
    
    // add layer as a child to scene
    CCLayer* layer = new GameScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void GameScene::menuCallbackMain(CCObject* sender)
{
    
}
