#include "AdventureScene.h"
#include "Bar.h"
#include "AddBarrier.h"
#include "Vibrator.h"
#include "SimpleAudioEngine.h"

#include "Final1Scene.h"
#include "Final2Scene.h"
#include "Final3Scene.h"

#include "CCStdC.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "GateMapLayer.h"


using namespace  rapidjson;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include<jni.h>
#include"platform/android/jni/JniHelper.h"

#endif

using namespace CocosDenshion;


// ��������ɫ�������
// �ϰ�����ɫ�������
// �ϰ�����״�������
// ���������ɫ���Դ��ݸ���
// �����ɫ���Դ��ݸ��ϰ���

// ��ʼ���λ�ÿ��������
// �ϰ����ʼ������λ��

// ������������ģʽ�����������峤�ȶ�����ģʽ���޳�


// ��ģʽ �����ײ���ϰ������Ϊ100�� �ϰ�����Ϊ5�� ������ĵ���Ϊ1.1
// ����ģʽ �����ײ���ϰ������Ϊ50�� �ϰ�����Ϊ10�� ������ĵ���Ϊ1.2
// ��̬ģʽ �����ײ���ϰ������Ϊ25�� �ϰ�����Ϊ15�� ������ĵ���Ϊ1.3
// ������ģʽ���Բ��ý���ֱ����
// ����ð��ģʽ��������С���ܳ��߽���Ϸʧ�ܣ�����С����ײ�ϰ�������ﵽ������Ϸʧ�ܣ����ʱ��ĳ��̼�Ϊ�÷֣�������С���ܳ��߽����һ���ķ������淨����֤��С���ܳ��߽�����������ײ�ϰ����������,�����ϰ���Ľǻ�ʹ����ײ������2�������ϰ���ı߻�ʹ����ײ������1���������������ٶȰ�һ����������
// ���߷����壬�ϰ����

int Adventure::maxCanContactCount = 0;
int Adventure::ballContactBarrierCount = 0;
cocos2d::Scene *Adventure::scene = nullptr;
cocos2d::Value Adventure::barrierNumber = cocos2d::Value(0);
cocos2d::Value Adventure::barRestitution = cocos2d::Value(0);
cocos2d::Value Adventure::choice(0);

Adventure::Adventure()
{

}
Adventure::~Adventure()
{	
	director->getEventDispatcher()->removeEventListener(listener);
	director->getEventDispatcher()->removeEventListener(colloisionListener);
}

Scene* Adventure::createScene(int choice)
{
	Adventure::choice = choice;
	if (choice == 1) {
		Adventure::maxCanContactCount = 100;
		Adventure::barrierNumber = 5;
		Adventure::barRestitution = 1.1f;
	}
	else if (choice == 2) {
		Adventure::maxCanContactCount = 50;
		Adventure::barrierNumber = 10;
		Adventure::barRestitution = 1.2f;
	}
	else {
		Adventure::maxCanContactCount = 25;
		Adventure::barrierNumber = 15;
		Adventure::barRestitution = 1.3f;
	}

	Adventure::ballContactBarrierCount = 0;
	scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setAutoStep(false);
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); //���Դ���
    auto layer = Adventure::create();
	// ����������岢��
	auto layerPhysicsBody = PhysicsBody::createEdgeBox(Director::getInstance()->getVisibleSize(), PhysicsMaterial(0, 1, 0), 15);
	layerPhysicsBody->setDynamic(false);
	layerPhysicsBody->setContactTestBitmask(0x00000001);
	layerPhysicsBody->setTag(2);
	layer->setPhysicsBody(layerPhysicsBody);

    scene->addChild(layer);

    return scene;
}

bool Adventure::init()
{
	if (!LayerColor::initWithColor(cocos2d::Color4B(cocos2d::Color3B(0, 0, 0))))
	{
		return false;
	}
	for (int i = 0; i < 6;i++)
	{
		m[6] = 0;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	
	director = Director::getInstance();
	ifStart = true;
	mainBall = nullptr;
	score = 0;
	ifEnd = false;
	ballVector.clear();

	Good = true;
	Great = true;
	Excellent = true;
	Amazing = true;
	Unbelievable = true;

	voice = true;
	shake = true;
	ifTouchBack = false;
	signForMusic = true;
	signForVoice = true;
	signForShake = true;


	// �����ϰ���
	AddBarrier(this, Adventure::barrierNumber.asInt(), 100, 3);	// ���ĸ�����������ڵ���3
	// ��������
	setListener();
	// ������ײ����
	solveContactEvent();
	// ����������ʾ��
	setLabel();
	// ���崩͸�������
	scheduleUpdate();

    return true;
}

void Adventure::onEnterTransitionDidFinish()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BackGroundMusicForAdventureScene.mp3", true); //���ű�������
}


