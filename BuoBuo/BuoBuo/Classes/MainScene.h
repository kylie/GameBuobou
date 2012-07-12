//
//  MainScene.h
//  BuoBuo
//
//  Created by JihyunChoi on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#ifndef BuoBuo_MainScene_h
#define BuoBuo_MainScene_h

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"

class MainScene : public cocos2d::CCLayer {
public:
    ~MainScene();
    MainScene();
    
    // returns a Scene that contains the MainScene as the only child
    static cocos2d::CCScene* scene();

    void menuCallbackStory(CCObject* sender);
    void menuCallbackPlay(CCObject* sender);
};

#endif // BuoBuo_MainScene_h
