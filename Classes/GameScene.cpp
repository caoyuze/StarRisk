#include "GameScene.h"

GameScene::GameScene()
{
	m_score = 0;
	m_doubleBulletCount = 0;
	m_multiBulletCount = 0;
	m_bombCount = 0;
}

GameScene* GameScene::create()
{
	auto pp = new GameScene();
	if(pp && pp->init())
	{
		pp->autorelease();
		return pp;
	}
	else
	{
		delete pp;
		pp = NULL;
		return NULL;
	}
}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();

	//"layer" is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	//return the scene
	return scene;
}

bool GameScene::init()
{
	//super init first
	if(!Layer::init())
	{
		return false;
	}

	//Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	
	//frame cache
	createFameCache();
	Enemy::clearLevelSpeed();
	//background music
	playBackgroundMusic();

	//menu(pause and resume)
	createMenu_PauseAndResume();
	
	//display the bombs
	displayBombs();

	//background
	createBackground();

	//the hero plane
	createHero(HeroOne);

	//score
	displayScore();

	//hero hp
	createHeroHP();

	//sprites
	createSprites();
	
	//update the frame
	this->scheduleUpdate();

	return true;
}

void GameScene::update(float dt)
{
	//the moving of background
	moveBackground();
	updateHeroHP();
	//updateBomb();
	flyBullets();
	flyEnemys();

    //crash check
	crashEnemyAndHeroAndBullet();
	
	//prop check
	crashPropAndHero();
}

//Bullet
void GameScene::createBullet(float) 
{
	Audio->playEffect("bullet.mp3");
	(m_multiBulletCount > 0)?createMultiBullet():
		((m_doubleBulletCount>0)?createDoubleBullet():createSingleBullet());
}

void GameScene::createSingleBullet()
{
	auto hero = this->getChildByTag(HERO_TAG);
	auto bullet = Bullet::create(SingleBullet);
	bullet->setPosition(hero->getPosition() + Point(0,hero->getContentSize().height / 2));
	this->addChild(bullet, BULLET_LAYER);
	//push this bullet into vector
	h_bullets.pushBack(bullet);
}

void GameScene::createDoubleBullet()
{
	auto hero = this->getChildByTag(HERO_TAG);

	//left bullet
	auto leftBullet = Bullet::create(DoubleBullet);
	leftBullet->setPosition(hero->getPositionX()-hero->getContentSize().width/3,
		hero->getPositionY() + hero->getContentSize().height/3);
	this->addChild(leftBullet, BULLET_LAYER);
	this->h_bullets.pushBack(leftBullet);

	//right bullet
	auto rightBullet = Bullet::create(DoubleBullet);
	rightBullet->setPosition(hero->getPositionX()+hero->getContentSize().width/3,
		hero->getPositionY() + hero->getContentSize().height/3);
	this->addChild(rightBullet, BULLET_LAYER);
	this->h_bullets.pushBack(rightBullet);
	m_doubleBulletCount--;
}

void GameScene::createMultiBullet()
{
	auto hero = this->getChildByTag(HERO_TAG);
	//createSingleBullet();
	//createDoubleBullet();
	//=========================
	m_multiBulletCount--;
}


//Enemy
void GameScene::createEnemy(EnemyType type)
{
    auto enemy = Enemy::create(type);
    float leftMinX = enemy->getContentSize().width / 2;
    float rightMaxX = VisSize.width - leftMinX;
    float x = rand() % (int)(rightMaxX - leftMinX + 1) + leftMinX; 
    float y = VisSize.height + enemy->getContentSize().height/2;
    enemy->setPosition(x, y);
    h_enemies.pushBack(enemy);
    this->addChild(enemy, 0);
}

void GameScene::createSmallEnemy(float) 
{
    this->createEnemy(SmallEnemy);
}

void GameScene::createMiddleEnemy(float) 
{
    this->createEnemy(MiddleEnemy);
}

void GameScene::createBigEnemy(float) 
{
    this->createEnemy(BigEnemy);
}

//Prop
void GameScene::createProp(float)
{
	srand((unsigned)time(NULL));
	PropType ptype;
	auto randNumber = (int)rand()%PROPTYPE_NUMBER;
	switch(randNumber)
	{
	case 0: 
		ptype = Enhance_Bullet; 
		break;
	case 1: 
		ptype = Bomb; 
		break;
	case 2: 
		ptype = Hp; 
		break;
	default: 
		break;
	}
	auto prop = Prop::create(ptype);
	float minX = prop->getContentSize().width/2;
	float maxX = VisSize.width - minX;
	float x = rand()%(int)(maxX - minX + 1) + minX;
	prop->setPosition(x, VisSize.height + prop->getContentSize().height/2);
	this->addChild(prop, BULLET_LAYER);
	this->h_props.pushBack(prop);

	auto move1 = MoveBy::create(1, Point(0, -VisSize.height/3));
	auto move2 = MoveTo::create(2, 
		Point(prop->getPositionX(), -prop->getContentSize().height/2));
	auto sequence = Sequence::create(move1, move1->reverse(),
		move2, RemoveSelf::create(), nullptr);
	prop->runAction(sequence);
}