void Adventure::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void Adventure::setListener() {
	for (int i = 0; i < 1000; i++) {
		lastLine[i] = nullptr;
	}
	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = [&](std::vector<Touch*>touch, Event *event) {
		if (ifStart) {
			for (unsigned int i = 0; i < touch.size(); i++)
			{
				makeBall(touch[i]->getLocation());
				// ���ü�ʱ��,ʵ���ڵ�һ�����Ѿ����õ�ǰ����ÿ��1������һ����
				schedule(schedule_selector(Adventure::timeHandlerForMakeBall), 1);
				// ���ü�ʱ��,ʵ�ֶԷ������ۼ�
				schedule(schedule_selector(Adventure::timeHandlerForUpadteScore), 1);
				// ���ü�ʱ����ʵ����ʾ����
				schedule(schedule_selector(Adventure::timeHandlerForshowScore), 1);
			}
			ifStart = !ifStart;
		}
		else
		{
			for (unsigned int i = 0; i < touch.size(); i++)
			{
				touch[i]->setTouchInfo(i, touch[i]->getLocation().x, touch[i]->getLocation().y);
			}
		}
	};
	listener->onTouchesMoved = [&](std::vector<Touch*>touch, Event *event) {
		for (unsigned int i = 0; i < touch.size(); i++)
		{
			auto touchID = touch[i]->getID();
			cocos2d::Vec2 startPosition(touch[i]->getStartLocation());
			cocos2d::Vec2 nowPosition(touch[i]->getLocation());
			if (lastLine[touchID] != nullptr) {
				lastLine[touchID]->clear();
				removeChild(lastLine[touchID]);
			}
			auto line = DrawNode::create();
			line->drawLine(startPosition, nowPosition, Color4F::WHITE);
			lastLine[touchID] = line;
			addChild(line);
		}
	};
	listener->onTouchesEnded = [&](std::vector<Touch*>touch, Event *event) {
		for (unsigned int i = 0; i < touch.size(); i++)
		{		
			auto touchID = touch[i]->getID();
			if (lastLine[touchID] != nullptr) {
				lastLine[touchID]->clear();
				removeChild(lastLine[touchID]);
				lastLine[touchID] = nullptr;
			}
			cocos2d::Vec2 startPosition(touch[i]->getStartLocation());
			cocos2d::Vec2 nowPosition(touch[i]->getLocation());
			// ����������
			auto bar = Bar::create(startPosition, nowPosition, Adventure::barRestitution.asFloat());
			addChild(bar);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto backButtunlistener = EventListenerKeyboard::create();
	backButtunlistener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event *e){
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE){
			if (!ifTouchBack){
				pauseGame();
			}
			else
			{
				Director::getInstance()->getScheduler()->resumeTargets(allTimeHandler);
				scheduleOnce(schedule_selector(Adventure::timeForDeletepauseLayer), 0);
				ifTouchBack = false;
				listener->setEnabled(true);
			}
		}
	};
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButtunlistener, this);
}

