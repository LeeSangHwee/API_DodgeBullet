#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	 pMenuItemPlay = MenuItemImage::create(
		"MainButton_Play.png",
		"MainButton_Play.png",
		CC_CALLBACK_1(HelloWorld::doClick, this));

	pMenuItemPlay->setTag(1);

/*********************	초기 메뉴 관련 *********************/
	pMenuPlay = Menu::create(pMenuItemPlay, nullptr);
	pMenuPlay->setPosition(Point(360, 590));
	pMenuPlay->setAnchorPoint(Point(0.0, 0.0));
	this->addChild(pMenuPlay);

	pMenuPlay->setTag(11);

    return true;
}

void HelloWorld::doClick(Ref* pSender)
{
	auto pMenuPlayTag = (Menu*)getChildByTag(11);
	pMenuPlayTag->setVisible(false);
	gameStart();
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:   MoveState = eLeft;   break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: MoveState = eRight; break;
	}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void HelloWorld::Initialize()
{
	Pos_X = 300.0f, Pos_Y = 10.0f;
	MoveState = eRight;
	score = 0;

	Arrow_begin.Arrow = Sprite::create("Images/normal_arrow.png");

	Arrow_begin.Rand_Speed = 0;
	Arrow_begin.Rand_PosX = 0;

	Arrow_begin.Arrow_bState = false;
	Arrow_begin.ArrowPos_fX = 2000;
	Arrow_begin.ArrowPos_fY = 2000;

	Arrow_begin.Arrow->setAnchorPoint(Point::ZERO);
	Arrow_begin.Arrow->setPosition(Point(2000, 2000));
	Arrow_begin.Arrow->setScale(0.3f);

	Layer_Arrow->addChild(Arrow_begin.Arrow);

	for (int i = 0; i < 100; i++)
		list_arrow.push_back(Arrow_begin);

	iter = list_arrow.begin();
}

void HelloWorld::gameStart()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music_BGM.wav", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);

	Layer_BackGround = Layer::create();
	this->addChild(Layer_BackGround);

	Layer_StickMan = Layer::create();
	this->addChild(Layer_StickMan);

	Layer_Arrow = Layer::create();
	this->addChild(Layer_Arrow);

	Layer_ScoreText = Layer::create();
	this->addChild(Layer_ScoreText);

	Layer_NumText = Layer::create();
	this->addChild(Layer_NumText);

	Layer_GameOver = Layer::create();
	this->addChild(Layer_GameOver);

	Call_Initialize = CallFunc::create(CC_CALLBACK_0(HelloWorld::Initialize, this));
	Call_CreateArrow = CallFunc::create(CC_CALLBACK_0(HelloWorld::CreateArrow, this));
	Call_BackGround = CallFunc::create(CC_CALLBACK_0(HelloWorld::CreateBackground, this));
	Call_StickMan = CallFunc::create(CC_CALLBACK_0(HelloWorld::CreateStickMan, this));

	Spawn_Sprites = Spawn::create(Call_BackGround, Call_StickMan, NULL);
	Sequence_Init = Sequence::create(Call_Initialize, Spawn_Sprites, NULL);

	this->runAction(Sequence_Init);

	Call_ScoreFunc = CallFunc::create(CC_CALLBACK_0(HelloWorld::ScoreFunc, this));
	Call_MoveStickMan = CallFunc::create(CC_CALLBACK_0(HelloWorld::MoveStateStickMan, this));
	Call_ColPlayer = CallFunc::create(CC_CALLBACK_0(HelloWorld::ColPlayer, this));
	Call_KeyListener = CallFunc::create(CC_CALLBACK_0(HelloWorld::KeyListener, this));

	StickMan_Action = Spawn::create(Call_KeyListener, Call_MoveStickMan, NULL);
	LifeCycle = Spawn::create(Call_ScoreFunc, Call_CreateArrow, StickMan_Action, Call_ColPlayer, NULL);

	PlayGame = RepeatForever::create(LifeCycle);
	this->runAction(PlayGame);
}

void HelloWorld::ScoreFunc()
{
	Layer_ScoreText->removeChild(ScoreText);

	ScoreText = Label::createWithSystemFont("", "arial.ttf", 35);

	ScoreText->setAnchorPoint(Vec2::ZERO);
	ScoreText->setPosition(Vec2(30, 900));
	ScoreText->setColor(Color3B::BLACK);
	ScoreText->setString(StringUtils::format("SCORE : %d", score));

	Layer_ScoreText->addChild(ScoreText);
}

