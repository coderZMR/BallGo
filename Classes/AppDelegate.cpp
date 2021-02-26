#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "PassDifficultyScene.h"
#include "AdventureScene.h"



USING_NS_CC;
using namespace CocosDenshion;

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("Demo", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = GLViewImpl::create("Demo");
#endif
		glview->setFrameSize(designResolutionSize.width, designResolutionSize.height);
		director->setOpenGLView(glview);
	}

	// turn on display FPS
	// director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
	Size frameSize = glview->getFrameSize();
	// if the frame's height is larger than the height of medium size.
	if (frameSize.height > mediumResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
	}
	// if the frame's height is larger than the height of small size.
	else if (frameSize.height > smallResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
	}
	// if the frame's height is smaller than the height of medium size.
	else
	{
		director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
	}

	register_all_packages();

	// create a scene. it's an autorelease object
	// 音乐预加载
	// 闯关模式
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BackGroundMusicForPassDifficultyScene.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("TransmitDoor.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("GetStart.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("DecreaseTime.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("WallDisappear.mp3");
	
	// 冒险模式
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BackGroundMusicForAdventureScene.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("VoiceGood.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("VoiceGreat.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("VoiceExcellent.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("VoiceAmazing.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("VoiceUnbelievable.mp3");
	
	SimpleAudioEngine::getInstance()->preloadEffect("Victory.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("Fail.mp3");

	// 通用
	SimpleAudioEngine::getInstance()->preloadEffect("kacha.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM.mp3");

	// auto scene = PassDifficulty::createScene(cocos2d::Value(2));	//////////////////////////////////////////////////////////////////////////入口
	// auto scene = Adventure::createScene(3);
	auto scene = Hello::createScene();
	
	 // run
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
