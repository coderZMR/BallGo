#include"GameOver5Scene.h"
#include"FinalThrough5Scene.h"
#include"SimpleAudioEngine.h"
#include"cocos2d.h"
#include"MainMenuScene.h"
#include"ChuangGuan.h"
#include "PassDifficultyScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;


Scene * GameOver5::createScene(){

	auto scene = Scene::create();
	auto layer1 = LayerColor::create(Color4B(255, 245, 247, 255));
	scene->addChild(layer1);

	auto layer = GameOver5::create();

	scene->addChild(layer);


	return scene;

}


void GameOver5::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playEffect("Fail.mp3");
}

bool GameOver5::init(){

	if (!Layer::init()){
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();

	//主菜单按钮
	auto backItem = MenuItemImage::create(
		"m51.png",
		"m52.png",
		CC_CALLBACK_1(GameOver5::backBtnMenu, this)

		);
	auto backBtn = Menu::create(backItem, NULL);
	backBtn->setPosition(visibleSize.width / 2 - 110, visibleSize.height / 2 - 50);

	this->addChild(backBtn);

	//设置文字“again”

	auto label0 = TextFieldTTF::create("Return", "fonts/arial.ttf", 20);
	//设置字体颜色
	label0->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label0->setPosition(visibleSize.width / 2 - 110, visibleSize.height / 2 - 130);
	//
	addChild(label0);


	//再玩一次按钮
	auto againItem = MenuItemImage::create(
		"r51.png",
		"r52.png",
		CC_CALLBACK_1(GameOver5::againBtnMenu, this)

		);
	auto againBtn = Menu::create(againItem, NULL);
	againBtn->setPosition(visibleSize.width / 2 + 110, visibleSize.height / 2 - 50);

	this->addChild(againBtn);


	//设置文字“again”

	auto label2 = TextFieldTTF::create("Again", "fonts/arial.ttf", 20);
	//设置字体颜色
	label2->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label2->setPosition(visibleSize.width / 2 + 110, visibleSize.height / 2 - 130);
	//
	addChild(label2);

	//添加“Game Over!”文字
	auto label = TextFieldTTF::create("Game Over!", "fonts/arial.ttf", 40);
	//设置字体颜色
	label->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 150);
	//
	addChild(label);


	scoreName = TextFieldTTF::create("Score:", "fonts/arial.ttf", 30);
	//设置字体颜色
	scoreName->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	scoreName->setPosition(visibleSize.width / 2 - 40, visibleSize.height / 2 + 80);
	//
	addChild(scoreName);

	//添加分数
	const std::string mark = StringUtils::format("%d",0);
	score = TextFieldTTF::create(mark, "fonts/arial.ttf", 30);
	//设置字体颜色
	score->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	score->setPosition(visibleSize.width / 2 + 65, visibleSize.height / 2 + 80);
	//
	addChild(score);



	return true;
}

//返回按钮（主菜单按钮）的回调函数
void GameOver5::backBtnMenu(Ref *ref){


	auto scene = ChuangGuan::createScene();
	//转场到主菜单界面
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
	//TransitionFadeBL::create(1, scene)

	//备选转场动画
	//TransitionFadeBL  淡出瓦片
	//TransitionRotoZoom  旋转
	//TransitionTurnOffTiles 小方块
	//TransitionProgressRadialCCW  顺时针旋转
}


void GameOver5::againBtnMenu(Ref *ref){

	auto scene = PassDifficulty::createScene(cocos2d::Value(5));

	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
	//TransitionFadeBL::create(1, scene)
}