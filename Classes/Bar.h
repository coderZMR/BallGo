#pragma once
#include <cocos2d.h>

class Bar : public cocos2d::Sprite
{
public:
	// �����崴��
	Bar();
	~Bar();
	// aΪ�߶���㣬bΪ�߶��յ㣬aRestitution���Ʒ�����С����ٶ���Ϊ1��Ԫ�ط���
	static Bar *create(cocos2d::Vec2 a, cocos2d::Vec2 b, float aRestitution);
	virtual bool init(cocos2d::Vec2 a, cocos2d::Vec2 b, float aRestitution);
	static Bar *create(cocos2d::Vec2 a, cocos2d::Vec2 b, bool sign);
	virtual bool init(cocos2d::Vec2 a, cocos2d::Vec2 b, bool sign);
	void connectBody();
	void createLineBoundary();
	void createLineBoundary(bool sign);
	// �Է��������ɫ����
	void setColor(cocos2d::Color3B color);
	cocos2d::Color3B getColor();
	cocos2d::Color3B makeColor();
	cocos2d::DrawNode *getDrawNode();
	cocos2d::PhysicsBody* getPhysicsBody();
private:
	cocos2d::PhysicsBody *physicsBody;
	cocos2d::DrawNode *drawNode;
	cocos2d::Color3B color;
	cocos2d::Vec2 a;
	cocos2d::Vec2 b;
};

