#ifndef __MainMenu_Scene_H__
#define __MainMenu_Scene_H__

#include"cocos2d.h"

USING_NS_CC;

class MainMenu :public cocos2d::LayerColor
{
public:
	//构造函数
	MainMenu();
	//析构函数
	~MainMenu();
	//MainMenu类的静态创建场景函数
	static cocos2d::Scene* createScene();
	//MainMenu类的初始化函数
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	//MainMenu类的创造函数
	CREATE_FUNC(MainMenu);
	//闯关模式按钮动作函数
	void passTimeForMove(float dt);
	//冒险模式按钮动作函数
	void braveTimeForMove(float dt);
	//排行榜按钮动作函数
	void rankTimeForMove(float dt);
	//退出游戏按钮动作函数
	void exitTimeForMove(float dt);
	//跳转到排行榜界面的函数
	void goToListScene(float dt);
	//跳转到冒险难度选择界面
	void goToAdventureScene(float dt);
	//跳转到闯关模式的选关界面
	void goToPassScene(float dt);
	//退出游戏函数
	void goToExit(float dt);


private:
	//可视区域大小
	Size visibleSize;  
	//闯关模式、冒险模式、排行榜和退出游戏按钮的背景层
	LayerColor *passMode, *braveMode, *rankList, *exit;
	//闯关、冒险、排行榜和退出游戏按钮名字的图片精灵指针
	cocos2d::Sprite *passBtnName, *braveBtnName, *rankBtnName, *exitBtnName;

};



#endif // !__MainMenu_Scene_H__
