// Tag	Ball body0 shape6		Barrier 静3	  惩罚球主球4 次球7	  边界 2	加分项 5	第三张地图中心方框区域边界8 9 10 11 第三张地图所有边界body 1 Node 1 第五张地图移动的边 12~24 body 12
// 第一关 障碍物被碰撞后会一直显示，只需收集两颗星星
// 第二关 障碍物被碰撞后会一直显示被碰撞的一部分，需收集四颗星星，此关中间部分有收集到3颗星星便自动消失的障碍物
// 第三关 障碍物会有规律的显示并且碰撞后会显示被碰撞的部分，需收集五颗星星
// 第四关 被碰撞的障碍物的部分在碰撞后会显示一段时间，之后消失，需收集五颗星星，此关加入随机传送的传送门
// 第五关 障碍物被碰撞时才显示，不碰撞后消失，需收集五颗星星，此关加入移动障碍物，传送门定向传送

// 玩法：在指定时间内收集到指定个数星星，碰到红色移动球（次惩罚球）会减剩余时间，碰到主惩罚球不减时间，碰到移动障碍物会减剩余时间，收集到星星会使球体增大
#include "SimpleAudioEngine.h"
#include "PassDifficultyScene.h"
#include "Barrier.h"
#include "Ball.h"
#include "Star.h"
#include "Bar.h"
#include "FinalThrough1Scene.h"
#include "FinalThrough2Scene.h"
#include "FinalThrough3Scene.h"
#include "FinalThrough4Scene.h"
#include "FinalThrough5Scene.h"
#include "GameOver1Scene.h"
#include "GameOver2Scene.h"
#include "GameOver3Scene.h"
#include "GameOver4Scene.h"
#include "GameOver5Scene.h"

#include "CCStdC.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "ChuangGuan.h"

using namespace  rapidjson;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include<jni.h>
#include"platform/android/jni/JniHelper.h"

#endif



// 徐依依
//#include "FinalThrough1Scene.h"
//#include "FinalThrough2Scene.h"
//#include "FinalThrough3Scene.h"
//#include "FinalThrough4Scene.h"
//#include "FinalThrough5Scene.h"
//#include "GameOver1Scene.h"
//#include "GameOver2Scene.h"
//#include "GameOver3Scene.h"
//#include "GameOver4Scene.h"
//#include "GameOver5Scene.h"



USING_NS_CC;
using namespace CocosDenshion;

cocos2d::Camera *PassDifficulty::camera = nullptr;
cocos2d::Scene *PassDifficulty::scene = nullptr;
cocos2d::Size PassDifficulty::visibleSize = cocos2d::Size(0, 0);
cocos2d::Vec2 PassDifficulty::origin = cocos2d::Vec2(0, 0);
cocos2d::Director *PassDifficulty::director = nullptr;
cocos2d::Value PassDifficulty::totalTime = cocos2d::Value(0);
cocos2d::Value PassDifficulty::totalStar = cocos2d::Value(0);
cocos2d::Value PassDifficulty::choice(0);


Scene* PassDifficulty::createScene(cocos2d::Value choice)
{
	srand(time(NULL));
	PassDifficulty::choice = choice;
	switch (choice.asInt())
	{
	case 1:
		PassDifficulty::totalTime = 300;
		PassDifficulty::totalStar = 2;
		break;
	case 2:
		PassDifficulty::totalTime = 200;
		PassDifficulty::totalStar = 4;
		break;
	case 3:
		PassDifficulty::totalTime = 200;
		PassDifficulty::totalStar = 5;
		break;
	case 4:
		PassDifficulty::totalTime = 250;
		PassDifficulty::totalStar = 5;
		break;
	case 5:
		PassDifficulty::totalTime = 300;
		PassDifficulty::totalStar = 5;
		break;
	default:
		break;
	}
    // 'scene' is an autorelease object
    scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setAutoStep(false);
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	PassDifficulty::camera = scene->getDefaultCamera();
    // 'layer' is an autorelease object
    auto layer = PassDifficulty::create();
	// 创建边界
	auto layerPhysicsBody = PhysicsBody::createEdgeBox(cocos2d::Size(visibleSize.width * 3, visibleSize.height * 3), PhysicsMaterial(0, 1, 0), 15);
	layerPhysicsBody->setDynamic(false);
	layerPhysicsBody->setContactTestBitmask(0x00000001);
	layerPhysicsBody->setTag(2);
	layer->setPhysicsBody(layerPhysicsBody);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PassDifficulty::init()
{
	if (!LayerColor::initWithColor(cocos2d::Color4B(cocos2d::Color3B(236, 236, 244))))
	{
		return false;
	}
	allSixCornerStartSprite.clear();
	movePunishBall.clear();
	contactPunishBall.clear();
	punishMainBall.clear();
	ifFirst = true;
	point = nullptr;
	lastHundredTimeChange = nullptr;
	getSixCornerStarCount = 0;
	ifGetDoor = false;
	ifFirstHidden = true;
	voice = true;
	ifTouchBack = false;
	signForMusic = true;
	signForVoice = true;

	auto fade1 = FadeOut::create(0.5f);
	auto fade_in1 = fade1->reverse();
	auto seq1 = Sequence::create(fade1, fade_in1, nullptr);
	auto repeat1 = RepeatForever::create(seq1);

	showLastTime = cocos2d::Label::createWithTTF(PassDifficulty::totalTime.asString(),
		"fonts/MarkerFelt.ttf",
		50);
	showLastTime->setTextColor(Color4B(Color3B::GREEN));
	showLastTime->runAction(repeat1);
	addChild(showLastTime);

	auto fade2 = FadeOut::create(0.5f);
	auto fade_in2 = fade2->reverse();
	auto seq2 = Sequence::create(fade2, fade_in2, nullptr);
	auto repeat2 = RepeatForever::create(seq2);

	point = cocos2d::Label::createWithTTF(".",
		"fonts/MarkerFelt.ttf",
		50);
	point->setTextColor(Color4B(Color3B::RED));
	point->runAction(repeat2);
	addChild(point);
	point->setVisible(false);

	auto fade3 = FadeOut::create(0.5f);
	auto fade_in3 = fade3->reverse();
	auto seq3 = Sequence::create(fade3, fade_in3, nullptr);
	auto repeat3 = RepeatForever::create(seq3);

	lastHundredTimeChange = cocos2d::Label::createWithTTF("99",
		"fonts/MarkerFelt.ttf",
		50);
	lastHundredTimeChange->setTextColor(Color4B(Color3B::RED));
	lastHundredTimeChange->runAction(repeat3);
	addChild(lastHundredTimeChange);
	lastHundredTimeChange->setVisible(false);

	showNeedStar = cocos2d::Label::createWithTTF(PassDifficulty::totalStar.asString(),
		"fonts/MarkerFelt.ttf",
		50);
	showNeedStar->setTextColor(cocos2d::Color4B(cocos2d::Color3B::RED));
	addChild(showNeedStar);

	currentUsedTime = cocos2d::Value(0);
	starVector.clear();
	ifGet = false;
	PassDifficulty::visibleSize = Director::getInstance()->getVisibleSize();
	PassDifficulty::origin = Director::getInstance()->getVisibleOrigin();
	director = Director::getInstance();

	this->setContentSize(cocos2d::Size(visibleSize.width * 3, visibleSize.height * 3));

	mapWidth = visibleSize.width * 3;
	mapHeight = visibleSize.height * 3;

	// 创建边框
	createRecGraphy();
	// 创建游戏区域外边框
	createBoundary();
	// 创建底纹
	createbackground();
	// 设置障碍物
	createMap();
	// 设置小球
	makeMainBall();
	// 设置碰撞监听
	solveContactEvent();
	// 设置加速度传感器监听
	setListener();		
	// 解决刚体穿透问题方案并设置相机位置
	scheduleUpdate();
	// 启动计算剩余时间定时器
	schedule(schedule_selector(PassDifficulty::timeForCalculateLastTime), 1);
	// 启动产生惩罚球计时器
	schedule(schedule_selector(PassDifficulty::timeHandlerForAddPunishBall), 3);
	// 对边框控制
	schedule(schedule_selector(PassDifficulty::timeHandlerForRectDrawNode), 1);

    return true;
}

void PassDifficulty::createBoundary()
{
	auto drawNode1 = cocos2d::DrawNode::create();
	drawNode1->drawSegment(Vec2(-350, -300), Vec2(-350, visibleSize.height * 3 + 300), 350, cocos2d::Color4F(cocos2d::Color3B(130, 57, 53)));
	addChild(drawNode1);

	auto drawNode2 = cocos2d::DrawNode::create();
	drawNode2->drawSegment(Vec2(visibleSize.width * 3 + 350, -300), Vec2(visibleSize.width * 3 + 350, visibleSize.height * 3 + 300), 350, cocos2d::Color4F(cocos2d::Color3B(130, 57, 53)));
	addChild(drawNode2);

	auto drawNode3 = cocos2d::DrawNode::create();
	drawNode3->drawSegment(Vec2(-300, -300), Vec2(visibleSize.width * 3 + 300, -300), 300, cocos2d::Color4F(cocos2d::Color3B(130, 57, 53)));
	addChild(drawNode3);

	auto drawNode4 = cocos2d::DrawNode::create();
	drawNode4->drawSegment(Vec2(-300, visibleSize.height * 3 + 300), Vec2(visibleSize.width * 3 + 300, visibleSize.height * 3 + 300), 300, cocos2d::Color4F(cocos2d::Color3B(130, 57, 53)));
	addChild(drawNode4);
}

void PassDifficulty::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BackGroundMusicForPassDifficultyScene.mp3", true); //播放背景音乐
}


void PassDifficulty::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void PassDifficulty::createMap()
{
	switch (PassDifficulty::choice.asInt())
	{
	case 1:
		makeFirstMap();
		break;
	case 2:
		makeSecondMap();
		break;
	case 3:
		makeThirdMap();
		break;
	case 4:
		makeFourthMap();
		break;
	case 5:
		makeFifthMap();
		break;
	default:
		break;
	}
}

void PassDifficulty::makeMainBall()
{
	ball = Ball::create(15, Color3B(0, 0, 0));
	ball->setPosition(visibleSize.width / 5, visibleSize.height / 5);
	addChild(ball);
}

void PassDifficulty::createRecGraphy()
{
	drawNode = DrawNode::create(10);
	drawNode->drawRect(cocos2d::Vec2(0, 0),
		cocos2d::Vec2(director->getVisibleSize().width * 3 - 10, director->getVisibleSize().height * 3 - 10),
		Color4F(Color3B::RED));
	drawNode->setAnchorPoint(cocos2d::Vec2(0, 0));
	drawNode->setPosition(0, 0);
	drawNode->setVisible(false);
	addChild(drawNode);
}

void PassDifficulty::createbackground()
{
	for (int i = 1; i <= 24; ++i) {
		auto drawNodeForBackGround = DrawNode::create();
		drawNodeForBackGround->drawLine(cocos2d::Vec2(mapWidth.asFloat() * i / 25, 0), cocos2d::Vec2(mapWidth.asFloat() * i / 25, mapHeight.asFloat()), cocos2d::Color4F(cocos2d::Color3B(199, 199, 199)));
		addChild(drawNodeForBackGround);
	}

	for (int i = 1; i <= 32; ++i) {
		auto drawNodeForBackGround = DrawNode::create();
		drawNodeForBackGround->drawLine(cocos2d::Vec2(0, mapHeight.asFloat() * i / 33), cocos2d::Vec2(mapWidth.asFloat(), mapHeight.asFloat() * i / 33), cocos2d::Color4F(cocos2d::Color3B(199, 199, 199)));
		addChild(drawNodeForBackGround);
	}
}

void PassDifficulty::setListener()
{
	auto accelerationListener = EventListenerAcceleration::create([&](cocos2d::Acceleration *a, Event *e){
		cocos2d::Value changeX(a->x * 1000);
		cocos2d::Value changeY(a->y * 1000);
		cocos2d::Value ballVelocityX(ball->getVelocity().x);
		cocos2d::Value ballVelocityY(ball->getVelocity().y);
		ball->getPhysicsBody()->setVelocity(cocos2d::Vec2(ballVelocityX.asFloat() + changeX.asFloat(), ballVelocityY.asFloat() + changeY.asFloat()));
	});
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(accelerationListener, ball);
	

	auto backButtunlistener = EventListenerKeyboard::create();
	backButtunlistener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event *e){
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE){		
			if (!ifTouchBack){
				pauseGame();
			}
			else
			{
				Director::getInstance()->getScheduler()->resumeTargets(allTimeHandler);
				Device::setAccelerometerEnabled(true);
				scheduleOnce(schedule_selector(PassDifficulty::timeForDeletepauseLayer), 0);
				ifTouchBack = false;
			}
		}
	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButtunlistener, this);
}

