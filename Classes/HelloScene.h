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

	// visibleSize��set��get����
	void setThisVisibleSize(cocos2d::Size size);
	cocos2d::Size getThisVisibleSize();

	// ����UI����
	void createUI();

	// ���ô�������
	void setListenerAndShowText(float dt);

	// ������ײ����
	void solveContactEvent();

	// ��ʧ�ص�����
	void removeFunc(cocos2d::Node* node);

	// ��Bar�ഴ�����������
	void reBuild(Bar *bar, float time, cocos2d::Vec2 moveDis);

	//����Json�ļ�
	void createJson();

private:
	cocos2d::Size visibleSize;
	cocos2d::Director *director;
	cocos2d::Value uiWidth;
	cocos2d::Value uiHeight;
	cocos2d::Label *textLabel;
};

#endif // __Hello_SCENE_H__
