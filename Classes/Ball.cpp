#include "Ball.h"


Ball::Ball()
{
}
Ball::~Ball()
{
}

// 在冒险模式中用于创建主角与每秒产生的小球
Ball *Ball::create(float radius, cocos2d::Color3B color, cocos2d::Vec2 minVelocity) {
	Ball *pRet = new Ball();
	if (pRet && pRet->init(radius, color, minVelocity)) {
		pRet->autorelease();
		// 链接刚体
		pRet->connectBody();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Ball::init(float radius, cocos2d::Color3B color, cocos2d::Vec2 minVelocity) {
	if (!Sprite::init())
		return false;
	// 设置刚体
	physicsBody = cocos2d::PhysicsBody::createCircle(radius, cocos2d::PhysicsMaterial(0, 1, 0));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setVelocity(createVelocity(minVelocity));
	physicsBody->setContactTestBitmask(0x00000001);
	physicsBody->setTag(0);
	// 初始化数据
	this->drawNode = nullptr;
	this->radius = radius;
	setColor(color);
	// 画边框
	createCircleGraphy();
	return true;
}
// 在闯关模式中用于创建主角
Ball *Ball::create(float radius, cocos2d::Color3B color) {
	Ball *pRet = new Ball();
	if (pRet && pRet->init(radius, color)) {
		pRet->autorelease();
		// 链接刚体
		pRet->connectBody();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Ball::init(float radius, cocos2d::Color3B color) {
	if (!Sprite::init())
		return false;
	// 设置刚体
	physicsBody = cocos2d::PhysicsBody::create();
	physicsBody->addShape(cocos2d::PhysicsShapeCircle::create(radius, cocos2d::PhysicsMaterial(0, 1, 0)));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setVelocity(cocos2d::Vec2(0, 0));
	physicsBody->setContactTestBitmask(0x00000001);
	physicsBody->setTag(0);
	// 初始化数据
	setCurrentZ(cocos2d::Value(5));
	setMaxRadius(cocos2d::Value(radius));
	this->drawNode = nullptr;
	this->radius = radius;
	this->maxRadius = radius;
	setColor(color);
	// 画边框
	createCircleGraphy();
	return true;
}

void Ball::connectBody() {
	setPhysicsBody(physicsBody);
}

void Ball::createCircleGraphy()
{
	if (drawNode != nullptr) this->removeChild(drawNode, true);
	drawNode = cocos2d::DrawNode::create(5);
	drawNode->drawSolidCircle(getAnchorPoint(), radius, 0, radius * 2.0, cocos2d::Color4F(color));
	this->addChild(drawNode, 5);
}

void Ball::setColor(cocos2d::Color3B color)
{
	this->color = color;
}

cocos2d::Color3B Ball::getColor()
{
	return this->color;
}

cocos2d::Vec2 Ball::createVelocity(cocos2d::Vec2 minVelocity)
{
	srand(time(NULL));
	int x = minVelocity.x + cocos2d::random() % (100 - (int)minVelocity.x + 1);
	int y = minVelocity.y + cocos2d::random() % (100 - (int)minVelocity.y + 1);
	velocity = cocos2d::Vec2(x, y);
	return velocity;
}

void Ball::setVelocity(cocos2d::Vec2 velocity)
{
	this->velocity = velocity;
}
cocos2d::Vec2 Ball::getVelocity()
{
	return velocity;
}

cocos2d::PhysicsBody *Ball::getPhysicsBody()
{
	return this->physicsBody;
}

float Ball::getRadius()
{
	return this->radius;
}

cocos2d::DrawNode *Ball::getDrawNode()
{
	return this->drawNode;
}


cocos2d::Value Ball::getCurrentZ()
{
	return this->currentZ;
}
void Ball::setCurrentZ(cocos2d::Value currentZ)
{
	this->currentZ = currentZ;
}


cocos2d::Value Ball::getMaxRadius()
{
	return this->maxRadius;
}
void Ball::setMaxRadius(cocos2d::Value maxRadius)
{
	this->maxRadius = maxRadius;
}

void Ball::setifFirstContect(bool state)
{
	this->ifFirstContect = state;
}

bool Ball::getifFirstContect()
{
	return this->ifFirstContect;
}