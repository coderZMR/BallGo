#include"MainMenuScene.h"
#include"ListScene.h"
#include"SimpleAudioEngine.h"
#include"ChuangGuan.h"
#include "GateMapLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

//构造函数
MainMenu::MainMenu(){
	
}

//析构函数
MainMenu::~MainMenu(){
	this->unscheduleUpdate();
	
}


//MainMenu类的创建场景函数
Scene* MainMenu::createScene(){
	
	auto scene = Scene::create();
	
	auto layer = MainMenu::create();
	
	scene->addChild(layer);

	return scene;

}

//MainMenu类的初始化函数
bool MainMenu::init(){
	//初始化界面颜色
	if (!LayerColor::initWithColor(Color4B(131,175,155,255)))
	{
		return false;
	}

	
	visibleSize = Director::getInstance()->getVisibleSize();
	
	//插入背景图片
	auto bg = Sprite::create("bg2.png");
	bg->setPosition(visibleSize/2);
	bg->setScale(1.2);
	addChild(bg);

	//自定义OntouchBegan闭包函数
	auto btnOnTouchBegan= [&](Touch *t,Event *e){
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
		{
			//执行按钮动作函数
		scheduleOnce(schedule_selector(MainMenu::passTimeForMove), 0);
		scheduleOnce(schedule_selector(MainMenu::braveTimeForMove), 0);
		scheduleOnce(schedule_selector(MainMenu::rankTimeForMove), 0);
		scheduleOnce(schedule_selector(MainMenu::exitTimeForMove), 0);

			return true;
		}
		return false;
	};	

	//插入闯关模式背景层
	passMode = LayerColor::create(Color4B(255,255,255, 70), 300,100);
	passMode->setPosition(visibleSize.width / 2.5, visibleSize.height / 16*12);
	passMode->setAnchorPoint(Vec2(0.5,0.5));
	addChild(passMode);

	//插入闯关模式名图片
	passBtnName = Sprite::create("passMode.png");
	passBtnName->setPosition(passMode->getContentSize()/2);
	passBtnName->setScale(2);
	passMode->addChild(passBtnName);

	//给按钮添加监听器
	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->onTouchBegan = btnOnTouchBegan;
	listener1->onTouchEnded = [&](Touch *t, Event *e){
		//执行转场函数
		scheduleOnce(schedule_selector(MainMenu::goToPassScene), 1);
		
	};
	//进行事件绑定
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, passMode);

	//添加冒险模式背景层
	braveMode = LayerColor::create(Color4B(200, 200, 200, 70), 300, 100);
	braveMode->setPosition(visibleSize.width / 2.5, visibleSize.height / 16*9);
	addChild(braveMode);

	//添加冒险模式按钮名字图片
	braveBtnName = Sprite::create("braveMode.png");
	braveBtnName->setPosition(passMode->getContentSize() / 2);
	braveBtnName->setScale(2);
	braveMode->addChild(braveBtnName);
	//添加监听器
	auto listener2 = EventListenerTouchOneByOne::create();
	listener2->onTouchBegan = btnOnTouchBegan;
	listener2->onTouchEnded = [&](Touch *t, Event *e){
		//执行转场函数
		scheduleOnce(schedule_selector(MainMenu::goToAdventureScene), 1);

	};
	//进行事件绑定
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, braveMode);

	//插入排行榜按钮背景层
	rankList = LayerColor::create(Color4B(255, 242, 0, 70), 300, 100);
	rankList->setPosition(visibleSize.width / 2.5, visibleSize.height / 16*6);
	addChild(rankList);
	//插入排行榜按钮名字图片
	rankBtnName = Sprite::create("rankList.png");
	rankBtnName->setPosition(passMode->getContentSize() / 2);
	rankBtnName->setScale(2);
	rankList->addChild(rankBtnName);
	//添加监听器
	auto listener3 = EventListenerTouchOneByOne::create();
	listener3->onTouchBegan = btnOnTouchBegan;
	listener3->onTouchEnded = [&](Touch *t, Event *e){
		//执行转场函数
		scheduleOnce(schedule_selector(MainMenu::goToListScene), 1);

	};
	//进行事件绑定
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener3, rankList);

	//插入退出游戏按钮背景层
	exit = LayerColor::create(Color4B(181, 130, 29, 70), 300, 100);
	exit->setPosition(visibleSize.width / 2.5, visibleSize.height / 16*3);
	addChild(exit);

	//插入退出游戏按钮名字图片
	exitBtnName = Sprite::create("exit.png");
	exitBtnName->setPosition(passMode->getContentSize() / 2);
	exitBtnName->setScale(2);
	exit->addChild(exitBtnName);

	//添加监听器
	auto listener4 = EventListenerTouchOneByOne::create();
	listener4->onTouchBegan = btnOnTouchBegan;
	listener4->onTouchEnded = [&](Touch *t, Event *e){
		//执行转场函数
		scheduleOnce(schedule_selector(MainMenu::goToExit), 1);
		
	};
	//进行事件绑定
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener4, exit);

	return true;
}

void MainMenu::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3", true); //播放背景音乐
}

//闯关模式按钮动作函数
void MainMenu::passTimeForMove(float dt) {
	auto moveAction = MoveBy::create(1, cocos2d::Vec2(-visibleSize.width, 0));
	auto repeatAction = Repeat::create(moveAction,1);
	passMode->runAction(repeatAction);
}

//冒险模式按钮动作函数
void MainMenu::braveTimeForMove(float dt){
	auto moveAction = MoveBy::create(1, cocos2d::Vec2(visibleSize.width, 0));
	auto repeatAction = Repeat::create(moveAction, 1);
	braveMode->runAction(repeatAction);
}

//排行榜按钮动作函数
void MainMenu::rankTimeForMove(float dt){
	auto moveAction = MoveBy::create(1, cocos2d::Vec2(-visibleSize.width, 0));
	auto repeatAction = Repeat::create(moveAction, 1);
	rankList->runAction(repeatAction);
}

//退出游戏按钮动作函数
void MainMenu::exitTimeForMove(float dt){
	auto moveAction = MoveBy::create(1, cocos2d::Vec2(visibleSize.width, 0));
	auto repeatAction = Repeat::create(moveAction, 1);
	exit->runAction(repeatAction);
}

//跳转到排行榜界面的函数
void MainMenu::goToListScene(float dt){
	auto scene = List::createScene();
	Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(1, scene));
}

//跳转到冒险难度选择界面
void MainMenu::goToAdventureScene(float dt){
	auto scene = GateMapLayer::createScene();
	Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(1, scene));
}

//跳转到闯关模式的选关界面
void MainMenu::goToPassScene(float dt){
	auto scene = ChuangGuan::createScene();
	Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(1, scene));
}

//退出游戏函数
void MainMenu::goToExit(float dt){
	Director::getInstance()->end();
}