// 返回键使用
void PassDifficulty::pauseGame()
{

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		
		JniMethodInfo methodInfo;
		std::string text = "";
		// 获取UmengGameActivity的静态方法openShareBoard
		bool isHave = JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/JNIConnect", "fun", "()Ljava/lang/String;");
		if (isHave)
		{
			// 实际调用UmengGameActivity中打开umeng分享平台选择面板
			jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
			text = JniHelper::jstring2string(jstr);
		}

		ifTouchBack = true;
		pauseLayer = cocos2d::LayerColor::create(Color4B(0, 0, 0, 128)); //创建一个半透明的颜色层
		pauseLayer->setContentSize(director->getVisibleSize());
		pauseLayer->setPosition(ball->getPosition().x - pauseLayer->getContentSize().width / 2, ball->getPosition().y - pauseLayer->getContentSize().height / 2);
		addChild(pauseLayer);
		allTimeHandler = director->getScheduler()->pauseAllTargets();
		Device::setAccelerometerEnabled(false);

		auto size = pauseLayer->getContentSize();
		// 继续游戏
		auto continueMenu = Menu::create(MenuItemImage::create("continue.png", "scontinue.png",
			[=](Ref* ref){
			Director::getInstance()->getScheduler()->resumeTargets(allTimeHandler);
			Device::setAccelerometerEnabled(true);
			scheduleOnce(schedule_selector(PassDifficulty::timeForDeletepauseLayer), 0);
			ifTouchBack = false;
		}), NULL);
		continueMenu->setPosition(size.width / 2, size.height / 2 + 200);
		pauseLayer->addChild(continueMenu);
		// 重新开始
		auto againMenu = Menu::create(MenuItemImage::create("again.png", "sagain.png",
			[=](Ref* ref){
			Director::getInstance()->getScheduler()->resumeTargets(allTimeHandler);
			Device::setAccelerometerEnabled(true);
			scheduleOnce(schedule_selector(PassDifficulty::timeForDeletepauseLayer), 0);
			auto scene = PassDifficulty::createScene(PassDifficulty::choice);
			director->replaceScene(scene);
		}), NULL);
		againMenu->setPosition(size.width / 2, size.height / 2 + 50);
		pauseLayer->addChild(againMenu);
		// 返回主菜单
		auto returnToMainMenu = Menu::create(MenuItemImage::create("choose.png", "schoose.png",
			[=](Ref* ref){
			Director::getInstance()->getScheduler()->resumeTargets(allTimeHandler);
			Device::setAccelerometerEnabled(true);
			scheduleOnce(schedule_selector(PassDifficulty::timeForDeletepauseLayer), 0);
			// 在此创建并切换到主场景
			auto scene = ChuangGuan::createScene();
			director->replaceScene(TransitionProgressRadialCCW::create(0.5, scene));
		}), NULL);
		returnToMainMenu->setPosition(size.width / 2, size.height / 2 - 100);
		pauseLayer->addChild(returnToMainMenu);
		
		// 停止背景音乐
		auto musicOn = MenuItemImage::create("music.png", "music.png");
		auto musicOff = MenuItemImage::create("musicClose.png", "musicClose.png");
		cocos2d::MenuItemToggle *backGroundMusic = nullptr;
		if (signForMusic.asBool()) backGroundMusic = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(PassDifficulty::callBackForMusic,this), musicOn, musicOff, NULL);
		else
		{
			backGroundMusic = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(PassDifficulty::callBackForMusic,this), musicOff, musicOn, NULL);
		}
		auto bGMMenu = Menu::create(backGroundMusic, NULL);
		bGMMenu->setPosition(size.width / 2 - 200, size.height / 2 - 250);
		pauseLayer->addChild(bGMMenu);

		// 停止音效
		auto voiceOn = MenuItemImage::create("voice.png", "voice.png");
		auto voiceOff = MenuItemImage::create("closeVoice.png", "closeVoice.png");
		cocos2d::MenuItemToggle *voiceCon = nullptr;
		if (signForVoice.asBool()) voiceCon = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(PassDifficulty::callBackForVoice,this), voiceOn, voiceOff, NULL);
		else
		{
			voiceCon = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(PassDifficulty::callBackForVoice,this), voiceOff, voiceOn, NULL);
		}
		auto vCMenu = Menu::create(voiceCon, NULL);
		vCMenu->setPosition(size.width / 2 + 200, size.height / 2 - 250);
		pauseLayer->addChild(vCMenu);

		// 屏幕截图
		auto getScreen = Menu::create(MenuItemImage::create("screenshot.png", "sscreenshot.png",
			[=](Ref* ref){

			pauseLayer->setVisible(false);

			time_t t;
			time(&t);
			char tmp[64];
			strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime((&t)));
			std::string outputFile = text + "/" + std::string("ScreenShot") + std::string(tmp) + std::string(".png");

			utils::captureScreen(CC_CALLBACK_2(PassDifficulty::afterCapture, this), outputFile);

		}), NULL);
		getScreen->setPosition(size.width / 2, size.height / 2 - 250);
		pauseLayer->addChild(getScreen);

		director->getEventDispatcher()->resumeEventListenersForTarget(pauseLayer, true);

	#endif

}

// 关音乐函数回调
void PassDifficulty::callBackForMusic(Ref *ref)
{
	auto mIT = (cocos2d::MenuItemToggle*) ref;
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		signForMusic = false;
	}
	else {
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		signForMusic = true;
	}
}

// 关音效函数回调
void PassDifficulty::callBackForVoice(Ref *ref)
{
	auto mIT = (cocos2d::MenuItemToggle*) ref;
	if (voice) {
		voice = false;
		signForVoice = false;
	}
	else
	{
		voice = true;
		signForVoice = true;
	}
}

void PassDifficulty::afterCapture(bool succed, const std::string& outputFile)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo methodInfo;
	// 获取UmengGameActivity的静态方法openShareBoard
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/JNIConnect", "fun1", "(Ljava/lang/String;)V");
	if (isHave)
	{
		// 实际调用UmengGameActivity中打开umeng分享平台选择面板
		jobject para=methodInfo.env->NewStringUTF(outputFile.c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,para);
		
	}
#endif
	SimpleAudioEngine::getInstance()->playEffect("kacha.mp3", false);
	GrayLayer = cocos2d::LayerColor::create(cocos2d::Color4B(158, 158, 158, 100));
	GrayLayer->setContentSize(director->getVisibleSize() * 3 / 4);
	GrayLayer->setAnchorPoint(Vec2(0.5, 0.5));
	GrayLayer->setPosition(ball->getPosition().x - GrayLayer->getContentSize().width / 2, ball->getPosition().y - GrayLayer->getContentSize().height / 2);
	addChild(GrayLayer);

	st = Sprite::create(outputFile);
	st->setScale(0.7f);
	st->setPosition(GrayLayer->getContentSize() / 2);
	GrayLayer->addChild(st);

	director->getEventDispatcher()->pauseEventListenersForTarget(pauseLayer, true);
	director->getEventDispatcher()->pauseEventListenersForTarget(st, true);

	auto fadeOutForSprite = cocos2d::FadeOut::create(0.5f);
	st->runAction(fadeOutForSprite);

	auto fadeOutForLayer = cocos2d::FadeOut::create(0.5f);
	auto actionSquenceForLayer = cocos2d::Sequence::create(fadeOutForLayer, CallFunc::create(CC_CALLBACK_0(PassDifficulty::timeForDeleteGetScreen, this)), nullptr);
	GrayLayer->runAction(actionSquenceForLayer);
}

void PassDifficulty::timeForDeleteGetScreen()
{
	GrayLayer->removeAllChildren();
	removeChild(GrayLayer, true);
}

void PassDifficulty::timeForDeletepauseLayer(float dt)
{
	pauseLayer->removeAllChildren();
	removeChild(pauseLayer, true);
}

