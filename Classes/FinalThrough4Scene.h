#ifndef __FinalThrough4_Scene_H__
#define __FinalThrough4_Scene_H__

#include "cocos2d.h"

USING_NS_CC;

class FinalThrough4 :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	CREATE_FUNC(FinalThrough4);

	void backBtnMenu(Ref *ref);


	void againBtnMenu(Ref *ref);

	void nextBtnMenu(Ref *ref);

private:
	//屏幕可见区域大小
	Size visibleSize;

	//分数Label
	Label *label1, *score1, *scoreName1;


};




#endif // !__FinalThrough4_Scene_H__
