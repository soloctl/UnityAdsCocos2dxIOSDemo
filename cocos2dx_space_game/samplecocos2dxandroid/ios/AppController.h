//
//  Cocos2DxFirstIosSampleAppController.h
//  Cocos2DxFirstIosSample
//
//  Created by Jorge Jordán Arenas on 10/03/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#import <UnityAds/UnityAds.h>
#import "MyObject-C-Interface.h"

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate, UIApplicationDelegate, UnityAdsDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;


- (int) doSomethingWith:(int) aParameter;

@end

