#include "Barrier.h"


Barrier::Barrier()
{
}


Barrier::~Barrier()
{
}

Barrier *Barrier::create(cocos2d::Vec2 *pointArray, int pointNumber) {
	Barrier *pRet = new Barrier();
	if (pRet && pRet->init(pointArray, pointNumber)) {
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

bool Barrier::init(cocos2d::Vec2 *pointArray, int pointNumber) {	// 障碍物默认固定，且速度为（0， 0）
	if (!Sprite::init())
		return false;
	physicsBody = cocos2d::PhysicsBody::create();
	physicsBody->addShape(cocos2d::PhysicsShapeEdgePolygon::create(pointArray, pointNumber, cocos2d::PhysicsMaterial(0, 1, 0)));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setContactTestBitmask(0x00000001);
	physicsBody->setTag(3);
	setColor(makeColor());
	for (int i = 0; i < pointNumber; ++i) {
		this->pointArray[i] = pointArray[i];
	}
	this->pointNumber = pointNumber;
	this->drawNode = nullptr;
	createPolygonGraphy();
	return true;
}

Barrier *Barrier::create(cocos2d::Vec2 *pointArray, int pointNumber, bool sign)
{
	Barrier *pRet = new Barrier();
	if (pRet && pRet->init(pointArray, pointNumber, true)) {
		pRet->autorelease();
		pRet->connectBody();
		pRet->getDrawNode()->setVisible(false);
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Barrier::init(cocos2d::Vec2 *pointArray, int pointNumber, bool sign)
{
	if (!Sprite::init())
		return false;
	physicsBody = cocos2d::PhysicsBody::create();
	physicsBody->addShape(cocos2d::PhysicsShapeEdgePolygon::create(pointArray, pointNumber, cocos2d::PhysicsMaterial(0, 1, 0), 10.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setContactTestBitmask(0x00000001);
	physicsBody->setTag(3);
	setColor(cocos2d::Color3B(236, 236, 244));
	for (int i = 0; i < pointNumber; ++i) {
		this->pointArray[i] = pointArray[i];
	}
	this->pointNumber = pointNumber;
	this->drawNode = nullptr;
	createPolygonGraphy(sign);
	return true;
}

void Barrier::connectBody() {
	setPhysicsBody(physicsBody);
}

void Barrier::createPolygonGraphy(bool sign)
{
	if (drawNode != nullptr) {
		drawNode->clear();
		removeChild(drawNode, true);
	}
	drawNode = cocos2d::DrawNode::create(5);
	drawNode->drawPolygon(pointArray, pointNumber, cocos2d::Color4F(cocos2d::Color3B(236, 236, 244)), 10, cocos2d::Color4F(getColor()));
	this->addChild(drawNode);
}

void Barrier::createPolygonGraphy()
{
	if (drawNode != nullptr) {
		drawNode->clear();
		removeChild(drawNode, true);
	}
	drawNode = cocos2d::DrawNode::create(5);
	drawNode->drawPoly(pointArray, pointNumber, true, cocos2d::Color4F(getColor()));
	this->addChild(drawNode);
}

void Barrier::setColor(cocos2d::Color3B color)
{
	this->color = color;
}

cocos2d::Color3B Barrier::getColor()
{
	return this->color;
}

cocos2d::Color3B Barrier::makeColor()
{
	GLubyte _r, _g, _b;
	srand(time(NULL));
	_r = rand() % 256;
	_g = rand() % 256;
	_b = rand() % 256;
	return cocos2d::Color3B(_r, _g, _b);
}

cocos2d::DrawNode * Barrier::getDrawNode()
{
	return drawNode;
}

cocos2d::PhysicsBody * Barrier::getPhysicsBody()
{
	return this->physicsBody;
}