// ���ؼ�ʹ��
void Adventure::pauseGame()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo methodInfo;
	std::string text = "";
	// ��ȡUmengGameActivity�ľ�̬����openShareBoard
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/JNIConnect", "fun", "()Ljava/lang/String;");
	if (isHave)
	{
		// ʵ�ʵ���UmengGameActivity�д�umeng����ƽ̨ѡ�����
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		text = JniHelper::jstring2string(jstr);
	}

	ifTouchBack = true;
	pauseLayer = cocos2d::LayerColor::create(Color4B(0, 0, 0, 128)); //����һ����͸������ɫ��
	pauseLayer->setContentSize(director->getVisibleSize());
	pauseLayer->setPosition(visibleSize.width / 2 - pauseLayer->getContentSize().width / 2, visibleSize.height / 2 - pauseLayer->getContentSize().height / 2);
	addChild(pauseLayer);
	allTimeHandler = director->getScheduler()->pauseAllTargets();

	listener->setEnabled(false);

	auto size = pauseLayer->getContentSize();
	// ������Ϸ
	auto continueMenu = Menu::create(MenuItemImage::create("continue.png", "scontinue.png",
		[=](Ref* ref){
		director->getEventDispatcher()->resumeEventListenersForTarget(this, true);
		Director::getInstance()->getScheduler()->resumeTargets(allTimeHandler);
		scheduleOnce(schedule_selector(Adventure::timeForDeletepauseLayer), 0);
		ifTouchBack = false;
		listener->setEnabled(true);
	}), NULL);
	continueMenu->setPosition(size.width / 2, size.height / 2 + 200);
	pauseLayer->addChild(continueMenu);
	// ���¿�ʼ
	auto againMenu = Menu::create(MenuItemImage::create("again.png", "sagain.png",
		[=](Ref* ref){
		director->getEventDispatcher()->resumeEventListenersForTarget(this, true);
		Director::getInstance()->getScheduler()->resumeTargets(allTimeHandler);
		scheduleOnce(schedule_selector(Adventure::timeForDeletepauseLayer), 0);
		auto scene = Adventure::createScene(Adventure::choice.asInt());
		listener->setEnabled(true);
		director->replaceScene(TransitionRotoZoom::create(1, scene));
	}), NULL);
	againMenu->setPosition(size.width / 2, size.height / 2 + 50);
	pauseLayer->addChild(againMenu);
	// �������˵�
	auto returnToMainMenu = Menu::create(MenuItemImage::create("choose.png", "schoose.png",
		[=](Ref* ref){
		director->getEventDispatcher()->resumeEventListenersForTarget(this, true);
		Director::getInstance()->getScheduler()->resumeTargets(allTimeHandler);
		scheduleOnce(schedule_selector(Adventure::timeForDeletepauseLayer), 0);
		listener->setEnabled(true);
		// �ڴ˴������л���������
		auto scene = GateMapLayer::createScene();
		director->replaceScene(TransitionTurnOffTiles::create(1, scene));
	}), NULL);
	returnToMainMenu->setPosition(size.width / 2, size.height / 2 - 100);
	pauseLayer->addChild(returnToMainMenu);

	// ֹͣ��������
	auto musicOn = MenuItemImage::create("music.png", "music.png");
	auto musicOff = MenuItemImage::create("musicClose.png", "musicClose.png");
	cocos2d::MenuItemToggle *backGroundMusic = nullptr;
	if (signForMusic.asBool()) backGroundMusic = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(Adventure::callBackForMusic, this), musicOn, musicOff, NULL);
	else
	{
		backGroundMusic = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(Adventure::callBackForMusic, this), musicOff, musicOn, NULL);
	}
	auto bGMMenu = Menu::create(backGroundMusic, NULL);
	bGMMenu->setPosition(size.width / 2 - 250, size.height / 2 - 250);
	pauseLayer->addChild(bGMMenu);

	// ֹͣ��Ч
	auto voiceOn = MenuItemImage::create("voice.png", "voice.png");
	auto voiceOff = MenuItemImage::create("closeVoice.png", "closeVoice.png");
	cocos2d::MenuItemToggle *voiceCon = nullptr;
	if (signForVoice.asBool()) voiceCon = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(Adventure::callBackForVoice, this), voiceOn, voiceOff, NULL);
	else
	{
		voiceCon = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(Adventure::callBackForVoice, this), voiceOff, voiceOn, NULL);
	}
	auto vCMenu = Menu::create(voiceCon, NULL);
	vCMenu->setPosition(size.width / 2 + 250, size.height / 2 - 250);
	pauseLayer->addChild(vCMenu);

	// ��Ļ��ͼ
	auto getScreen = Menu::create(MenuItemImage::create("screenshot.png", "sscreenshot.png",
		[=](Ref* ref){

		pauseLayer->setVisible(false);

		time_t t;
		time(&t);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S", localtime((&t)));
		std::string outputFile = text + "/" + std::string("ScreenShot") + std::string(tmp) + std::string(".png");

		utils::captureScreen(CC_CALLBACK_2(Adventure::afterCapture, this), outputFile);

	}), NULL);
	getScreen->setPosition(size.width / 2 + 100, size.height / 2 - 250);
	pauseLayer->addChild(getScreen);

	// �ֻ���
	auto shakeOn = MenuItemImage::create("zhendong.png", "zhendong.png");
	auto shakeOff = MenuItemImage::create("zhendongClose.png", "zhendongClose.png");
	cocos2d::MenuItemToggle *shakeCon = nullptr;
	if (signForShake.asBool()) shakeCon = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(Adventure::callBackForShake, this), shakeOn, shakeOff, NULL);
	else
	{
		shakeCon = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(Adventure::callBackForShake, this), shakeOff, shakeOn, NULL);
	}
	auto shMenu = Menu::create(shakeCon, NULL);
	shMenu->setPosition(size.width / 2 - 100, size.height / 2 - 250);
	pauseLayer->addChild(shMenu);

	director->getEventDispatcher()->resumeEventListenersForTarget(pauseLayer, true);

#endif

}


// �����ֺ����ص�
void Adventure::callBackForMusic(Ref *ref)
{
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		signForMusic = false;
	}
	else {
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		signForMusic = true;
	}
}

// ����Ч�����ص�
void Adventure::callBackForVoice(Ref *ref)
{
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

void Adventure::callBackForShake(Ref *ref)
{
	if (shake) {
		shake = false;
		signForShake = false;
	}
	else
	{
		shake = true;
		signForShake = true;
	}
}


void Adventure::afterCapture(bool succed, const std::string& outputFile)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo methodInfo;
	// ��ȡUmengGameActivity�ľ�̬����openShareBoard
	bool isHave = JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/JNIConnect", "fun1", "(Ljava/lang/String;)V");
	if (isHave)
	{
		// ʵ�ʵ���UmengGameActivity�д�umeng����ƽ̨ѡ�����
		jobject para = methodInfo.env->NewStringUTF(outputFile.c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para);

	}
#endif
	SimpleAudioEngine::getInstance()->playEffect("kacha.mp3", false);
	GrayLayer = cocos2d::LayerColor::create(cocos2d::Color4B(158, 158, 158, 100));
	GrayLayer->setContentSize(director->getVisibleSize() * 3 / 4);
	GrayLayer->setAnchorPoint(Vec2(0.5, 0.5));
	GrayLayer->setPosition(visibleSize.width / 2 - GrayLayer->getContentSize().width / 2, visibleSize.height / 2 - GrayLayer->getContentSize().height / 2);
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
	auto actionSquenceForLayer = cocos2d::Sequence::create(fadeOutForLayer, CallFunc::create(CC_CALLBACK_0(Adventure::timeForDeleteGetScreen, this)), nullptr);
	GrayLayer->runAction(actionSquenceForLayer);
}

