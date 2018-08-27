#include "LoadingScene.h"

LoadingScene::LoadingScene()
{
	
}

LoadingScene* LoadingScene::create()
{
	auto pp = new LoadingScene();
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

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();

	//"layer" is an autorelease object
	auto layer = LoadingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	//return the scene
	return scene;
}

bool LoadingScene::init()
{
	srand((unsigned)(time(NULL)));

	//preload backgroundmusic and effect
	preloadMusic();

	//frame cache
	createFrameCache();

	//animation cache
	createAnimationCache();

	//background
	createBackground();

	//the hero plane
	createHero(HeroOne);

	//loading animation
	createLoadingAnimation();

	return true;
}

void LoadingScene::preloadMusic()
{
	Audio->preloadBackgroundMusic("game_music.mp3");
	Audio->preloadEffect ("big_spaceship_fly.mp3");
	Audio->preloadEffect("bullet.mp3");
	for(int i = 0; i < 3; i++)
	{
		auto effectName = StringUtils::format("enemy%d_down.mp3", i+1);
	}
	Audio->preloadEffect("game_over.mp3");
	Audio->preloadEffect("get_bomb.mp3");
	Audio->preloadEffect("get_double_laser.mp3");
	Audio->preloadEffect("out_prop.mp3");
	Audio->preloadEffect("use_bomb.mp3");
}

void LoadingScene::createAnimationCache()
{
	//hero fly
	auto heroFly = Animation::create();
	heroFly->addSpriteFrame(MyFrameCache->getSpriteFrameByName("hero1.png"));
	heroFly->addSpriteFrame(MyFrameCache->getSpriteFrameByName("hero2.png"));

	heroFly->setDelayPerUnit(HERO_FLY_DELAY);
	heroFly->setLoops(REPEAT_FOREVER);
	MyAnimationCache->addAnimation(heroFly, "hero_fly");

	//big enemy fly
	auto bigEnemyFly = Animation::create();	
	bigEnemyFly->addSpriteFrame(MyFrameCache->getSpriteFrameByName("enemy3_n1.png"));
	bigEnemyFly->addSpriteFrame(MyFrameCache->getSpriteFrameByName("enemy3_n2.png"));

	bigEnemyFly->setDelayPerUnit(BIG_FLY_DELAY);
	bigEnemyFly->setLoops(REPEAT_FOREVER);
	MyAnimationCache->addAnimation(bigEnemyFly, "big_enemy_fly");

	//enemy hurt
	//middle enemy
	auto middleEnemyHurt = Animation::create();
    middleEnemyHurt->addSpriteFrame(MyFrameCache->getSpriteFrameByName("enemy2_hit.png"));
    middleEnemyHurt->addSpriteFrame(MyFrameCache->getSpriteFrameByName("enemy2.png"));
	middleEnemyHurt->setDelayPerUnit(HIT_DELAY);
	MyAnimationCache->addAnimation(middleEnemyHurt, "middle_enemy_hurt");

	//big enemy
	auto bigEnemyHurt = Animation::create();
    bigEnemyHurt->addSpriteFrame(MyFrameCache->getSpriteFrameByName("enemy3_hit.png"));
    bigEnemyHurt->addSpriteFrame(MyFrameCache->getSpriteFrameByName("enemy3_n1.png"));
	bigEnemyHurt->setDelayPerUnit(HIT_DELAY);
	MyAnimationCache->addAnimation(bigEnemyHurt, "big_enemy_hurt");

	//hero destroy
	auto heroDestroy = Animation::create();
	for(int i = 0; i < 4; i ++)
	{
            auto png = StringUtils::format("hero_blowup_n%d.png", i+1);
            heroDestroy->addSpriteFrame(MyFrameCache->getSpriteFrameByName(png));
	}
	heroDestroy->setDelayPerUnit(HERO_DESTROY_DELAY);
	MyAnimationCache->addAnimation(heroDestroy, "hero_destroy");

	//enemy destroy
	//small enemy destroy
	auto smallEnemyDown = Animation::create();
	for (int i = 0; i < 4; i++)
    {
        auto png = StringUtils::format("enemy1_down%d.png", i+1);
        smallEnemyDown->addSpriteFrame(MyFrameCache->getSpriteFrameByName(png));
    }
	smallEnemyDown->setDelayPerUnit(ENEMY_DESTROY_DELAY);
	//auto animate = Animate::create(animation);
	MyAnimationCache->addAnimation(smallEnemyDown, "small_enemy_destroy");
	
	//middle enemy destroy
	auto middleEnemyDown = Animation::create();
	for (int i = 0; i < 4; i++)
    {
        auto png = StringUtils::format("enemy2_down%d.png", i+1);
        middleEnemyDown->addSpriteFrame(MyFrameCache->getSpriteFrameByName(png));
    }   
	middleEnemyDown->setDelayPerUnit(ENEMY_DESTROY_DELAY);
	MyAnimationCache->addAnimation(middleEnemyDown, "middle_enemy_destroy");

	//big enemy destroy
	auto bigEnemyDown = Animation::create();
	for (int i = 0; i < 4; i++)
    {
        auto png = StringUtils::format("enemy3_down%d.png", i+1);
        bigEnemyDown->addSpriteFrame(MyFrameCache->getSpriteFrameByName(png));
    }   
	bigEnemyDown->setDelayPerUnit(ENEMY_DESTROY_DELAY);
	MyAnimationCache->addAnimation(bigEnemyDown, "big_enemy_destroy");
}

void LoadingScene::createFrameCache()
{
	MyFrameCache->addSpriteFramesWithFile("shoot.plist");
	MyFrameCache->addSpriteFramesWithFile("shoot_background.plist");
}

void LoadingScene::createBackground()
{
	//the first background picture
	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	bg1->setAnchorPoint(Point(0,0));
	bg1->getTexture()->setAliasTexParameters();
	this->addChild(bg1, BG_LAYER, BG1_TAG);
}

void LoadingScene::createHero(HeroType htype)
{
	auto bg1 = this->getChildByTag(BG1_TAG);
	auto hero = Hero::create(htype);
	hero->setPositionX(VisSize.width / 2);
	hero->setPositionY(VisSize.height / 6);
	hero->fly(); 
	this->addChild(hero, HERO_LAYER, HERO_TAG);
}

void LoadingScene::createLoadingAnimation()
{
	Vector<SpriteFrame* > frames;
	for (int i = 0; i < 4; i++)
	{
		auto frameName = StringUtils::format("game_loading%d.png", i+1);
		frames.pushBack(MyFrameCache->getSpriteFrameByName(frameName));
	}
	auto loadAni = Animation::createWithSpriteFrames(frames, 0.2f, 2);

	auto loading = Sprite::create();
	loading->setPosition(Point(VisSize / 2));
	this->addChild(loading);
	
	auto seq = Sequence::create(Animate::create(loadAni), CallFunc::create([]()
	{
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}), nullptr);

	loading->runAction(seq);	
}