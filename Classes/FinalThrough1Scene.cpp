#include"FinalThrough1Scene.h"
#include"GameOver1Scene.h"
#include"SimpleAudioEngine.h"
#include"MainMenuScene.h"
#include "ChuangGuan.h"
#include "PassDifficultyScene.h"

#include"cocos2d.h"

#include "CCStdC.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

using namespace  rapidjson;

USING_NS_CC;


Scene * FinalThrough1::createScene(){

	auto scene = Scene::create();
	auto layer1 = LayerColor::create(Color4B(255, 245, 247, 255));
	scene->addChild(layer1);

	auto layer = FinalThrough1::create();

	scene->addChild(layer);


	return scene;
	
}


void FinalThrough1::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playEffect("Victory.mp3");
}

bool FinalThrough1::init(){

	if (!Layer::init()){
		return false;
	}

	//获取屏幕可见区域大小
	visibleSize = Director::getInstance()->getVisibleSize();

	auto path = FileUtils::getInstance()->getWritablePath();
	path.append("myhero.json");

	//*** 读取 json 文件 ***
	rapidjson::Document readdoc;
	bool bRet = false;
	ssize_t size = 0;
	std::string load_str;

	// getFileData 如果不指定，读取根目录是 Resource 文件夹
	unsigned char* titlech = FileUtils::getInstance()->getFileData(path, "r", &size);
	load_str = std::string((const char*)titlech, size);

	//load_str = cocos2d::FileUtils::getInstance()->getStringFromFile("..\\myhero.json");
	readdoc.Parse<0>(load_str.c_str());


	rapidjson::Value& _json = readdoc["json"];
	const char* ch = _json.GetString();

	rapidjson::Value& _array = readdoc["array"];

	//添加返回按钮，即主菜单按钮
	auto backItem = MenuItemImage::create(
		"m11.png",
		"m12.png",
		CC_CALLBACK_1(FinalThrough1::backBtnMenu, this)

		);
	auto backBtn = Menu::create(backItem, NULL);
	backBtn->setPosition(visibleSize.width / 2-220, visibleSize.height / 2 -70);

	this->addChild(backBtn);

	//设置文字“again”

	auto label0 = TextFieldTTF::create("Return", "fonts/arial.ttf", 20);
	//设置字体颜色
	label0->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label0->setPosition(visibleSize.width / 2-220, visibleSize.height / 2-150 );
	//
	addChild(label0);


	//再玩一次按钮
	auto againItem = MenuItemImage::create(
	
		"r11.png",
		"r12.png",
		CC_CALLBACK_1(FinalThrough1::againBtnMenu, this)

		);
	auto againBtn = Menu::create(againItem, NULL);
	againBtn->setPosition(visibleSize.width / 2, visibleSize.height / 2 -70);

	this->addChild(againBtn);


	//设置文字“again”

	auto label2 = TextFieldTTF::create("Again", "fonts/arial.ttf", 20);
	//设置字体颜色
	label2->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label2->setPosition(visibleSize.width / 2 , visibleSize.height / 2 - 150);
	//
	addChild(label2);


	//下一关按钮
	auto nextItem = MenuItemImage::create(
		"m31_n.png",
		"m311.png",
		CC_CALLBACK_1(FinalThrough1::nextBtnMenu, this)

		);
	auto nextBtn = Menu::create(nextItem, NULL);
	nextBtn->setPosition(visibleSize.width / 2 + 220, visibleSize.height / 2 - 70);

	this->addChild(nextBtn);


	//设置文字“next”

	auto label3 = TextFieldTTF::create("Next", "fonts/arial.ttf", 20);
	//设置字体颜色
	label3->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label3->setPosition(visibleSize.width / 2 + 220, visibleSize.height / 2 - 150);
	//
	addChild(label3);


	////添加一个继续游戏按钮

	//auto continueItem = MenuItemImage::create(
	//	"btn2.png",
	//	"btn2.png",
	//	CC_CALLBACK_1(FinalThrough::continueBtnMenu, this)

	//	);
	//auto continueBtn = Menu::create(continueItem, NULL);
	//continueBtn->setPosition(visibleSize.width /4, visibleSize.height /4 );
	//continueBtn->setScale(0.5);
	//this->addChild(continueBtn);


	//添加“恭喜通关”文字

	//const std::string name = "恭喜通关！";
	auto label1 = TextFieldTTF::create("Congratulation!", "fonts/arial.ttf", 40);
	//设置字体颜色
	label1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label1->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 180);
	//
	addChild(label1);

	//设置最高分
	scoreName1 = TextFieldTTF::create("BestScore:", "fonts/arial.ttf", 40);
	//设置字体颜色
	scoreName1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	scoreName1->setPosition(visibleSize.width / 2 - 35, visibleSize.height / 2 + 110);
	//
	addChild(scoreName1);

	//添加分数

	int a;
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("c1"))
			{
				a = arraydoc["c1"].GetInt();
			}
		}
	}
	const std::string mark = StringUtils::format("%d", a);

	score1 = TextFieldTTF::create(mark, "fonts/arial.ttf", 40);
	//设置字体颜色
	score1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	score1->setPosition(visibleSize.width / 2 + 130, visibleSize.height / 2 + 110);
	//
	addChild(score1);

	//设置当前分数
	scoreName1 = TextFieldTTF::create("Score:", "fonts/arial.ttf",30);
	//设置字体颜色
	scoreName1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	scoreName1->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 2 + 50);
	//
	addChild(scoreName1);

	//添加分数
	int a1;
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("temp"))
			{
				a1 = arraydoc["temp"].GetInt();
			}
		}
	}
	const std::string mark1 = StringUtils::format("%d", a1);

	score1 = TextFieldTTF::create(mark1, "fonts/arial.ttf", 30);
	//设置字体颜色
	score1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	score1->setPosition(visibleSize.width / 2+75, visibleSize.height / 2 + 50);
	//
	addChild(score1);




	return true;
}


// 返回至闯关模式选关界面
void FinalThrough1::backBtnMenu(Ref *ref){

	auto scene = ChuangGuan::createScene();
	//转场到闯关模式选关界面
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));

	//TransitionFadeBL::create(1, scene)
	//备选转场动画
	//TransitionFadeBL  淡出瓦片
	//TransitionRotoZoom  旋转
	//TransitionTurnOffTiles 小方块
	//TransitionProgressRadialCCW  顺时针旋转
}

// 重玩按钮(跳转到当前游戏界面）的回调函数
void FinalThrough1::againBtnMenu(Ref *ref){

	auto scene = PassDifficulty::createScene(cocos2d::Value(1));
	//转场到当前游戏界面

	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
	//TransitionFadeBL::create(1, scene)
}

// 下一关按钮(跳转到下一关游戏界面）的回调函数
void FinalThrough1::nextBtnMenu(Ref *ref){
	auto scene = PassDifficulty::createScene(cocos2d::Value(2));
	//转场到下一关界面

	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
}