#include "Enemy.h"

float Enemy::e_LevelSpeed = 0;

Point Enemy::e_smallGroupCreatePosition = Point::ZERO;

CurveDirection Enemy::e_curveDirection = None;

Enemy::Enemy()
{
	e_type = SmallEnemy1;
	e_speed = 0;
	e_hp = 0;
	e_score = 0;
	e_bigFlySoundId = 0;
	e_canMove = true;
}

Enemy::~Enemy()
{
	//�������������ӵ���ը���ݻ��Լ�������Ļ����ֹͣ��ɻ���������
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

//���ݲ�ͬ�ĵл�������ͬ��ͼƬЧ��
bool Enemy::init(EnemyType type) 
{
    e_type = type;
    switch (type)
    {
    case SmallEnemy1:
	case SmallEnemy2:
        e_hp = HP_SMALL;
		e_speed = SP_SMALL;      
		e_score = SCORE_SMALL;
        Sprite::initWithSpriteFrameName("enemy1.png");
        break;
    case MiddleEnemy:
        e_hp = HP_MIDDLE;
		e_speed = SP_MIDDLE;
		e_score = SCORE_MIDDLE;
        Sprite::initWithSpriteFrameName("enemy2.png");
        break;
    case BigEnemy:
        e_hp = HP_BIG;
		e_speed = SP_BIG;
		e_score = SCORE_BIG;
		Sprite::initWithSpriteFrameName("enemy3_n1.png");
		{
			//���ж���
			auto animation = MyAnimationCache->getAnimation("big_enemy_fly");
			this->runAction(Animate::create(animation));
			//��������
			e_bigFlySoundId = Audio->playEffect("big_spaceship_flying.mp3", true);
		}
        break;
    default:
        break;
    }
    return true;
}

//���Ч��
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

//�ݻ�Ч��
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
		case SmallEnemy2:
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
					p8 = Point(-this->getContentSize().height, h/5),
					p9 = Point(w*3/4, h/5),
					p10 = Point(w+this->getContentSize().height, h/5);
				if (e_curveDirection == Right)
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
					/*
					auto array = CCPointArray::create(20);
					array->addControlPoint(Point(424, 500));
					array->addControlPoint(Point(345, 410));
					array->addControlPoint(Point(161, 425));
					array->addControlPoint(Point(163, 500));
					array->addControlPoint(Point(255, 575));
					array->addControlPoint(Point(344, 500));
					array->addControlPoint(Point(260, 358));
					array->addControlPoint(Point(0, 250));
					auto move = CCCardinalSplineTo::create(10, array, 10);;
					this->runAction(move);
					*/
				}
				else if (e_curveDirection == Left )
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
			break;
		case SmallEnemy1:
		case MiddleEnemy:
		case BigEnemy:
			{
 				this->setPosition(this->getPosition() + Point(x, y));
			}
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