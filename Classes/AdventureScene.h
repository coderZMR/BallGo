#ifndef __Adventure_SCENE_H__
#define __Adventure_SCENE_H__

#include "cocos2d.h"
#include "Ball.h"
#include "HelloScene.h"

class Adventure : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(int choice);

	Adventure();
	~Adventure();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	// ���ô�������
	void setListener();
	// ������ײ���
	void solveContactEvent();
	// ����С��
	void makeBall(cocos2d::Vec2 point);
	//// ����С���ʱ��
	void timeHandlerForMakeBall(float dt);
	//// ���������ʱ��
	void timeHandlerForUpadteScore(float dt);
	//// ��ʾ������ʱ��
	void timeHandlerForshowScore(float dt);
	// ����������ʾ��
	void setLabel();
	// ��ʾʵʱ����
	void showScore();	
	// ������崩͸���ⷽ��
	virtual void update(float delta);
	// ��÷���
	int getScore();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Adventure);

	// ���ؼ�ʹ��
	void pauseGame();
	// ������Ч��
	void afterCapture(bool succed, const std::string& outputFile);
	// ɾ�����ؼ���
	void timeForDeletepauseLayer(float dt);
	// ����ͼƬɾ��
	void timeForDeleteGetScreen();
	// �������ֺ����ص�
	void callBackForMusic(Ref *ref);
	// ������Ч�����ص�
	void callBackForVoice(Ref *ref);
	// �����𶯺����ص�
	void callBackForShake(Ref *ref);

	static cocos2d::Value choice;
private:

	int m[6];
	static cocos2d::Scene *scene;
	// �������
	cocos2d::DrawNode* lastLine[1000];
	cocos2d::Size visibleSize;
	cocos2d::Director *director;
	Ball *mainBall;
	std::vector<Ball*> ballVector;
	bool ifStart;
	// �������
    static int maxCanContactCount;
	static int ballContactBarrierCount;
	int score;
	cocos2d::Label *labelForScore;
	cocos2d::Label *labelForBCBC;	// ��ʾʣ�������ײ�ϰ���Ĵ���
	bool ifEnd;
	// ��Ϸ�����
	cocos2d::EventListenerTouchAllAtOnce *listener;
	cocos2d::EventListenerPhysicsContact* colloisionListener;
	static cocos2d::Value barrierNumber;
	static cocos2d::Value barRestitution;

	cocos2d::Value Good;
	cocos2d::Value Great;
	cocos2d::Value Excellent;
	cocos2d::Value Amazing;
	cocos2d::Value Unbelievable;

	cocos2d::LayerColor* pauseLayer;

	bool voice;
	bool shake;
	bool ifTouchBack;
	std::set<void*> allTimeHandler;
	cocos2d::LayerColor* GrayLayer;
	cocos2d::Sprite* st;

	cocos2d::Value signForMusic;
	cocos2d::Value signForVoice;
	cocos2d::Value signForShake;
};

#endif // __Adventure_SCENE_H__
