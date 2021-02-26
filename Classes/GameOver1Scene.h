#ifndef __GameOver1_Scene_H__
#define __GameOver1_Scene_H__

#include"cocos2d.h"

USING_NS_CC;

class GameOver1:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	CREATE_FUNC(GameOver1);

	void backBtnMenu(Ref *ref);

	void againBtnMenu(Ref *ref);


private:

	Size visibleSize;
	Label *score,*label,*scoreName;
};



#endif // !__GameOver1_Scene_H__
