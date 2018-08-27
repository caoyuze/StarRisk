#pragma once 

#include "CommonData.h"
#include "Prop.h"

//enemy type
enum EnemyType
{
    SmallEnemy1,  //Ö±Ïß
	SmallEnemy2,  //ÇúÏß
    MiddleEnemy,
    BigEnemy
};

enum CurveDirection
{
	Left,
	Right,
	None
};

class Enemy : public Sprite 
{
public:
	Enemy();
	~Enemy();
    static Enemy* create(EnemyType);
    bool init(EnemyType);

    void hitAnim();
    void destroyedAnim();

	//type visitor
	EnemyType getType() { return e_type; };
	void setType(EnemyType type) { e_type = type; };

	//speed visitor
	float getSpeed() { return e_speed + e_LevelSpeed; };
	void setSpeed(float sp) { e_speed = sp; };

	//hp visitor
	int getHP() { return e_hp; };
	void setHP(int hp) { e_hp = hp < 0 ? 0 : hp; };

	//score visitor
	int getScore() { return e_score; };
	void setScore(int score) { e_score = score; };

	bool isDestroyed() { return e_hp <= 0; };

	void move(float x, float y);

	//curveDirection visitor
	//CurveDirection getCurveDirection() { return e_curveDirection; };
	//void setCurveDirection(CurveDirection curveDirection) { e_curveDirection = curveDirection; };

	static void updateLevelSpeed(int);
	static void clearLevelSpeed();

	static Point e_smallGroupCreatePosition;
	static CurveDirection e_curveDirection;

private:
    EnemyType e_type;

	int e_bigFlySoundId;

    float e_speed;

    int e_hp; 
	//score
	int e_score;
	//the prop that enemy catch
	PropType e_prop;

	static float e_LevelSpeed;

	bool e_canMove;
};