#pragma once 

#include "CommonData.h"
#include "Prop.h"

//enemy type
enum EnemyType
{
    SmallEnemy1,  //ֱ��
	SmallEnemy2,  //����
    MiddleEnemy,
    BigEnemy,
	Boss
};

enum CurveDirection
{
	CLeft,
	CRight,
	CNone
};

enum LineDirection
{
	LLeft,
	LRight,
	LStraight
};

enum BossDirection
{
	LeftUp,
	LeftDown,
	RightUp,
	RightDown,
	Up,
	Down,
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

	//small enemy 2
	void move2();
	//others
	void move(float x, float y);

	bool canChangeLineDirection();
	//line direction visitor
	LineDirection getLineDirection() { return e_lineDirection; };
	void changeLineDirection();

	void updateBossDirection();

	static void updateLevelSpeed(int);
	static void clearLevelSpeed();

	//judge whether the enemy is out of screen
	bool beyondLimitX();
	bool beyondLimitY();

	bool bossBeyondLimitMinX();
	bool bossBeyondLimitMinY();
	bool bossBeyondLimitMaxX();
	bool bossBeyondLimitMaxY();

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

	LineDirection e_lineDirection;

	int cnt;

	BossDirection e_bossDirection;
};