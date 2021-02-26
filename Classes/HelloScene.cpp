#include "HelloScene.h"
#include "SimpleAudioEngine.h"
#include "Bar.h"
#include "Ball.h"
#include "MainMenuScene.h"
#include <fstream>
#include <iostream>


#include "CCStdC.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "SimpleAudioEngine.h"

using namespace  rapidjson;
using namespace std;

// 王森
#include "ChuangGuan.h"		// 闯关
#include "GateMapLayer.h"	// 冒险
#include "SelectGate.h"		// 滑动基类
#define FILENAME "myhero.json"


USING_NS_CC;
using namespace CocosDenshion;

// 欢迎界面

Scene* Hello::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = Hello::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Hello::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	director = Director::getInstance();

	setThisVisibleSize(Director::getInstance()->getVisibleSize());
	auto path = FileUtils::getInstance()->getWritablePath();
	fstream _file;
	_file.open(path+FILENAME, ios::in);

	if (!_file)
	{
		createJson();
	}


	
	
	uiWidth = this->visibleSize.width;
	uiHeight = this->visibleSize.height;
    
	createUI();

	solveContactEvent();

	scheduleOnce(schedule_selector(Hello::setListenerAndShowText), 2.0f);

    return true;
}

void Hello::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3", true); //播放背景音乐
}

void Hello::createJson(){
	log("fdsfsdfsdfsdfsdfsdfdffsdfsdfsdfdfdfdsfsdfdfdfdfdf");
	//*** 生成 json 文件，存储在 getWritablePath 文件夹下 ***
	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	rapidjson::Value object(rapidjson::kObjectType);

	// json object 格式添加 “名称/值” 对
	object.AddMember("temp", 0, allocator);
	//闯关分数
	object.AddMember("c1", 0, allocator);
	object.AddMember("c2", 0, allocator);
	object.AddMember("c3", 0, allocator);
	object.AddMember("c4", 0, allocator);
	object.AddMember("c5", 0, allocator);
	//冒险分数
	object.AddMember("m11", 0, allocator);
	object.AddMember("m12", 0, allocator);
	object.AddMember("m13", 0, allocator);
	object.AddMember("m14", 0, allocator);
	object.AddMember("m15", 0, allocator);
	object.AddMember("m16", 0, allocator);

	object.AddMember("m21", 0, allocator);
	object.AddMember("m22", 0, allocator);
	object.AddMember("m23", 0, allocator);
	object.AddMember("m24", 0, allocator);
	object.AddMember("m25", 0, allocator);
	object.AddMember("m26", 0, allocator);

	object.AddMember("m31", 0, allocator);
	object.AddMember("m32", 0, allocator);
	object.AddMember("m33", 0, allocator);
	object.AddMember("m34", 0, allocator);
	object.AddMember("m35", 0, allocator);
	object.AddMember("m36", 0, allocator);
	//object.AddMember("doubletag", 1.0, allocator);
	//object.AddMember("booltag", true, allocator);
	//object.AddMember("hellotag", "helloworld", allocator);

	// json 加入数组
	array.PushBack(object, allocator);

	// json object 格式添加 “名称/值” 对
	writedoc.AddMember("json", "json string", allocator);
	writedoc.AddMember("array", array, allocator);

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	auto path = FileUtils::getInstance()->getWritablePath();
	//log(path.c_str());
	path.append("myhero.json");

	FILE* file = fopen(path.c_str(), "wb");
	if (file)
	{
		log("123456789");
		fputs(buffer.GetString(), file);
		fclose(file);
	}
}
void Hello::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Hello::setThisVisibleSize(cocos2d::Size size)
{
	visibleSize = size;
}

Size Hello::getThisVisibleSize()
{
	return visibleSize;
}


