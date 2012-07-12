//
//  BuoBuoAppController.h
//  BuoBuo
//
//  Created by Kylie Kim on 7/12/12.
//  Copyright Moglue Inc. 2012. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController	*viewController;
}

@end

