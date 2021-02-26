#include"ListScene.h"
#include"MainMenuScene.h"
#include"SimpleAudioEngine.h"

#include "CCStdC.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "SimpleAudioEngine.h"

using namespace  rapidjson;

USING_NS_CC;
using namespace CocosDenshion;

//构造函数
List::List(){
	

}

//析构函数
List::~List(){
	this->unscheduleUpdate();
	
}

//创建场景函数
Scene* List::createScene(){
	
	auto scene = Scene::create();
	
	auto layer = List::create();
	
	scene->addChild(layer);

	return scene;

}

void List::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3", true); //播放背景音乐
}

//初始化函数
bool List::init(){
	
	if (!LayerColor::initWithColor(Color4B(200,200,200, 255)))
	{
		return false;
	}

	//可见区域大小初始化
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

	//初始化排行榜名字
	listName="easyList.png";

	//初始化冒险模式其他label的Text
	AllOfBraveListName[0] = "No:";
	AllOfBraveListName[1] = "Score:";
	AllOfBraveListName[2] = "No1";
	AllOfBraveListName[3] = "No2";
	AllOfBraveListName[4] = "No3";
	AllOfBraveListName[5] = "No4";
	AllOfBraveListName[6] = "No5";

	//初始化闯关模式其他Label的Text
	AllOfPassListName[0] = "Pass:";
	AllOfPassListName[1] = "MaxScore:";
	AllOfPassListName[2] = "pass1";
	AllOfPassListName[3] = "pass2";
	AllOfPassListName[4] = "pass3";
	AllOfPassListName[5] = "pass4";
	AllOfPassListName[6] = "pass5";

	//插入背景图片
	auto bg = Sprite::create("bg2.png");
	bg->setPosition(visibleSize / 2);
	bg->setScale(1.2);
	addChild(bg);
	
	//在这里更新分数数据update Score data 
	if (_array.IsArray())
	{
		for (int i = 0; i < _array.Capacity(); i++)
		{
			rapidjson::Value& arraydoc = _array[i];
			if (arraydoc.HasMember("m11"))
			{
				mark[0] = arraydoc["m11"].GetInt();
				log("mark0%d",mark[0]);
			}
			if (arraydoc.HasMember("m12"))
			{
				mark[1] = arraydoc["m12"].GetInt();
				
			}
			if (arraydoc.HasMember("m13"))
			{
				mark[2] = arraydoc["m13"].GetInt();
			}
			if (arraydoc.HasMember("m14"))
			{
				mark[3] = arraydoc["m14"].GetInt();
			}
			if (arraydoc.HasMember("m15"))
			{
				mark[4] = arraydoc["m15"].GetInt();
			}

			if (arraydoc.HasMember("m21"))
			{
				mark[5] = arraydoc["m21"].GetInt();
			}
			if (arraydoc.HasMember("m22"))
			{
				mark[6] = arraydoc["m22"].GetInt();
			}
			if (arraydoc.HasMember("m23"))
			{
				mark[7] = arraydoc["m23"].GetInt();
			}
			if (arraydoc.HasMember("m24"))
			{
				mark[8] = arraydoc["m24"].GetInt();
			}
			if (arraydoc.HasMember("m25"))
			{
				mark[9] = arraydoc["m25"].GetInt();
			}

			if (arraydoc.HasMember("m31"))
			{
				mark[10] = arraydoc["m31"].GetInt();
			}
			if (arraydoc.HasMember("m32"))
			{
				mark[11] = arraydoc["m32"].GetInt();
			}
			if (arraydoc.HasMember("m33"))
			{
				mark[12] = arraydoc["m33"].GetInt();
			}
			if (arraydoc.HasMember("m34"))
			{
				mark[13] = arraydoc["m34"].GetInt();
			}
			if (arraydoc.HasMember("m35"))
			{
				mark[14] = arraydoc["m35"].GetInt();
			}

			if (arraydoc.HasMember("c1"))
			{
				mark[15] = arraydoc["c1"].GetInt();
			}
			if (arraydoc.HasMember("c2"))
			{
				mark[16] = arraydoc["c2"].GetInt();
			}
			if (arraydoc.HasMember("c3"))
			{
				mark[17] = arraydoc["c3"].GetInt();
			}
			if (arraydoc.HasMember("c4"))
			{
				mark[18] = arraydoc["c4"].GetInt();
			}
			if (arraydoc.HasMember("c5"))
			{
				mark[19] = arraydoc["c5"].GetInt();
			}

		}
	}
	//初始化排行榜界面
	//初始化排行榜标号
	tag = 1;
	addChildList(AllOfBraveListName);

	//左按钮
	auto leftBtn1 = MenuItemImage::create(
		"leftTop.png",
		"leftButton.png",
		CC_CALLBACK_1(List::leftBtn, this)

		);
	auto leftItem = Menu::create(leftBtn1, NULL);
	leftItem->setPosition(100, visibleSize.height / 2);
	addChild(leftItem);

	//右按钮
	auto rightBtn1 = MenuItemImage::create(
		"rightTop.png",
		"rightButton.png",
		CC_CALLBACK_1(List::rightBtn, this)

		);
	auto rightItem = Menu::create(rightBtn1, NULL);
	rightItem->setPosition(visibleSize.width - 100, visibleSize.height / 2);
	addChild(rightItem);

	//返回按钮
	back = Label::createWithTTF("Back", "fonts/arial.ttf", 30);
	back->setPosition(visibleSize.width / 16, visibleSize.height / 16 * 15);
	back->setColor(Color3B(255,255,255));
	back->setContentSize(Size(300,150));
	addChild(back);

	//添加监听器
	auto listener = EventListenerTouchOneByOne::create();	
	auto btnOnTouchBegan = [&](Touch *t, Event *e){
		if (e->getCurrentTarget()->getBoundingBox().containsPoint(t->getLocation()))
		{
			back->setScale(1.5);
			scheduleOnce(schedule_selector(List::turnBigBackBtn), 0.1);
			scheduleOnce(schedule_selector(List::goToMainMenuScene), 0.3);
			

			return true;
		}
		return false;
	};
	listener->onTouchBegan = btnOnTouchBegan;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, back);


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