void Hello::createUI()
{
	auto backGroundPic = cocos2d::Sprite::create("bg2.png");
	backGroundPic->setScale(1.2f);
	addChild(backGroundPic);
	backGroundPic->setPosition(visibleSize / 2);
	// B
	auto bar1 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 1 / 15, uiHeight.asFloat() * 5 / 5), cocos2d::Vec2(uiWidth.asFloat() * 2 / 15, uiHeight.asFloat() * 5 / 5), true);
	reBuild(bar1, 2.0f, cocos2d::Vec2(0, -uiHeight.asFloat() * 1 / 5));
	addChild(bar1);

	auto bar2 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 1 / 15, uiHeight.asFloat() * 3 / 5), cocos2d::Vec2(uiWidth.asFloat() * 2 / 15, uiHeight.asFloat() * 3 / 5), true);
	bar2->getDrawNode()->setVisible(true);
	addChild(bar2);

	auto bar3 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 1 / 15, uiHeight.asFloat() * 1 / 5), cocos2d::Vec2(uiWidth.asFloat() * 2 / 15, uiHeight.asFloat() * 1 / 5), true);
	reBuild(bar3, 2.0f, cocos2d::Vec2(0, uiHeight.asFloat() * 1 / 5));
	addChild(bar3);

	auto bar4 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 0, uiHeight.asFloat() * 2 / 5), cocos2d::Vec2(uiWidth.asFloat() * 0, uiHeight.asFloat() * 4 / 5), true);
	reBuild(bar4, 2.0f, cocos2d::Vec2(uiWidth.asFloat() * 1 / 15, 0));
	addChild(bar4);

	auto bar5 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 3 / 15, uiHeight.asFloat() * 2 / 5), cocos2d::Vec2(uiWidth.asFloat() * 3 / 15, uiHeight.asFloat() * 4 / 5), true);
	reBuild(bar5, 2.0f, cocos2d::Vec2(-uiWidth.asFloat() * 1 / 15, 0));
	addChild(bar5);

	// a
	auto bar6 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 3 / 15, uiHeight.asFloat() * 1 / 5), cocos2d::Vec2(uiWidth.asFloat() * 3 / 15, uiHeight.asFloat() * 2 / 5), true);
	reBuild(bar6, 2.0f, cocos2d::Vec2(0, uiHeight.asFloat() * 1 / 5));
	addChild(bar6);

	auto bar7 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 3 / 15, uiHeight.asFloat() * 5 / 5), cocos2d::Vec2(uiWidth.asFloat() * 4 / 15, uiHeight.asFloat() * 5 / 5), true);
	reBuild(bar7, 2.0f, cocos2d::Vec2(0, -uiHeight.asFloat() * 2 / 5));
	addChild(bar7);

	auto bar8 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 3 / 15, uiHeight.asFloat() * 2 / 5), cocos2d::Vec2(uiWidth.asFloat() * 4 / 15, uiHeight.asFloat() * 3 / 5), true);
	bar8->getDrawNode()->setVisible(true);
	addChild(bar8);

	auto bar9 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 4 / 15, uiHeight.asFloat() * 4 / 5), cocos2d::Vec2(uiWidth.asFloat() * 4.5 / 15, uiHeight.asFloat() * 3 / 5), true);
	reBuild(bar9, 2.0f, cocos2d::Vec2(0, -uiHeight.asFloat() * 1 / 5));
	addChild(bar9);

	// L
	auto bar10 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 5.5 / 15, uiHeight.asFloat() * 1 / 5), cocos2d::Vec2(uiWidth.asFloat() * 5.5 / 15, uiHeight.asFloat() * 3 / 5), true);
	reBuild(bar10, 2.0f, cocos2d::Vec2(0, uiHeight.asFloat() * 1 / 5));
	addChild(bar10);

	auto bar11 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 6.5 / 15, uiHeight.asFloat() * 2 / 5), cocos2d::Vec2(uiWidth.asFloat() * 7 / 15, uiHeight.asFloat() * 2.5 / 5), true);
	reBuild(bar11, 2.0f, cocos2d::Vec2(-uiWidth.asFloat() * 1 / 15, 0));
	addChild(bar11);

	// L
	auto bar12 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 6.5 / 15, uiHeight.asFloat() * 3 / 5), cocos2d::Vec2(uiWidth.asFloat() * 6.5 / 15, uiHeight.asFloat() * 5 / 5), true);
	reBuild(bar12, 2.0f, cocos2d::Vec2(0, -uiHeight.asFloat() * 1 / 5));
	addChild(bar12);

	auto bar13 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 7.5 / 15, uiHeight.asFloat() * 2 / 5), cocos2d::Vec2(uiWidth.asFloat() * 8 / 15, uiHeight.asFloat() * 2.5 / 5), true);
	reBuild(bar13, 2.0f, cocos2d::Vec2(-uiWidth.asFloat() * 1 / 15, 0));
	addChild(bar13);

	// G
	auto bar14 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 9 / 15, uiHeight.asFloat() * 5 / 5), cocos2d::Vec2(uiWidth.asFloat() * 11 / 15, uiHeight.asFloat() * 5 / 5), true);
	reBuild(bar14, 2.0f, cocos2d::Vec2(0, -uiHeight.asFloat() * 1 / 5));
	addChild(bar14);

	auto bar15 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 8 / 15, uiHeight.asFloat() * 2 / 5), cocos2d::Vec2(uiWidth.asFloat() * 8 / 15, uiHeight.asFloat() * 4 / 5), true);
	reBuild(bar15, 2.0f, cocos2d::Vec2(uiWidth.asFloat() * 1 / 15, 0));
	addChild(bar15);

	auto bar16 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 9 / 15, uiHeight.asFloat() * 1 / 5), cocos2d::Vec2(uiWidth.asFloat() * 11 / 15, uiHeight.asFloat() * 1.5 / 5), true);
	reBuild(bar16, 2.0f, cocos2d::Vec2(0, uiHeight.asFloat() * 1 / 5));
	addChild(bar16);

	auto bar17 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 12 / 15, uiHeight.asFloat() * 2 / 5), cocos2d::Vec2(uiWidth.asFloat() * 12 / 15, uiHeight.asFloat() * 3 / 5), true);
	reBuild(bar17, 2.0f, cocos2d::Vec2(-uiWidth.asFloat() * 1 / 15, 0));
	addChild(bar17);

	auto bar18 = Bar::create(cocos2d::Vec2(uiWidth.asFloat() * 10 / 15, uiHeight.asFloat() * 3 / 5), cocos2d::Vec2(uiWidth.asFloat() * 11.5 / 15, uiHeight.asFloat() * 3 / 5), true);
	bar18->getDrawNode()->setVisible(true);
	addChild(bar18);

	// o
	auto ball = Ball::create(uiWidth.asFloat() * 1 / 30, cocos2d::Color3B::RED);
	ball->getPhysicsBody()->setVelocity(cocos2d::Vec2(70, 70));
	ball->setPosition(uiWidth.asFloat() * 10 / 15, uiHeight.asFloat() * 2 / 5);
	addChild(ball);

	// !
	auto line = cocos2d::DrawNode::create();
	line->drawSegment(cocos2d::Vec2(uiWidth.asFloat() * 13.5 / 15, uiHeight.asFloat() * 2.5 / 5), cocos2d::Vec2(uiWidth.asFloat() * 14 / 15, uiHeight.asFloat() * 4 / 5), 10, cocos2d::Color4F(cocos2d::Color3B::RED));
	auto blinkAction1 = cocos2d::Blink::create(1.0f, 2);
	auto repeatAction1 = cocos2d::RepeatForever::create(blinkAction1);
	line->runAction(repeatAction1);
	line->setTag(3);
	addChild(line);

	auto circle = cocos2d::DrawNode::create();
	circle->drawSolidCircle(cocos2d::Vec2(uiWidth.asFloat() * 13.3 / 15, uiHeight.asFloat() * 2.1 / 5), uiWidth.asFloat() * 1 / 40, 0, uiWidth.asFloat() * 2 / 40, cocos2d::Color4F(cocos2d::Color3B::RED));
	auto blinkAction2 = cocos2d::Blink::create(1.0f, 2);
	auto repeatAction2 = cocos2d::RepeatForever::create(blinkAction2);
	circle->runAction(repeatAction2);
	circle->setTag(3);
	addChild(circle);
}


