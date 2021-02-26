#ifndef __List_Scene_H__
#define __List_Scene_H__

#include"cocos2d.h"


USING_NS_CC;

class List :public cocos2d::LayerColor
{
public:
	//���캯��
	List();
	//��������
	~List();

	//List��ľ�̬������������
	static cocos2d::Scene* createScene();
	//List���ʼ������
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	//List�ഴ�캯��
	CREATE_FUNC(List);

	//�Ƴ���ť�ĺ���
	void removeLeft(float dt);
	//�Ƴ��Ұ�ť�ĺ���
	void removeRight(float dt);
	//���ذ�ť�Ŵ���
	void turnBigBackBtn(float dt);
	//ת�������˵�����
	void goToMainMenuScene(float dt);
	//��ť�����ص�����
	void leftBtn(Ref *ref);
	//�Ұ�ť�����ص�����
	void rightBtn(Ref *ref);
	//�Ƴ����а�㺯��
	void removeChildList();
	//������а�㺯��
	void addChildList(const std::string AllOfListName[]);

private:
	
	Size visibleSize;					//��Ļ�ɼ������С
	
	int  mark[20];	//�����洢���飬0-4Ϊ�򵥼�ǰ������5-9Ϊ���Ѽ���
							//10-14Ϊ��̬���������Ϊ����ģʽ1-5�ص�ÿ�ص���߷�
	int tag;				//���а�ı��

	cocos2d::Label *back;		//���ذ�ť

	cocos2d::Label *No, *Score;	//����������

	cocos2d::Sprite *left, *right, *floorLeft, *floorRight;	//���Ұ�ť

	Sprite *list[4];					//���а�����ͼƬ

	LayerColor *board[4];		//������

	Label *NoAndScore[12];	//���κͷ���

	std::string listName;			//���а�ͼƬ��

	std::string AllOfBraveListName[7];		//ð��ģʽ���а�Label��Text����

	std::string AllOfPassListName[7];		//����ģʽ���а�Label��Text����
	
};



#endif // !__List_Scene_H__
