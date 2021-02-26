#include "Bar.h"


Bar::Bar()
{
}


Bar::~Bar()
{
}

Bar *Bar::create(cocos2d::Vec2 a, cocos2d::Vec2 b, float aRestitution) {
	Bar *pRet = new Bar();
	if (pRet && pRet->init(a, b, aRestitution)) {
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

bool Bar::init(cocos2d::Vec2 a, cocos2d::Vec2 b, float aRestitution) {
	if (!Sprite::init())
		return false;
	physicsBody = cocos2d::PhysicsBody::createEdgeSegment(a, b, cocos2d::PhysicsMaterial(0, aRestitution, 0), 3.0f);
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setContactTestBitmask(0x00000001);
	physicsBody->setTag(1);
	setColor(makeColor());
	this->a = a;
	this->b = b;
	createLineBoundary();
	return true;
}

Bar *Bar::create(cocos2d::Vec2 a, cocos2d::Vec2 b, bool sign)
{
	Bar *pRet = new Bar();
	if (pRet && pRet->init(a, b, sign)) {
		pRet->autorelease();
		pRet->connectBody();
		pRet->getDrawNode()->setVisible(false);
		pRet->setTag(1);
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Bar::init(cocos2d::Vec2 a, cocos2d::Vec2 b, bool sign)
{
	if (!Sprite::init())
		return false;
	this->a = a;
	this->b = b;
	this->drawNode = nullptr;
	physicsBody = cocos2d::PhysicsBody::createEdgeSegment(a, b, cocos2d::PhysicsMaterial(0, 1, 0), 10.0f);
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setContactTestBitmask(0x00000001);
	physicsBody->setTag(1);
	setColor(makeColor());
	createLineBoundary(sign);
	return true;
}

void Bar::connectBody() {
	setPhysicsBody(physicsBody);
}


void Bar::createLineBoundary()
{
	drawNode = cocos2d::DrawNode::create(5);
	drawNode->drawSegment(this->a, this->b, 3.0f, cocos2d::Color4F(getColor()));
	this->addChild(drawNode);
}

void Bar::createLineBoundary(bool sign)
{
	if (drawNode != nullptr) {
		drawNode->clear();
		removeChild(drawNode, true);
	}
	drawNode = cocos2d::DrawNode::create();
	drawNode->drawSegment(this->a, this->b, 10, cocos2d::Color4F(getColor()));
	this->addChild(drawNode);
}


void Bar::setColor(cocos2d::Color3B color)
{
	this->color = color;
}

cocos2d::Color3B Bar::getColor()
{
	if (this == nullptr) return cocos2d::Color3B::RED;
	return this->color;
}

cocos2d::Color3B Bar::makeColor()
{
	GLubyte _r, _g, _b;
	srand(time(NULL));
	_r = rand() % 256;
	_g = rand() % 256;
	_b = rand() % 256;
	return cocos2d::Color3B(_r, _g, _b);
}

cocos2d::DrawNode *Bar::getDrawNode()
{
	return this->drawNode;
}

cocos2d::PhysicsBody* Bar::getPhysicsBody()
{
	return this->physicsBody;
}