void PassDifficulty::solveContactEvent()
{

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

	auto colloisionListener = EventListenerPhysicsContact::create();
	colloisionListener->onContactBegin = [&](PhysicsContact &contact){

		int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0;

		int m1 = 0, m2 = 0, m3 = 0, m4 = 0, m5 = 0, m6 = 0;
		int m21 = 0, m22 = 0, m23 = 0, m24 = 0, m25 = 0, m26 = 0;
		int m31 = 0, m32 = 0, m33 = 0, m34 = 0, m35 = 0, m36 = 0;
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

		if (_array.IsArray())
		{
			for (int i = 0; i < _array.Capacity(); i++)
			{
				rapidjson::Value& arraydoc = _array[i];
				if (arraydoc.HasMember("c1"))
				{
					c1 = arraydoc["c1"].GetInt();
				}
				if (arraydoc.HasMember("c2"))
				{
					c2 = arraydoc["c2"].GetInt();
				}
				if (arraydoc.HasMember("c3"))
				{
					c3 = arraydoc["c3"].GetInt();
				}
				if (arraydoc.HasMember("c4"))
				{
					c4 = arraydoc["c4"].GetInt();
				}
				if (arraydoc.HasMember("c5"))

				{
					c5 = arraydoc["c5"].GetInt();
				}
				if (arraydoc.HasMember("m11"))
				{
					m1 = arraydoc["m11"].GetInt();
				}
				if (arraydoc.HasMember("m12"))
				{
					m2 = arraydoc["m12"].GetInt();
				}
				if (arraydoc.HasMember("m13"))
				{
					m3 = arraydoc["m13"].GetInt();
				}
				if (arraydoc.HasMember("m14"))
				{
					m4 = arraydoc["m14"].GetInt();
				}
				if (arraydoc.HasMember("m15"))

				{
					m5 = arraydoc["m15"].GetInt();
				}

				if (arraydoc.HasMember("m21"))
				{
					m21 = arraydoc["m21"].GetInt();
				}
				if (arraydoc.HasMember("m22"))
				{
					m22 = arraydoc["m22"].GetInt();
				}
				if (arraydoc.HasMember("m23"))
				{
					m23 = arraydoc["m23"].GetInt();
				}
				if (arraydoc.HasMember("m24"))
				{
					m24 = arraydoc["m24"].GetInt();
				}
				if (arraydoc.HasMember("m25"))

				{
					m25 = arraydoc["m25"].GetInt();
				}
				if (arraydoc.HasMember("m31"))
				{
					m31 = arraydoc["m31"].GetInt();
				}
				if (arraydoc.HasMember("m32"))
				{
					m32 = arraydoc["m32"].GetInt();
				}
				if (arraydoc.HasMember("m33"))
				{
					m33 = arraydoc["m33"].GetInt();
				}
				if (arraydoc.HasMember("m34"))
				{
					m34 = arraydoc["m34"].GetInt();
				}
				if (arraydoc.HasMember("m35"))

				{
					m35 = arraydoc["m35"].GetInt();
				}
				
			}

		}
		auto spriteAPhysicsBody = contact.getShapeA()->getBody();
		auto spriteBPhysicsBody = contact.getShapeB()->getBody();
		// 主球碰到地图边界的效果
		if (spriteAPhysicsBody->getTag() == 2 && spriteBPhysicsBody->getTag() == 0 || spriteBPhysicsBody->getTag() == 2 && spriteAPhysicsBody->getTag() == 0) {
			drawNode->setVisible(true);
			return true;
		}
		if (spriteAPhysicsBody->getTag() == 12 && spriteBPhysicsBody->getTag() == 5 || spriteBPhysicsBody->getTag() == 12 && spriteAPhysicsBody->getTag() == 5) return false;
		
		if (spriteAPhysicsBody->getTag() == 3 && spriteBPhysicsBody->getTag() == 0) {
			auto barrierSprite = (Barrier*)(spriteAPhysicsBody->getNode());
			if (!barrierSprite->getDrawNode()->isVisible()) barrierSprite->getDrawNode()->setVisible(true);
			barrierSprite->setColor(barrierSprite->makeColor());
			barrierSprite->createPolygonGraphy(true);
		}
		if (spriteBPhysicsBody->getTag() == 3 && spriteAPhysicsBody->getTag() == 0) {
			auto barrierSprite = (Barrier*)(spriteBPhysicsBody->getNode());
			if (!barrierSprite->getDrawNode()->isVisible()) barrierSprite->getDrawNode()->setVisible(true);
			barrierSprite->setColor(barrierSprite->makeColor());
			barrierSprite->createPolygonGraphy(true);
		}

		if (spriteAPhysicsBody->getTag() == 4 && spriteBPhysicsBody->getTag() == 5 || spriteBPhysicsBody->getTag() == 4 && spriteAPhysicsBody->getTag() == 5)
			return false;

		if (spriteAPhysicsBody->getTag() == 5 && spriteBPhysicsBody->getTag() == 0) {
			if (voice)SimpleAudioEngine::getInstance()->playEffect("GetStart.mp3", false);
			auto star = (Star*)(spriteAPhysicsBody->getNode());
			auto ball = (Ball*)(spriteBPhysicsBody->getNode());
			if (!star->getifFirstContact()) return false;
			star->setifFirstContact(false);
			this->currentHadStar = this->currentHadStar.asInt() + 1;
			dynamicshowNeedStar(star->getColor());
			if (currentHadStar == PassDifficulty::totalStar)
			{
				 // 在此添加游戏过关代码
				 int score = totalTime.asInt() - currentUsedTime.asInt();


				 if (PassDifficulty::choice.asInt() == 1) {
					 if (score > c1){
						 c1 = score;
					 }
					 rapidjson::Document writedoc;
					 writedoc.SetObject();
					 rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					 rapidjson::Value array(rapidjson::kArrayType);
					 rapidjson::Value object(rapidjson::kObjectType);

					 // json object 格式添加 “名称/值” 对
					 object.AddMember("temp", score, allocator);
					 //闯关分数
					 object.AddMember("c1", c1, allocator);
					 object.AddMember("c2", c2, allocator);
					 object.AddMember("c3", c3, allocator);
					 object.AddMember("c4", c4, allocator);
					 object.AddMember("c5", c5, allocator);
					 //冒险分数
					 object.AddMember("m11", m1, allocator);
					 object.AddMember("m12", m2, allocator);
					 object.AddMember("m13", m3, allocator);
					 object.AddMember("m14", m4, allocator);
					 object.AddMember("m15", m5, allocator);
					 object.AddMember("m16", 0, allocator);

					 object.AddMember("m21", m21, allocator);
					 object.AddMember("m22", m22, allocator);
					 object.AddMember("m23", m23, allocator);
					 object.AddMember("m24", m24, allocator);
					 object.AddMember("m25", m25, allocator);
					 object.AddMember("m26", 0, allocator);

					 object.AddMember("m31", m31, allocator);
					 object.AddMember("m32", m32, allocator);
					 object.AddMember("m33", m33, allocator);
					 object.AddMember("m34", m34, allocator);
					 object.AddMember("m35", m35, allocator);
					 object.AddMember("m36", 0, allocator);

					 // json 加入数组
					 array.PushBack(object, allocator);

					 // json object 格式添加 “名称/值” 对
					 writedoc.AddMember("json", "json string", allocator);
					 writedoc.AddMember("array", array, allocator);

					 StringBuffer buffer;
					 rapidjson::Writer<StringBuffer> writer(buffer);
					 writedoc.Accept(writer);

					 FILE* file = fopen(path.c_str(), "wb");
					 if (file)
					 {
						 log("123456789");
						 fputs(buffer.GetString(), file);
						 fclose(file);
					 }
					 
					 auto firstThrough = FinalThrough1::createScene();
					 director->replaceScene(TransitionFadeBL::create(0.5, firstThrough));
				 }
				 else if (PassDifficulty::choice.asInt() == 2) {
					 
					 if (score > c2){
						 c2 = score;
					 }
					 rapidjson::Document writedoc;
					 writedoc.SetObject();
					 rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					 rapidjson::Value array(rapidjson::kArrayType);
					 rapidjson::Value object(rapidjson::kObjectType);

					 // json object 格式添加 “名称/值” 对
					 object.AddMember("temp", score, allocator);
					 //闯关分数
					 object.AddMember("c1", c1, allocator);
					 object.AddMember("c2", c2, allocator);
					 object.AddMember("c3", c3, allocator);
					 object.AddMember("c4", c4, allocator);
					 object.AddMember("c5", c5, allocator);
					 //冒险分数
					 object.AddMember("m11", m1, allocator);
					 object.AddMember("m12", m2, allocator);
					 object.AddMember("m13", m3, allocator);
					 object.AddMember("m14", m4, allocator);
					 object.AddMember("m15", m5, allocator);
					 object.AddMember("m16", 0, allocator);

					 object.AddMember("m21", m21, allocator);
					 object.AddMember("m22", m22, allocator);
					 object.AddMember("m23", m23, allocator);
					 object.AddMember("m24", m24, allocator);
					 object.AddMember("m25", m25, allocator);
					 object.AddMember("m26", 0, allocator);

					 object.AddMember("m31", m31, allocator);
					 object.AddMember("m32", m32, allocator);
					 object.AddMember("m33", m33, allocator);
					 object.AddMember("m34", m34, allocator);
					 object.AddMember("m35", m35, allocator);
					 object.AddMember("m36", 0, allocator);

					 // json 加入数组
					 array.PushBack(object, allocator);

					 // json object 格式添加 “名称/值” 对
					 writedoc.AddMember("json", "json string", allocator);
					 writedoc.AddMember("array", array, allocator);

					 StringBuffer buffer;
					 rapidjson::Writer<StringBuffer> writer(buffer);
					 writedoc.Accept(writer);

					 FILE* file = fopen(path.c_str(), "wb");
					 if (file)
					 {
						 log("123456789");
						 fputs(buffer.GetString(), file);
						 fclose(file);
					 }

					 auto secondThrough = FinalThrough2::createScene();
					 director->replaceScene(TransitionFadeBL::create(0.5, secondThrough));
				 }
				 else if (PassDifficulty::choice.asInt() == 3) {
					 
					 if (score > c3){
						 c3 = score;
					 }
					 rapidjson::Document writedoc;
					 writedoc.SetObject();
					 rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					 rapidjson::Value array(rapidjson::kArrayType);
					 rapidjson::Value object(rapidjson::kObjectType);

					 // json object 格式添加 “名称/值” 对
					 object.AddMember("temp", score, allocator);
					 //闯关分数
					 object.AddMember("c1", c1, allocator);
					 object.AddMember("c2", c2, allocator);
					 object.AddMember("c3", c3, allocator);
					 object.AddMember("c4", c4, allocator);
					 object.AddMember("c5", c5, allocator);
					 //冒险分数
					 object.AddMember("m11", m1, allocator);
					 object.AddMember("m12", m2, allocator);
					 object.AddMember("m13", m3, allocator);
					 object.AddMember("m14", m4, allocator);
					 object.AddMember("m15", m5, allocator);
					 object.AddMember("m16", 0, allocator);

					 object.AddMember("m21", m21, allocator);
					 object.AddMember("m22", m22, allocator);
					 object.AddMember("m23", m23, allocator);
					 object.AddMember("m24", m24, allocator);
					 object.AddMember("m25", m25, allocator);
					 object.AddMember("m26", 0, allocator);

					 object.AddMember("m31", m31, allocator);
					 object.AddMember("m32", m32, allocator);
					 object.AddMember("m33", m33, allocator);
					 object.AddMember("m34", m34, allocator);
					 object.AddMember("m35", m35, allocator);
					 object.AddMember("m36", 0, allocator);

					 // json 加入数组
					 array.PushBack(object, allocator);

					 // json object 格式添加 “名称/值” 对
					 writedoc.AddMember("json", "json string", allocator);
					 writedoc.AddMember("array", array, allocator);

					 StringBuffer buffer;
					 rapidjson::Writer<StringBuffer> writer(buffer);
					 writedoc.Accept(writer);

					 FILE* file = fopen(path.c_str(), "wb");
					 if (file)
					 {
						 log("123456789");
						 fputs(buffer.GetString(), file);
						 fclose(file);
					 }

					 auto thirdThrough = FinalThrough3::createScene();
					 director->replaceScene(TransitionFadeBL::create(0.5, thirdThrough));
				 }
				 else if (PassDifficulty::choice.asInt() == 4) {

					 if (score > c4){
						 c4 = score;
					 }
					 rapidjson::Document writedoc;
					 writedoc.SetObject();
					 rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					 rapidjson::Value array(rapidjson::kArrayType);
					 rapidjson::Value object(rapidjson::kObjectType);

					 // json object 格式添加 “名称/值” 对
					 object.AddMember("temp", score, allocator);
					 //闯关分数
					 object.AddMember("c1", c1, allocator);
					 object.AddMember("c2", c2, allocator);
					 object.AddMember("c3", c3, allocator);
					 object.AddMember("c4", c4, allocator);
					 object.AddMember("c5", c5, allocator);
					 //冒险分数
					 object.AddMember("m11", m1, allocator);
					 object.AddMember("m12", m2, allocator);
					 object.AddMember("m13", m3, allocator);
					 object.AddMember("m14", m4, allocator);
					 object.AddMember("m15", m5, allocator);
					 object.AddMember("m16", 0, allocator);

					 object.AddMember("m21", m21, allocator);
					 object.AddMember("m22", m22, allocator);
					 object.AddMember("m23", m23, allocator);
					 object.AddMember("m24", m24, allocator);
					 object.AddMember("m25", m25, allocator);
					 object.AddMember("m26", 0, allocator);

					 object.AddMember("m31", m31, allocator);
					 object.AddMember("m32", m32, allocator);
					 object.AddMember("m33", m33, allocator);
					 object.AddMember("m34", m34, allocator);
					 object.AddMember("m35", m35, allocator);
					 object.AddMember("m36", 0, allocator);

					 // json 加入数组
					 array.PushBack(object, allocator);

					 // json object 格式添加 “名称/值” 对
					 writedoc.AddMember("json", "json string", allocator);
					 writedoc.AddMember("array", array, allocator);

					 StringBuffer buffer;
					 rapidjson::Writer<StringBuffer> writer(buffer);
					 writedoc.Accept(writer);

					 FILE* file = fopen(path.c_str(), "wb");
					 if (file)
					 {
						 log("123456789");
						 fputs(buffer.GetString(), file);
						 fclose(file);
					 }


					 auto fourthThrough = FinalThrough4::createScene();
					 director->replaceScene(TransitionFadeBL::create(0.5, fourthThrough));
				 }
				 else if (PassDifficulty::choice.asInt() == 5) {

					 if (score > c5){
						 c5 = score;
					 }
					 rapidjson::Document writedoc;
					 writedoc.SetObject();
					 rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					 rapidjson::Value array(rapidjson::kArrayType);
					 rapidjson::Value object(rapidjson::kObjectType);

					 // json object 格式添加 “名称/值” 对
					 object.AddMember("temp", score, allocator);
					 //闯关分数
					 object.AddMember("c1", c1, allocator);
					 object.AddMember("c2", c2, allocator);
					 object.AddMember("c3", c3, allocator);
					 object.AddMember("c4", c4, allocator);
					 object.AddMember("c5", c5, allocator);
					 //冒险分数
					 object.AddMember("m11", m1, allocator);
					 object.AddMember("m12", m2, allocator);
					 object.AddMember("m13", m3, allocator);
					 object.AddMember("m14", m4, allocator);
					 object.AddMember("m15", m5, allocator);
					 object.AddMember("m16", 0, allocator);

					 object.AddMember("m21", m21, allocator);
					 object.AddMember("m22", m22, allocator);
					 object.AddMember("m23", m23, allocator);
					 object.AddMember("m24", m24, allocator);
					 object.AddMember("m25", m25, allocator);
					 object.AddMember("m26", 0, allocator);

					 object.AddMember("m31", m31, allocator);
					 object.AddMember("m32", m32, allocator);
					 object.AddMember("m33", m33, allocator);
					 object.AddMember("m34", m34, allocator);
					 object.AddMember("m35", m35, allocator);
					 object.AddMember("m36", 0, allocator);

					 // json 加入数组
					 array.PushBack(object, allocator);

					 // json object 格式添加 “名称/值” 对
					 writedoc.AddMember("json", "json string", allocator);
					 writedoc.AddMember("array", array, allocator);

					 StringBuffer buffer;
					 rapidjson::Writer<StringBuffer> writer(buffer);
					 writedoc.Accept(writer);

					 FILE* file = fopen(path.c_str(), "wb");
					 if (file)
					 {
						 log("123456789");
						 fputs(buffer.GetString(), file);
						 fclose(file);
					 }
					 auto fifthThrough = FinalThrough5::createScene();
					 director->replaceScene(TransitionFadeBL::create(0.5, fifthThrough));
				 }
			}
 			auto drawNode = cocos2d::DrawNode::create();
			drawNode->drawSolidCircle(ball->getAnchorPoint(), ball->getMaxRadius().asFloat() * 1.5, 0, ball->getMaxRadius().asFloat(), cocos2d::Color4F(star->getColor()));
			ball->addChild(drawNode, ball->getCurrentZ().asInt() - 1);
			ball->setCurrentZ(cocos2d::Value(ball->getCurrentZ().asInt() - 1));
			scheduleOnce(schedule_selector(PassDifficulty::timeForChangeBall), 1 / 30);
			return false;
		}
		if (spriteBPhysicsBody->getTag() == 5 && spriteAPhysicsBody->getTag() == 0) {
			if (voice) SimpleAudioEngine::getInstance()->playEffect("GetStart.mp3", false);
			auto star = (Star*)(spriteBPhysicsBody->getNode());
			auto ball = (Ball*)(spriteAPhysicsBody->getNode());
			if (!star->getifFirstContact()) return false;
			star->setifFirstContact(false);
			this->currentHadStar = this->currentHadStar.asInt() + 1;
			dynamicshowNeedStar(star->getColor());
			
				
				int score = totalTime.asInt() - currentUsedTime.asInt();

				if (currentHadStar == PassDifficulty::totalStar)
				{
					// 在此添加游戏过关代码
				if (PassDifficulty::choice.asInt() == 1) {

					if (score > c1){
						c1 = score;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object 格式添加 “名称/值” 对
					object.AddMember("temp", score, allocator);
					//闯关分数
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//冒险分数
					object.AddMember("m11", m1, allocator);
					object.AddMember("m12", m2, allocator);
					object.AddMember("m13", m3, allocator);
					object.AddMember("m14", m4, allocator);
					object.AddMember("m15", m5, allocator);
					object.AddMember("m16", 0, allocator);

					object.AddMember("m21", m21, allocator);
					object.AddMember("m22", m22, allocator);
					object.AddMember("m23", m23, allocator);
					object.AddMember("m24", m24, allocator);
					object.AddMember("m25", m25, allocator);
					object.AddMember("m26", 0, allocator);

					object.AddMember("m31", m31, allocator);
					object.AddMember("m32", m32, allocator);
					object.AddMember("m33", m33, allocator);
					object.AddMember("m34", m34, allocator);
					object.AddMember("m35", m35, allocator);
					object.AddMember("m36", 0, allocator);

					// json 加入数组
					array.PushBack(object, allocator);

					// json object 格式添加 “名称/值” 对
					writedoc.AddMember("json", "json string", allocator);
					writedoc.AddMember("array", array, allocator);

					StringBuffer buffer;
					rapidjson::Writer<StringBuffer> writer(buffer);
					writedoc.Accept(writer);

					FILE* file = fopen(path.c_str(), "wb");
					if (file)
					{
						log("123456789");
						fputs(buffer.GetString(), file);
						fclose(file);
					}

					auto firstThrough = FinalThrough1::createScene();
					director->replaceScene(TransitionFadeBL::create(0.5, firstThrough));
				}
				else if (PassDifficulty::choice.asInt() == 2) {

					if (score > c2){
						c2 = score;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object 格式添加 “名称/值” 对
					object.AddMember("temp", score, allocator);
					//闯关分数
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//冒险分数
					object.AddMember("m11", m1, allocator);
					object.AddMember("m12", m2, allocator);
					object.AddMember("m13", m3, allocator);
					object.AddMember("m14", m4, allocator);
					object.AddMember("m15", m5, allocator);
					object.AddMember("m16", 0, allocator);

					object.AddMember("m21", m21, allocator);
					object.AddMember("m22", m22, allocator);
					object.AddMember("m23", m23, allocator);
					object.AddMember("m24", m24, allocator);
					object.AddMember("m25", m25, allocator);
					object.AddMember("m26", 0, allocator);

					object.AddMember("m31", m31, allocator);
					object.AddMember("m32", m32, allocator);
					object.AddMember("m33", m33, allocator);
					object.AddMember("m34", m34, allocator);
					object.AddMember("m35", m35, allocator);
					object.AddMember("m36", 0, allocator);

					// json 加入数组
					array.PushBack(object, allocator);

					// json object 格式添加 “名称/值” 对
					writedoc.AddMember("json", "json string", allocator);
					writedoc.AddMember("array", array, allocator);

					StringBuffer buffer;
					rapidjson::Writer<StringBuffer> writer(buffer);
					writedoc.Accept(writer);

					FILE* file = fopen(path.c_str(), "wb");
					if (file)
					{
						log("123456789");
						fputs(buffer.GetString(), file);
						fclose(file);
					}

					auto secondThrough = FinalThrough2::createScene();
					director->replaceScene(TransitionFadeBL::create(0.5, secondThrough));
				}
				else if (PassDifficulty::choice.asInt() == 3) {

					if (score > c3){
						c3 = score;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object 格式添加 “名称/值” 对
					object.AddMember("temp", score, allocator);
					//闯关分数
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//冒险分数
					object.AddMember("m11", m1, allocator);
					object.AddMember("m12", m2, allocator);
					object.AddMember("m13", m3, allocator);
					object.AddMember("m14", m4, allocator);
					object.AddMember("m15", m5, allocator);
					object.AddMember("m16", 0, allocator);

					object.AddMember("m21", m21, allocator);
					object.AddMember("m22", m22, allocator);
					object.AddMember("m23", m23, allocator);
					object.AddMember("m24", m24, allocator);
					object.AddMember("m25", m25, allocator);
					object.AddMember("m26", 0, allocator);

					object.AddMember("m31", m31, allocator);
					object.AddMember("m32", m32, allocator);
					object.AddMember("m33", m33, allocator);
					object.AddMember("m34", m34, allocator);
					object.AddMember("m35", m35, allocator);
					object.AddMember("m36", 0, allocator);

					// json 加入数组
					array.PushBack(object, allocator);

					// json object 格式添加 “名称/值” 对
					writedoc.AddMember("json", "json string", allocator);
					writedoc.AddMember("array", array, allocator);

					StringBuffer buffer;
					rapidjson::Writer<StringBuffer> writer(buffer);
					writedoc.Accept(writer);

					FILE* file = fopen(path.c_str(), "wb");
					if (file)
					{
						log("123456789");
						fputs(buffer.GetString(), file);
						fclose(file);
					}

					auto thirdThrough = FinalThrough3::createScene();
					director->replaceScene(TransitionFadeBL::create(0.5, thirdThrough));
				}
				else if (PassDifficulty::choice.asInt() == 4) {

					if (score > c4){
						c4 = score;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object 格式添加 “名称/值” 对
					object.AddMember("temp", score, allocator);
					//闯关分数
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//冒险分数
					object.AddMember("m11", m1, allocator);
					object.AddMember("m12", m2, allocator);
					object.AddMember("m13", m3, allocator);
					object.AddMember("m14", m4, allocator);
					object.AddMember("m15", m5, allocator);
					object.AddMember("m16", 0, allocator);

					object.AddMember("m21", m21, allocator);
					object.AddMember("m22", m22, allocator);
					object.AddMember("m23", m23, allocator);
					object.AddMember("m24", m24, allocator);
					object.AddMember("m25", m25, allocator);
					object.AddMember("m26", 0, allocator);

					object.AddMember("m31", m31, allocator);
					object.AddMember("m32", m32, allocator);
					object.AddMember("m33", m33, allocator);
					object.AddMember("m34", m34, allocator);
					object.AddMember("m35", m35, allocator);
					object.AddMember("m36", 0, allocator);

					// json 加入数组
					array.PushBack(object, allocator);

					// json object 格式添加 “名称/值” 对
					writedoc.AddMember("json", "json string", allocator);
					writedoc.AddMember("array", array, allocator);

					StringBuffer buffer;
					rapidjson::Writer<StringBuffer> writer(buffer);
					writedoc.Accept(writer);

					FILE* file = fopen(path.c_str(), "wb");
					if (file)
					{
						log("123456789");
						fputs(buffer.GetString(), file);
						fclose(file);
					}

					auto fourthThrough = FinalThrough4::createScene();
					director->replaceScene(TransitionFadeBL::create(0.5, fourthThrough));
				}
				else if (PassDifficulty::choice.asInt() == 5) {

					if (score > c5){
						c5 = score;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object 格式添加 “名称/值” 对
					object.AddMember("temp", score, allocator);
					//闯关分数
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//冒险分数
					object.AddMember("m11", m1, allocator);
					object.AddMember("m12", m2, allocator);
					object.AddMember("m13", m3, allocator);
					object.AddMember("m14", m4, allocator);
					object.AddMember("m15", m5, allocator);
					object.AddMember("m16", 0, allocator);

					object.AddMember("m21", m21, allocator);
					object.AddMember("m22", m22, allocator);
					object.AddMember("m23", m23, allocator);
					object.AddMember("m24", m24, allocator);
					object.AddMember("m25", m25, allocator);
					object.AddMember("m26", 0, allocator);

					object.AddMember("m31", m31, allocator);
					object.AddMember("m32", m32, allocator);
					object.AddMember("m33", m33, allocator);
					object.AddMember("m34", m34, allocator);
					object.AddMember("m35", m35, allocator);
					object.AddMember("m36", 0, allocator);

					// json 加入数组
					array.PushBack(object, allocator);

					// json object 格式添加 “名称/值” 对
					writedoc.AddMember("json", "json string", allocator);
					writedoc.AddMember("array", array, allocator);

					StringBuffer buffer;
					rapidjson::Writer<StringBuffer> writer(buffer);
					writedoc.Accept(writer);

					FILE* file = fopen(path.c_str(), "wb");
					if (file)
					{
						log("123456789");
						fputs(buffer.GetString(), file);
						fclose(file);
					}

					auto fifthThrough = FinalThrough5::createScene();
					director->replaceScene(TransitionFadeBL::create(0.5, fifthThrough));
				}
			}
			auto drawNode = cocos2d::DrawNode::create();
			drawNode->drawSolidCircle(ball->getAnchorPoint(), ball->getMaxRadius().asFloat() * 1.5, 0, ball->getMaxRadius().asFloat(), cocos2d::Color4F(star->getColor()));
			ball->addChild(drawNode, ball->getCurrentZ().asInt() - 1);
			ball->setCurrentZ(cocos2d::Value(ball->getCurrentZ().asInt() - 1));
			scheduleOnce(schedule_selector(PassDifficulty::timeForChangeBall), 1 / 30);
			return false;
		}

		if (spriteAPhysicsBody->getTag() == 7 && spriteBPhysicsBody->getTag() == 0) {
			if (voice) SimpleAudioEngine::getInstance()->playEffect("DecreaseTime.mp3");
			auto punishBall = (Ball*)(spriteAPhysicsBody->getNode());
			auto mainBall = (Ball*)(spriteBPhysicsBody->getNode());
			if (punishBall->getTag() == 4) {
				currentUsedTime = currentUsedTime.asInt() + 10;
				return false;
			}
			if (!punishBall->getifFirstContect()) return false;
			currentUsedTime = currentUsedTime.asInt() + 10;
			punishBall->setifFirstContect(false);
			auto blink = Blink::create(0.2f, 3);
			auto fedeOut = FadeOut::create(0.3f);
			auto seq = Sequence::create(blink, fedeOut, nullptr);
			punishBall->runAction(seq);
			contactPunishBall.push_back(punishBall);
			scheduleOnce(schedule_selector(PassDifficulty::timeForFadeOut), 0.5f);
			return false;
		}
		if (spriteBPhysicsBody->getTag() == 7 && spriteAPhysicsBody->getTag() == 0) {
			if (voice) SimpleAudioEngine::getInstance()->playEffect("DecreaseTime.mp3");
			auto punishBall = (Ball*)(spriteBPhysicsBody->getNode());
			auto mainBall = (Ball*)(spriteAPhysicsBody->getNode());
			if (punishBall->getTag() == 4) {
				currentUsedTime = currentUsedTime.asInt() + 10;
				return false;
			}
			if (!punishBall->getifFirstContect()) return false;
			currentUsedTime = currentUsedTime.asInt() + 10;
			punishBall->setifFirstContect(false);
			auto blink = Blink::create(0.2f, 3);
			auto fedeOut = FadeOut::create(0.3f);
			auto seq = Sequence::create(blink, fedeOut, nullptr);
			punishBall->runAction(seq);
			contactPunishBall.push_back(punishBall);
			scheduleOnce(schedule_selector(PassDifficulty::timeForFadeOut), 0.5f);
			return false;
		}

		if (spriteAPhysicsBody->getTag() == 1 && spriteBPhysicsBody->getTag() == 0) {
			auto barSprite = (Bar*)(spriteAPhysicsBody->getNode());
			auto mainBallSprite = (Ball*)(spriteBPhysicsBody->getNode());
			if (!barSprite->getDrawNode()->isVisible()) barSprite->getDrawNode()->setVisible(true);
			barSprite->setColor(barSprite->makeColor());
			barSprite->createLineBoundary(true);
		}
		if (spriteBPhysicsBody->getTag() == 1 && spriteAPhysicsBody->getTag() == 0) {
			auto barSprite = (Bar*)(spriteBPhysicsBody->getNode());
			auto mainBallSprite = (Ball*)(spriteAPhysicsBody->getNode());
			if (!barSprite->getDrawNode()->isVisible()) barSprite->getDrawNode()->setVisible(true);
			barSprite->setColor(barSprite->makeColor());
			barSprite->createLineBoundary(true);
		}

		if (spriteBPhysicsBody->getTag() == 5 && spriteAPhysicsBody->getTag() == 7) {
			auto punishBall = (Ball*)(spriteAPhysicsBody->getNode());
			if (punishBall->getTag() == 4) return false;
		}
		if (spriteAPhysicsBody->getTag() == 5 && spriteBPhysicsBody->getTag() == 7) {
			auto punishBall = (Ball*)(spriteBPhysicsBody->getNode());
			if (punishBall->getTag() == 4) return false;
		}

		if (spriteAPhysicsBody->getTag() == 12 && spriteBPhysicsBody->getTag() == 0) {
			if (voice) SimpleAudioEngine::getInstance()->playEffect("DecreaseTime.mp3");
			auto moveBar = (Bar*)(spriteAPhysicsBody->getNode());
			auto mainBall = (Ball*)(spriteBPhysicsBody->getNode());
			moveBar->setColor(moveBar->makeColor());
			moveBar->createLineBoundary(true);
			currentUsedTime = currentUsedTime.asInt() + 10;
			auto fadeOutAction = cocos2d::FadeOut::create(0.5);
			auto fadeInAction = cocos2d::FadeIn::create(0.5);
			auto sequenceAction = cocos2d::Sequence::create(fadeOutAction, fadeInAction, nullptr);
			mainBall->runAction(sequenceAction);
			return false;
		}
		if (spriteBPhysicsBody->getTag() == 12 && spriteAPhysicsBody->getTag() == 0) {
			if (voice) SimpleAudioEngine::getInstance()->playEffect("DecreaseTime.mp3");
			auto moveBar = (Bar*)(spriteBPhysicsBody->getNode());
			auto mainBall = (Ball*)(spriteAPhysicsBody->getNode());
			moveBar->setColor(moveBar->makeColor());
			moveBar->createLineBoundary(true);
			currentUsedTime = currentUsedTime.asInt() + 10;
			auto fadeOutAction = cocos2d::FadeOut::create(0.5);
			auto fadeInAction = cocos2d::FadeIn::create(0.5);
			auto sequenceAction = cocos2d::Sequence::create(fadeOutAction, fadeInAction, nullptr);
			mainBall->runAction(sequenceAction);
			return false;
		}
		return true;
	};
	colloisionListener->onContactSeparate = [&](PhysicsContact &contact) {
		auto spriteAPhysicsBody = contact.getShapeA()->getBody();
		auto spriteBPhysicsBody = contact.getShapeB()->getBody();
		if (spriteAPhysicsBody->getTag() == 2 && spriteBPhysicsBody->getTag() == 0 || spriteBPhysicsBody->getTag() == 2 && spriteAPhysicsBody->getTag() == 0)
		{
			drawNode->runAction(Blink::create(1, 4));
		}
		if (PassDifficulty::choice.asInt() == 3) {
			if (spriteBPhysicsBody->getTag() == 1 && spriteAPhysicsBody->getTag() == 0) {
				auto barSprite = (Bar*)(spriteBPhysicsBody->getNode());
				if (barSprite->getDrawNode()->isVisible()) barSprite->getDrawNode()->setVisible(false);
			}
			if (spriteAPhysicsBody->getTag() == 1 && spriteBPhysicsBody->getTag() == 0) {
				auto barSprite = (Bar*)(spriteAPhysicsBody->getNode());
				if (barSprite->getDrawNode()->isVisible()) barSprite->getDrawNode()->setVisible(false);
			}
		}
	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(colloisionListener, this);
}

// pos决定星星的位置，color为加分项的颜色
void PassDifficulty::drawStar(cocos2d::Vec2 pos, cocos2d::Color3B color)	// int pos, cocos2d::Color3B color
{
	const float o = 0; //180
	const float w = 10;	 //20
	const float h = 25;	 //50
	Vec2 star[] = {
		Vec2(o, o), Vec2(o + w, o - h), Vec2(o + w * 2, o),        // lower spike
		Vec2(o + w * 2 + h, o + w), Vec2(o + w * 2, o + w * 2),    // right spike
		Vec2(o + w, o + w * 2 + h), Vec2(o, o + w * 2),               // top spike			
		Vec2(o - h, o + w),                                     // left spike
	};
	RefPtr<Star> spriteStar = Star::create(star, 8, color);
	spriteStar->setPosition(pos);		//////////////////////////////////////////////////////////////////////////
	addChild(spriteStar);
	starVector.push_back(spriteStar);
}


void PassDifficulty::createMainPunishBall(cocos2d::Vec2 pos, bool ifCanMake)
{
	if (ifCanMake) {
		auto roundBarrier = Ball::create(40, cocos2d::Color3B::RED);
		roundBarrier->setPosition(pos);
		roundBarrier->getPhysicsBody()->setTag(4);
		roundBarrier->setTag(4);
		punishMainBall.push_back(roundBarrier);
		addChild(roundBarrier);
	}
	else
	{
		auto roundBarrier = Ball::create(40, cocos2d::Color3B::RED);
		roundBarrier->setPosition(pos);
		roundBarrier->getPhysicsBody()->setTag(7);
		roundBarrier->setTag(4);
		movePunishBall.push_back(roundBarrier);
		addChild(roundBarrier);
	}
}

void PassDifficulty::timeForChangeBall(float dt)
{
	for (unsigned int i = 0; i < starVector.size(); ++i) {
		if (!starVector[i]->getifFirstContact() && !starVector[i]->getifDelete()) {
			removeChild(starVector[i]);
			starVector[i]->setifDelete(true);
		}
	}
	auto shape = cocos2d::PhysicsShapeCircle::create(ball->getMaxRadius().asFloat() * 1.5, cocos2d::PhysicsMaterial(0, 1, 0));
	shape->setContactTestBitmask(0x00000001);
	ball->getPhysicsBody()->addShape(shape);
	ball->setMaxRadius(cocos2d::Value(ball->getMaxRadius().asFloat() * 1.5));
}

void PassDifficulty::timeForFadeOut(float dt)
{
	for (unsigned int i = 0; i < contactPunishBall.size(); ++i)
	{
		if (contactPunishBall[i] != nullptr && !contactPunishBall[i]->getifFirstContect()) {
			removeChild(contactPunishBall[i]);
			contactPunishBall[i] = nullptr;
		}
	}
}

void PassDifficulty::timeHandlerForAddPunishBall(float dt)
{
	float forX = 100;
	float forY = 100;
	if (cocos2d::random() % 2 == 1) forX *= (-1);
	if (cocos2d::random() % 2 == 1) forY *= (-1);
	for (unsigned int i = 0; i < punishMainBall.size(); ++i){
		auto mainPunishBall = punishMainBall[i];
		mainPunishBall->setVelocity(cocos2d::Vec2(forX, forY));
		auto punishBall = Ball::create(mainPunishBall->getRadius(), mainPunishBall->getColor(), mainPunishBall->getVelocity());
		punishBall->getPhysicsBody()->setTag(7);
		punishBall->setPosition(mainPunishBall->getPosition());
		punishBall->setifFirstContect(true);
		addChild(punishBall);
	}
}

void PassDifficulty::update(float delta)
{
	for (int i = 0; i < 3; ++i)
	{
		scene->getPhysicsWorld()->step(1 / 180.0f);
	}
	camera->setPosition(ball->getPosition());
	showLastTime->setPosition(ball->getPosition().x + 110, ball->getPosition().y + 110);
	showNeedStar->setPosition(ball->getPosition().x + 110, ball->getPosition().y - 110);
	point->setPosition(ball->getPosition().x + 110 + 26, ball->getPosition().y + 110);
	lastHundredTimeChange->setPosition(ball->getPosition().x + 110 + 26 + 29, ball->getPosition().y + 110);
	if (PassDifficulty::choice.asInt() == 2 && currentHadStar.asInt() == 3)
	{
		removeChildByTag(8);
		removeChildByTag(9);
		removeChildByTag(10);
		removeChildByTag(11);
		// 在此添加移除障碍物效果
		if (ifFirstHidden.asBool()) {
			scheduleOnce(schedule_selector(PassDifficulty::timeForHiddenBarrier), 1.5f);
			ifFirstHidden = false;
		}
		
	}

	if (PassDifficulty::choice.asInt() == 4 || PassDifficulty::choice.asInt() == 5) {
		for (int i = 0; i < allSixCornerStartSprite.size(); ++i) {
			if (allSixCornerStartSprite.at(i)->getPositionX() - 10 <= ball->getPositionX() && ball->getPositionX() <= allSixCornerStartSprite.at(i)->getPositionX() + 10 && allSixCornerStartSprite.at(i)->getPositionY() - 10 <= ball->getPositionY() && ball->getPositionY() <= allSixCornerStartSprite.at(i)->getPositionY() + 10) {
				choiceForNewPos = i;
				ifGetDoor = true;
				auto actionRotateBy = cocos2d::RotateBy::create(1.0f, 360);
				auto actionSquence = cocos2d::Sequence::create(actionRotateBy, CallFunc::create(CC_CALLBACK_0(PassDifficulty::afterTransmit, this)), nullptr);
				allSixCornerStartSprite.at(i)->runAction(actionSquence);
				if (voice) SimpleAudioEngine::getInstance()->playEffect("TransmitDoor.mp3");
			}
		}
	}

	if (ifGetDoor.asBool()) {
		ball->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
		Device::setAccelerometerEnabled(false);
	}
	else
	{
		Device::setAccelerometerEnabled(true);
	}

}

void PassDifficulty::timeForHiddenBarrier(float dt) {
	if (voice) SimpleAudioEngine::getInstance()->playEffect("WallDisappear.mp3", false);
}

// 小球传送后的效果
void PassDifficulty::afterTransmit()
{
	if (PassDifficulty::choice.asInt() == 4) {
		int choiceForPos = cocos2d::random() % 4;
		if (getSixCornerStarCount.asInt() == 5) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 1.5 / 18, mapHeight.asFloat() * 1.5 / 12));
		else if (choiceForPos == 0) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 1.5 / 18, mapHeight.asFloat() * 1.5 / 12));
		else if (choiceForPos == 1) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 1.5 / 18, mapHeight.asFloat() * 0.5 / 12));
		else if (choiceForPos == 2) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 17.5 / 18, mapHeight.asFloat() * 9.5 / 12));
		else ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 2.5 / 18, mapHeight.asFloat() * 11.5 / 12));

		if (getSixCornerStarCount.asInt() == 5) getSixCornerStarCount = 0;
		else getSixCornerStarCount = getSixCornerStarCount.asInt() + 1;
	}
	else if (PassDifficulty::choice.asInt() == 5)
	{
		if (choiceForNewPos.asInt() == 0) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 1.5 / 18, mapHeight.asFloat() * 10.5 / 12));
		else if (choiceForNewPos.asInt() == 1 || choiceForNewPos.asInt() == 4) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 1.5 / 18, mapHeight.asFloat() * 4.5 / 12));
		else if (choiceForNewPos.asInt() == 2 || choiceForNewPos.asInt() == 5) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 10.5 / 18, mapHeight.asFloat() * 4.5 / 12));
		else if (choiceForNewPos.asInt() == 3 || choiceForNewPos.asInt() == 10) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 10.5 / 18, mapHeight.asFloat() * 7.5 / 12));
		else if (choiceForNewPos.asInt() == 6) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 11.5 / 18, mapHeight.asFloat() * 3.5 / 12));
		else if (choiceForNewPos.asInt() == 12) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 10.5 / 18, mapHeight.asFloat() * 7 / 12));
		else if (choiceForNewPos.asInt() == 7) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 6 / 12));
		else if (choiceForNewPos.asInt() == 8) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 13.5 / 18, mapHeight.asFloat() * 4.5 / 12));
		else if (choiceForNewPos.asInt() == 9) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 16.5 / 18, mapHeight.asFloat() * 2.5 / 12));
		else if (choiceForNewPos.asInt() == 13) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 16.5 / 18, mapHeight.asFloat() * 4.5 / 12));
		else if (choiceForNewPos.asInt() == 11) ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 16.5 / 18, mapHeight.asFloat() * 9 / 12));
		else ball->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 17.5 / 18, mapHeight.asFloat() * 11.5 / 12));
	}

	ifGetDoor = false;
}

