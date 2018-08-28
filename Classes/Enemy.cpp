#include "Enemy.h"

float Enemy::e_LevelSpeed = 0;

Point Enemy::e_smallGroupCreatePosition = Point::ZERO;

CurveDirection Enemy::e_curveDirection = CNone;

Enemy::Enemy()
{
	e_type = SmallEnemy1;
	e_speed = 0;
	e_hp = 0;
	e_score = 0;
	e_bigFlySoundId = 0;
	e_canMove = true;
	e_lineDirection = LStraight;
	cnt = 0;
	e_bossDirection = None;
}

Enemy::~Enemy()
{
	//stop music
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
    case SmallEnemy1:
		e_hp = HP_SMALL;
		e_speed = SP_SMALL;      
		e_score = SCORE_SMALL;
		e_lineDirection = LStraight;
		Sprite::initWithSpriteFrameName("enemy1.png");
		break;
	case SmallEnemy2:
        e_hp = HP_SMALL;
		e_speed = SP_SMALL;      
		e_score = SCORE_SMALL;
		//e_lineDirection = lStraight;
        Sprite::initWithSpriteFrameName("enemy1.png");
		move2();  //curve animation
        break;
    case MiddleEnemy:
        e_hp = HP_MIDDLE;
		e_speed = SP_MIDDLE;
		e_score = SCORE_MIDDLE;
		e_lineDirection = (rand() % 2 == 0 ? LLeft : LRight);
        Sprite::initWithSpriteFrameName("enemy2.png");
        break;
    case BigEnemy:
        e_hp = HP_BIG;
		e_speed = SP_BIG;
		e_score = SCORE_BIG;
		e_lineDirection = (rand() % 2 == 0 ? LLeft : LRight);
		Sprite::initWithSpriteFrameName("enemy3_n1.png");
		{
			//fly animation
			auto animation = MyAnimationCache->getAnimation("big_enemy_fly");
			this->runAction(Animate::create(animation));
			//fly music
			e_bigFlySoundId = Audio->playEffect("big_spaceship_flying.mp3", true);
		}
		break;
	case Boss:
		e_hp = HP_BOSS;
		e_speed = SP_BOSS;  //Y offset
		e_score = SCORE_BOSS;
		e_bossDirection = Down;
		Sprite::initWithFile("boss.png");

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
    case SmallEnemy1:
	case SmallEnemy2:
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

void Enemy::move(float x, float y)
{
	if (e_canMove)
	{
		switch(e_type)
		{
		case SmallEnemy1:
		case MiddleEnemy:
		case BigEnemy:
			{
				float dx, dy;
				if (this->e_lineDirection != LStraight)
				{
					cnt++;
				}
				if (canChangeLineDirection())
				{
					changeLineDirection();
				}
				switch (e_lineDirection)
				{
				case LLeft:
					dx = -x;
					dy = -y;
					break;
				case LRight:
					dx = x;
					dy = -y;
					break;
				case LStraight:
					dx = 0;
					dy = -y;
					break;
				default:
					dx = 0;
					dy = 0;
					break;
				}
				this->setPosition(this->getPosition() + Point(dx, dy));
			}
			break;
		case Boss:
			float dx, dy;
			if (e_bossDirection != Up && e_bossDirection != Down)
			{
				updateBossDirection();
			}
			else
			{
				cnt++;
			}
			switch (e_bossDirection)
			{
			case LeftUp:
				dx = -x;
				dy = y;
				break;
			case LeftDown:
				dx = -x;
				dy = -y;
				break;
			case RightUp:
				dx = x;
				dy = y;
				break;
			case RightDown:
				dx = x;
				dy = -y;
				break;
			case Down:
				if (cnt >= (int)VisSize.height/3 + (int)this->getContentSize().height/2)
				{
					e_bossDirection = Up;
					cnt = 0;
					return;
				}
				else
				{
					dx = 0;
					dy = -1;
				}
				break;
			case Up:
				if (cnt >= (int)VisSize.height/6)
				{
					int randnum = rand() % 4;
					if (randnum == 0)
					{
						e_bossDirection = LeftUp;
					}
					else if (randnum == 1)
					{
						e_bossDirection = LeftDown;
					}
					else if (randnum == 2)
					{
						e_bossDirection == RightUp;
					}
					else if (randnum == 3)
					{
						e_bossDirection == RightDown;
					}
					cnt = 0;
					return;
				}
				else
				{
					dx = 0;
					dy = 1;
				}
				break;
			default:
				dx = 0;
				dy = 0;
				break;
			}
			this->setPosition(this->getPosition() + Point(dx, dy));
		case SmallEnemy2:
			break;
		default:
			break;
		}
	}
	else
	{
		return;
	}
}

void Enemy::move2()
{
	if (e_canMove)
	{
		if (e_type == SmallEnemy2)
		{
			auto w = VisSize.width, h = VisSize.height; 
			ccBezierConfig bezier;
			Point p1 = Point(w*3/4, h/3),
				p2 = Point(w/4, h/3),
				p3 = Point(w/8, h*2/3),
				p4 = Point(0, h),
				p5 = Point(w, h),
				p6 = Point(w*7/8, h*2/3),
				p7 = Point(w/4, h/5),
				p8 = Point(-this->getContentSize().width-10, h/5),
				p9 = Point(w*3/4, h/5),
				p10 = Point(w+this->getContentSize().width+10, h/5);
			if (e_curveDirection == CRight)
			{
				bezier.controlPoint_1 = p1;
				bezier.controlPoint_2 = p2;
				bezier.endPosition = p3;
				auto move1 = BezierTo::create(2.0f, bezier);
				bezier.controlPoint_1 = p4;
				bezier.controlPoint_2 = p5;
				bezier.endPosition = p6;
				auto move2 = BezierTo::create(2.0f, bezier);
				bezier.controlPoint_1 = p1;
				bezier.controlPoint_2 = p7;
				bezier.endPosition = p8;
				auto move3 = BezierTo::create(2.0f, bezier);
				this->runAction(Sequence::create(move1, move2, move3, NULL));
			}
			else if (e_curveDirection == CLeft)
			{
				bezier.controlPoint_1 = p2;
				bezier.controlPoint_2 = p1;
				bezier.endPosition = p6;
				auto move1 = BezierTo::create(2.0f, bezier);
				bezier.controlPoint_1 = p5;
				bezier.controlPoint_2 = p4;
				bezier.endPosition = p3;
				auto move2 = BezierTo::create(2.0f, bezier);
				bezier.controlPoint_1 = p2;
				bezier.controlPoint_2 = p9;
				bezier.endPosition = p10;
				auto move3 = BezierTo::create(2.0f, bezier);
				this->runAction(Sequence::create(move1, move2, move3, NULL));
			}
			e_canMove = false;
		}
	}
	else
	{
		return;
	}
}

void Enemy::changeLineDirection()
{
	switch (e_lineDirection)
	{
	case LLeft:
		e_lineDirection = LRight;
		break;
	case LRight:
		e_lineDirection = LLeft;
		break;
	case LStraight:
		break;
	default:
		break;
	}
	cnt = 0;
}

void Enemy::updateBossDirection()
{
	if (bossBeyondLimitMinX())
	{
		switch (e_bossDirection)
		{
		case LeftUp:
			e_bossDirection = RightUp;
			break;
		case LeftDown:
			e_bossDirection = RightDown;
			break;
		default:
			break;
		}
		if (bossBeyondLimitMinY())
		{
			e_bossDirection = RightUp;
		}
		else if (bossBeyondLimitMaxY())
		{
			e_bossDirection = RightDown;
		}
	}
	else if (bossBeyondLimitMaxX())
	{
		switch (e_bossDirection)
		{
		case RightUp:
			e_bossDirection = LeftUp;
			break;
		case RightDown:
			e_bossDirection = LeftDown;
			break;
		default:
			break;
		}
		if (bossBeyondLimitMinY())
		{
			e_bossDirection = LeftUp;
		}
		else if (bossBeyondLimitMaxY())
		{
			e_bossDirection = LeftDown;
		}
	}
	else if (bossBeyondLimitMinY())
	{
		switch (e_bossDirection)
		{
		case LeftDown:
			e_bossDirection = LeftUp;
			break;
		case RightDown:
			e_bossDirection = RightUp;
			break;
		default:
			break;
		}
	}
	else if (bossBeyondLimitMaxY())
	{
		switch (e_bossDirection)
		{
		case LeftUp:
			e_bossDirection = LeftDown;
			break;
		case RightUp:
			e_bossDirection = RightDown;
			break;
		default:
			break;
		}
	}
}

bool Enemy::canChangeLineDirection()
{
	return cnt == OFFSET_NUMBER || beyondLimitX();
}

bool Enemy::beyondLimitX()
{
	return this->getPositionX() + this->getContentSize().width/2 < 0 ||
		this->getPositionX() - this->getContentSize().width/2 > VisSize.width;
}

bool Enemy::beyondLimitY()
{
	return this->getPositionY() + this->getContentSize().height/2 < 0;
}

bool Enemy::bossBeyondLimitMinX()
{
	return this->getPositionX() - this->getContentSize().width/2 <= 0;
}

bool Enemy::bossBeyondLimitMaxX()
{
	return this->getPositionX() + this->getContentSize().width/2 >= VisSize.width;
}

bool Enemy::bossBeyondLimitMinY()
{
	return this->getPositionY() - this->getContentSize().height/2 <= VisSize.height/2;
}

bool Enemy::bossBeyondLimitMaxY()
{
	return this->getPositionY() + this->getContentSize().height/2 >= VisSize.height;
}