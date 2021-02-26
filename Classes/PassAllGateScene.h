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

	//���ذ�ť�Ŵ���
	void turnBigBackBtn(float dt);

	//ת�������˵�����
	void goToMainMenuScene(float dt);



private:
	//��Ļ�ɼ������С
	Size visibleSize;

	cocos2d::Label *back;




};




#endif // !__PassAllGate_Scene_H__
