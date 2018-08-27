#include "Enemy.h"

float Enemy::e_LevelSpeed =  0;

Enemy::Enemy()
{
	e_type = SmallEnemy;
	e_speed = 0;
	e_hp = 0;
	e_score = 0;
	e_bigFlySoundId = 0;
}

Enemy::~Enemy()
{
	//对象被析构（被子弹、炸弹摧毁以及超出屏幕）后停止大飞机飞行音乐
	if (e_type == BigEnemy)
	{
		Audio->stopEffect(e_bigFlySoundId);
	}
}

Enemy* Enemy::create(EnemyType type) 
{
    auto enemy = new Enemy();
    if(enemy && enemy->init(type)) 
	{
        enemy->autorelease();
        return enemy;
    }
    delete enemy;
    enemy = NULL;
    return NULL;
}

//根据不同的敌机建立不同的图片效果
bool Enemy::init(EnemyType type) 
{
    e_type = type;
    switch (type)
    {
    case  SmallEnemy:
        e_hp = HP_SMALL;
		e_speed = SP_SMALL;      
		e_score = SCORE_SMALL;
        Sprite::initWithSpriteFrameName("enemy1.png");
        break;
    case  MiddleEnemy:
        e_hp = HP_MIDDLE;
		e_speed = SP_MIDDLE;
		e_score = SCORE_MIDDLE;
        Sprite::initWithSpriteFrameName("enemy2.png");
        break;
    case  BigEnemy:
        e_hp = HP_BIG;
		e_speed = SP_BIG;
		e_score = SCORE_BIG;
		Sprite::initWithSpriteFrameName("enemy3_n1.png");
		{
			//飞行动画
			auto animation = MyAnimationCache->getAnimation("big_enemy_fly");
			this->runAction(Animate::create(animation));
			//飞行音乐
			e_bigFlySoundId = Audio->playEffect("big_spaceship_flying.mp3", true);
		}
        break;
    default:
        break;
    }
    return true;
}

//打击效果
void Enemy::hitAnim() 
{ 
    auto animation = Animation::create();
    switch (this->e_type)
    {
    case MiddleEnemy:
		animation = MyAnimationCache->getAnimation("middle_enemy_hurt");
		break;
    case BigEnemy:
		animation = MyAnimationCache->getAnimation("big_enemy_hurt");
        break;
    default:
        break;
    }
    auto animate = Animate::create(animation);
    this->runAction(animate);
}   

//摧毁效果
void Enemy::destroyedAnim() 
{
    auto animation = Animation::create();
    switch(e_type) 
	{
    case SmallEnemy:       
		animation=MyAnimationCache->getAnimation("small_enemy_destroy");
		Audio->playEffect("enemy1_down.mp3");
        break;
    case MiddleEnemy:
		animation = MyAnimationCache->getAnimation("middle_enemy_destroy");
		Audio->playEffect("enemy2_down.mp3");
        break;
    case BigEnemy:
		animation = MyAnimationCache->getAnimation("big_enemy_destroy");
		Audio->playEffect("enemy3_down.mp3");
        break;
    default:
        break;
    }
    auto animate = Animate::create(animation);
    auto callFuncN = CallFuncN::create([=](Node* node) 
	{
		node->removeFromParentAndCleanup(true);
    });

	if (isDestroyed())
	{
		this->runAction(Sequence::create(animate, callFuncN, NULL));
	}
	else
	{
		this->runAction(animate);
	}
}

void Enemy::updateLevelSpeed(int multi)
{
	if(Enemy::e_LevelSpeed >= 10)
	{
		return;
	}
	Enemy::e_LevelSpeed = SP_ENEMY_INCREMENT * multi;
}

void Enemy::clearLevelSpeed()
{
	Enemy::e_LevelSpeed = 0; 
}