//移除左按钮的函数
void List::removeLeft(float dt){
	removeChild(floorLeft,true);
	
}

//移除右按钮的函数
void List::removeRight(float dt){
	removeChild(floorRight,true);
}

//返回按钮放大函数
void List::turnBigBackBtn(float dt){
	back->setScale(1);
}

//转场到主菜单界面
void List::goToMainMenuScene(float dt){
	auto scene = MainMenu::createScene();
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("turnSceneMusic.mp3");
	
	Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(1, scene));
}

//左按钮动作回调函数
void List::leftBtn(Ref *ref){
	if (tag == 1){
				
		removeChildList();
		tag = 4;
		listName = "passList.png";
		addChildList( AllOfPassListName);

	}
	else if (tag == 2)
	{		
		
		removeChildList();
		tag = 1;
		listName = "easyList.png";
		addChildList( AllOfBraveListName);

	}
	else if (tag == 3)
	{		
		
		removeChildList();
		tag = 2;
		listName = "difficultList.png";
		addChildList( AllOfBraveListName);

	}
	else if (tag == 4)
	{		
		
		removeChildList();
		tag = 3;
		listName = "btList.png";
		addChildList( AllOfBraveListName);
	}

}

//右按钮动作回调函数
void List::rightBtn(Ref *ref){
	
	if (tag == 1){
		
		removeChildList();
		tag = 2;
		listName = "difficultList.png";
		addChildList(AllOfBraveListName);

	}
	else if (tag == 2)
	{
		
		
		removeChildList();
		tag = 3;
		listName = "btList.png";
		addChildList(AllOfBraveListName);

	}
	else if (tag == 3)
	{
				
		removeChildList();
		tag = 4;
		listName = "passList.png";
		addChildList( AllOfPassListName);

	}
	else if (tag == 4)
	{		
		removeChildList();
		tag = 1;
		listName = "easyList.png";
		addChildList( AllOfBraveListName);

	}
}

//移除排行榜层函数
void List::removeChildList(){
	removeChild(list[tag-1], true);
	removeChild(board[tag - 1], true);
	removeChild(NoAndScore[0], true);
	removeChild(NoAndScore[1], true);
	removeChild(NoAndScore[2], true);
	removeChild(NoAndScore[3], true);
	removeChild(NoAndScore[4], true);
	removeChild(NoAndScore[5], true);
	removeChild(NoAndScore[6], true);
	removeChild(NoAndScore[7], true);
	removeChild(NoAndScore[8], true);
	removeChild(NoAndScore[9], true);
	removeChild(NoAndScore[10], true);
	removeChild(NoAndScore[11], true);

}

