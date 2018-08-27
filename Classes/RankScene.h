#pragma once

#include "CommonData.h"
#include "GameScene.h"

class RankScene : public Layer
{
private:
	vector<int> r_scores;
public:
	RankScene();
	static Scene* createScene();
	static RankScene* create();
	bool init();
	void readScore();
	void createBackground();
	void createMenu_Restart();
	void displayRank();
};