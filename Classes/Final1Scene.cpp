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

	//��ȡ��Ļ�ɼ������С
	visibleSize = Director::getInstance()->getVisibleSize();


	//��ӷ��ذ�ť�������˵���ť
	auto backItem = MenuItemImage::create(
		"m2.png",
		"m02.png",
		CC_CALLBACK_1(Final1::backBtnMenu, this)

		);
	auto backBtn = Menu::create(backItem, NULL);
	backBtn->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 5*2);

	this->addChild(backBtn);

	//�������֡�again��

	auto label0 = TextFieldTTF::create("Return", "fonts/arial.ttf", 30);
	//����������ɫ
	label0->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	label0->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 10*3);
	//
	addChild(label0);


	//����һ�ΰ�ť
	auto againItem = MenuItemImage::create(
		"m1.png",
		"m01.png",
		CC_CALLBACK_1(Final1::againBtnMenu, this)

		);
	auto againBtn = Menu::create(againItem, NULL);
	againBtn->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 5*2);

	this->addChild(againBtn);


	//�������֡�again��

	auto label2 = TextFieldTTF::create("Again", "fonts/arial.ttf", 30);
	//����������ɫ
	label2->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	label2->setPosition(visibleSize.width / 2 + 100, visibleSize.height / 10*3);
	//
	addChild(label2);



	//������߷�
	scoreName1 = TextFieldTTF::create("BestScore:", "fonts/arial.ttf", 48);
	//����������ɫ
	scoreName1->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	scoreName1->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 3 * 2);
	//
	addChild(scoreName1);
   //��
	auto path = FileUtils::getInstance()->getWritablePath();
	path.append("myhero.json");

	//*** ��ȡ json �ļ� ***
	rapidjson::Document readdoc;
	bool bRet = false;
	ssize_t size = 0;
	std::string load_str;

	// getFileData �����ָ������ȡ��Ŀ¼�� Resource �ļ���
	unsigned char* titlech = FileUtils::getInstance()->getFileData(path, "r", &size);
	load_str = std::string((const char*)titlech, size);

	//load_str = cocos2d::FileUtils::getInstance()->getStringFromFile("..\\myhero.json");
	readdoc.Parse<0>(load_str.c_str());



	rapidjson::Value& _json = readdoc["json"];
	const char* ch = _json.GetString();

	rapidjson::Value& _array = readdoc["array"];

	//��ӷ���
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
	//����������ɫ
	score1->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	score1->setPosition(visibleSize.width / 2 +150, visibleSize.height / 3 * 2);
	//
	addChild(score1);

	//���õ�ǰ����
	scoreName1 = TextFieldTTF::create("Score:", "fonts/arial.ttf", 40);
	//����������ɫ
	scoreName1->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	scoreName1->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 2 + 80);
	//
	addChild(scoreName1);

	//��ӷ���
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
	//����������ɫ
	score1->setTextColor(cocos2d::Color4B(255, 245, 247, 255));

	score1->setPosition(visibleSize.width / 2 + 75, visibleSize.height / 2 + 80);
	//
	addChild(score1);





	return true;
}


// ������ʩ��ǿ����
void Final1::backBtnMenu(Ref *ref){

	auto scene = GateMapLayer::createScene();
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
void Final1::againBtnMenu(Ref *ref){

	auto scene = Adventure::createScene(1);
	//ת����ѡ�ؽ���

	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
	//TransitionFadeBL::create(1, scene)
}