//添加排行榜层函数
void List::addChildList( const std::string AllOfListName[]){

	board[tag-1] = LayerColor::create(Color4B(0, 0, 0, 70), visibleSize.width / 3, visibleSize.height / 1.2);
	board[tag - 1]->setPosition(visibleSize.width / 3, visibleSize.height / 6);
	addChild(board[tag - 1]);

	list[tag - 1] = Sprite::create(listName);
	list[tag - 1]->setPosition(visibleSize.width / 2, visibleSize.height / 16 * 12);
	list[tag - 1]->setScale(2);
	addChild(list[tag - 1]);

	NoAndScore[0] = cocos2d::Label::createWithTTF(AllOfListName[0], "fonts/arial.ttf", 48);
	NoAndScore[0]->setPosition(visibleSize.width / 2 - 120, visibleSize.height / 8 * 5);
	addChild(NoAndScore[0]);

	NoAndScore[1] = cocos2d::Label::createWithTTF(AllOfListName[1], "fonts/arial.ttf", 48);
	NoAndScore[1]->setPosition(visibleSize.width / 2 + 80, visibleSize.height / 8 * 5);
	addChild(NoAndScore[1]);


	NoAndScore[2] = Label::createWithTTF(AllOfListName[2], "fonts/arial.ttf", 30);
	NoAndScore[2]->setPosition(visibleSize.width / 2 - 120, visibleSize.height / 16 * 9);
	addChild(NoAndScore[2]);

	NoAndScore[3] = Label::createWithTTF(AllOfListName[3], "fonts/arial.ttf", 30);
	NoAndScore[3]->setPosition(visibleSize.width / 2 - 120, visibleSize.height / 16 * 8);
	addChild(NoAndScore[3]);

	NoAndScore[4] = Label::createWithTTF(AllOfListName[4], "fonts/arial.ttf", 30);
	NoAndScore[4]->setPosition(visibleSize.width / 2 - 120, visibleSize.height / 16 * 7);
	addChild(NoAndScore[4]);

	NoAndScore[5] = Label::createWithTTF(AllOfListName[5], "fonts/arial.ttf", 30);
	NoAndScore[5]->setPosition(visibleSize.width / 2 - 120, visibleSize.height / 16 * 6);
	addChild(NoAndScore[5]);

	NoAndScore[6] = Label::createWithTTF(AllOfListName[6], "fonts/arial.ttf", 30);
	NoAndScore[6]->setPosition(visibleSize.width / 2 - 120, visibleSize.height / 16 * 5);
	addChild(NoAndScore[6]);

	NoAndScore[7] = Label::createWithTTF(StringUtils::format("%d", mark[5*tag-5]), "fonts/arial.ttf", 30);
	NoAndScore[7]->setPosition(visibleSize.width / 2 + 80, visibleSize.height / 16 * 9);
	addChild(NoAndScore[7]);

	NoAndScore[8] = Label::createWithTTF(StringUtils::format("%d", mark[5*tag-4]), "fonts/arial.ttf", 30);
	NoAndScore[8]->setPosition(visibleSize.width / 2 + 80, visibleSize.height / 16 * 8);
	addChild(NoAndScore[8]);

	NoAndScore[9] = Label::createWithTTF(StringUtils::format("%d", mark[5*tag-3]), "fonts/arial.ttf", 30);
	NoAndScore[9]->setPosition(visibleSize.width / 2 + 80, visibleSize.height / 16 * 7);
	addChild(NoAndScore[9]);

	NoAndScore[10] = Label::createWithTTF(StringUtils::format("%d", mark[5*tag-2]), "fonts/arial.ttf", 30);
	NoAndScore[10]->setPosition(visibleSize.width / 2 + 80, visibleSize.height / 16 * 6);
	addChild(NoAndScore[10]);

	NoAndScore[11] = Label::createWithTTF(StringUtils::format("%d", mark[5*tag-1]), "fonts/arial.ttf", 30);
	NoAndScore[11]->setPosition(visibleSize.width / 2 + 80, visibleSize.height / 16 * 5);
	addChild(NoAndScore[11]);

}

