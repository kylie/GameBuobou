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

USING_NS_CC;

class StoryScene : public cocos2d::CCLayer {
protected:
    CCSprite* buobuo;
    CCSprite* enemy1;
public:
    ~StoryScene();
    StoryScene();

	bool isGameOver;
    int interval;
    int time;
    
	b2World *world;
    
    // returns a Scene that contains the MainScene as the only child
    static cocos2d::CCScene* scene();
    
	bool createPhysicsWorld();
	bool addPhysicsBodyToSprite(cocos2d::CCSprite *sprite);
	void tick(cocos2d::ccTime dt);
    void menuCallbackMain(CCObject* sender);
	
	// CCAccelerometerDelegate method
	virtual void didAccelerate(cocos2d::CCAcceleration *pAcceleration);
    
    void destroyEnemy(CCSprite *enemy);
    void gameOver();
    void buobuoDied();
    void addEnemy();
};

#endif // BuoBuo_StoryScene_h

