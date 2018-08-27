#include "OverScene.h"

OverScene::OverScene(int score)
{
	o_score = score;
}

OverScene* OverScene::create(int score)
{
	auto pp = new OverScene(score);
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

Scene* OverScene::createWithScore(int score)
{
	auto scene = Scene::create();

	//"layer" is an autorelease object
	auto layer = OverScene::create(score);

	// add layer as a child to scene
	scene->addChild(layer);

	//return the scene
	return scene;
}



bool OverScene::init()
{
	//background
	createBackground();

	//score
	displayScore();

	//Restart
	createMenu_Restart();

	//Ranking
	createMenu_Rank();

	return true;
}

void OverScene::writeScore()
{
	if(MyUserDefault->getBoolForKey("isExist"))
	{
		MyUserDefault->setIntegerForKey("ScoreNumbers", scores.size());
		for(int i = 1; i <= scores.size(); i++)
		{
			MyUserDefault->setIntegerForKey(StringUtils::format("score_%dth", i).c_str(), scores.at(i-1));
		}
	}
	else 
	{
		log("Write scores failed");
	}
}

void OverScene::readScore()
{
	//xml exists
	if(MyUserDefault->getBoolForKey("isExist", false))
	{
		for( int i = 1; i <= MyUserDefault->getIntegerForKey("ScoreNumbers", 0); i++)
		{
			scores.push_back(MyUserDefault->getIntegerForKey(StringUtils::format("score_%dth", i).c_str(), 0));
		}
	}
	//xml not exists
	else
	{
		MyUserDefault->setBoolForKey("isExist", true);
		for(int i = 1; i <= MAX_RANK_NUMBER; i++)
		{
			MyUserDefault->setIntegerForKey(StringUtils::format("scores_%dth", i).c_str(), 0);
		}
	}
}

void OverScene::createBackground()
{
	auto bg = Sprite::createWithSpriteFrameName("gameover.png");
	bg->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
	bg->setPosition(0, VisSize.height);
	this->addChild(bg, BG1_TAG);
}

void OverScene::displayScore()
{
	auto scoreLable = Label::createWithBMFont("font.fnt","0");
	scoreLable->setAnchorPoint(Vect::ANCHOR_MIDDLE);
	scoreLable->setPosition(VisSize.width / 2, VisSize.height / 3);
	scoreLable->setColor(Color3B::BLACK);
	scoreLable->setScale(0.8);
	scoreLable->setString(StringUtils::format("%d", o_score));
	this->addChild(scoreLable, UI_LAYER, FINALSCORELABEL_TAG);

	readScore();
	scores.push_back(o_score);
	sort(begin(scores), end(scores), greater<int>());
	while(scores.size() > MAX_RANK_NUMBER)
	{ 
		scores.pop_back();
	}
	auto lblHighScore = Label::createWithBMFont("font.fnt", StringUtils:: format("%d",scores.front()));	
	lblHighScore->setPosition(Point(VisSize)/2 + Point(0, 200));
	lblHighScore->setColor(Color3B::BLACK);
	this->addChild(lblHighScore, UI_LAYER, HIGHSCORE_TAG);	
	writeScore();
}

void OverScene::createMenu_Restart()
{
	auto spRestart = Sprite::createWithSpriteFrameName("btn_finish.png");
	auto itemRestart = MenuItemSprite::create(spRestart, spRestart, [](Ref *) 
	{
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});
	auto menuRestart = Menu::create();
	menuRestart->addChild(itemRestart, UI_LAYER, RESTART_TAG);
	menuRestart->setPosition(VisSize.width-80, 40);
	this->addChild(menuRestart, UI_LAYER, RESTARTMENU_TAG);
}

void OverScene::createMenu_Rank()
{
	auto spRank = Sprite::create("Rank3.png");
	auto itemRank = MenuItemSprite::create(spRank, spRank, [](Ref*)
	{
		auto rankScene = RankScene::createScene();
		Director::getInstance()->replaceScene(rankScene);
	});
	auto menuRank = Menu::create();
	menuRank->addChild(itemRank, UI_LAYER, RANK_TAG);
	menuRank->setPosition(VisSize.width-80, VisSize.height-40);
	this->addChild(menuRank, UI_LAYER, RANKMENU_TAG);
}