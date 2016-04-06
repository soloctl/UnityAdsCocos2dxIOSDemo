#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"

USING_NS_CC;

typedef enum {
    KENDREASONWIN,
    KENDREASONLOSE
} EndReason;

class HelloWorld : public cocos2d::CCLayer
{
private:
	cocos2d::CCSpriteBatchNode * _batchNode;
	cocos2d::CCSprite * _ship;

    CCParallaxNodeExtras *_backgroundNode;
    CCSprite *_spacedust1;
    CCSprite *_spacedust2;
    CCSprite *_planetsunrise;
    CCSprite *_galaxy;
    CCSprite *_spacialanomaly;
    CCSprite *_spacialanomaly2;
    CCMenuItemLabel *_resultItem;
    
    float _shipPointsPerSecY;

    CCArray* _asteroids;
    int _nextAsteroid;
    float _nextAsteroidSpawn;
    
    CCArray* _shipLasers;
    int _nextShipLaser;
    
    int _lives;
    
    
    double _gameOverTime;
    bool _gameOver;
    
    // scheduled Update
    void update(float dt);
    
    void endScene(EndReason endReason);
    void restartTapped();
    void resultTapped();
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    
    float randomValueBetween(float low, float high);
    void setInvisible(CCNode * node);
    float getTimeTick();
    
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    virtual void rewardItemOne();
};

#endif // __HELLOWORLD_SCENE_H__
