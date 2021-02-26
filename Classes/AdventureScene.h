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

	// 设置触摸监听
	void setListener();
	// 设置碰撞检测
	void solveContactEvent();
	// 创建小球
	void makeBall(cocos2d::Vec2 point);
	//// 产生小球计时器
	void timeHandlerForMakeBall(float dt);
	//// 计算分数计时器
	void timeHandlerForUpadteScore(float dt);
	//// 显示分数计时器
	void timeHandlerForshowScore(float dt);
	// 创建分数显示板
	void setLabel();
	// 显示实时分数
	void showScore();	
	// 解决刚体穿透问题方案
	virtual void update(float delta);
	// 获得分数
	int getScore();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Adventure);

	// 返回键使用
	void pauseGame();
	// 截屏后效果
	void afterCapture(bool succed, const std::string& outputFile);
	// 删除返回键层
	void timeForDeletepauseLayer(float dt);
	// 截屏图片删除
	void timeForDeleteGetScreen();
	// 开关音乐函数回调
	void callBackForMusic(Ref *ref);
	// 开关音效函数回调
	void callBackForVoice(Ref *ref);
	// 开关震动函数回调
	void callBackForShake(Ref *ref);

	static cocos2d::Value choice;
private:

	int m[6];
	static cocos2d::Scene *scene;
	// 动画相关
	cocos2d::DrawNode* lastLine[1000];
	cocos2d::Size visibleSize;
	cocos2d::Director *director;
	Ball *mainBall;
	std::vector<Ball*> ballVector;
	bool ifStart;
	// 分数相关
    static int maxCanContactCount;
	static int ballContactBarrierCount;
	int score;
	cocos2d::Label *labelForScore;
	cocos2d::Label *labelForBCBC;	// 显示剩余可以碰撞障碍物的次数
	bool ifEnd;
	// 游戏性相关
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
