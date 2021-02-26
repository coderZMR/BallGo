#pragma once

#include <cocos2d.h>

class Star : public cocos2d::Sprite
{
public:
	Star();
	~Star();
	static Star *create(cocos2d::Vec2 *pointArray, int pointNumber, cocos2d::Color3B color);
	virtual bool init(cocos2d::Vec2 *pointArray, int pointNumber, cocos2d::Color3B color);
	cocos2d::Color3B getColor();
	void connectBody();
	void createPolygonGraphy(cocos2d::Color3B color);
	bool getifFirstContact();
	void setifFirstContact(bool state);
	bool getifDelete();
	void setifDelete(bool state);
private:
	cocos2d::Color3B color;
	cocos2d::PhysicsBody *physicsBody;
	cocos2d::DrawNode *drawNode; 
	cocos2d::Vec2 pointArray[8];
	int pointNumber;
	bool ifFirstContact;
	bool ifDelete;
};

