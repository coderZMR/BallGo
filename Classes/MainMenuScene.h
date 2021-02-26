#ifndef __MainMenu_Scene_H__
#define __MainMenu_Scene_H__

#include"cocos2d.h"

USING_NS_CC;

class MainMenu :public cocos2d::LayerColor
{
public:
	//���캯��
	MainMenu();
	//��������
	~MainMenu();
	//MainMenu��ľ�̬������������
	static cocos2d::Scene* createScene();
	//MainMenu��ĳ�ʼ������
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	//MainMenu��Ĵ��캯��
	CREATE_FUNC(MainMenu);
	//����ģʽ��ť��������
	void passTimeForMove(float dt);
	//ð��ģʽ��ť��������
	void braveTimeForMove(float dt);
	//���а�ť��������
	void rankTimeForMove(float dt);
	//�˳���Ϸ��ť��������
	void exitTimeForMove(float dt);
	//��ת�����а����ĺ���
	void goToListScene(float dt);
	//��ת��ð���Ѷ�ѡ�����
	void goToAdventureScene(float dt);
	//��ת������ģʽ��ѡ�ؽ���
	void goToPassScene(float dt);
	//�˳���Ϸ����
	void goToExit(float dt);


private:
	//���������С
	Size visibleSize;  
	//����ģʽ��ð��ģʽ�����а���˳���Ϸ��ť�ı�����
	LayerColor *passMode, *braveMode, *rankList, *exit;
	//���ء�ð�ա����а���˳���Ϸ��ť���ֵ�ͼƬ����ָ��
	cocos2d::Sprite *passBtnName, *braveBtnName, *rankBtnName, *exitBtnName;

};



#endif // !__MainMenu_Scene_H__
