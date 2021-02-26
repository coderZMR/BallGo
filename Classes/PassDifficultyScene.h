#ifndef __PassDifficulty_SCENE_H__
#define __PassDifficulty_SCENE_H__

#include "cocos2d.h"
#include "Ball.h"
#include "Star.h"




class PassDifficulty : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene(cocos2d::Value choice);

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(PassDifficulty);
	// 创建边框
	void createRecGraphy();
	// 创建底纹
	void createbackground();
	// 设置碰撞监听
	void solveContactEvent();
	// 设置触摸监听
	void setListener();
	// 创建地图
	void createMap();
	// 创建边框
	void createBoundary();
	// 设置小球
	void makeMainBall();
	// 惩罚球增加
	void timeHandlerForAddPunishBall(float dt);
	// 解决刚体穿透问题方案		
	virtual void update(float delta);
	// 创建加分项星
	void drawStar(cocos2d::Vec2 pos, cocos2d::Color3B color);	// int pos, cocos2d::Color3B color
	// 创建移动惩罚主球
	void createMainPunishBall(cocos2d::Vec2 pos, bool ifCanMake);
	// 改变主角球
	void timeForChangeBall(float dt);
	// 计算剩余时间
	void timeForCalculateLastTime(float dt);
	// 当时间小于100时，时间动画
	void timeForTimeChange(float dt);
	// 惩罚次球消失效果
	void timeForFadeOut(float dt);
	// 对边框控制
	void timeHandlerForRectDrawNode(float dt);
	// 显示剩余时间
	void dynamicShowLastTime();
	// 显示所需的星星
	void dynamicshowNeedStar(cocos2d::Color3B color);
	// 第一张地图创建
	void makeFirstMap();
	// 第二张地图创建
	void makeSecondMap();
	// 第二张地图障碍物消除效果
	void timeForHiddenBarrier(float dt);
	// 第三张地图创建
	void makeThirdMap();
	// 对第三张地图的所有障碍物显示
	void timeForShowAllBarrier(float dt);
	void timeForHiddenAllBarrier(float dt);
	// 第四张地图创建
	void makeFourthMap();
	// 第四张地图的六芒星绘制
	cocos2d::Sprite* drawSixCorner(double triangleLength);
	// 对第四张地图的所有已经显示的障碍物隐藏
	void timeForHiddenAllShowedBarrier(float dt);
	// 小球传送后的效果
	void afterTransmit();
	// 第五张地图创建
	void makeFifthMap();
	// 返回键使用
	void pauseGame();
	// 截屏后效果
	void afterCapture(bool succed, const std::string& outputFile);
	// 删除返回键层
	void timeForDeletepauseLayer(float dt);
	// 截屏图片删除
	void timeForDeleteGetScreen();
	// 关音乐函数回调
	void callBackForMusic(Ref *ref);
	// 关音效函数回调
	void callBackForVoice(Ref *ref);

	static cocos2d::Value choice;
private:
	static cocos2d::Camera *camera;
	static cocos2d::Director *director;
	static cocos2d::Size visibleSize;
	static cocos2d::Vec2 origin;
	static cocos2d::Scene *scene;

	Ball *ball;
	cocos2d::DrawNode *drawNode;
	bool ifGet;
	std::vector< cocos2d::RefPtr<Star> > starVector;

	static cocos2d::Value totalTime;
	cocos2d::Value currentUsedTime;
	cocos2d::Label *showLastTime;

	static cocos2d::Value totalStar;
	cocos2d::Value currentHadStar;
	cocos2d::Label *showNeedStar;

	cocos2d::Label *lastHundredTimeChange;
	cocos2d::Label *point;

	cocos2d::Value mapWidth;
	cocos2d::Value mapHeight;

	bool ifFirst;

	std::vector<Ball*> punishMainBall;
	std::vector<Ball*> movePunishBall;
	
	std::vector< cocos2d::RefPtr<Ball> > contactPunishBall;

	// 针对于第二张地图
	cocos2d::Value ifFirstHidden;

	// 针对于第三张与第四张地图
	cocos2d::Vector<Node*> allChild;

	// 针对于第四张与第五张地图
	cocos2d::Vector<cocos2d::Sprite*> allSixCornerStartSprite;

	cocos2d::Value getSixCornerStarCount;
	
	// 判断是否进入传送门
	cocos2d::Value ifGetDoor;

	// 对于第五张地图的传送位置选择
	cocos2d::Value choiceForNewPos;

	cocos2d::LayerColor* pauseLayer;

	bool voice;
	bool ifTouchBack;
	std::set<void*> allTimeHandler;
	cocos2d::LayerColor* GrayLayer;
	cocos2d::Sprite* st;

	cocos2d::Value signForMusic;
	cocos2d::Value signForVoice;
};

#endif // __PassDifficulty_SCENE_H__
