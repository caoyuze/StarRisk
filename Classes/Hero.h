#pragma once

#include "CommonData.h"

enum HeroType
{
	HeroOne,
	HeroTwo,
	HeroThree
} ;

class Hero : public Sprite
{
private:
	HeroType h_type;
	int h_hp;
	bool h_canMove;
	bool h_isActive;
	Point h_vec;
public:
	static Hero* create(HeroType type);
	bool init(HeroType);
	int getHP() { return h_hp; };
	void setHP(int hp) { h_hp = hp; };
	void touchMove();
	void fly();
	void destroyed();
	void setActive(bool isActive) { h_isActive = isActive; };
	void setMove(bool move) { h_canMove = move; };
	bool getActive() { return h_isActive; };
	void rebirthHero();
	//void displayPH();
};