#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <time.h>
#include <list>

#define TAG_LABEL_TTF_SCORE 10000

using namespace cocos2d;
using namespace std;

struct ARROW
{
	Sprite* Arrow;
	MoveBy* MoveByArrow;

	int Arrow_Count;
	int Rand_Speed;
	int Rand_PosX;

	bool Arrow_bState;
	float ArrowPos_fX;
	float ArrowPos_fY;
};

enum Move_state
{
	eLeft,
	eRight,
};

class HelloWorld : public cocos2d::Scene
{
public:
    static Scene* createScene();
    virtual bool init();        
    void menuCloseCallback(Ref* pSender);    
    CREATE_FUNC(HelloWorld);

public:
	list <ARROW> list_arrow;
	list <ARROW>::iterator iter;
	ARROW Arrow_begin;

	int MoveState;
	int Pos_X;
	int Pos_Y;
	int frame;
	int score;
	int countTick;

	Layer* Layer_BackGround;
	Layer* Layer_StickMan;
	Layer* Layer_Arrow;
	Layer* Layer_ScoreText;
	Layer* Layer_NumText;
	Layer* Layer_GameOver;

	Texture2D* texture;
	Animation* animation;

	CallFunc* Call_Initialize;
	CallFunc* Call_ScoreFunc;
	CallFunc* Call_CreateArrow;
	CallFunc* Call_BackGround;
	CallFunc* Call_StickMan;
	CallFunc* Call_MoveStickMan;
	CallFunc* Call_MoveArrow;
	CallFunc* Call_ColPlayer;
	CallFunc* Call_KeyListener;

	Spawn* Spawn_Sprites;
	Sequence* Sequence_Init;
	Spawn* StickMan_Action;
	Spawn* LifeCycle;
	RepeatForever* PlayGame;

	MoveBy* MoveByStickMan;
	RotateTo* Arrow_AxisFix;

	Sprite* GameOver;
	Sprite* background;
	Sprite* StickMan;

	Label* ScoreText;
	//////////////////////////////////////////////////////
	MenuItemImage* pMenuItemPlay;
	
	Menu* pMenuPlay;

	void doClick(Ref* pSender);
	void gameStart();

	//////////////////////////////////////////////////////////////
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void Initialize();
	void ScoreFunc();
	void CreateArrow();
	void ResetArrow();
	void MoveArrow();
	void ColPlayer();
	void GameOverFunc();
	void CreateBackground();
	void CreateStickMan();
	void MoveStateStickMan();
	void KeyListener();
};

#endif // __HELLOWORLD_SCENE_H__