void Hello::reBuild(Bar *bar, float time, cocos2d::Vec2 moveDis)
{
	bar->getDrawNode()->setVisible(true);
	bar->getPhysicsBody()->setDynamic(true);
	bar->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	auto moveAction = cocos2d::MoveBy::create(time, moveDis);
	bar->runAction(moveAction);
}


void Hello::solveContactEvent()
{
	auto colloisionListener = EventListenerPhysicsContact::create();
	colloisionListener->onContactBegin = [&](PhysicsContact &contact){
		auto spriteAPhysicsBody = contact.getShapeA()->getBody();
		auto spriteBPhysicsBody = contact.getShapeB()->getBody();
		if (spriteAPhysicsBody->getTag() == 1 && spriteBPhysicsBody->getTag() == 0) {
			auto barSprite = (Bar*)(spriteAPhysicsBody->getNode());
			auto mainBallSprite = (Ball*)(spriteBPhysicsBody->getNode());
			barSprite->setColor(barSprite->makeColor());
			barSprite->createLineBoundary(true);
		}
		if (spriteBPhysicsBody->getTag() == 1 && spriteAPhysicsBody->getTag() == 0) {
			auto barSprite = (Bar*)(spriteBPhysicsBody->getNode());
			auto mainBallSprite = (Ball*)(spriteAPhysicsBody->getNode());
			barSprite->setColor(barSprite->makeColor());
			barSprite->createLineBoundary(true);
		}
		return true;
	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(colloisionListener, this);
}


void Hello::setListenerAndShowText(float dt)
{
	textLabel = cocos2d::Label::createWithTTF("Start",
		"fonts/arial.ttf", 
		50);	
	textLabel->setTextColor(cocos2d::Color4B(cocos2d::Color3B(133, 175, 155)));
	textLabel->setPosition(visibleSize.width / 2, uiHeight.asFloat() * 1 / 5);
	textLabel->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(cocos2d::FadeOut::create(1.0f), cocos2d::FadeIn::create(1.0f), nullptr)));
	addChild(textLabel);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch *touch, Event *event) {
		// 在此添加场景切换界面
		auto scene = MainMenu::createScene();
		director->replaceScene(TransitionFadeBL::create(1, scene));
		return false;
	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

