#ifndef __List_Scene_H__
#define __List_Scene_H__

#include"cocos2d.h"


USING_NS_CC;

class List :public cocos2d::LayerColor
{
public:
	//构造函数
	List();
	//析构函数
	~List();

	//List类的静态创建场景函数
	static cocos2d::Scene* createScene();
	//List类初始化函数
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	//List类创造函数
	CREATE_FUNC(List);

	//移除左按钮的函数
	void removeLeft(float dt);
	//移除右按钮的函数
	void removeRight(float dt);
	//返回按钮放大函数
	void turnBigBackBtn(float dt);
	//转场到主菜单界面
	void goToMainMenuScene(float dt);
	//左按钮动作回调函数
	void leftBtn(Ref *ref);
	//右按钮动作回调函数
	void rightBtn(Ref *ref);
	//移除排行榜层函数
	void removeChildList();
	//添加排行榜层函数
	void addChildList(const std::string AllOfListName[]);

private:
	
	Size visibleSize;					//屏幕可见区域大小
	
	int  mark[20];	//分数存储数组，0-4为简单级前五名，5-9为困难级，
							//10-14为变态级，后五个为闯关模式1-5关的每关的最高分
	int tag;				//排行榜的标记

	cocos2d::Label *back;		//返回按钮

	cocos2d::Label *No, *Score;	//排名，分数

	cocos2d::Sprite *left, *right, *floorLeft, *floorRight;	//左右按钮

	Sprite *list[4];					//排行榜名字图片

	LayerColor *board[4];		//背景层

	Label *NoAndScore[12];	//名次和分数

	std::string listName;			//排行榜图片名

	std::string AllOfBraveListName[7];		//冒险模式排行榜Label的Text内容

	std::string AllOfPassListName[7];		//闯关模式排行榜Label的Text内容
	
};



#endif // !__List_Scene_H__
