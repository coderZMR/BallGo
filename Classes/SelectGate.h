#ifndef __SelectGate__H__
#define __SelectGate__H__

#include<iostream>
#include<cocos2d.h>

//�˵���Ĵ�С����Ļ�ı�������Ȼ����ͨ��setContentSize����  
#define CONTENT_SIZE_SCALE (2.0/3)  
//�˵�����С���� ��С�ı�����1-MENU_SCALE  
#define MENU_SCALE 0.3  
//�˵�����б�� ���Ϊ45��  
#define MENU_ASLOPE 60.0  
//��������ʱ��  
#define ANIMATION_DURATION  0.3f   
//calcFunction(x) Ϊ x/(x+a),����aΪ����  
#define CALC_A 1  
//�˵������˵����ȵı��� ����һ���˵���ȣ��˵���仯һ��  
#define ITEM_SIZE_SCALE (1.0/4)  
USING_NS_CC;
using namespace cocos2d;
class SelectGate :public Layer
{
public:
	//���췽��  
	CREATE_FUNC(SelectGate);
	//��Ӳ˵���  
	void addMenuItem(cocos2d::MenuItem *item);
	//����λ��  
	void updatePosition();
	//����λ�ã��ж���  
	void updatePositionWithAnimation();
	//λ�ý���  �޸�λ��forwardΪ�ƶ�����  ������1/3����1  
	//true Ϊ����  false ��  
	void rectify(bool forward);
	//��ʼ��  
	virtual bool init();
	//����  ��ʾ����������Ϊ0  
	void reset();
public:
	//���õ�ǰ��ʾ����  
	void setIndex(int index);
	//���õ�ǰ��ʾ�˵����������  
	float getIndex();
	//���ر�ѡ�е�item  
	cocos2d::MenuItem * getCurrentItem();
	//��ѧ����ʽwidth*index/(abs(index)+CALC_A) ,����CALC_AΪ����  
	float calcFunction(float index, float width);
private:
	//�˵��˵����������  
	float _index;
	//��һ�β˵����������  
	float _lastIndex;
	//�˵����,_children˳���仯���½����鱣��˳��  
	cocos2d::Vector<cocos2d::MenuItem *> _items;
	//��������  
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	//���������ú����������Ҫ��ȷ����һ���˵�����ǰ��  
	void actionEndCallBack(float dx);
	//��ǰ��ѡ���item  
	cocos2d::MenuItem *_selectedItem;
};
#endif