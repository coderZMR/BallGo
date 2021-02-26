#ifndef __SelectGate__H__
#define __SelectGate__H__

#include<iostream>
#include<cocos2d.h>

//菜单项的大小与屏幕的比例，当然可以通过setContentSize设置  
#define CONTENT_SIZE_SCALE (2.0/3)  
//菜单的缩小比例 最小的比例是1-MENU_SCALE  
#define MENU_SCALE 0.3  
//菜单的倾斜度 最多为45度  
#define MENU_ASLOPE 60.0  
//动画运行时间  
#define ANIMATION_DURATION  0.3f   
//calcFunction(x) 为 x/(x+a),其中a为常数  
#define CALC_A 1  
//菜单项长度与菜单长度的比例 滑动一个菜单项长度，菜单项变化一个  
#define ITEM_SIZE_SCALE (1.0/4)  
USING_NS_CC;
using namespace cocos2d;
class SelectGate :public Layer
{
public:
	//构造方法  
	CREATE_FUNC(SelectGate);
	//添加菜单项  
	void addMenuItem(cocos2d::MenuItem *item);
	//更新位置  
	void updatePosition();
	//更新位置，有动画  
	void updatePositionWithAnimation();
	//位置矫正  修改位置forward为移动方向  当超过1/3，进1  
	//true 为正向  false 负  
	void rectify(bool forward);
	//初始化  
	virtual bool init();
	//重置  显示所引号设置为0  
	void reset();
public:
	//设置当前显示索引  
	void setIndex(int index);
	//设置当前显示菜单项的索引号  
	float getIndex();
	//返回被选中的item  
	cocos2d::MenuItem * getCurrentItem();
	//数学计算式width*index/(abs(index)+CALC_A) ,其中CALC_A为常数  
	float calcFunction(float index, float width);
private:
	//菜单菜单项的索引号  
	float _index;
	//上一次菜单项的索引号  
	float _lastIndex;
	//菜单项集合,_children顺序会变化，新建数组保存顺序  
	cocos2d::Vector<cocos2d::MenuItem *> _items;
	//监听函数  
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	//动画完结调用函数，这个主要是确定哪一个菜单项在前面  
	void actionEndCallBack(float dx);
	//当前被选择的item  
	cocos2d::MenuItem *_selectedItem;
};
#endif