#ifndef Vibrator_hpp

#define Vibrator_hpp


#include <stdio.h>

#include "cocos2d.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>

#include <android/log.h>

#include "platform/android/jni/JniHelper.h"

#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"

#endif

// 控制手机震动类
class Vibrator

{

public:

	//震动时间 

	static void vibrate(int time);

	/*** 取消震动 */

	static void cancelVibrate();

};

#endif /* Vibrator_hpp */