void PassDifficulty::timeForCalculateLastTime(float dt)
{
	currentUsedTime = currentUsedTime.asInt() + 1;
	if (currentUsedTime.asInt() >= PassDifficulty::totalTime.asInt())
	{
		// 在此添加游戏失败代码
		if (PassDifficulty::choice.asInt() == 1) {
			auto firstOver = GameOver1::createScene();
			director->replaceScene(TransitionTurnOffTiles::create(1, firstOver));
		}
		else if (PassDifficulty::choice.asInt() == 2) {
			auto secondOver = GameOver2::createScene();
			director->replaceScene(TransitionTurnOffTiles::create(1, secondOver));
		}
		else if (PassDifficulty::choice.asInt() == 3) {
			auto thirdOver = GameOver3::createScene();
			director->replaceScene(TransitionTurnOffTiles::create(1, thirdOver));
		}
		else if (PassDifficulty::choice.asInt() == 4) {
			auto fourthOver = GameOver4::createScene();
			director->replaceScene(TransitionTurnOffTiles::create(1, fourthOver));
		}
		else if (PassDifficulty::choice.asInt() == 5) {
			auto fifthOver = GameOver5::createScene();
			director->replaceScene(TransitionTurnOffTiles::create(1, fifthOver));
		}
	}
	dynamicShowLastTime();
}

