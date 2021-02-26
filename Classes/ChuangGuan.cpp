
#include "cocos2d.h"
#include "ChuangGuan.h"

#include "CCStdC.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


#include "PassDifficultyScene.h"

#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

using namespace  rapidjson;

USING_NS_CC;
Scene* ChuangGuan::createScene()
{
	auto scene = Scene::create();
	auto layer = ChuangGuan::create();
	scene->addChild(layer);
	return scene;
}
bool ChuangGuan::init()
{
	if (!LayerColor::initWithColor(Color4B(235,236,244,255)))
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//bgp
	auto bg = Sprite::create("bb.jpg");
	//bg->setRotation(90);
	bg->setScale(8);
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(bg);

	//�ؿ�ѡ��ͼƬ
	auto guanQia = Sprite::create("guanqia.png");
	guanQia->setPosition(120, 640);
	this->addChild(guanQia, 1);

	//�����Ч
	Vector<SpriteFrame*> allframe;
	for (int i = 1; i <= 13; i++){

		auto sf = SpriteFrame::create(StringUtils::format("%d.PNG", i), Rect(0, 0, 180, 180));
		allframe.pushBack(sf);
	}
	auto animation = Animation::createWithSpriteFrames(allframe, 0.3);//ÿ��0.3�벥��һ֡
	auto animate = Animate::create(animation);
	auto sp = Sprite::create();
	sp->setPosition(100, 100);
	addChild(sp, 1);
	sp->runAction(RepeatForever::create(animate));

	//��ӷ�ת��ͼƬ
	auto gateOneItem = MenuItemImage::create(
		"gq1.png",
		"gq11.png",
		CC_CALLBACK_1(ChuangGuan::gateOneCall, this)); // �˳�

	auto gateTwoItem = MenuItemImage::create(
		"gq2.png",
		"gq22.png",
		CC_CALLBACK_1(ChuangGuan::gateTwoCall, this)); // �˳�

	auto gateThreeItem = MenuItemImage::create(
		"gq3.png",
		"gq33.png",
		CC_CALLBACK_1(ChuangGuan::gateThreeCall, this)); // �˳�
	auto gateFourItem = MenuItemImage::create(
		"gq4.png",
		"gq44.png",
		CC_CALLBACK_1(ChuangGuan::gateFourCall, this)); // �˳�
	auto gateFiveItem = MenuItemImage::create(
		"gq5.png",
		"gq55.png",
		CC_CALLBACK_1(ChuangGuan::gateFiveCall, this)); // �˳�

	selectGateMenu = SelectGate::create();
	selectGateMenu->addMenuItem(gateOneItem);
	selectGateMenu->addMenuItem(gateTwoItem);
	selectGateMenu->addMenuItem(gateThreeItem);
	selectGateMenu->addMenuItem(gateFourItem);
	selectGateMenu->addMenuItem(gateFiveItem);
	selectGateMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 );
	this->addChild(selectGateMenu, 2);


	auto backButtunlistener = EventListenerKeyboard::create();
	backButtunlistener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event *e){
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE){
			auto scene = MainMenu::createScene();
			Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(1, scene));
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButtunlistener, this);

	return true;
}
void ChuangGuan::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3", true);
}
// a selector callback  
void ChuangGuan::gateOneCall(cocos2d::Ref* pSender)
{

	auto scene = PassDifficulty::createScene(cocos2d::Value(1));
	Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
}
void ChuangGuan::gateTwoCall(cocos2d::Ref* pSender)
{
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

	readdoc.Parse<0>(load_str.c_str());

	rapidjson::Value& _json = readdoc["json"];
	const char* ch = _json.GetString();

	rapidjson::Value& _array = readdoc["array"];
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("c1"))
			{
				int c1 = arraydoc["c1"].GetInt();
				if (c1>0){
				
					auto scene = PassDifficulty::createScene(cocos2d::Value(2));
					Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
				}
				else{
					auto warn = Sprite::create("warn.png");
					warn->setPosition(visibleSize.width / 2, 120);
					warn->runAction(FadeOut::create(1));
					addChild(warn);
				}
			}
		}
	}


}
void ChuangGuan::gateThreeCall(cocos2d::Ref* pSender)
{
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
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("c2"))
			{
				int c2 = arraydoc["c2"].GetInt();
				if (c2>0){

					auto scene = PassDifficulty::createScene(cocos2d::Value(3));
					Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
				}
				else{
					auto warn = Sprite::create("warn.png");
					warn->setPosition(visibleSize.width / 2, 120);
					warn->runAction(FadeOut::create(1));
					addChild(warn);
				}
			}
		}
	}
}
void ChuangGuan::gateFourCall(cocos2d::Ref* pSender)
{
	
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
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("c3"))
			{
				int c3 = arraydoc["c3"].GetInt();
				if (c3>0){
					auto scene = PassDifficulty::createScene(cocos2d::Value(4));
					Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
				}
				else{
					auto warn = Sprite::create("warn.png");
					warn->setPosition(visibleSize.width / 2, 120);
					warn->runAction(FadeOut::create(1));
					addChild(warn);
				}
			}
		}
	}
}
void ChuangGuan::gateFiveCall(cocos2d::Ref* pSender)
{
	
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
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("c4"))
			{
				int c4 = arraydoc["c4"].GetInt();
				if (c4>0){
					auto scene = PassDifficulty::createScene(cocos2d::Value(5));
					Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(1, scene));
				}
				else{
					auto warn = Sprite::create("warn.png");
					warn->setPosition(visibleSize.width / 2, 120);
					warn->runAction(FadeOut::create(1));
					addChild(warn);
				}
			}
		}
	}
}

