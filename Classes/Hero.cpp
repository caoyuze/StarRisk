#include "Hero.h"

Hero* Hero::create(HeroType type)
{
	auto hero = new Hero();
	if(hero&& hero->init(type))
	{
		hero ->autorelease();
		return hero;
	}
	delete hero;
	hero = nullptr;
	return nullptr;
}

bool Hero::init(HeroType type)
{
	h_canMove = true;
	h_isActive = true;
	h_vec = Vect::ZERO;
	h_type = type;
	switch(type)
	{
	case HeroOne:
		this->Sprite::initWithSpriteFrameName("hero1.png");
		h_hp = HERO_ONE_HP;
		break;
	default:
		break;
	//set the position
	return true;
	}
}

void Hero::fly()
{
	auto animation = MyAnimationCache->getAnimation("hero_fly");
	auto animate = Animate::create(animation);
	this->runAction(animate);
}

void Hero::touchMove()
{
	//touch 
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [=](Touch* touch, Event*) 
	{
		//if the game is paused
		if(Director::getInstance()->isPaused() || !h_canMove)
		{
			return false;
		}

		this->h_vec = this->getPosition() - touch->getLocation();
		bool isContain = this->getBoundingBox().containsPoint(touch->getLocation());
		return isContain;
	};

	//the boundary of this moving
	float minX = this->getContentSize().width/2;
	float maxX = Director::getInstance()->getVisibleSize().width - minX;
	float minY = this->getContentSize().height/2;
	float maxY = Director::getInstance()->getVisibleSize().height - minY;

	//move
	touchListener->onTouchMoved = [=](Touch* touch, Event*) 
	{
		if(!h_canMove)
			return;
		auto desP = touch->getLocation() + this->h_vec;
		this->setPosition(MAX(minX,MIN(desP.x, maxX)), MAX(minY,MIN(desP.y, maxY)));
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event*)
	{
		//
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,this);

}

void Hero::destroyed()
{
	auto animation = MyAnimationCache->getAnimation("hero_destroy");
    auto animate = Animate::create(animation);
	this->runAction(animate);
}

void Hero::rebirthHero()
{
	h_hp--;
	auto stopHero = CallFunc::create([=]()
	{
		setActive(false);
		setMove(false);
	});
	auto animation = MyAnimationCache->getAnimation("hero_destroy");
	auto animate = Animate::create(animation);
	auto getBack = CallFunc::create([=]()
	{
		setPositionX(VisSize.width / 2);
		setPositionY(VisSize.height / 8);
	});
	auto activeHero = CallFunc::create([=]()
	{
		setActive(true);
	});
	auto moveHero = CallFunc::create([=]()
	{
		h_canMove = true;
	});
	auto blin = Blink::create(2, 4);
	this->runAction(Sequence::create(stopHero, animate, getBack, moveHero, blin, activeHero, nullptr));
}