void PassDifficulty::timeForTimeChange(float dt)
{
	auto curTimeString = lastHundredTimeChange->getString();
	cocos2d::Value curTime = cocos2d::Value(atoi(curTimeString.c_str()));
	if (curTime.asInt() - 1 <= 0) curTime = 99;
	else
	{
		curTime = curTime.asInt() - 1;
	}
	lastHundredTimeChange->setString(curTime.asString());
}

void PassDifficulty::timeHandlerForRectDrawNode(float dt)
{
	if (drawNode->isVisible()) drawNode->setVisible(false);
}

// 对第三张地图的所有障碍物显示
void PassDifficulty::timeForShowAllBarrier(float dt)
{
	for (int i = 0; i < allChild.size(); ++i) {
		if (allChild.at(i)->getTag() == 1)
		{
			if (!(((Bar*)allChild.at(i))->getDrawNode()->isVisible())) ((Bar*)allChild.at(i))->getDrawNode()->setVisible(true);
		}
	}
	scheduleOnce(schedule_selector(PassDifficulty::timeForHiddenAllBarrier), 1);
}

void PassDifficulty::timeForHiddenAllBarrier(float dt)
{
	for (int i = 0; i < allChild.size(); ++i) {
		if (allChild.at(i)->getTag() == 1)
		{
			if ((((Bar*)allChild.at(i))->getDrawNode()->isVisible())) ((Bar*)allChild.at(i))->getDrawNode()->setVisible(false);
		}
	}
}

