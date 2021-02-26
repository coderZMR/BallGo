#pragma once
#include "Barrier.h"
#include "cocos2d.h"

class AddBarrier
{
public:
	AddBarrier(cocos2d::Layer *Target, int barrierNumber, int interval, int pointMinNumber);
	~AddBarrier();
};

