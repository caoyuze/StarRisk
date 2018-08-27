#pragma once

//libraries and namespace
#include <cocos2d.h>
#include <SimpleAudioEngine.h>
using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

//Singleton Object
#define VisSize				Director::getInstance()->getVisibleSize()
#define Audio				SimpleAudioEngine::getInstance()
#define MyAnimationCache	AnimationCache::getInstance()
#define MyUserDefault		UserDefault::getInstance()
#define MyFrameCache		SpriteFrameCache::getInstance()

//tag
#define BG1_TAG					1
#define BG2_TAG					2
#define HERO_TAG				3
#define SL_TAG					4
#define PROP_TAG				5
#define MENU_TAG				6
#define ITEM_BOMB_TAG			7
#define LABEL_BOMB_TAG			8
#define RESTART_TAG				9
#define RESTARTMENU_TAG			10
#define FINALSCORELABEL_TAG		11
#define HIGHSCORE_TAG			12
#define RANK_TAG				13
#define RANKMENU_TAG			14
#define RANKNAME_TAG			15
#define HEROPH_TAG				16

//layer
#define BG_LAYER				-1
#define HERO_LAYER				0
#define BULLET_LAYER			1
#define ENEMY_LAYER				2
#define UI_LAYER				4

//hp
#define HP_SMALL				1
#define HP_MIDDLE				10
#define HP_BIG					30
#define HP_HERO					3

//speed (px)
#define SP_BG					2
#define SP_SMALL				4
#define SP_MIDDLE				1
#define SP_BIG					0.5f
#define SP_HERO_BULLET			5
#define SP_ENEMY_BULLET			1
#define SP_ENEMY_INCREMENT		0.1f

//delay (s)
#define HIT_DELAY				0.2f
#define ENEMY_DESTROY_DELAY		0.1f
#define HERO_DESTROY_DELAY		0.2f
#define BIG_FLY_DELAY			0.2f
#define HERO_FLY_DELAY			0.1f

//score lable off
#define SL_XOFF					20
#define SL_YOFF					20

//score
#define SCORE_SMALL				10
#define SCORE_MIDDLE			50
#define SCORE_BIG				100
#define SPEED_LEVELUP_SCORE		500

//create interval (s)
#define HERO_BULLET_INTERVAL	0.2f
#define ENEMY_BULLET_INTERVAL	1
#define SMALLENEMY_INTERVAL		0.8f
#define MIDDLEENEMY_INTERVAL	10
#define BIGENEMY_INTERVAL		20
#define PROP_INTERVAL			15

//repeate
#define REPEAT_FOREVER			-1

//create delay (s)
#define SMALLENEMY_DELAY		2
#define MIDDLEENEMY_DELAY		8
#define BIGENEMY_DELAY			14
#define PROP_DELAY				16

//limit
#define DOUBLEBULLET_NUMBER		40
#define MULTIBULLET_NUMBER		20
#define MAX_RANK_NUMBER			8
#define PROPTYPE_NUMBER			3
#define MAX_BOMB_NUMBER			10

//hero
#define HERO_ONE_HP				3

//attack
#define	BULLET1_ATTACK			1
#define BULLET2_ATTACK			2
#define BOMB_ATTACK				100
#define HERO_HIT_ATTACK			50