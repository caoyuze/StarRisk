#pragma once

#include "CommonData.h"
#include "GameScene.h"
#include "Hero.h"

class LoadingScene : public Layer
{
public:
	LoadingScene();
	static Scene* createScene();
	static LoadingScene* create();
	bool init();
	void preloadMusic();
	void createAnimationCache();
	void createFrameCache();
	void createBackground();
	void createHero(HeroType htype);
	void createLoadingAnimation();
private:

};