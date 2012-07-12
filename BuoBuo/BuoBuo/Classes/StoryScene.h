//
//  StoryScene.h
//  BuoBuo
//
//  Created by JihyunChoi on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#ifndef BuoBuo_StoryScene_h
#define BuoBuo_StoryScene_h

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"

class StoryScene : public cocos2d::CCLayer {
public:
    ~StoryScene();
    StoryScene();
    
    // returns a Scene that contains the MainScene as the only child
    static cocos2d::CCScene* scene();
    
    void menuCallbackMain(CCObject* sender);
};

#endif // BuoBuo_StoryScene_h

