//
//  GameScene.h
//  BuoBuo
//
//  Created by JihyunChoi on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#ifndef BuoBuo_GameScene_h
#define BuoBuo_GameScene_h

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"

class GameScene : public cocos2d::CCLayer {
public:
    ~GameScene();
    GameScene();
    
    // returns a Scene that contains the MainScene as the only child
    static cocos2d::CCScene* scene();
    
    void menuCallbackMain(CCObject* sender);
};

#endif // BuoBuo_GameScene_h

