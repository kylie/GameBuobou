//
//  FirstStage.h
//  BuoBuo
//
//  Created by Kylie Kim on 7/12/12.
//  Copyright (c) 2012 Moglue Inc. All rights reserved.
//

#ifndef BuoBuo_FirstStage_h
#define BuoBuo_FirstStage_h

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class FirstStage : cocos2d::CCScene {
	
public:
	FirstStage();
	~FirstStage();
	
	b2World *world;
	
	virtual void onEnter();
	virtual void onExit();
	void tick(cocos2d::ccTime dt);
	
	void createPhysicsWorld();
	bool preloadResources();
};


#endif
