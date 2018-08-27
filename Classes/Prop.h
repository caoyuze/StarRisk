#pragma once

#include "CommonData.h"

enum PropType
{
	Enhance_Bullet,
	Bomb,
	Hp
};

class Prop : public Sprite 
{
private:
	PropType p_type;
public:
	Prop();
	static Prop* create(PropType type);
	bool init(PropType type);
	PropType getType(){return p_type;};
};