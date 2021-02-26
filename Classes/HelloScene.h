#ifndef __Hello_SCENE_H__
#define __Hello_SCENE_H__

#include "cocos2d.h"
#include "Bar.h"

class Hello : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onEnterTransitionDidFinish();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Hello);

	// visibleSize的set与get函数
	void setThisVisibleSize(cocos2d::Size size);
	cocos2d::Size getThisVisibleSize();

	// 创建UI界面
	void createUI();

	// 设置触摸监听
	void setListenerAndShowText(float dt);

	// 设置碰撞监听
	void solveContactEvent();

	// 消失回调函数
	void removeFunc(cocos2d::Node* node);

	// 对Bar类创建对象的设置
	void reBuild(Bar *bar, float time, cocos2d::Vec2 moveDis);

	//创建Json文件
	void createJson();

private:
	cocos2d::Size visibleSize;
	cocos2d::Director *director;
	cocos2d::Value uiWidth;
	cocos2d::Value uiHeight;
	cocos2d::Label *textLabel;
};

#endif // __Hello_SCENE_H__
