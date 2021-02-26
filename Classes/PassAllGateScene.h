#ifndef __PassAllGate_Scene_H__
#define __PassAllGate_Scene_H__

#include "cocos2d.h"

USING_NS_CC;

class PassAllGate :public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(PassAllGate);

	//返回按钮放大函数
	void turnBigBackBtn(float dt);

	//转场到主菜单界面
	void goToMainMenuScene(float dt);



private:
	//屏幕可见区域大小
	Size visibleSize;

	cocos2d::Label *back;




};




#endif // !__PassAllGate_Scene_H__
