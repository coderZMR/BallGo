


#ifndef __GateMapLayer__H__
#define __GateMapLayer__H__

//#include <jni.h>
//#include "platform/android/jni/JniHelper.h"

#include <math.h> 
#include "SelectGate.h"
#define PI acos(-1)  
#include "cocos2d.h"
USING_NS_CC;


class GateMapLayer : public cocos2d::Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(GateMapLayer);
	SelectGate* selectGateMenu; // 当前处于中间的被选的关卡
	void gateOneCallBack(Ref* pSender);
	void gateTwoCallBack(Ref* pSender);
	void gateThreeCallBack(Ref* pSender);
private:
	//int m_iSelectGate;
	Size visibleSize;
};
#endif
