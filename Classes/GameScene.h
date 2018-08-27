#pragma once

#include "CommonData.h"
#include "OverScene.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Prop.h"
#include "Hero.h"

class GameScene : public Layer 
{
public:
	GameScene();
	static Scene* createScene();
	static GameScene* create();
	bool init();
	void update(float dt);

private:
	int m_score;
	int m_doubleBulletCount;
	int m_multiBulletCount;
	int m_bombCount;
	Vector<Bullet*> h_bullets;
	vector<Bullet*> removableBullets;
	void createBullet(float); 	
	void createSingleBullet();
	void createDoubleBullet();
	void createMultiBullet();

    Vector<Enemy*> h_enemies; 
	vector<Enemy*> removableEnemies;
    void createSmallEnemy1(float);
	void createSmallEnemy2(float);
	void createSmallEnemy2Group(float);
    void createMiddleEnemy(float);
    void createBigEnemy(float);

    void createEnemy(EnemyType);	
	//create enemy with Position (x,y)
	void createEnemy(EnemyType, Point);	

	void menuCloseCallback(Ref* pSender);
	void PauseAndResume(Ref* ref);
	void bomb(Ref* ref);

	//create prop
	Vector<Prop*> h_props;
	vector<Prop*> removableProps;
	void createProp(float);

	//create hero
	void createHero(HeroType htype);
	Vector<Sprite*> hero_hp;

	//create background
	void createBackground();
	void createFameCache();
	void playBackgroundMusic();
	void createMenu_PauseAndResume();
	void displayBombs();
	void displayScore();
	void updateHeroHP();
	void createSprites();
	void createHeroHP();
	void updateBomb();
	void updateSpeed();

	//
	void moveBackground();
	void flyBullets();
	void flyEnemys();
	void crashEnemyAndHeroAndBullet();
	void crashPropAndHero();
};