void Adventure::timeForDeleteGetScreen()
{
	GrayLayer->removeAllChildren();
	removeChild(GrayLayer, true);
}

void Adventure::timeForDeletepauseLayer(float dt)
{
	pauseLayer->removeAllChildren();
	removeChild(pauseLayer, true);
}

void Adventure::solveContactEvent() {
	

	
	//UserDefault::getInstance()->flush();
	

	colloisionListener = EventListenerPhysicsContact::create();
	colloisionListener->onContactBegin = [&](PhysicsContact &contact) {

		int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0;
		int m1 = 0, m2 = 0, m3 = 0, m4 = 0, m5 = 0, m6 = 0;
		int m21 = 0, m22 = 0, m23 = 0, m24 = 0, m25 = 0, m26 = 0;
		int m31 = 0, m32 = 0, m33 = 0, m34 = 0, m35 = 0, m36 = 0;

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
		auto spriteA = contact.getShapeA()->getBody();
		auto spriteB = contact.getShapeB()->getBody();
		if (spriteA->getNode() == nullptr || spriteB->getNode() == nullptr) return false;
		if (spriteA->getTag() == 2 || spriteB->getTag() == 2) {
			if (spriteA->getTag() == 0) {
				auto ball = (Ball*)(spriteA->getNode());
				removeChild(ball, true);
				if (ball == mainBall) {
					// �ڴ���ӽ�����������
					SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
					voice = false;
					shake = false;
					ifEnd = true;
					unschedule(schedule_selector(Adventure::timeHandlerForMakeBall));
					unschedule(schedule_selector(Adventure::timeHandlerForUpadteScore));
					if (Adventure::choice.asInt() == 1) {
						m6 = score;
						log("score%d",m6);
							int temp;
							if (m6 > m5)
							{
								m5 = m6;
							}
							if (m5 > m4)
							{
								temp = m4;
								m4 = m5;
								m5 = temp;
							}
							if (m4 > m3)
							{
								temp = m3;
								m3 = m4;
								m4 = temp;
							}
							if (m3 > m2)
							{
								temp = m2;
								m2 = m3;
								m3 = temp;
							}
							if (m2 > m1)
							{
								temp = m1;
								m1 = m2;
								m2 = temp;
							}
						rapidjson::Document writedoc;
						writedoc.SetObject();
						rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
						rapidjson::Value array(rapidjson::kArrayType);
						rapidjson::Value object(rapidjson::kObjectType);

						// json object ��ʽ��� ������/ֵ�� ��
						object.AddMember("temp", score, allocator);
						//���ط���
						object.AddMember("c1", c1, allocator);
						object.AddMember("c2", c2, allocator);
						object.AddMember("c3", c3, allocator);
						object.AddMember("c4", c4, allocator);
						object.AddMember("c5", c5, allocator);
						//ð�շ���
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

						// json ��������
						array.PushBack(object, allocator);

						// json object ��ʽ��� ������/ֵ�� ��
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
						auto scene = Final1::createScene();
						director->replaceScene(TransitionFadeBL::create(1, scene));
					}
					else if (Adventure::choice.asInt() == 2) {
						
						m26 = score;
						log("score%d", m26);
						int temp;
						if (m26 > m25)
						{
							m25 = m26;
						}
						if (m25 > m24)
						{
							temp = m24;
							m24 = m25;
							m25 = temp;
						}
						if (m24 > m23)
						{
							temp = m23;
							m23 = m24;
							m24 = temp;
						}
						if (m23 > m22)
						{
							temp = m22;
							m22 = m23;
							m23 = temp;
						}
						if (m22 > m21)
						{
							temp = m21;
							m21 = m22;
							m22 = temp;
						}
						rapidjson::Document writedoc;
						writedoc.SetObject();
						rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
						rapidjson::Value array(rapidjson::kArrayType);
						rapidjson::Value object(rapidjson::kObjectType);

						// json object ��ʽ��� ������/ֵ�� ��
						object.AddMember("temp", score, allocator);
						//���ط���
						object.AddMember("c1", c1, allocator);
						object.AddMember("c2", c2, allocator);
						object.AddMember("c3", c3, allocator);
						object.AddMember("c4", c4, allocator);
						object.AddMember("c5", c5, allocator);
						//ð�շ���
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

						// json ��������
						array.PushBack(object, allocator);

						// json object ��ʽ��� ������/ֵ�� ��
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

						auto scene = Final2::createScene();
						director->replaceScene(TransitionFadeBL::create(1, scene));
					}
					else {
					
						m36 = score;
						log("score%d", m36);
						int temp;
						if (m36 > m35)
						{
							m35 = m36;
						}
						if (m35 > m34)
						{
							temp = m34;
							m34 = m35;
							m35 = temp;
						}
						if (m34 > m33)
						{
							temp = m33;
							m33 = m34;
							m34 = temp;
						}
						if (m33 > m32)
						{
							temp = m32;
							m32 = m33;
							m33 = temp;
						}
						if (m32 > m31)
						{
							temp = m31;
							m31 = m32;
							m32 = temp;
						}
						rapidjson::Document writedoc;
						writedoc.SetObject();
						rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
						rapidjson::Value array(rapidjson::kArrayType);
						rapidjson::Value object(rapidjson::kObjectType);

						// json object ��ʽ��� ������/ֵ�� ��
						object.AddMember("temp", score, allocator);
						//���ط���
						object.AddMember("c1", c1, allocator);
						object.AddMember("c2", c2, allocator);
						object.AddMember("c3", c3, allocator);
						object.AddMember("c4", c4, allocator);
						object.AddMember("c5", c5, allocator);
						//ð�շ���
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

						// json ��������
						array.PushBack(object, allocator);

						// json object ��ʽ��� ������/ֵ�� ��
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


						auto scene = Final3::createScene();
						director->replaceScene(TransitionFadeBL::create(1, scene));
					}
				}
				else
				{
					score -= 5;
					if (score < 0) score = 0;
				}
			}
			if (spriteB->getTag() == 0) {
				auto ball = (Ball*)(spriteB->getNode());
				removeChild(ball, true);
				if (ball == mainBall) {
					// �ڴ���ӽ�����������
					SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
					voice = false;
					shake = false;
					ifEnd = true;
					unschedule(schedule_selector(Adventure::timeHandlerForMakeBall));
					unschedule(schedule_selector(Adventure::timeHandlerForUpadteScore));
					if (Adventure::choice.asInt() == 1) {

						m6 = score;
						log("score%d", m6);
						int temp;
						if (m6 > m5)
						{
							m5 = m6;
						}
						if (m5 > m4)
						{
							temp = m4;
							m4 = m5;
							m5 = temp;
						}
						if (m4 > m3)
						{
							temp = m3;
							m3 = m4;
							m4 = temp;
						}
						if (m3 > m2)
						{
							temp = m2;
							m2 = m3;
							m3 = temp;
						}
						if (m2 > m1)
						{
							temp = m1;
							m1 = m2;
							m2 = temp;
						}
						rapidjson::Document writedoc;
						writedoc.SetObject();
						rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
						rapidjson::Value array(rapidjson::kArrayType);
						rapidjson::Value object(rapidjson::kObjectType);

						// json object ��ʽ��� ������/ֵ�� ��
						object.AddMember("temp", score, allocator);
						//���ط���
						object.AddMember("c1", c1, allocator);
						object.AddMember("c2", c2, allocator);
						object.AddMember("c3", c3, allocator);
						object.AddMember("c4", c4, allocator);
						object.AddMember("c5", c5, allocator);
						//ð�շ���
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

						// json ��������
						array.PushBack(object, allocator);

						// json object ��ʽ��� ������/ֵ�� ��
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

						auto scene = Final1::createScene();
						director->replaceScene(TransitionFadeBL::create(1, scene));
					}
					else if (Adventure::choice.asInt() == 2) {

						m26 = score;
						log("score%d", m26);
						int temp;
						if (m26 > m25)
						{
							m25 = m26;
						}
						if (m25 > m24)
						{
							temp = m24;
							m24 = m25;
							m25 = temp;
						}
						if (m24 > m23)
						{
							temp = m23;
							m23 = m24;
							m24 = temp;
						}
						if (m23 > m22)
						{
							temp = m22;
							m22 = m23;
							m23 = temp;
						}
						if (m22 > m21)
						{
							temp = m21;
							m21 = m22;
							m22 = temp;
						}
						rapidjson::Document writedoc;
						writedoc.SetObject();
						rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
						rapidjson::Value array(rapidjson::kArrayType);
						rapidjson::Value object(rapidjson::kObjectType);

						// json object ��ʽ��� ������/ֵ�� ��
						object.AddMember("temp", score, allocator);
						//���ط���
						object.AddMember("c1", c1, allocator);
						object.AddMember("c2", c2, allocator);
						object.AddMember("c3", c3, allocator);
						object.AddMember("c4", c4, allocator);
						object.AddMember("c5", c5, allocator);
						//ð�շ���
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

						// json ��������
						array.PushBack(object, allocator);

						// json object ��ʽ��� ������/ֵ�� ��
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

						auto scene = Final2::createScene();
						director->replaceScene(TransitionFadeBL::create(1, scene));
					}
					else
					{

						m36 = score;
						log("score%d", m36);
						int temp;
						if (m36 > m35)
						{
							m35 = m36;
						}
						if (m35 > m34)
						{
							temp = m34;
							m34 = m35;
							m35 = temp;
						}
						if (m34 > m33)
						{
							temp = m33;
							m33 = m34;
							m34 = temp;
						}
						if (m33 > m32)
						{
							temp = m32;
							m32 = m33;
							m33 = temp;
						}
						if (m32 > m31)
						{
							temp = m31;
							m31 = m32;
							m32 = temp;
						}
						rapidjson::Document writedoc;
						writedoc.SetObject();
						rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
						rapidjson::Value array(rapidjson::kArrayType);
						rapidjson::Value object(rapidjson::kObjectType);

						// json object ��ʽ��� ������/ֵ�� ��
						object.AddMember("temp", score, allocator);
						//���ط���
						object.AddMember("c1", c1, allocator);
						object.AddMember("c2", c2, allocator);
						object.AddMember("c3", c3, allocator);
						object.AddMember("c4", c4, allocator);
						object.AddMember("c5", c5, allocator);
						//ð�շ���
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

						// json ��������
						array.PushBack(object, allocator);

						// json object ��ʽ��� ������/ֵ�� ��
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


						auto scene = Final3::createScene();
						director->replaceScene(TransitionFadeBL::create(1, scene));
					}
				}
				else
				{
					score -= 5;
					if (score < 0) score = 0;
				}
			}
			return false;	// ��ײ�����ԣ��򽫴ӱ߽��˶���ȥ
		}
		if (spriteA->getTag() == 0 && spriteB->getTag() == 1) {	
			// �ڴ����������������ʱ��Ч������
			auto ball = (Ball*)(spriteA->getNode());
			auto bar = (Bar*)(spriteB->getNode());
			ball->setColor(bar->getColor());
			ball->createCircleGraphy();
			removeChild((spriteB->getNode()), true);
		}
		else if (spriteB->getTag() == 0 && spriteA->getTag() == 1) {	
			// �ڴ����������������ʱ��Ч������
			auto ball = (Ball*)(spriteB->getNode());
			auto bar = (Bar*)(spriteA->getNode());
			ball->setColor(bar->getColor());
			ball->createCircleGraphy();
			removeChild((spriteA->getNode()), true);
		}
		if (spriteA->getTag() == 3 && spriteB->getTag() == 0) {
			// �ڴ�����������ϰ���ʱ��Ч������
			auto barrier = (Barrier*)(spriteA->getNode());
			auto ball = (Ball*)(spriteB->getNode());
			barrier->setColor(ball->getColor());
			barrier->createPolygonGraphy();
			auto barrierDrawNode = barrier->getDrawNode();
			barrierDrawNode->runAction(Blink::create(0.5, 2));
			if (shake) Vibrator::vibrate(100);
			++ballContactBarrierCount;
			if (maxCanContactCount - ballContactBarrierCount > 7) {
				labelForBCBC->setString(cocos2d::Value(maxCanContactCount - ballContactBarrierCount).asString());
				labelForBCBC->setColor(cocos2d::Color3B::GREEN);
			}
			else if (maxCanContactCount - ballContactBarrierCount <= 7 && maxCanContactCount - ballContactBarrierCount > 3){
				labelForBCBC->setString(cocos2d::Value(maxCanContactCount - ballContactBarrierCount).asString());
				labelForBCBC->setColor(cocos2d::Color3B::YELLOW);

			}
			else if (maxCanContactCount - ballContactBarrierCount >= 0){
				labelForBCBC->setString(cocos2d::Value(maxCanContactCount - ballContactBarrierCount).asString());
				labelForBCBC->setColor(cocos2d::Color3B::RED);
				labelForBCBC->runAction(ScaleTo::create(0.5f, 1.5f));
				labelForBCBC->setScale(labelForBCBC->getScale());
			}
			if (ballContactBarrierCount >= maxCanContactCount)
			{
				// �˴������Ϸ��������
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				ifEnd = true;
				voice = false;
				shake = false;
				unschedule(schedule_selector(Adventure::timeHandlerForMakeBall));
				unschedule(schedule_selector(Adventure::timeHandlerForUpadteScore));
				if (Adventure::choice.asInt() == 1) {
					
					m6 = score;
					log("score%d", m6);
					int temp;
					if (m6 > m5)
					{
						m5 = m6;
					}
					if (m5 > m4)
					{
						temp = m4;
						m4 = m5;
						m5 = temp;
					}
					if (m4 > m3)
					{
						temp = m3;
						m3 = m4;
						m4 = temp;
					}
					if (m3 > m2)
					{
						temp = m2;
						m2 = m3;
						m3 = temp;
					}
					if (m2 > m1)
					{
						temp = m1;
						m1 = m2;
						m2 = temp;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object ��ʽ��� ������/ֵ�� ��
					object.AddMember("temp", score, allocator);
					//���ط���
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//ð�շ���
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

					// json ��������
					array.PushBack(object, allocator);

					// json object ��ʽ��� ������/ֵ�� ��
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

					auto scene = Final1::createScene();
					director->replaceScene(TransitionFadeBL::create(1, scene));
				}
				else if (Adventure::choice.asInt() == 2) {

					m26 = score;
					log("score%d", m26);
					int temp;
					if (m26 > m25)
					{
						m25 = m26;
					}
					if (m25 > m24)
					{
						temp = m24;
						m24 = m25;
						m25 = temp;
					}
					if (m24 > m23)
					{
						temp = m23;
						m23 = m24;
						m24 = temp;
					}
					if (m23 > m22)
					{
						temp = m22;
						m22 = m23;
						m23 = temp;
					}
					if (m22 > m21)
					{
						temp = m21;
						m21 = m22;
						m22 = temp;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object ��ʽ��� ������/ֵ�� ��
					object.AddMember("temp", score, allocator);
					//���ط���
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//ð�շ���
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

					// json ��������
					array.PushBack(object, allocator);

					// json object ��ʽ��� ������/ֵ�� ��
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

					auto scene = Final2::createScene();
					director->replaceScene(TransitionFadeBL::create(1, scene));
				}
				else
				{
					m36 = score;
					log("score33333333%d", m36);
					int temp;
					if (m36 > m35)
					{
						m35 = m36;
					}
					if (m35 > m34)
					{
						temp = m34;
						m34 = m35;
						m35 = temp;
					}
					if (m34 > m33)
					{
						temp = m33;
						m33 = m34;
						m34 = temp;
					}
					if (m33 > m32)
					{
						temp = m32;
						m32 = m33;
						m33 = temp;
					}
					if (m32 > m31)
					{
						temp = m31;
						m31 = m32;
						m32 = temp;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object ��ʽ��� ������/ֵ�� ��
					object.AddMember("temp", score, allocator);
					//���ط���
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//ð�շ���
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

					// json ��������
					array.PushBack(object, allocator);

					// json object ��ʽ��� ������/ֵ�� ��
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


					auto scene = Final3::createScene();
					director->replaceScene(TransitionFadeBL::create(1, scene));
				}
			}
		}
		else if (spriteB->getTag() == 3 && spriteA->getTag() == 0) {
			// �ڴ�����������ϰ���ʱ��Ч������
			auto barrier = (Barrier*)(spriteB->getNode());
			auto ball = (Ball*)(spriteA->getNode());
			barrier->setColor(ball->getColor());
			barrier->createPolygonGraphy();
			auto barrierDrawNode = barrier->getDrawNode();
			barrierDrawNode->runAction(Blink::create(0.5, 2));
			if (shake) Vibrator::vibrate(100);	
			++ballContactBarrierCount;
			if (maxCanContactCount - ballContactBarrierCount > 7) {
				labelForBCBC->setString(cocos2d::Value(maxCanContactCount - ballContactBarrierCount).asString());
				labelForBCBC->setColor(cocos2d::Color3B::GREEN);
			}
			else if (maxCanContactCount - ballContactBarrierCount <= 7 && maxCanContactCount - ballContactBarrierCount > 3){
				labelForBCBC->setString(cocos2d::Value(maxCanContactCount - ballContactBarrierCount).asString());
				labelForBCBC->setColor(cocos2d::Color3B::YELLOW);

			}
			else if (maxCanContactCount - ballContactBarrierCount >= 0){
				labelForBCBC->setString(cocos2d::Value(maxCanContactCount - ballContactBarrierCount).asString());
				labelForBCBC->setColor(cocos2d::Color3B::RED);
				labelForBCBC->runAction(ScaleTo::create(0.5f, 1.5f));
				labelForBCBC->setScale(labelForBCBC->getScale());
			}
			if (ballContactBarrierCount >= maxCanContactCount)
			{
				// �˴������Ϸ��������
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				ifEnd = true;
				voice = false;
				shake = false;
				unschedule(schedule_selector(Adventure::timeHandlerForMakeBall));
				unschedule(schedule_selector(Adventure::timeHandlerForUpadteScore));
				if (Adventure::choice.asInt() == 1) {

					m6 = score;
					log("score%d", m6);
					int temp;
					if (m6 > m5)
					{
						m5 = m6;
					}
					if (m5 > m4)
					{
						temp = m4;
						m4 = m5;
						m5 = temp;
					}
					if (m4 > m3)
					{
						temp = m3;
						m3 = m4;
						m4 = temp;
					}
					if (m3 > m2)
					{
						temp = m2;
						m2 = m3;
						m3 = temp;
					}
					if (m2 > m1)
					{
						temp = m1;
						m1 = m2;
						m2 = temp;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object ��ʽ��� ������/ֵ�� ��
					object.AddMember("temp", score, allocator);
					//���ط���
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//ð�շ���
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

					// json ��������
					array.PushBack(object, allocator);

					// json object ��ʽ��� ������/ֵ�� ��
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
					
					auto scene = Final1::createScene();
					director->replaceScene(TransitionFadeBL::create(1, scene));
				}
				else if (Adventure::choice.asInt() == 2) {
					m26 = score;
					log("score%d", m26);
					int temp;
					if (m26 > m25)
					{
						m25 = m26;
					}
					if (m25 > m24)
					{
						temp = m24;
						m24 = m25;
						m25 = temp;
					}
					if (m24 > m23)
					{
						temp = m23;
						m23 = m24;
						m24 = temp;
					}
					if (m23 > m22)
					{
						temp = m22;
						m22 = m23;
						m23 = temp;
					}
					if (m22 > m21)
					{
						temp = m21;
						m21 = m22;
						m22 = temp;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object ��ʽ��� ������/ֵ�� ��
					object.AddMember("temp", score, allocator);
					//���ط���
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//ð�շ���
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

					// json ��������
					array.PushBack(object, allocator);

					// json object ��ʽ��� ������/ֵ�� ��
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

					
					auto scene = Final2::createScene();
					director->replaceScene(TransitionFadeBL::create(1, scene));
				}
				else
				{
					m36 = score;
					log("score33333333%d", m36);
					int temp;
					if (m36 > m35)
					{
						m35 = m36;
					}
					if (m35 > m34)
					{
						temp = m34;
						m34 = m35;
						m35 = temp;
					}
					if (m34 > m33)
					{
						temp = m33;
						m33 = m34;
						m34 = temp;
					}
					if (m33 > m32)
					{
						temp = m32;
						m32 = m33;
						m33 = temp;
					}
					if (m32 > m31)
					{
						temp = m31;
						m31 = m32;
						m32 = temp;
					}
					rapidjson::Document writedoc;
					writedoc.SetObject();
					rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
					rapidjson::Value array(rapidjson::kArrayType);
					rapidjson::Value object(rapidjson::kObjectType);

					// json object ��ʽ��� ������/ֵ�� ��
					object.AddMember("temp", score, allocator);
					//���ط���
					object.AddMember("c1", c1, allocator);
					object.AddMember("c2", c2, allocator);
					object.AddMember("c3", c3, allocator);
					object.AddMember("c4", c4, allocator);
					object.AddMember("c5", c5, allocator);
					//ð�շ���
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

					// json ��������
					array.PushBack(object, allocator);

					// json object ��ʽ��� ������/ֵ�� ��
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
		
					auto scene = Final3::createScene();
					director->replaceScene(TransitionFadeBL::create(1, scene));
				}
			}
		}
		return true;
	};

	colloisionListener->onContactSeparate = [&](PhysicsContact &contact) {
		auto spriteA = contact.getShapeA()->getBody();
		auto spriteB = contact.getShapeB()->getBody();
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(colloisionListener, this);	
}


void Adventure::makeBall(cocos2d::Vec2 point)
{
	Ball *ball = nullptr;
	if (ifStart)  ball = Ball::create(30, Color3B::RED, cocos2d::Vec2(-100, -100));	// ����뾶Ϊ40�� Ĭ����ɫΪ��ɫ
	else ball = Ball::create(20, Color3B::RED, cocos2d::Vec2(-50, -50));	// ����뾶Ϊ20�� Ĭ����ɫΪ��ɫ
	ball->setAnchorPoint(Vec2(0.5, 0.5));
	ball->setPosition(point);
	addChild(ball);
	if (ifStart) mainBall = ball;
	ballVector.push_back(ball);
}

void Adventure::setLabel()
{
	labelForScore = cocos2d::Label::createWithTTF("0",
		"fonts/MarkerFelt.ttf",
		100);
	labelForScore->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(labelForScore);

	labelForBCBC = cocos2d::Label::createWithTTF(cocos2d::Value(maxCanContactCount).asString(),
		"fonts/MarkerFelt.ttf",
		30);
	labelForBCBC->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);
	labelForBCBC->setColor(cocos2d::Color3B::GREEN);
	addChild(labelForBCBC);
}

void Adventure::showScore()			
{
	if (ifEnd) labelForScore->setString("Game End!");
	else if (Good.asBool() && (score == 20 || score == 25)) {
		labelForScore->setString("Good!");
		if (voice) SimpleAudioEngine::getInstance()->playEffect("VoiceGood.mp3", false);
		Good = false;
	}
	else if (Great.asBool() && (score == 50 || score == 55)) {
		labelForScore->setString("Great!");
		if (voice) SimpleAudioEngine::getInstance()->playEffect("VoiceGreat.mp3", false);
		Great = false;
	}
	else if (Excellent.asBool() && (score == 90 || score == 95)) {
		labelForScore->setString("Excellent!");
		if (voice) SimpleAudioEngine::getInstance()->playEffect("VoiceExcellent.mp3", false);
		Excellent = false;
	}
	else if (Amazing.asBool() && (score == 150 || score == 155)) {
		labelForScore->setString("Amazing!");
		if (voice) SimpleAudioEngine::getInstance()->playEffect("VoiceAmazing.mp3", false);
		Amazing = false;
	}
	else if (Unbelievable.asBool() && (score == 350 || score == 355)) {
		labelForScore->setString("Unbelievable!");
		if (voice) SimpleAudioEngine::getInstance()->playEffect("VoiceUnbelievable.mp3", false);
		Unbelievable = false;
	}
	else {
		cocos2d::Value output(score);
		labelForScore->setString(output.asString());
	}
}

void Adventure::timeHandlerForshowScore(float dt)
{
	showScore();
	if (ifEnd) unschedule(schedule_selector(Adventure::timeHandlerForshowScore));
}

void Adventure::timeHandlerForMakeBall(float dt)
{
	if (ifStart) return;
	makeBall(mainBall->getPosition());
}

void Adventure::timeHandlerForUpadteScore(float dt)
{
	if (ifStart) return;
	score += 10;
}

// ������崩͸���ⷽ��
void Adventure::update(float delta)
{
	for (int i = 0; i < 3; ++i)
	{
		scene->getPhysicsWorld()->step(1 / 180.0f);
	}
}

// ��÷���
int Adventure::getScore() {
	return this->score;
}