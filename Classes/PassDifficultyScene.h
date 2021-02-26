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
	// �����߿�
	void createRecGraphy();
	// ��������
	void createbackground();
	// ������ײ����
	void solveContactEvent();
	// ���ô�������
	void setListener();
	// ������ͼ
	void createMap();
	// �����߿�
	void createBoundary();
	// ����С��
	void makeMainBall();
	// �ͷ�������
	void timeHandlerForAddPunishBall(float dt);
	// ������崩͸���ⷽ��		
	virtual void update(float delta);
	// �����ӷ�����
	void drawStar(cocos2d::Vec2 pos, cocos2d::Color3B color);	// int pos, cocos2d::Color3B color
	// �����ƶ��ͷ�����
	void createMainPunishBall(cocos2d::Vec2 pos, bool ifCanMake);
	// �ı�������
	void timeForChangeBall(float dt);
	// ����ʣ��ʱ��
	void timeForCalculateLastTime(float dt);
	// ��ʱ��С��100ʱ��ʱ�䶯��
	void timeForTimeChange(float dt);
	// �ͷ�������ʧЧ��
	void timeForFadeOut(float dt);
	// �Ա߿����
	void timeHandlerForRectDrawNode(float dt);
	// ��ʾʣ��ʱ��
	void dynamicShowLastTime();
	// ��ʾ���������
	void dynamicshowNeedStar(cocos2d::Color3B color);
	// ��һ�ŵ�ͼ����
	void makeFirstMap();
	// �ڶ��ŵ�ͼ����
	void makeSecondMap();
	// �ڶ��ŵ�ͼ�ϰ�������Ч��
	void timeForHiddenBarrier(float dt);
	// �����ŵ�ͼ����
	void makeThirdMap();
	// �Ե����ŵ�ͼ�������ϰ�����ʾ
	void timeForShowAllBarrier(float dt);
	void timeForHiddenAllBarrier(float dt);
	// �����ŵ�ͼ����
	void makeFourthMap();
	// �����ŵ�ͼ����â�ǻ���
	cocos2d::Sprite* drawSixCorner(double triangleLength);
	// �Ե����ŵ�ͼ�������Ѿ���ʾ���ϰ�������
	void timeForHiddenAllShowedBarrier(float dt);
	// С���ͺ��Ч��
	void afterTransmit();
	// �����ŵ�ͼ����
	void makeFifthMap();
	// ���ؼ�ʹ��
	void pauseGame();
	// ������Ч��
	void afterCapture(bool succed, const std::string& outputFile);
	// ɾ�����ؼ���
	void timeForDeletepauseLayer(float dt);
	// ����ͼƬɾ��
	void timeForDeleteGetScreen();
	// �����ֺ����ص�
	void callBackForMusic(Ref *ref);
	// ����Ч�����ص�
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

	// ����ڵڶ��ŵ�ͼ
	cocos2d::Value ifFirstHidden;

	// ����ڵ�����������ŵ�ͼ
	cocos2d::Vector<Node*> allChild;

	// ����ڵ�����������ŵ�ͼ
	cocos2d::Vector<cocos2d::Sprite*> allSixCornerStartSprite;

	cocos2d::Value getSixCornerStarCount;
	
	// �ж��Ƿ���봫����
	cocos2d::Value ifGetDoor;

	// ���ڵ����ŵ�ͼ�Ĵ���λ��ѡ��
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