// 对第四张地图的所有已经显示的障碍物隐藏
void PassDifficulty::timeForHiddenAllShowedBarrier(float dt)
{
	for (int i = 0; i < allChild.size(); ++i) {
		if (allChild.at(i)->getTag() == 1)
		{
			if ((((Bar*)allChild.at(i))->getDrawNode()->isVisible())) ((Bar*)allChild.at(i))->getDrawNode()->setVisible(false);
		}
	}
}

void PassDifficulty::dynamicShowLastTime()
{
	cocos2d::Value showTime(totalTime.asInt() - currentUsedTime.asInt());
	showLastTime->setString(showTime.asString());
	if (showTime.asInt() >= 250) showLastTime->setTextColor(Color4B(Color3B::GREEN));
	else if (showTime.asInt() >= 50) showLastTime->setTextColor(Color4B(Color3B::YELLOW));
	else {
		if (ifFirst)
		{
			showLastTime->setTextColor(Color4B(Color3B::RED));
			lastHundredTimeChange->setVisible(true);
			point->setVisible(true);
			schedule(schedule_selector(PassDifficulty::timeForTimeChange), 1 / 100);
			ifFirst = false;
		}
	}
}

void PassDifficulty::dynamicshowNeedStar(cocos2d::Color3B color)
{
	cocos2d::Value showNeed(totalStar.asInt() - currentHadStar.asInt());
	showNeedStar->setString(showNeed.asString());
	showNeedStar->setTextColor(cocos2d::Color4B(color));
}

// 第一张地图创建
void PassDifficulty::makeFirstMap()
{
	// 障碍物碰撞后不消失，只有两个要获得的物品
	// 固定的障碍物
	Vec2 firstBarrier[4];
	firstBarrier[0] = Vec2(mapWidth.asFloat() * 1 / 6, mapHeight.asFloat() * 1 / 3);
	firstBarrier[1] = Vec2(mapWidth.asFloat() * 1 / 3, mapHeight.asFloat() * 1 / 6);
	firstBarrier[2] = Vec2(mapWidth.asFloat() * ((float)1 / 3 + (float)2 / 15), mapHeight.asFloat() * 1 / 3);
	firstBarrier[3] = Vec2(mapWidth.asFloat() * 1 / 3, mapHeight.asFloat() * 1 / 2);
	auto firstBarrierSprite = Barrier::create(firstBarrier, 4, true);
	addChild(firstBarrierSprite);

	Vec2 secondBarrier[4];
	secondBarrier[0] = Vec2(mapWidth.asFloat() * ((float)1 / 3 + (float)3 / 15), mapHeight.asFloat() * 1 / 3);
	secondBarrier[1] = Vec2(mapWidth.asFloat() * 2 / 3, mapHeight.asFloat() * 1 / 6);
	secondBarrier[2] = Vec2(mapWidth.asFloat() * 5 / 6, mapHeight.asFloat() * 1 / 3);
	secondBarrier[3] = Vec2(mapWidth.asFloat() * 2 / 3, mapHeight.asFloat() * 1 / 2);
	auto secondBarrierSprite = Barrier::create(secondBarrier, 4, true);
	addChild(secondBarrierSprite);

	Vec2 thirdBarrier[3];
	thirdBarrier[0] = Vec2(0, mapHeight.asFloat() * 1 / 2);
	thirdBarrier[1] = Vec2(mapWidth.asFloat() * 1 / 6, mapHeight.asFloat() * 2 / 3);
	thirdBarrier[2] = Vec2(0, mapHeight.asFloat() * 5 / 6);
	auto thirdBarrierSprite = Barrier::create(thirdBarrier, 3, true);
	addChild(thirdBarrierSprite);

	Vec2 fourthBarrier[3];
	fourthBarrier[0] = Vec2(mapWidth.asFloat() * 5 / 6, mapHeight.asFloat() * 2 / 3);
	fourthBarrier[1] = Vec2(mapWidth.asFloat(), mapHeight.asFloat() * 1 / 2);
	fourthBarrier[2] = Vec2(mapWidth.asFloat(), mapHeight.asFloat() * 5 / 6);
	auto fourthBarrierSprite = Barrier::create(fourthBarrier, 3, true);
	addChild(fourthBarrierSprite);

	Vec2 fifthBarrier[3];
	fifthBarrier[0] = Vec2(mapWidth.asFloat() * 1 / 6, mapHeight.asFloat());
	fifthBarrier[1] = Vec2(mapWidth.asFloat() * 1 / 3, mapHeight.asFloat() * 5 / 6);
	fifthBarrier[2] = Vec2(mapWidth.asFloat() * 1 / 2, mapHeight.asFloat());
	auto fifthBarrierSprite = Barrier::create(fifthBarrier, 3, true);
	addChild(fifthBarrierSprite);

	Vec2 sixthBarrier[3];
	sixthBarrier[0] = Vec2(mapWidth.asFloat() * 1 / 2, mapHeight.asFloat());
	sixthBarrier[1] = Vec2(mapWidth.asFloat() * 2 / 3, mapHeight.asFloat() * 5 / 6);
	sixthBarrier[2] = Vec2(mapWidth.asFloat() * 5 / 6, mapHeight.asFloat());
	auto sixthBarrierSprite = Barrier::create(sixthBarrier, 3, true);
	addChild(sixthBarrierSprite);

	// 活动的障碍物
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 1 / 6, mapHeight.asFloat() * 5 / 6), true);
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 1 / 2, mapHeight.asFloat() * 2 / 3), true);

	// 设置加分项
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 2 / 6, mapHeight.asFloat() * 4 / 6), Color3B::RED);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 5 / 6, mapHeight.asFloat() * 1 / 6), Color3B::YELLOW);
}

// 第二张地图创建
void PassDifficulty::makeSecondMap()
{
	// 障碍物碰撞后只显示碰撞的那条线，有四个要获得的物品
	// 固定的障碍物
	auto bar1 = Bar::create(cocos2d::Vec2(0, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar1);
	auto bar2 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(0, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar2);
	auto bar3 = Bar::create(cocos2d::Vec2(0, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar3);
	auto bar4 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(0, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar4);
	
	auto bar5 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar5);
	auto bar6 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar6);
	auto bar7 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar7);

	auto bar8 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar8);
	auto bar9 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar9);

	auto bar10 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 12 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar10);
	auto bar11 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 0 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar11);

	auto bar12 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar12);
	auto bar13 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar13);
	auto bar14 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar14);
	auto bar15 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar15);

	auto bar16 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar16);
	bar16->setTag(8);
	auto bar17 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar17);
	bar17->setTag(9);
	auto bar18 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar18);
	bar18->setTag(10);
	auto bar19 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar19);
	bar19->setTag(11);
	
	auto bar20 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar20);
	auto bar21 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar21);

	auto bar22 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 12 / 12), true);
	addChild(bar22);

	auto bar23 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar23);
	auto bar24 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar24);
	auto bar25 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar25);
	auto bar26 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar26);
	auto bar27 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar27);
	auto bar28 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar28);
	auto bar29 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar29);

	auto bar30 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 0 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar30);

	auto bar31 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar31);
	auto bar32 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar32);

	auto bar33 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar33);
	auto bar34 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar34);

	auto bar35 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 18 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar35);
	auto bar36 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 18 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar36);
	auto bar37 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 18 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar37);
	auto bar38 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 18 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar38);

	// 活动的障碍物
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 10 / 12), true);
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 10 / 12), true);
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 2 / 12), true);
	
	// 设置加分项
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 11 / 12), Color3B::RED);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 11 / 12), Color3B::YELLOW);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 1 / 12), Color3B::ORANGE);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 6 / 12), Color3B::BLUE);
}

// 第三张地图创建
void PassDifficulty::makeThirdMap()
{
	auto bar1 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 12 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar1);
	auto bar2 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar2);
	auto bar3 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar3);
	auto bar4 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar4);
	auto bar5 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 7.5 / 12 - 20), true);
	addChild(bar5);

	auto bar6 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 6.5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar6);
	auto bar7 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar7);
	auto bar8 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar8);
	auto bar9 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar9);
	auto bar10 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar10);

	auto bar11 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar11);
	auto bar12 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18 - 2.5 * 30, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar12);

	auto bar13 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar13);
	auto bar14 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar14);
	auto bar15 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar15);
	auto bar16 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar16);
	auto bar17 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar17);
	auto bar18 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar18);
	auto bar19 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar19);
	auto bar20 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar20);
	auto bar21 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar21);
	auto bar22 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar22);
	auto bar23 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar23);

	auto bar24 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar24);

	auto bar25 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar25);

	auto bar26 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 7 / 12 - 3.5 * 15), true);
	addChild(bar26);

	auto bar27 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar27);

	auto bar28 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar28);

	auto bar29 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar29);
	auto bar30 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar30);
	auto bar31 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar31);

	auto bar32 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar32);
	auto bar33 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 12 / 12), true);
	addChild(bar33);

	auto bar34 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar34);
	auto bar35 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar35);
	auto bar36 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar36);
	auto bar37 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar37);
	auto bar38 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar38);

	auto bar39 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar39);

	auto bar40 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar40);

	auto bar41 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar41);

	auto bar42 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar42);

	auto bar43 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar43);
	auto bar44 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar44);

	auto bar45 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar45);

	auto bar46 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar46);
	auto bar47 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar47);

	auto bar48 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar48);
	auto bar49 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar49);
	auto bar50 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar50);

	auto bar51 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar51);

	auto bar52 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar52);

	auto bar53 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar53);

	auto bar54 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar54);
	auto bar55 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar55);
	auto bar56 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar56);

	auto bar57 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9.5 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar57);

	auto bar58 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar58);

	auto bar59 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar59);

	auto bar60 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar60);
	auto bar60A = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar60A);

	auto bar61 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar61);

	auto bar62 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar62);

	auto bar63 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar63);

	auto bar64 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar64);
	
	auto bar65 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar65);

	auto bar66 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar66);

	auto bar67 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar67);

	auto bar68 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 12 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar68);

	auto bar69 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar69);

	auto bar70 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar70);

	auto bar71 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar71);

	auto bar72 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 12 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar72);
	auto bar73 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar73);
	auto bar74 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar74);

	auto bar75 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar75);

	auto bar76 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar76);

	auto bar77 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar77);

	auto bar78 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar78);

	auto bar79 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar79);
	auto bar80 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar80);
	auto bar81 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar81);

	auto bar82 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar82);
	auto bar83 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar83);
	auto bar84 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar84);
	auto bar85 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar85);

	auto bar86 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar86);
	auto bar87 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar87);
	auto bar88 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar88);
	auto bar89 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar89);

	auto bar90 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar90);

	auto bar91 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar91);

	auto bar92 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar92);

	auto bar93 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar93);

	auto bar94 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar94);
	auto bar95 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar95);

	auto bar96 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar96);

	auto bar97 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar97);

	auto bar98 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar98);
	auto bar99 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar99);

	// 活动的障碍物
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 11 / 12), true);
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 17.5 / 18, mapHeight.asFloat() * 0.5 / 12), true);
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 7.5 / 18, mapHeight.asFloat() * 2.5 / 12), false);
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 10.5 / 18, mapHeight.asFloat() * 2.5 / 12), false);
	for (unsigned int i = 0; i < movePunishBall.size(); ++i)
	{
		movePunishBall[i]->getPhysicsBody()->setVelocity(cocos2d::Vec2(300, 0));
	}

	// 设置加分项
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 11 / 12), Color3B::RED);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 3 / 12), Color3B::YELLOW);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 11 / 12), Color3B::ORANGE);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 9.5 / 18, mapHeight.asFloat() * 2.5 / 12), Color3B::BLUE);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 8 / 12), Color3B::GREEN);

	allChild.clear();
	allChild = this->getChildren();
	schedule(schedule_selector(PassDifficulty::timeForShowAllBarrier), 3);
}

