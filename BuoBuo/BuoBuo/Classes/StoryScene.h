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
public:
    ~StoryScene();
    StoryScene();
	
	b2World *world;
	const char *min;
	const char *sec;
    
    // returns a Scene that contains the MainScene as the only child
    static cocos2d::CCScene* scene();
	void tick(cocos2d::ccTime dt);
    
	bool preStartGameScene();
	bool startGameScene();
	
	bool createPhysicsWorld();
	bool createBackgroundObjects();
	bool createChracterObjects();
	bool createInterfaceObjects();
	
	void startBackgroundAnimation();
	void startTimeoutAnimation(ccTime dt);

	bool addPhysicsBodyToSprite(cocos2d::CCSprite *sprite);
    void menuCallbackMain(CCObject* sender);
	
	// CCAccelerometerDelegate method
	virtual void didAccelerate(cocos2d::CCAcceleration *pAcceleration);
	
	// CCScene delegate
	virtual void onEnter();
	virtual void onExit();
    virtual void onEnterTransitionDidFinish();
};

#endif // BuoBuo_StoryScene_h

