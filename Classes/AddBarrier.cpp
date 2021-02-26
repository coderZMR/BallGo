#include "AddBarrier.h"


AddBarrier::AddBarrier(cocos2d::Layer *Target, int barrierNumber, int interval, int pointMinNumber)	// interval控制障碍物的大小
{ 
	srand(time(NULL));
	int pointNumber = 0;
	cocos2d::Vec2 lastPoint = cocos2d::Vec2(0, 0);
	bool sign = true;
	bool sign_x = true;
	bool sign_y = true;
	const int constInterval = interval;
	int choiceForPosition = 0;
	float posX = 0;
	float posY = 0;
	for (int i = 1; i <= barrierNumber; i++) {
		cocos2d::Vec2 pointArray[30];
		interval = constInterval;
		pointNumber = pointMinNumber + cocos2d::random() % (20 - pointMinNumber + 1);
		choiceForPosition = cocos2d::random() % 12;
		switch (choiceForPosition)
		{
		case 0:
			posX = (0) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (0) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 1:
			posX = (cocos2d::Director::getInstance()->getVisibleSize().width / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (0) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 2:
			posX = (cocos2d::Director::getInstance()->getVisibleSize().width / 2) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (0) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 3:
			posX = (cocos2d::Director::getInstance()->getVisibleSize().width * 3 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (0) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 4:
			posX = (cocos2d::Director::getInstance()->getVisibleSize().width * 3 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (cocos2d::Director::getInstance()->getVisibleSize().height / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 5:
			posX = (cocos2d::Director::getInstance()->getVisibleSize().width * 3 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (cocos2d::Director::getInstance()->getVisibleSize().height / 2) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 6:
			posX = (cocos2d::Director::getInstance()->getVisibleSize().width * 3 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (cocos2d::Director::getInstance()->getVisibleSize().height * 3 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 7:
			posX = (cocos2d::Director::getInstance()->getVisibleSize().width / 2) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (cocos2d::Director::getInstance()->getVisibleSize().height * 3 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 8:
			posX = (cocos2d::Director::getInstance()->getVisibleSize().width * 1 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (cocos2d::Director::getInstance()->getVisibleSize().height * 3 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 9:
			posX = (0) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (cocos2d::Director::getInstance()->getVisibleSize().height * 3 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 10:
			posX = (0) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (cocos2d::Director::getInstance()->getVisibleSize().height * 1 / 2) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		case 11:
			posX = (0) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 4);
			posY = (cocos2d::Director::getInstance()->getVisibleSize().height * 1 / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 4);
			break;
		}
		lastPoint = cocos2d::Vec2(
			posX, posY
			// 注释代码效果为在中间区域添加障碍物
			/*(cocos2d::Director::getInstance()->getVisibleSize().width / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().width / 2),
			(cocos2d::Director::getInstance()->getVisibleSize().height / 4) + cocos2d::random() % (int)(cocos2d::Director::getInstance()->getVisibleSize().height / 2)*/
		);
		pointArray[0] = lastPoint;
		for (int i = 1; i < pointNumber; i++) {
			if (sign) {
				if (sign_x) pointArray[i] = cocos2d::Vec2(lastPoint.x + interval, lastPoint.y);
				else
				{
					pointArray[i] = cocos2d::Vec2(lastPoint.x - interval, lastPoint.y);
				}
				sign_x = !sign_x;
			}
			else
			{
				if (sign_y) pointArray[i] = cocos2d::Vec2(lastPoint.x, lastPoint.y + interval);
				else 
				{
					pointArray[i] = cocos2d::Vec2(lastPoint.x, lastPoint.y - interval);
				}
				sign_y = !sign_y;
			}
			sign = !sign;
			lastPoint = pointArray[i];
			interval = 0.9 * interval;
		}
		pointArray[pointNumber] = pointArray[0];
		auto barrier = Barrier::create(pointArray, pointNumber);
		Target->addChild(barrier);
	}
}


AddBarrier::~AddBarrier()
{
}


