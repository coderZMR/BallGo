#ifndef __Final2_Scene_H__
#define __Final2_Scene_H__

#include "cocos2d.h"


USING_NS_CC;

class Final2 :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	CREATE_FUNC(Final2);

	void backBtnMenu(Ref *ref);


	void againBtnMenu(Ref *ref);

private:
	//��Ļ�ɼ������С
	Size visibleSize;

	//����Label
	Label *label1, *score1, *scoreName1;


};




#endif // !__Final2_Scene_H__
