#pragma once

#include "CommonData.h"
#include "GameScene.h"
#include "RankScene.h"
#include <string>
#include <vector>
#include <algorithm>

class OverScene: public Layer
{
public:
	OverScene(int score);
	static Scene* createScene(int score);
	static OverScene* create(int score);
	static Scene* createWithScore(int score);
	bool init();
	void setScore(int score){o_score = score;};
	void writeScore();
	void readScore();
	void createBackground();
	void displayScore();
	void createMenu_Restart();
	void createMenu_Rank();
	//sort prescend
private:
	int o_score;
	vector<int> scores;
};