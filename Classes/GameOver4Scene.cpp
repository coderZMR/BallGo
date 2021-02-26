#include"GameOver4Scene.h"
#include"FinalThrough4Scene.h"
#include"SimpleAudioEngine.h"
#include"cocos2d.h"
#include"MainMenuScene.h"
#include"ChuangGuan.h"
#include "PassDifficultyScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;


Scene * GameOver4::createScene(){

	auto scene = Scene::create();
	auto layer1 = LayerColor::create(Color4B(255, 245, 247, 255));
	scene->addChild(layer1);

	auto layer = GameOver4::create();

	scene->addChild(layer);


	return scene;

}

void GameOver4::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playEffect("Fail.mp3");
}


bool GameOver4::init(){

	if (!Layer::init()){
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();

	//���˵���ť
	auto backItem = MenuItemImage::create(
		"m41.png",
		"m42.png",
		CC_CALLBACK_1(GameOver4::backBtnMenu, this)

		);
	auto backBtn = Menu::create(backItem, NULL);
	backBtn->setPosition(visibleSize.width / 2 - 110, visibleSize.height / 2 - 50);

	this->addChild(backBtn);

	//�������֡�again��

	auto label0 = TextFieldTTF::create("Return", "fonts/arial.ttf", 20);
	//����������ɫ
	label0->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label0->setPosition(visibleSize.width / 2 - 110, visibleSize.height / 2 - 130);
	//
	addChild(label0);


	//����һ�ΰ�ť
	auto againItem = MenuItemImage::create(
		"r41.png",
		"r42.png",
		CC_CALLBACK_1(GameOver4::againBtnMenu, this)

		);
	auto againBtn = Menu::create(againItem, NULL);
	againBtn->setPosition(visibleSize.width / 2 + 110, visibleSize.height / 2 - 50);

	this->addChild(againBtn);


	//�������֡�again��

	auto label2 = TextFieldTTF::create("Again", "fonts/arial.ttf", 20);
	//����������ɫ
	label2->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label2->setPosition(visibleSize.width / 2 + 110, visibleSize.height / 2 - 130);
	//
	addChild(label2);

	//��ӡ�Game Over!������
	auto label = TextFieldTTF::create("Game Over!", "fonts/arial.ttf", 40);
	//����������ɫ
	label->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 150);
	//
	addChild(label);


	scoreName = TextFieldTTF::create("Score:", "fonts/arial.ttf", 30);
	//����������ɫ
	scoreName->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	scoreName->setPosition(visibleSize.width / 2 - 40, visibleSize.height / 2 + 80);
	//
	addChild(scoreName);

	//��ӷ���
	const std::string mark = StringUtils::format("%d",0);
	score = TextFieldTTF::create(mark, "fonts/arial.ttf", 30);
	//����������ɫ
	score->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	score->setPosition(visibleSize.width / 2 + 65, visibleSize.height / 2 + 80);
	//
	addChild(score);



	return true;
}

// ������ʩ��ǿ����
void GameOver4::backBtnMenu(Ref *ref){

	auto scene = ChuangGuan::createScene();
	//ת�������˵�����
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));

	//TransitionFadeBL::create(1, scene)
	//��ѡת������
	//TransitionFadeBL  ������Ƭ
	//TransitionRotoZoom  ��ת
	//TransitionTurnOffTiles С����
	//TransitionProgressRadialCCW  ˳ʱ����ת
}

// ���ذ�ť(��ת����ɭð��ѡ�ؽ��棩�Ļص�����
void GameOver4::againBtnMenu(Ref *ref){

	auto scene = PassDifficulty::createScene(cocos2d::Value(4));
	//ת����ѡ�ؽ���

	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
	//TransitionFadeBL::create(1, scene)
}