// 第四张地图创建
void PassDifficulty::makeFourthMap()
{
	auto bar1 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 0 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar1);
	auto bar2 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar2);
	auto bar3 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar3);
	auto bar4 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar4);

	auto bar5 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar5);
	auto bar6 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar6);
	auto bar7 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar7);
	auto bar8 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar8);
	auto bar9 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar9);
	auto bar10 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar10);
	auto bar11 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar11);

	auto bar12 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar12);
	auto bar13 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar13);
	auto bar14 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar14);
	auto bar15 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 0 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar15);

	auto bar16 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar16);

	auto bar17 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 0 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar17);

	auto bar18 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar18);

	auto bar19 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar19);

	auto bar20 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar20);

	auto bar21 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar21);

	auto bar22 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar22);

	auto bar23 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar23);

	auto bar24 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar24);

	auto bar25 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar25);

	auto bar26 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar26);

	auto bar27 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar27);

	auto bar28 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar28);

	auto bar29 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar29);

	auto bar30 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 0 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar30);

	auto bar31 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar31);

	auto bar32 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar32);

	auto bar33 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar33);

	auto bar34 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar34);

	auto bar35 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar35);

	auto bar36 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 12 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar36);

	auto bar37 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar37);

	auto bar38 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar38);

	auto bar39 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar39);

	auto bar40 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 12 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar40);

	auto bar41 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar41);

	auto bar42 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar42);
	auto bar43 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar43);
	auto bar44 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar44);
	auto bar45 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar45);
	auto bar46 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar46);
	auto bar47 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar47);
	auto bar48 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar48);
	auto bar49 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar49);
	auto bar50 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar50);
	auto bar51 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar51);

	auto bar52 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar52);
	auto bar53 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar53);
	auto bar54 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar54);
	auto bar55 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar55);
	auto bar56 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar56);
	auto bar57 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar57);
	auto bar58 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar58);
	auto bar59 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar59);

	auto bar60 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar60);

	auto bar61 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar61);

	auto bar62 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar62);
	auto bar63 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar63);

	auto bar64 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar64);

	auto bar65 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar65);

	auto bar66 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar66);

	auto bar67 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar67);

	auto bar68 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar68);

	auto bar69 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar69);

	auto bar70 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar70);

	auto bar71 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar71);

	auto bar72 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar72);

	auto bar73 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar73);

	auto bar74 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar74);

	auto bar75 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar75);

	auto bar76 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar76);

	auto bar77 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar77);
	auto bar78 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar78);
	auto bar79 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar79);
	auto bar80 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 6 / 12), true);
	addChild(bar80);
	auto bar81 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar81);
	auto bar82 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar82);
	auto bar83 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar83);
	auto bar84 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar84);

	auto bar85 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar85);
	auto bar86 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar86);
	auto bar87 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar87);
	auto bar88 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar88);
	
	auto bar90 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 6 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar90);
	auto bar91 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar91);
	auto bar92 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar92);
	auto bar93 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar93);
	auto bar94 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar94);

	auto bar95 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar95);
	auto bar96 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar96);
	auto bar97 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar97);
	auto bar98 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar98);
	auto bar99 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar99);

	auto bar100 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar100);
	auto bar101 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar101);
	auto bar102 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar102);
	auto bar103 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar103);
	auto bar104 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar104);
	auto bar105 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar105);

	auto bar106 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar106);
	auto bar107 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar107);

	auto bar108 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar108);

	auto bar109 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar109);
	auto bar110 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar110);
	auto bar111 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar111);

	auto bar112 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar112);

	auto bar113 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar113);

	auto bar114 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar114);

	auto bar115 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar115);

	auto bar116 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar116);

	auto bar117 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar117);

	auto bar118 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar118);

	auto bar119 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 6.5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 6.5 / 12), true);
	addChild(bar119);

	auto bar120 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 6.5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 6.5 / 12), true);
	addChild(bar120);

	auto bar121 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar121);

	auto bar122 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 5.5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 5.5 / 12), true);
	addChild(bar122);

	auto bar123 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 5.5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 5.5 / 12), true);
	addChild(bar123);

	auto bar124 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar124);

	auto bar125 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar125);

	auto bar126 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar126);

	auto bar127 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar127);

	auto bar128 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar128);

	auto bar129 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar129);

	auto bar130 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar130);

	auto bar131 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar131);

	auto bar132 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar132);
	auto bar133 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar133);
	auto bar134 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar134);
	auto bar135 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar135);

	auto bar136 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar136);
	auto bar137 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar137);
	auto bar138 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar138);
	auto bar139 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar139);
	auto bar140 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar140);

	auto bar141 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar141);
	auto bar142 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar142);
	auto bar143 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar143);
	auto bar144 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar144);
	auto bar145 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar145);

	auto bar146 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar146);
	auto bar147 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar147);
	auto bar148 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar148);
	auto bar149 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar149);
	auto bar150 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar150);
	auto bar151 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar151);

	auto bar152 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar152);

	auto bar153 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar153);

	auto bar154 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar154);

	auto bar155 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar155);

	// 加入传送门
	auto sixCornerStartSprite1 = drawSixCorner(150);
	sixCornerStartSprite1->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 7.5 / 12));
	addChild(sixCornerStartSprite1);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite1);

	auto sixCornerStartSprite2 = drawSixCorner(150);
	sixCornerStartSprite2->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 17.5 / 18, mapHeight.asFloat() * 11.5 / 12));
	addChild(sixCornerStartSprite2);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite2);

	auto sixCornerStartSprite3 = drawSixCorner(150);
	sixCornerStartSprite3->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 2.5 / 18, mapHeight.asFloat() * 3.5 / 12));
	addChild(sixCornerStartSprite3);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite3);

	auto sixCornerStartSprite4 = drawSixCorner(150);
	sixCornerStartSprite4->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 0.5 / 18, mapHeight.asFloat() * 11.5 / 12));
	addChild(sixCornerStartSprite4);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite4);

	//设置加分项
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 0.5 / 18, mapHeight.asFloat() * 7.5 / 12), Color3B::RED);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 15.5 / 18, mapHeight.asFloat() * 9.5 / 12), Color3B::YELLOW);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 4.5 / 18, mapHeight.asFloat() * 11.5 / 12), Color3B::ORANGE);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 11.5 / 18, mapHeight.asFloat() * 1.5 / 12), Color3B::BLUE);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 3.5 / 18, mapHeight.asFloat() * 2.5 / 12), Color3B::GREEN);

	// 活动的障碍物
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 13.5 / 18, mapHeight.asFloat() * 8.5 / 12), true);
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 8.5 / 18, mapHeight.asFloat() * 5.5 / 12), true);

	allChild.clear();
	allChild = this->getChildren();
	schedule(schedule_selector(PassDifficulty::timeForHiddenAllShowedBarrier), 5);
}

// 第四张地图的六芒星绘制
Sprite* PassDifficulty::drawSixCorner(double triangleLength)
{
	auto sprite = Sprite::create();

	auto upTriangle = DrawNode::create();
	cocos2d::Vec2 upTriangleArray[3];
	upTriangleArray[0] = cocos2d::Vec2(-triangleLength * 1 / 2, -triangleLength * sqrt((double)3) / 6);
	upTriangleArray[1] = cocos2d::Vec2(triangleLength * 1 / 2, -triangleLength * sqrt((double)3) / 6);
	upTriangleArray[2] = cocos2d::Vec2(0, triangleLength * sqrt((double)3) / 3);
	upTriangle->drawPolygon(upTriangleArray, 3, cocos2d::Color4F(cocos2d::Color3B::BLUE), 3, cocos2d::Color4F(cocos2d::Color3B::BLUE));

	auto downTriangle = DrawNode::create();
	cocos2d::Vec2 downTriangleArray[3];
	downTriangleArray[0] = cocos2d::Vec2(-triangleLength * 1 / 2, triangleLength * sqrt((double)3) / 6);
	downTriangleArray[1] = cocos2d::Vec2(triangleLength * 1 / 2, triangleLength * sqrt((double)3) / 6);
	downTriangleArray[2] = cocos2d::Vec2(0, -triangleLength * sqrt((double)3) / 3);
	downTriangle->drawPolygon(downTriangleArray, 3, cocos2d::Color4F(cocos2d::Color3B::BLUE), 3, cocos2d::Color4F(cocos2d::Color3B::BLUE));

	auto circle = DrawNode::create();
	circle->drawSolidCircle(cocos2d::Vec2(0, 0), triangleLength * sqrt((double)3) / 3, 0, triangleLength * sqrt((double)3) / 3 * 2, cocos2d::Color4F(cocos2d::Color3B::RED));

	sprite->addChild(circle, 1);
	sprite->addChild(upTriangle, 2);
	sprite->addChild(downTriangle, 3);

	auto actionRotateBy = cocos2d::RotateBy::create(1, 90);
	auto actionRepeat = cocos2d::RepeatForever::create(actionRotateBy);
	sprite->runAction(actionRepeat);

	return sprite;
}