void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

//Pause and Resume
void GameScene::PauseAndResume(Ref* ref)
{
	auto toggle = (MenuItemToggle*) ref;
	auto hero = (Hero*)this->getChildByTag(HERO_TAG);
	if(toggle->getSelectedIndex() == 0)
	{
		Audio->playEffect("button.mp3");
		hero->setMove(true);
		Audio->resumeBackgroundMusic();
		Audio->resumeAllEffects();
		Director::getInstance()->resume();
	}
	else
	{
		Audio->playEffect("button.mp3");
		hero->setMove(false);
		Audio->pauseBackgroundMusic();
		Audio->pauseAllEffects();
		Director::getInstance()->pause();
	}
}

//use bomb
void GameScene::bomb(Ref* ref)
{
	if(Director::getInstance()->isPaused() || !m_bombCount)
	{
		return;
	}

	auto lblScore = (Label *)this->getChildByTag(SL_TAG);
	Audio->playEffect("use_bomb.mp3");

	m_bombCount--;
	updateBomb();

	for(auto enemy : h_enemies)
	{
		enemy->setHP(enemy->getHP()-BOMB_ATTACK);
		enemy->destroyedAnim();
		if(enemy->isDestroyed())
		{
			removableEnemies.push_back(enemy);
			m_score += enemy->getScore();
			lblScore->setString(StringUtils::format("%d", m_score));
			Enemy::updateLevelSpeed(m_score / SP_ENEMY_INCREMENT);
		}
	}
	//remove destroyed enemies
	for(auto enemy:removableEnemies)
	{
		h_enemies.eraseObject(enemy);
	}
	removableEnemies.clear();
	removableEnemies.shrink_to_fit();
	
}

void GameScene::createHero(HeroType htype)
{
	auto bg1 = this->getChildByTag(BG1_TAG);
	auto hero = Hero::create(htype);
	hero->setPositionX(VisSize.width / 2);
	hero->setPositionY(VisSize.height / 6);
	this->addChild(hero, HERO_LAYER, HERO_TAG);
	hero->fly();
	hero->touchMove();
}

void GameScene::createFameCache()
{
	MyFrameCache->addSpriteFramesWithFile("shoot.plist");
	MyFrameCache->addSpriteFramesWithFile("shoot_background.plist");
}

void GameScene::createBackground()
{
	//the first background picture
	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	bg1->setAnchorPoint(Point(0,0));
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BG_LAYER, BG1_TAG);

	//the second background picture
	auto bg2 = Sprite::createWithSpriteFrameName("background.png");
	bg2->setAnchorPoint(Point::ZERO);
	bg2->setPosition(0,bg1->getContentSize().height);
	bg2->getTexture()->setAliasTexParameters();
	this->addChild(bg2, BG_LAYER, BG2_TAG);
}

void GameScene::playBackgroundMusic()
{
	Audio->playBackgroundMusic("game_music.mp3", true);
}

void GameScene::createMenu_PauseAndResume()
{
		//pause
	auto spritePauseNormal = Sprite::createWithSpriteFrameName ("game_pause_nor.png");
	auto spritePausePressed = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto menuItemPause = MenuItemSprite::create(spritePauseNormal, spritePausePressed);

	//resume
	auto spriteResumeNormal = Sprite::createWithSpriteFrameName("game_resume_nor.png");
	auto spriteResumePressed = Sprite::createWithSpriteFrameName("game_resume_pressed.png");
	auto menuItemResume = MenuItemSprite::create(spriteResumeNormal, spriteResumePressed);
	
	//menuToggle
	auto menuItemToggle = MenuItemToggle::createWithCallback
		(CC_CALLBACK_1(GameScene::PauseAndResume, this), menuItemPause, menuItemResume, nullptr);
	menuItemToggle->setPosition(Point(VisSize -menuItemPause->getContentSize()));

	auto menu = Menu::create();
	menu->addChild(menuItemToggle);
	menu->setPosition(Vect::ZERO);
	this->addChild(menu, UI_LAYER, MENU_TAG);
}

