
#include "PassAllGateScene.h"
#include "MainMenuScene.h"


USING_NS_CC;


Scene * PassAllGate::createScene(){

	auto scene = Scene::create();


	auto layer = PassAllGate::create();

	scene->addChild(layer);


	return scene;

}

bool PassAllGate::init(){

	if (!LayerColor::initWithColor(cocos2d::Color4B(131, 175, 155, 255))){
		return false;
	}

	//获取屏幕可见区域大小
	visibleSize = Director::getInstance()->getVisibleSize();

	auto label1 = Label::createWithTTF("Congratulations  To  You", "fonts/arial.ttf", 48);
	label1->setPosition(visibleSize.width / 2, visibleSize.height / 4 * 3);
	addChild(label1);


	auto label2 = Label::createWithTTF("Pass All Of Gate !", "fonts/arial.ttf", 48);
	label2->setPosition(visibleSize.width / 2, visibleSize.height / 8*5);
	addChild(label2);

	//返回按钮
	back = Label::createWithTTF("Back", "fonts/arial.ttf", 48);
	back->setPosition(visibleSize.width / 2, visibleSize.height / 4);
	back->setColor(Color3B(0,0,0));
	back->setContentSize(Size(300, 150));
	addChild(back);

	//添加监听器
	auto listener = EventListenerTouchOneByOne::create();
	auto btnOnTouchBegan = [&](Touch *t, Event *e){
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
		{
			back->setScale(1.5);
			scheduleOnce(schedule_selector(PassAllGate::turnBigBackBtn), 0.1);
			scheduleOnce(schedule_selector(PassAllGate::goToMainMenuScene), 0.3);


			return true;
		}
		return false;
	};
	listener->onTouchBegan = btnOnTouchBegan;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, back);




	return true;
}


//返回按钮放大函数
void PassAllGate::turnBigBackBtn(float dt){
	back->setScale(1);
}

//转场到主菜单界面
void PassAllGate::goToMainMenuScene(float dt){
	auto scene = MainMenu::createScene();
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("turnSceneMusic.mp3");

	Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(1, scene));
}
