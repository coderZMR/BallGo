
#include "cocos2d.h"
#include "GateMapLayer.h"
#include "ChuangGuan.h"

#include "CCStdC.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;



#include"cocostudio/cocostudio.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace cocostudio;

#include "AdventureScene.h"		//////////////////////////////////////////////////////////////////////////�����
#include "MainMenuScene.h"

USING_NS_CC;
Scene* GateMapLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = GateMapLayer::create();
	scene->addChild(layer);
	return scene;
}
bool GateMapLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}


	visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
		
	//��ӱ���
	auto bg = Sprite::create("14-111214155I7.jpg");
	bg->setRotation(90);
	bg->setScale(3);
	bg->setPosition(visibleSize.width/2,visibleSize.height/2);
	addChild(bg);

	//�ؿ�ѡ��ͼƬ
	auto guanQia= Sprite::create("guanqia.png");
	 guanQia->setPosition(120,640);
	 this->addChild(guanQia,1);
	
	//�����Ч
	Vector<SpriteFrame*> allframe;
	for (int i = 1; i <= 13; i++){

		auto sf = SpriteFrame::create(StringUtils::format("%d.PNG", i), Rect(0, 0, 180, 180));
		allframe.pushBack(sf);
	}
	auto animation = Animation::createWithSpriteFrames(allframe, 0.3);//ÿ��0.3�벥��һ֡
	auto animate = Animate::create(animation);
	auto sp = Sprite::create();
	sp->setPosition(100,100);
	addChild(sp, 1);
	sp->runAction(RepeatForever::create(animate));

	//��ӷ�ת��ͼƬ
	auto gateOneItem = MenuItemImage::create(
		"tavern_bg_3.png",
		"easy1.png",
		CC_CALLBACK_1(GateMapLayer::gateOneCallBack, this)); // �˳�

	auto gateTwoItem = MenuItemImage::create(
		"kunnan.png",
		"kunnan1.png",
		CC_CALLBACK_1(GateMapLayer::gateTwoCallBack, this)); // �˳�

	auto gateThreeItem = MenuItemImage::create(
		"bt.png",
		"bt1.png",
		CC_CALLBACK_1(GateMapLayer::gateThreeCallBack, this)); // �˳�

	selectGateMenu = SelectGate::create();
	selectGateMenu->addMenuItem(gateOneItem);
	selectGateMenu->addMenuItem(gateTwoItem);
	selectGateMenu->addMenuItem(gateThreeItem);
	//selectGateMenu->addMenuItem(gateFourItem);
	//selectGateMenu->addMenuItem(gateFiveItem);
	selectGateMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 );
	this->addChild(selectGateMenu, 2);

	auto backButtunlistener = EventListenerKeyboard::create();
	backButtunlistener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event *e){
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE){
			auto scene = MainMenu::createScene();
			Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(0.5, scene));
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButtunlistener, this);

	return true;
}

void GateMapLayer::onEnterTransitionDidFinish() 
{
	//play bgm
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3", true);
}
// a selector callback  
void GateMapLayer::gateOneCallBack(cocos2d::Ref* pSender)
{
	UserDefault::getInstance()->flush();

	////////////////////////////////////////////////////////////////////////// �����
	auto scene = Adventure::createScene(1);
	cocos2d::Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5, scene));
}
void GateMapLayer::gateTwoCallBack(cocos2d::Ref* pSender)
{

	
	auto scene = Adventure::createScene(2);
	cocos2d::Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5, scene));

}
void GateMapLayer::gateThreeCallBack(cocos2d::Ref* pSender)
{
	
	auto scene = Adventure::createScene(3);
	cocos2d::Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5, scene));
	
}


