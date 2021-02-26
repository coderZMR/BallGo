#ifndef __ChuangGuan__H__
#define __ChuangGuan__H__

#include <math.h> 
#include "SelectGate.h"
#define PI acos(-1)  
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;
class ChuangGuan : public LayerColor
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	CREATE_FUNC(ChuangGuan);
	SelectGate* selectGateMenu; // 当前处于中间的被选的关卡
	void gateOneCall(Ref* pSender);
	void gateTwoCall(Ref* pSender);
	void gateThreeCall(Ref* pSender);
	void gateFourCall(Ref* pSender);
	void gateFiveCall(Ref* pSender);
private:
	Size visibleSize;
};
#endif
