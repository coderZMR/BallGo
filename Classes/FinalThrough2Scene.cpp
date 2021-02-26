#include"FinalThrough2Scene.h"
#include"GameOver2Scene.h"
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


Scene * FinalThrough2::createScene(){

	auto scene = Scene::create();
	auto layer1 = LayerColor::create(Color4B(255, 245, 247, 255));
	scene->addChild(layer1);

	auto layer = FinalThrough2::create();

	scene->addChild(layer);


	return scene;

}

void FinalThrough2::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playEffect("Victory.mp3");
}

bool FinalThrough2::init(){

	if (!Layer::init()){
		return false;
	}

	//��ȡ��Ļ�ɼ������С
	visibleSize = Director::getInstance()->getVisibleSize();

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

	//��ӷ��ذ�ť�������˵���ť
	auto backItem = MenuItemImage::create(
		"m21.png",
		"m22.png",
		CC_CALLBACK_1(FinalThrough2::backBtnMenu, this)

		);
	auto backBtn = Menu::create(backItem, NULL);
	backBtn->setPosition(visibleSize.width / 2 - 220, visibleSize.height / 2 - 70);

	this->addChild(backBtn);

	//�������֡�again��

	auto label0 = TextFieldTTF::create("Return", "fonts/arial.ttf", 20);
	//����������ɫ
	label0->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label0->setPosition(visibleSize.width / 2 -220, visibleSize.height / 2 - 150);
	//
	addChild(label0);


	//����һ�ΰ�ť
	auto againItem = MenuItemImage::create(
		"r21.png",
		"r22.png",
		CC_CALLBACK_1(FinalThrough2::againBtnMenu, this)

		);
	auto againBtn = Menu::create(againItem, NULL);
	againBtn->setPosition(visibleSize.width / 2 , visibleSize.height / 2 -70);

	this->addChild(againBtn);


	//�������֡�again��

	auto label2 = TextFieldTTF::create("Again", "fonts/arial.ttf", 20);
	//����������ɫ
	label2->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label2->setPosition(visibleSize.width / 2 , visibleSize.height / 2 - 150);
	//
	addChild(label2);

	//��һ�ذ�ť
	auto nextItem = MenuItemImage::create(
		"m32_n.png",
		"m321.png",
		CC_CALLBACK_1(FinalThrough2::nextBtnMenu, this)

		);
	auto nextBtn = Menu::create(nextItem, NULL);
	nextBtn->setPosition(visibleSize.width / 2 + 220, visibleSize.height / 2 - 70);

	this->addChild(nextBtn);


	//�������֡�next��

	auto label3 = TextFieldTTF::create("Next", "fonts/arial.ttf", 20);
	//����������ɫ
	label3->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label3->setPosition(visibleSize.width / 2 + 220, visibleSize.height / 2 - 150);
	//
	addChild(label3);


	////���һ��������Ϸ��ť

	//auto continueItem = MenuItemImage::create(
	//	"btn2.png",
	//	"btn2.png",
	//	CC_CALLBACK_1(FinalThrough::continueBtnMenu, this)

	//	);
	//auto continueBtn = Menu::create(continueItem, NULL);
	//continueBtn->setPosition(visibleSize.width /4, visibleSize.height /4 );
	//continueBtn->setScale(0.5);
	//this->addChild(continueBtn);


	//��ӡ���ϲͨ�ء�����

	//const std::string name = "��ϲͨ�أ�";
	auto label1 = TextFieldTTF::create("Congratulation!", "fonts/arial.ttf", 40);
	//����������ɫ
	label1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	label1->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 180);
	//
	addChild(label1);

	//������߷�
	scoreName1 = TextFieldTTF::create("BestScore:", "fonts/arial.ttf", 40);
	//����������ɫ
	scoreName1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	scoreName1->setPosition(visibleSize.width / 2 - 35, visibleSize.height / 2 + 110);
	//
	addChild(scoreName1);

	//��ӷ���
	int a;
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("c2"))
			{
				a = arraydoc["c2"].GetInt();
			}
		}
	}
	const std::string mark = StringUtils::format("%d", a);

	score1 = TextFieldTTF::create(mark, "fonts/arial.ttf", 40);
	//����������ɫ
	score1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	score1->setPosition(visibleSize.width / 2 + 130, visibleSize.height / 2 + 110);
	//
	addChild(score1);

	//���õ�ǰ����
	scoreName1 = TextFieldTTF::create("Score:", "fonts/arial.ttf", 30);
	//����������ɫ
	scoreName1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	scoreName1->setPosition(visibleSize.width / 2 - 50, visibleSize.height / 2 + 50);
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

	score1 = TextFieldTTF::create(mark1, "fonts/arial.ttf", 30);
	//����������ɫ
	score1->setTextColor(cocos2d::Color4B(119, 119, 119, 255));

	score1->setPosition(visibleSize.width / 2 + 75, visibleSize.height / 2 + 50);
	//
	addChild(score1);




	return true;
}


// ������ʩ��ǿ����
void FinalThrough2::backBtnMenu(Ref *ref){

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
void FinalThrough2::againBtnMenu(Ref *ref){

	auto scene = PassDifficulty::createScene(cocos2d::Value(2));
	//ת����ѡ�ؽ���

	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
	//TransitionFadeBL::create(1, scene)
}


// ��һ�ذ�ť(��ת����һ����Ϸ���棩�Ļص�����
void FinalThrough2::nextBtnMenu(Ref *ref){
	auto scene = PassDifficulty::createScene(cocos2d::Value(3));
	//ת������һ�ؽ���

	Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));
}