void GameScene::displayBombs()
{
	auto menu = this->getChildByTag(MENU_TAG);
	auto spBomb = Sprite::createWithSpriteFrameName("bomb.png");
	auto itemBomb = MenuItemSprite::create(spBomb, spBomb, 
		CC_CALLBACK_1(GameScene::bomb, this));

	itemBomb->setPosition(Point(spBomb->getContentSize()));
	menu->addChild(itemBomb, UI_LAYER, ITEM_BOMB_TAG);
	itemBomb->setVisible(false);

	auto lblBomb = Label::createWithBMFont("font.fnt", "0");
	lblBomb ->setPosition(spBomb->getContentSize().width + lblBomb->getContentSize().width + 40, 
		spBomb->getContentSize().height);
	this->addChild(lblBomb, UI_LAYER, LABEL_BOMB_TAG);
	lblBomb->setColor(Color3B::BLACK);
	lblBomb->setString(StringUtils::format("X%d", m_bombCount));
	lblBomb->setVisible(false);
}

void GameScene::updateBomb()
{
	auto menu = this->getChildByTag(MENU_TAG);
	auto itemBomb = menu->getChildByTag(ITEM_BOMB_TAG);
	auto lblBomb = (Label*)this->getChildByTag(LABEL_BOMB_TAG);
	lblBomb->setString(StringUtils::format("X%d", m_bombCount));
	if(m_bombCount <=0)
	{
		itemBomb->setVisible(false);
		lblBomb->setVisible(false);
	}
	else if(m_bombCount == 1)
	{
		itemBomb->setVisible(true);
		lblBomb->setVisible(false);
	}
	else
	{
		itemBomb->setVisible(true);
		lblBomb->setVisible(true);
	}
}


void GameScene::displayScore()
{
	//the "score" tag
	auto label = LabelTTF::create("Score:", "Arial", 30);
	label->setPosition(SL_XOFF, VisSize.height-SL_YOFF);
	label->setAnchorPoint(Vect::ANCHOR_TOP_LEFT);
	label->setColor(Color3B::BLACK);
	this->addChild(label, UI_LAYER, 5);

	//the score lable
	auto scoreLable = Label::createWithBMFont("font.fnt","0");
	scoreLable->setAnchorPoint(Vect::ANCHOR_TOP_LEFT);
	scoreLable->setPosition(label-> getContentSize().width + SL_XOFF, VisSize.height-label->getContentSize().height/3*2);
	scoreLable->setColor(Color3B::BLACK);
	scoreLable->setScale(0.8f);
	this->addChild(scoreLable, UI_LAYER, SL_TAG);
}

void GameScene::createHeroHP()
{
	//the hero ph 
	for(int i = 0; i < HP_HERO; i++)
	{
		auto heroHP = Sprite::create("hero_hp.png");
		//heroPH->setScale(0.4f);
		heroHP->setPosition(Point(VisSize.width - (i+1)*(heroHP->getContentSize().width + 10), heroHP->getContentSize().height));	
		this->addChild(heroHP, UI_LAYER);
		hero_hp.pushBack(heroHP);
	}
}

void GameScene::updateHeroHP()
{
	auto hero =(Hero*) this->getChildByTag(HERO_TAG);
	for(int i = 0; i < HP_HERO; i++)
	{
		this->removeChild(hero_hp.at(i));
	}
	for(int i = 0; i < hero->getHP(); i++)
	{
		this->addChild(hero_hp.at(i));
	}
}

void GameScene::createSprites()
{
	//create bullets
	schedule(schedule_selector(GameScene::createBullet),HERO_BULLET_INTERVAL);

	//create enemies
    schedule(schedule_selector(GameScene::createSmallEnemy), SMALLENEMY_INTERVAL, REPEAT_FOREVER, SMALLENEMY_DELAY);
	schedule(schedule_selector(GameScene::createMiddleEnemy), MIDDLEENEMY_INTERVAL, REPEAT_FOREVER, MIDDLEENEMY_DELAY);
	schedule(schedule_selector(GameScene::createBigEnemy), BIGENEMY_INTERVAL, REPEAT_FOREVER, BIGENEMY_DELAY);

	//create prop
	schedule(schedule_selector(GameScene::createProp), PROP_INTERVAL, REPEAT_FOREVER, PROP_DELAY); 

}

void GameScene::moveBackground()
{
	auto bg1 = this->getChildByTag(BG1_TAG);
	auto bg2 = this->getChildByTag(BG2_TAG);

	//the moving of background
	bg1->setPosition(bg1->getPositionX(),bg1->getPositionY() - SP_BG);
	bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
	if(bg2->getPositionY()<0)
	{
		bg1->setPositionY(0);
	}
}

