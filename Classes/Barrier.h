#pragma once

#include <cocos2d.h>

class Barrier : public cocos2d::Sprite
{
public:
	// 障碍物的创建
	Barrier();
	~Barrier();
	static Barrier *create(cocos2d::Vec2 *pointArray, int pointNumber);
	virtual bool init(cocos2d::Vec2 *pointArray, int pointNumber);
	static Barrier *create(cocos2d::Vec2 *pointArray, int pointNumber, bool sign);
	virtual bool init(cocos2d::Vec2 *pointArray, int pointNumber, bool sign);
	void connectBody();
	void createPolygonGraphy();
	void createPolygonGraphy(bool sign);
	// 对障碍物的颜色控制
	void setColor(cocos2d::Color3B color);
	cocos2d::Color3B getColor();
	cocos2d::Color3B makeColor();
	cocos2d::DrawNode *getDrawNode();
	cocos2d::PhysicsBody *getPhysicsBody();
private:
	cocos2d::PhysicsBody *physicsBody;
	cocos2d::Vec2 pointArray[30];
	int pointNumber;
	cocos2d::DrawNode *drawNode;
	cocos2d::Color3B color;
};
