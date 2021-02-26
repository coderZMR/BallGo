#include "Final1Scene.h"
#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"
#include "GateMapLayer.h"
#include "AdventureScene.h"

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


Scene * Final1::createScene(){

	auto scene = Scene::create();
	auto layer1 = LayerColor::create(Color4B(119, 119, 119, 255));
	scene->addChild(layer1);

	auto layer = Final1::create();

	scene->addChild(layer);


	return scene;

}

void Final1::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playEffect("Victory.mp3");
}

bool Final1::init(){

	if (!Layer::init()){
		return false;
	}

	//获取屏幕可见区域大小
	visibleSize = Director::getInstance()->getVisibleSize();


	//添加返回按钮，即主菜单按钮
	auto backItem = MenuItemImage::create(
		"m2.png",
		"m02.png",
		CC_CALLBACK_1(Final1::backBtnMenu, this)

		);
	auto backBtn = Menu::create(backItem, NULL);
	backBtn->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 5*2);

	this->addChild(backBtn);

	//设置文字“again”

	auto label0 = TextFieldTTF::create("Return", "fonts/arial.ttf", 30);
	//设置字体颜色
	label0->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	label0->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 10*3);
	//
	addChild(label0);


	//再玩一次按钮
	auto againItem = MenuItemImage::create(
		"m1.png",
		"m01.png",
		CC_CALLBACK_1(Final1::againBtnMenu, this)

		);
	auto againBtn = Menu::create(againItem, NULL);
	againBtn->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 5*2);

	this->addChild(againBtn);


	//设置文字“again”

	auto label2 = TextFieldTTF::create("Again", "fonts/arial.ttf", 30);
	//设置字体颜色
	label2->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	label2->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 10*3);
	//
	addChild(label2);



	//设置最高分
	scoreName1 = TextFieldTTF::create("BestScore:", "fonts/arial.ttf", 48);
	//设置字体颜色
	scoreName1->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	scoreName1->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 3 * 2);
	//
	addChild(scoreName1);
   //读
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

	//添加分数
	int a;
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("m11"))
			{
				a = arraydoc["m11"].GetInt();
			}
		}
	}
	const std::string mark = StringUtils::format("%d", a);

	score1 = TextFieldTTF::create(mark, "fonts/arial.ttf", 48);
	//设置字体颜色
	score1->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	score1->setPosition(visibleSize.width / 2 +150, visibleSize.height / 3 * 2);
	//
	addChild(score1);

	//设置当前分数
	scoreName1 = TextFieldTTF::create("Score:", "fonts/arial.ttf", 40);
	//设置字体颜色
	scoreName1->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	scoreName1->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 2 + 80);
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

	score1 = TextFieldTTF::create(mark1, "fonts/arial.ttf", 40);
	//设置字体颜色
	score1->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	score1->setPosition(visibleSize.width / 2 + 75, visibleSize.height / 2 + 80);
	//
	addChild(score1);





	return true;
}


// 返回至施文强界面
void Final1::backBtnMenu(Ref *ref){

	auto scene = GateMapLayer::createScene();
	//转场到主菜单界面
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));

	//TransitionFadeBL::create(1, scene)
	//备选转场动画
	//TransitionFadeBL  淡出瓦片
	//TransitionRotoZoom  旋转
	//TransitionTurnOffTiles 小方块
	//TransitionProgressRadialCCW  顺时针旋转
}

// 返回按钮(跳转到王森冒险选关界面）的回调函数
void Final1::againBtnMenu(Ref *ref){

	auto scene = Adventure::createScene(1);
	//转场到选关界面

	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
	//TransitionFadeBL::create(1, scene)
}