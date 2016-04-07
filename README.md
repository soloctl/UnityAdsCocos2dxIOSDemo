This demo project shows how to integrate the Unity Ads SDK into a Cocos2Dx iOS game

*Game original project provided by Raywenderlich - Jean-Yves Mengant - https://www.raywenderlich.com/33752

# How to integrate Unity Ads into your Cocos2dx iOS project

## Operating on UniytAds dashboard

### Create a Game Project on the [Unity Ads Dashboard](https://dashboard.unityads.unity3d.com)
- Log into the [dashboard](https://dashboard.unityads.unity3d.com) using your [UDN Account](https://accounts.unity3d.com/sign-in)
- Create a new game project
- Look for your iOS **Game ID** in the project, a 7-digit number that you will use in your integration


## Integrate Unity Ads in Objective C side
### Import the Unity Ads Framework

Download the Unity SDK from https://github.com/Applifier/unity-ads-sdk
  1. [Download the SDK zip file](https://github.com/Applifier/unity-ads-sdk/archive/master.zip)
  2. Unzip the project, and locate **UnityAds.framework** and **UnityAds.bundle**

Import **UnityAds.framework** and **UnityAds.bundle** into your project
  1. Drag and drop the files into your project's file manager
  2. Select the box next to **"Copy items if needed"**

Make sure the following dependancies are enabled in your project  

> **AdSupport**.framework  
> **AVFoundation**.framework  
> **CFNetwork**.framework  
> **CoreFoundation**.framework  
> **CoreMedia**.framework  
> **CoreTelephony**.framework  
> **StoreKit**.framework  
> **SystemConfiguration**.framework  



  1. Click your project settings
  2. select **Build Phases** > **Link Binary With Library**
  3. Click the **+** button > select the Framework > Click **Add**



### Implement C bridging functions for calling UnityAds methods from Cocos2dx

Add C bridging header file
  1. Right click on **Cocos2DxFirstIosSample->ios** folder, choose **New File**, then select **Header File**. Enter what ever file name you like, for example **C-Interface**
  2. Add basic UnityAds wrapper functions declaration in the c header file  
      ```C
      int UnityAdsInit (void *parameter);
      int UnityAdsCanShow (void *parameter);
      int UnityAdsShow (void *parameter);
      ```

Implement Unity Ads wrapper functions in your **AppController**
  1. Add **`#import "MyObject-C-Interface.h"`** in the top of **AppController**
  2. Add below implementation in the bottom of **AppController**  
      ```ObjC
      int UnityAdsInit (void *gameIdP)
      {
          return 1;
      }
      int UnityAdsCanShow (void *zoneStringP)
      {
          return 1;
      }
      int UnityAdsShow (void* zoneStringP)
      {
          return 1;
      }
      ```

### Initialize Unity Ads

Add UnityAds to your **AppController**
  1. Import UnityAds header file **`#import <UnityAds/UnityAds.h>`** in **AppController.h** file
  2. Make your **AppController** conform to **UnityAdsDelegate** protocol  
      ```ObjC
      @interface AppController :
                  NSObject <UIAccelerometerDelegate,
                            UIAlertViewDelegate,
                            UITextFieldDelegate,
                            UIApplicationDelegate,
                            UnityAdsDelegate> {
      ```
  3. Implement **UnityAdsDelegate**'s *@required* callback in **AppController**  
      ```ObjC
      #pragma UnityAdsDelegate methods
      - (void)unityAdsVideoCompleted:(NSString*)rewardItemKey
                             skipped:(BOOL)skipped
      {
      }
      ```
  4. Initialize *UnityAds* in **UnityAdsInit** function, we are passing in the gameId parameter from cpp class, so no need to worry what is it now  
      ```ObjC
        UIApplication* app = [UIApplication sharedApplication];
        AppController* controller = (AppController*)[app delegate];
        UIViewController* rootController = [controller.window rootViewController];
        NSString* gameId = [NSString stringWithFormat:@"%s", gameIdP];
        [[UnityAds sharedInstance] startWithGameId:gameId andViewController:rootController];
      ```
  5. Set **AppController** as UnityAds' delegate  
      ```ObjC
      [[UnityAds sharedInstance] setDelegate:controller];
      ```

### Implement show an ad function
  1. Replace **return 1;** with below code snippet  
      ```ObjC
      NSString* zoneId = [NSString stringWithFormat:@"%s", zoneStringP];
      [[UnityAds sharedInstance] setZone:zoneId];
      if ([[UnityAds sharedInstance] canShow]) {
          [[UnityAds sharedInstance] show];
          return 1;
      } else {
          return 0;
      }
      ```

## Use the c bridge in your Cocos2dx game

### Import the bridging header
  1. Open **HelloWorldScene.cpp** and add **#include "MyObject-C-Interface.h"`** in the top

### Implement wrapper functions
  1. Open **AppDelegate.h** and in the *public:* area, add **virtual void rewardItemOne();**  
  2. Open **AppDelegate.cpp** and add below code in the bottom of this class  
      ```Cpp
      void AppDelegate::rewardItemOne()
      {
          CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
          HelloWorld* gameScene = dynamic_cast<HelloWorld*>(scene->getChildren()->objectAtIndex(0));
          if(gameScene != NULL)
          {
              gameScene->rewardItemOne();
          }
      }
      ```
  3. Open **HelloWorldScene.h**, in the *pblic:* area, add **virtual void rewardItemOne();**
  4. Open **HelloWorldScene.cpp**, in the bottom in this class, add below implementation  
      ```Cpp
      void HelloWorld::rewardItemOne()
      {
          CCLOG("[UnityAds cpp] rewarded");
      }
      ```

### Change the result label to show ad button
  1. Open **HelloWorldScene.cpp** and create a new *CCMenuItemLabel*  
      ```Cpp
      CCMenuItemLabel *resultItem = CCMenuItemLabel::create(label, this, menu_selector(HelloWorld::resultTapped) );
      ```
  2. Replace original *CCLabel* to below  
      ```C
      resultItem->setScale(0.1);
      resultItem->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
      menu->addChild(resultItem);
      resultItem ->runAction(CCScaleTo::create(0.5, 1.0));
      ```
  3. Add *resultTapped* function  
      ```Cpp
      void HelloWorld::resultTapped() {
          char* zoneString = "rewardedVideo";
          UnityAdsShow(zoneString);
      }
      ```
  4. Before calling *UnityAdsShow*, we must make sure the ad is initialized, in beginning, add below code  
      ```C
      char* gameIdString = "1003843";
      UnityAdsInit(gameIdString);
      ```
> NOTE: The game ID in the example project is **1003843**, you need to replace this number with your own game ID


Arrived here, you should already able to show an ad by tapping **resultItem**.

### Reward the player
The idea is we want to reward the player by giving additional life in the next play.

  1. Add **int _extraLives = 0;** definition in **HelloWorldScene.cpp**
  2. Move *resultItem* declaration to the header file, in the *private:* area  
      ```C
      CCMenuItemLabel *_resultItem;
      ```
  3. Add extra life to *_lives*  
      ```C
      _lives = 1+_extraLives;
      _extraLives = 0;
      ```
  4. Add up an extra life after the player watched a video in *HelloWorld::rewardItemOne* function  
      ```C
          _extraLives = 1;
      ```
  5. Modify *resultItem*'s' implementation, only appear if there isn't extra life.  
      ```Cpp
      if(_extraLives <= 0) {
          strcpy(message,"You Win");
          if ( endReason == KENDREASONLOSE)
              strcpy(message,"Watch video to get extra life");
          CCLabelBMFont * label ;
          label = CCLabelBMFont::create(message, "Arial.fnt");
          _resultItem = CCMenuItemLabel::create(label, this, menu_selector(HelloWorld::resultTapped) );
          _resultItem->setScale(0.1);
          _resultItem->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
          menu->addChild(_resultItem);
          _resultItem ->runAction(CCScaleTo::create(0.5, 1.0));
      }
      ```
  6. Add hide **_resultItem** logic in *rewardItemOne* function  
      ```Cpp
      void HelloWorld::rewardItemOne()
      {
          _extraLives = 1;
          if(_resultItem != NULL) {
              _resultItem->removeFromParentAndCleanup(true);
              _resultItem = NULL;
          }
          CCLOG("[UnityAds cpp] rewarded");
      }
      ```

Finally, we finished a rewarded video. Player would enjoy more after watching a video for an extra life in the next play.

For more information, check out the [iOS Integration Guide](http://unityads.unity3d.com/help/monetization/integration-guide-ios), the [support Forum](http://forum.unity3d.com/forums/unity-ads.67/), or contact unityads-sales@unity3d.com
