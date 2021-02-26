#pragma once
#include <cocos2d.h>

class Ball : public cocos2d::Sprite
{
public:
	// 小球的创建
	Ball();
	~Ball();
	static Ball *create(float radius, cocos2d::Color3B color, cocos2d::Vec2 minVelocity);
	virtual bool init(float radius, cocos2d::Color3B color, cocos2d::Vec2 minVelocity);
	static Ball *create(float radius, cocos2d::Color3B color);
	virtual bool init(float radius, cocos2d::Color3B color);
	
	void connectBody();
	
	void createCircleGraphy();
	
	cocos2d::Vec2 createVelocity(cocos2d::Vec2 minVelocity);
	
	void setVelocity(cocos2d::Vec2 velocity);
	cocos2d::Vec2 getVelocity();
	
	cocos2d::PhysicsBody *getPhysicsBody();
	// 对球的颜色控制
	
	void setColor(cocos2d::Color3B color);
	cocos2d::Color3B getColor();
	
	float getRadius();
	
	cocos2d::DrawNode *getDrawNode();
	
	cocos2d::Value getCurrentZ();
	void setCurrentZ(cocos2d::Value currentZ);
	
	cocos2d::Value getMaxRadius();
	void setMaxRadius(cocos2d::Value maxRadius);

	void setifFirstContect(bool state);
	bool getifFirstContect();
	
private:
	float radius;
	cocos2d::PhysicsBody *physicsBody;
	cocos2d::DrawNode *drawNode;
	cocos2d::Color3B color;
	cocos2d::Vec2 velocity;
	cocos2d::Value currentZ;
	cocos2d::Value maxRadius;
	bool ifFirstContect;
};