void GameScene::flyBullets()
{
	//display the bullet and clean bullets fly out
	for(auto bullet: h_bullets) 
	{		
		bullet->setPositionY(bullet->getPositionY() + SP_HERO_BULLET);
		if(bullet->getPositionY() >= VisSize.height)
		{
			removableBullets.push_back(bullet);
			this->removeChild(bullet);
		}
	}
	for(auto bullet: removableBullets)
	{
		h_bullets.eraseObject(bullet);
	}
	removableBullets.clear();
	removableBullets.shrink_to_fit();
}

void GameScene::flyEnemys()
{
	//fly the enemies
	for(auto enemy:h_enemies)
	{
        enemy->setPositionY(enemy->getPositionY() - enemy->getSpeed());
        //clean
        if(enemy->getPositionY() + enemy->getContentSize().height/2 <= 0) 
		{ 
			removableEnemies.push_back(enemy);
            this->removeChild(enemy);
        } 
    }
	for(auto enemy:removableEnemies)
	{
		h_enemies.eraseObject(enemy);
	}
	removableEnemies.clear();
	removableEnemies.shrink_to_fit();
}

void GameScene::crashEnemyAndHeroAndBullet()
{
	auto hero = (Hero*)this->getChildByTag(HERO_TAG);
	auto sl = (Label*)this->getChildByTag(SL_TAG);
	//crash check
    //march destroyed enemies
	for(auto enemy : h_enemies)
	{
		//hero and enemy
		if((enemy)->getBoundingBox().intersectsRect(hero->getBoundingBox()) && hero->getActive())
		{
			//enemy is hit by hero
			enemy->setHP(enemy->getHP() - HERO_HIT_ATTACK);
			enemy->destroyedAnim();
			if(enemy->isDestroyed()) 
			{ 
				removableEnemies.push_back(enemy);
			}

			if(hero->getHP() <= 1)
			{
				hero->setMove(false);
				hero->setActive(false);
				Audio->playEffect("game_over.mp3");
				auto animation = MyAnimationCache->getAnimation("hero_destroy");
				auto animate = Animate::create(animation);
				auto jumptoOver = CallFunc::create([=](){
					auto scene = OverScene::createWithScore(this->m_score);
					Director::getInstance()->replaceScene(scene);
				});
				hero->runAction(Sequence::create(animate, jumptoOver, nullptr));
			}
			else
			{
				hero->rebirthHero();
			}
		}
        //march destroyed bullets
        for(auto bullet : h_bullets)
		{
            //crash
            if(bullet->getBoundingBox().intersectsRect(enemy->getBoundingBox())) 
			{  
            //clean the bullet and set flag to ture
				removableBullets.push_back(bullet);
                this->removeChild((bullet));                                
				
				enemy->hitAnim();
				//get hurt
				enemy->setHP(enemy->getHP() - bullet->getAttack()); 

				if(enemy->isDestroyed()) 
				{ 
					//update the score
					m_score += enemy->getScore();
					//display the socre
					sl->setString(StringUtils::format("%d", m_score));
					enemy->destroyedAnim();    
					removableEnemies.push_back(enemy);
					Enemy::updateLevelSpeed(m_score / SPEED_LEVELUP_SCORE);
				}
            } 
        }       
		
    }

	for(auto bullet:removableBullets)
	{
		h_bullets.eraseObject(bullet);
	}
	removableBullets.clear();
	removableBullets.shrink_to_fit();

	for(auto enemy:removableEnemies)
	{
		h_enemies.eraseObject(enemy);
	}
	removableEnemies.clear();
	removableBullets.shrink_to_fit();
}

void GameScene::crashPropAndHero()
{
	auto hero = (Hero*)this->getChildByTag(HERO_TAG);
	//prop check
	for (auto prop : this->h_props)
	{ 
		if (prop->getBoundingBox().intersectsRect(hero->getBoundingBox()))
		{
			switch(prop->getType())
			{
			case Enhance_Bullet:
				Audio->playEffect("get_double_laser.mp3");
				(m_doubleBulletCount > 0)?(m_multiBulletCount = MULTIBULLET_NUMBER)
					:(m_doubleBulletCount = DOUBLEBULLET_NUMBER);
				break;
			case Bomb:
				Audio->playEffect("get_bomb.mp3");
				if(m_bombCount < MAX_BOMB_NUMBER)
				{
					m_bombCount++;
					updateBomb();
				}
				break;
			case Hp:
				if(hero->getHP() < HP_HERO)
				{
					hero->setHP(hero->getHP()+1);
				}
				break;
			default:
				break;
			}
			this->removeChild(prop);
			removableProps.push_back(prop);
		}

		if(prop->getPositionY() ==  -prop->getContentSize().height/2)
		{
			removableProps.push_back(prop);
		}
	}

	for(auto prop : removableProps) 
	{
		this->h_props.eraseObject(prop);
	}
	removableProps.clear();

}