void HelloWorld::CreateArrow()
{
	frame++;

	if (frame >= 30)
	{
		if (iter != list_arrow.end())
		{
			if (iter->Arrow_bState == false)
			{
				iter->Arrow = Sprite::create("Images/normal_arrow.png");

				iter->Rand_Speed = ((rand() % 3) + 1);
				iter->Rand_PosX = (rand() % 630) + 30;

				iter->Arrow_bState = true;
				iter->ArrowPos_fX = (float)iter->Rand_PosX;
				iter->ArrowPos_fY = 1130.0f;

				iter->Arrow->setAnchorPoint(Point::ZERO);
				iter->Arrow->setPosition(Point(iter->ArrowPos_fX, iter->ArrowPos_fY));
				iter->Arrow->setScale(0.2f);

				Arrow_AxisFix = RotateTo::create(0.0f, 90.f);
				iter->Arrow->runAction(Arrow_AxisFix);

				Layer_Arrow->addChild(iter->Arrow);
				list_arrow.push_back(*iter);

				iter++;
				frame = 0;
			}
		}
	}
	MoveArrow();
	ResetArrow();
}

void HelloWorld::ResetArrow()
{
	for (list<ARROW>::iterator iterReset = list_arrow.begin(); iterReset != list_arrow.end(); ++iterReset)
	{
		if (iterReset->Arrow_bState == true)
		{
			if (iterReset->Arrow->getPositionY() <= -1.0f)
			{
				iterReset->Rand_Speed = ((rand() % 3) + 1);
				iterReset->Rand_PosX = (rand() % 630) + 30;

				iterReset->Arrow_bState = true;
				iterReset->ArrowPos_fX = (float)iterReset->Rand_PosX;
				iterReset->ArrowPos_fY = 1130.0f;

				iterReset->Arrow->setAnchorPoint(Point::ZERO);
				iterReset->Arrow->setPosition(Point(iterReset->ArrowPos_fX, iterReset->ArrowPos_fY));

				Arrow_AxisFix = RotateTo::create(0.0f, 90.f);
				iter->Arrow->runAction(Arrow_AxisFix);

				score += 10;
			}
		}
	}
}

void HelloWorld::MoveArrow()
{
	for (list<ARROW>::iterator iterMove = list_arrow.begin(); iterMove != list_arrow.end(); ++iterMove)
	{
		if (iterMove->Arrow_bState == true)
		{
			iterMove->MoveByArrow = MoveBy::create(0.1f, Point(0.0f, -(float)iterMove->Rand_Speed));
			iterMove->Arrow->runAction(iterMove->MoveByArrow);
		}
	}
}

void HelloWorld::ColPlayer()
{
	Rect Rect_StickMan = StickMan->getBoundingBox();

	for (list <ARROW>::iterator iter_Col = list_arrow.begin(); iter_Col != list_arrow.end(); ++iter_Col)
	{
		Rect Rect_Arrow = iter_Col->Arrow->getBoundingBox();

		if (Rect_StickMan.intersectsRect(Rect_Arrow))
		{
			background->pauseSchedulerAndActions();
			StickMan->pauseSchedulerAndActions();

			for (list <ARROW>::iterator iter_Pause = list_arrow.begin(); iter_Pause != list_arrow.end(); ++iter_Pause)
				iter_Pause->Arrow->pauseSchedulerAndActions();

			GameOverFunc();
		}
	}
}

void HelloWorld::GameOverFunc()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	GameOver = Sprite::create("Images/GameOver.png");
	GameOver->setAnchorPoint(Point::ZERO);
	GameOver->setPosition(Point(90, 270));

	Layer_GameOver->addChild(GameOver);
}

void HelloWorld::CreateBackground()
{
	background = Sprite::create("Images/BackGround.png");
	background->setAnchorPoint(Point::ZERO);
	background->setPosition(Point::ZERO);

	Layer_BackGround->addChild(background);
}

void HelloWorld::CreateStickMan()
{
	texture = Director::getInstance()->getTextureCache()->addImage("Images/StickMan/StickMan.png");
	animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	for (int i = 0; i < 5; i++)
	{
		int  col = i % 5;   int  row = i / 5;
		animation->addSpriteFrameWithTexture(texture, Rect(col * 127, row * 135, 127, 135));
	}

	StickMan = Sprite::createWithTexture(texture, Rect(0, 0, 127, 135));
	StickMan->setAnchorPoint(Point::ZERO);
	StickMan->setPosition(Point(Pos_X, Pos_Y));
	StickMan->setScale(0.5f);

	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);

	Layer_StickMan->addChild(StickMan);
	StickMan->runAction(rep);
}

void HelloWorld::MoveStateStickMan()
{
	if (MoveState == eLeft && StickMan->getPositionX() >= 60.0f)
	{
		MoveByStickMan = MoveBy::create(0.3f, Point(-5.0f, 0.0f));

		StickMan->setFlippedX(true);
		StickMan->runAction(MoveByStickMan);
	}
	else if (MoveState == eRight && StickMan->getPositionX() <= 600.0f)
	{
		MoveByStickMan = MoveBy::create(0.3f, Point(5.0f, 0.0f));

		StickMan->setFlippedX(false);
		StickMan->runAction(MoveByStickMan);
	}
}

void HelloWorld::KeyListener()
{
	auto keylistener = EventListenerKeyboard::create();

	keylistener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
}