// 第五张地图创建
void PassDifficulty::makeFifthMap()
{
	auto bar1 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar1);
	auto bar2 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar2);
	auto bar3 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar3);
	auto bar4 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar4);

	auto bar5 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar5);
	auto bar6 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar6);

	auto bar7 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar7);

	auto bar8 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar8);

	auto bar9 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar9);
	auto bar10 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar10);

	auto bar11 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar11);

	auto bar12 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar12);

	auto bar13 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar13);
	auto bar14 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar14);

	auto bar15 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar15);

	auto bar16 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 2 / 12), true);
	bar16->getPhysicsBody()->setDynamic(true);
	bar16->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar16->getPhysicsBody()->setTag(12);
	bar16->getDrawNode()->setVisible(true);
	bar16->setTag(12);
	addChild(bar16);
	auto moveByActionLeftFor12 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, 0));
	auto moveByActionRightFor12 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 7 / 18, 0));
	auto sequenceActionFor12 = cocos2d::Sequence::create(moveByActionLeftFor12, moveByActionRightFor12, nullptr);
	auto repeatActionFor12 = cocos2d::RepeatForever::create(sequenceActionFor12);
	bar16->runAction(repeatActionFor12);

	auto bar17 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar17);
	auto bar18 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar18);

	auto bar19 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar19);

	auto bar20 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar20);
	auto bar21 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar21);

	auto bar22 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar22);

	auto bar23 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar23);
	auto bar24 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar24);
	auto bar25 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar25);
	auto bar26 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar26);

	auto bar27 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar27);
	
	auto bar28 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar28);

	auto bar29 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar29);

	auto bar30 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar30);
	auto bar31 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar31);
	auto bar32 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar32);

	auto bar33 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar33);

	auto bar34 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar34);

	auto bar35 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar35);
	auto bar36 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 8 / 12), true);
	addChild(bar36);

	auto bar37 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar37);

	auto bar38 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar38);

	auto bar39 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 5 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar39);
	auto bar40 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar40);

	auto bar41 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 9 / 12), true);
	addChild(bar41);
	auto bar42 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar42);

	auto bar43 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 8 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar43);
	auto bar44 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 7 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 10 / 12), true);
	addChild(bar44);

	auto bar45 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar45);
	auto bar46 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar46);
	auto bar47 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 5 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar47);
	auto bar48 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 1 / 12), true);
	addChild(bar48);

	auto bar49 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar49);

	auto bar50 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar50);

	auto bar51 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar51);

	auto bar52 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar52);

	auto bar53 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar53);

	auto bar54 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar54);

	auto bar55 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar55);

	auto bar56 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar56);

	auto bar57 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar57);
	auto bar58 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar58);

	auto bar59 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 1 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 2 / 12), true);
	addChild(bar59);

	auto bar60 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 3 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar60);

	auto bar61 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar61);

	auto bar62 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 5 / 12), true);
	addChild(bar62);

	auto bar63 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 2 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar63);

	auto bar65 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 3 / 12), true);
	addChild(bar65);

	auto bar66 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 4 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 4 / 12), true);
	addChild(bar66);

	auto bar67 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar67);
	auto bar68 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 11 / 12), true);
	addChild(bar68);
	auto bar69 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 11 / 12), cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar69);
	auto bar70 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 17 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 10 / 18, mapHeight.asFloat() * 7 / 12), true);
	addChild(bar70);

	auto bar71 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 8 / 12), true);
	bar71->getPhysicsBody()->setDynamic(true);
	bar71->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar71->getDrawNode()->setVisible(true);
	bar71->getPhysicsBody()->setTag(12);
	bar71->setTag(13);
	addChild(bar71);
	auto moveByActionLeftFor13 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, 0));
	auto moveByActionRightFor13 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 3 / 18, 0));
	auto sequenceActionFor13 = cocos2d::Sequence::create(moveByActionLeftFor13, moveByActionRightFor13, nullptr);
	auto repeatActionFor13 = cocos2d::RepeatForever::create(sequenceActionFor13);
	bar71->runAction(repeatActionFor13);

	auto bar72 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 8 / 12), true);
	bar72->getPhysicsBody()->setDynamic(true);
	bar72->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar72->getDrawNode()->setVisible(true);
	bar72->getPhysicsBody()->setTag(12);
	bar72->setTag(14);
	addChild(bar72);
	auto moveByActionLeftFor14 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, 0));
	auto moveByActionRightFor14 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 3 / 18, 0));
	auto sequenceActionFor14 = cocos2d::Sequence::create(moveByActionLeftFor14, moveByActionRightFor14, nullptr);
	auto repeatActionFor14 = cocos2d::RepeatForever::create(sequenceActionFor14);
	bar72->runAction(repeatActionFor14);

	auto bar73 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 7 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 8 / 12), true);
	bar73->getPhysicsBody()->setDynamic(true);
	bar73->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar73->getDrawNode()->setVisible(true);
	bar73->getPhysicsBody()->setTag(12);
	bar73->setTag(15);
	addChild(bar73);
	auto moveByActionLeftFor15 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 3 / 18, 0));
	auto moveByActionRightFor15 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 3 / 18, 0));
	auto sequenceActionFor15 = cocos2d::Sequence::create(moveByActionLeftFor15, moveByActionRightFor15, nullptr);
	auto repeatActionFor15 = cocos2d::RepeatForever::create(sequenceActionFor15);
	bar73->runAction(repeatActionFor15);


	auto bar74 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 11 / 18, mapHeight.asFloat() * 9 / 12), true);
	bar74->getPhysicsBody()->setDynamic(true);
	bar74->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar74->getDrawNode()->setVisible(true);
	bar74->getPhysicsBody()->setTag(12);
	bar74->setTag(16);
	addChild(bar74);
	auto moveByActionLeftFor16 = cocos2d::MoveBy::create(1.0f, cocos2d::Vec2(-mapWidth.asFloat() * 1 / 18, 0));
	auto moveByActionRightFor16 = cocos2d::MoveBy::create(1.0f, cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, 0));
	auto sequenceActionFor16 = cocos2d::Sequence::create(moveByActionLeftFor16, moveByActionRightFor16, nullptr);
	auto repeatActionFor16 = cocos2d::RepeatForever::create(sequenceActionFor16);
	bar74->runAction(repeatActionFor16);

	auto bar75 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 9 / 12), true);
	bar75->getPhysicsBody()->setDynamic(true);
	bar75->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar75->getDrawNode()->setVisible(true);
	bar75->getPhysicsBody()->setTag(12);
	bar75->setTag(17);
	addChild(bar75);
	auto moveByActionLeftFor17 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, 0));
	auto moveByActionRightFor17 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 4 / 18, 0));
	auto sequenceActionFor17 = cocos2d::Sequence::create(moveByActionLeftFor17, moveByActionRightFor17, nullptr);
	auto repeatActionFor17 = cocos2d::RepeatForever::create(sequenceActionFor17);
	bar75->runAction(repeatActionFor17);

	auto bar76 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 8 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 9 / 12), true);
	bar76->getPhysicsBody()->setDynamic(true);
	bar76->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar76->getDrawNode()->setVisible(true);
	bar76->getPhysicsBody()->setTag(12);
	bar76->setTag(18);
	addChild(bar76);
	auto moveByActionLeftFor18 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 4 / 18, 0));
	auto moveByActionRightFor18 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 4 / 18, 0));
	auto sequenceActionFor18 = cocos2d::Sequence::create(moveByActionLeftFor18, moveByActionRightFor18, nullptr);
	auto repeatActionFor18 = cocos2d::RepeatForever::create(sequenceActionFor18);
	bar76->runAction(repeatActionFor18);

	auto bar77 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 13 / 18, mapHeight.asFloat() * 10 / 12), true);
	bar77->getPhysicsBody()->setDynamic(true);
	bar77->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar77->getDrawNode()->setVisible(true);
	bar77->getPhysicsBody()->setTag(12);
	bar77->setTag(19);
	addChild(bar77);
	auto moveByActionLeftFor19 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, 0));
	auto moveByActionRightFor19 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 2 / 18, 0));
	auto sequenceActionFor19 = cocos2d::Sequence::create(moveByActionLeftFor19, moveByActionRightFor19, nullptr);
	auto repeatActionFor19 = cocos2d::RepeatForever::create(sequenceActionFor19);
	bar77->runAction(repeatActionFor19);

	auto bar78 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 10 / 12), true);
	bar78->getPhysicsBody()->setDynamic(true);
	bar78->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar78->getDrawNode()->setVisible(true);
	bar78->getPhysicsBody()->setTag(12);
	bar78->setTag(20);
	addChild(bar78);
	auto moveByActionLeftFor20 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, 0));
	auto moveByActionRightFor20 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 2 / 18, 0));
	auto sequenceActionFor20 = cocos2d::Sequence::create(moveByActionLeftFor20, moveByActionRightFor20, nullptr);
	auto repeatActionFor20 = cocos2d::RepeatForever::create(sequenceActionFor20);
	bar78->runAction(repeatActionFor20);

	auto bar79 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 9 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 10 / 12), true);
	bar79->getPhysicsBody()->setDynamic(true);
	bar79->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar79->getDrawNode()->setVisible(true);
	bar79->getPhysicsBody()->setTag(12);
	bar79->setTag(21);
	addChild(bar79);
	auto moveByActionLeftFor21 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(mapWidth.asFloat() * 2 / 18, 0));
	auto moveByActionRightFor21 = cocos2d::MoveBy::create(5.0f, cocos2d::Vec2(-mapWidth.asFloat() * 2 / 18, 0));
	auto sequenceActionFor21 = cocos2d::Sequence::create(moveByActionLeftFor21, moveByActionRightFor21, nullptr);
	auto repeatActionFor21 = cocos2d::RepeatForever::create(sequenceActionFor21);
	bar79->runAction(repeatActionFor21);

	auto bar80 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 11 / 12), true);
	bar80->getPhysicsBody()->setDynamic(true);
	bar80->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar80->getDrawNode()->setVisible(true);
	bar80->getPhysicsBody()->setTag(12);
	bar80->setTag(22);
	addChild(bar80);
	auto moveByActionLeftFor22 = cocos2d::MoveBy::create(2.0f, cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, 0));
	auto moveByActionRightFor22 = cocos2d::MoveBy::create(2.0f, cocos2d::Vec2(-mapWidth.asFloat() * 1 / 18, 0));
	auto sequenceActionFor22 = cocos2d::Sequence::create(moveByActionLeftFor22, moveByActionRightFor22, nullptr);
	auto repeatActionFor22 = cocos2d::RepeatForever::create(sequenceActionFor22);
	bar80->runAction(repeatActionFor22);

	auto bar81 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 11 / 12), true);
	bar81->getPhysicsBody()->setDynamic(true);
	bar81->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar81->getDrawNode()->setVisible(true);
	bar81->getPhysicsBody()->setTag(12);
	bar81->setTag(23);
	addChild(bar81);
	auto moveByActionLeftFor23 = cocos2d::MoveBy::create(2.0f, cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, 0));
	auto moveByActionRightFor23 = cocos2d::MoveBy::create(2.0f, cocos2d::Vec2(-mapWidth.asFloat() * 1 / 18, 0));
	auto sequenceActionFor23 = cocos2d::Sequence::create(moveByActionLeftFor23, moveByActionRightFor23, nullptr);
	auto repeatActionFor23 = cocos2d::RepeatForever::create(sequenceActionFor23);
	bar81->runAction(repeatActionFor23);

	auto bar82 = Bar::create(cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 10 / 12), cocos2d::Vec2(mapWidth.asFloat() * 16 / 18, mapHeight.asFloat() * 11 / 12), true);
	bar82->getPhysicsBody()->setDynamic(true);
	bar82->getPhysicsBody()->setVelocity(cocos2d::Vec2(0, 0));
	bar82->getDrawNode()->setVisible(true);
	bar82->getPhysicsBody()->setTag(12);
	bar82->setTag(24);
	addChild(bar82);
	auto moveByActionLeftFor24 = cocos2d::MoveBy::create(2.0f, cocos2d::Vec2(mapWidth.asFloat() * 1 / 18, 0));
	auto moveByActionRightFor24 = cocos2d::MoveBy::create(2.0f, cocos2d::Vec2(-mapWidth.asFloat() * 1 / 18, 0));
	auto sequenceActionFor24 = cocos2d::Sequence::create(moveByActionLeftFor24, moveByActionRightFor24, nullptr);
	auto repeatActionFor24 = cocos2d::RepeatForever::create(sequenceActionFor24);
	bar82->runAction(repeatActionFor24);

	//设置加分项
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 7.5 / 18, mapHeight.asFloat() * 4.5 / 12), Color3B::RED);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 4.5 / 18, mapHeight.asFloat() * 9.5 / 12), Color3B::YELLOW);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 16.5 / 18, mapHeight.asFloat() * 10.5 / 12), Color3B::ORANGE);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 15.5 / 18, mapHeight.asFloat() * 4.5 / 12), Color3B::BLUE);
	drawStar(cocos2d::Vec2(mapWidth.asFloat() * 14 / 18, mapHeight.asFloat() * 2 / 12), Color3B::GREEN);

	//设置传送门
	auto sixCornerStartSprite1 = drawSixCorner(150);
	sixCornerStartSprite1->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 6 / 18, mapHeight.asFloat() * 6 / 12));
	addChild(sixCornerStartSprite1);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite1);

	auto sixCornerStartSprite2 = drawSixCorner(150);
	sixCornerStartSprite2->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 6 / 12));
	addChild(sixCornerStartSprite2);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite2);

	auto sixCornerStartSprite3 = drawSixCorner(150);
	sixCornerStartSprite3->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 8 / 12));
	addChild(sixCornerStartSprite3);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite3);

	auto sixCornerStartSprite4 = drawSixCorner(150);
	sixCornerStartSprite4->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 4 / 12));
	addChild(sixCornerStartSprite4);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite4);

	auto sixCornerStartSprite5 = drawSixCorner(150);
	sixCornerStartSprite5->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 6.5 / 18, mapHeight.asFloat() * 9.5 / 12));
	addChild(sixCornerStartSprite5);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite5);

	auto sixCornerStartSprite6 = drawSixCorner(150);
	sixCornerStartSprite6->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 12 / 18, mapHeight.asFloat() * 6 / 12));
	addChild(sixCornerStartSprite6);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite6);

	auto sixCornerStartSprite7 = drawSixCorner(150);
	sixCornerStartSprite7->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 6.5 / 18, mapHeight.asFloat() * 3.5 / 12));
	addChild(sixCornerStartSprite7);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite7);

	auto sixCornerStartSprite8 = drawSixCorner(150);
	sixCornerStartSprite8->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 11.5 / 18, mapHeight.asFloat() * 4.5 / 12));
	addChild(sixCornerStartSprite8);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite8);

	auto sixCornerStartSprite9 = drawSixCorner(150);
	sixCornerStartSprite9->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 10.5 / 18, mapHeight.asFloat() * 1.5 / 12));	
	addChild(sixCornerStartSprite9);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite9);

	auto sixCornerStartSprite10 = drawSixCorner(150);
	sixCornerStartSprite10->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 12.5 / 18, mapHeight.asFloat() * 4.5 / 12));
	addChild(sixCornerStartSprite10);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite10);

	auto sixCornerStartSprite11 = drawSixCorner(150);
	sixCornerStartSprite11->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 15 / 18, mapHeight.asFloat() * 3.5 / 12));
	addChild(sixCornerStartSprite11);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite11);

	auto sixCornerStartSprite12 = drawSixCorner(150);
	sixCornerStartSprite12->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 11.5 / 18, mapHeight.asFloat() * 1.5 / 12));
	addChild(sixCornerStartSprite12);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite12);

	auto sixCornerStartSprite13 = drawSixCorner(150);
	sixCornerStartSprite13->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 16.5 / 18, mapHeight.asFloat() * 1.5 / 12));
	addChild(sixCornerStartSprite13);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite13);

	auto sixCornerStartSprite14 = drawSixCorner(150);
	sixCornerStartSprite14->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 16.5 / 18, mapHeight.asFloat() * 7.5 / 12));
	addChild(sixCornerStartSprite14);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite14);

	auto sixCornerStartSprite15 = drawSixCorner(150);
	sixCornerStartSprite15->setPosition(cocos2d::Vec2(mapWidth.asFloat() * 10.5 / 18, mapHeight.asFloat() * 10.5 / 12));
	addChild(sixCornerStartSprite15);
	allSixCornerStartSprite.pushBack(sixCornerStartSprite15);

	// 创建活动障碍物
	createMainPunishBall(cocos2d::Vec2(mapWidth.asFloat() * 9 / 18, mapHeight.asFloat() * 6 / 12), true);

	// 对已显示的障碍物进行隐藏
	allChild.clear();
	allChild = this->getChildren();
	schedule(schedule_selector(PassDifficulty::timeForHiddenAllShowedBarrier), 1);
}