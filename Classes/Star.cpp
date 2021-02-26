#include "Star.h"


Star::Star()
{
}


Star::~Star()
{
}


Star* Star::create(cocos2d::Vec2 *pointArray, int pointNumber, cocos2d::Color3B color)
{
	Star *pRet = new Star();
	if (pRet && pRet->init(pointArray, pointNumber, color)) {
		pRet->autorelease();
		pRet->connectBody();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}


bool Star::init(cocos2d::Vec2 *pointArray, int pointNumber, cocos2d::Color3B color)
{
	if (!Sprite::init())
		return false;
	for (int i = 0; i < pointNumber; ++i) {
		this->pointArray[i] = pointArray[i];
	}
	this->pointNumber = pointNumber;
	this->color = color;
	this->ifFirstContact = true;
	this->ifDelete = false;
	physicsBody = cocos2d::PhysicsBody::create();
	physicsBody->addShape(cocos2d::PhysicsShapeEdgePolygon::create(pointArray, 8, cocos2d::PhysicsMaterial(0, 0, 0)));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setContactTestBitmask(0x00000001);
	physicsBody->setTag(5);
	setColor(color);
	this->drawNode = nullptr;
	createPolygonGraphy(color);
	return true;
}


void Star::createPolygonGraphy(cocos2d::Color3B color)
{
	drawNode = cocos2d::DrawNode::create();
	drawNode->drawPolygon(pointArray, 8, cocos2d::Color4F(color), 1, cocos2d::Color4F(color));
	this->addChild(drawNode);
}


cocos2d::Color3B Star::getColor()
{
	return this->color;
}


void Star::connectBody()
{
	setPhysicsBody(physicsBody);
}

bool Star::getifFirstContact()
{
	return this->ifFirstContact;
}
void Star::setifFirstContact(bool state)
{
	this->ifFirstContact = state;
}

bool Star::getifDelete()
{
	return this->ifDelete;
}
void Star::setifDelete(bool state)
{
	this->ifDelete = state;
}