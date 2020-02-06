#include "Summoner.h"
#include "PatternsLib.h"
#include "EnemyManager.h"
#include "LinkedEnemy.h"
#include "Enemy.h"

C_Summoner * C_Summoner::create()
{
	C_Summoner* pSummoner(nullptr);

	pSummoner = new(std::nothrow) C_Summoner();

	pSummoner->init();
	pSummoner->preset();

	return pSummoner;
}

void C_Summoner::startPattern()
{
	MoveTo* pMove(nullptr);
	CallFunc* pFunc(nullptr);
	
	pMove = MoveTo::create(2.0f, Vec2(getPositionX(), -50.0f));
	pFunc = CallFunc::create(CC_CALLBACK_0(C_Summoner::endPattern, this));

	runAction(Sequence::create(pMove, pFunc, nullptr));
}

bool C_Summoner::init()
{
	if (!C_Enemy::init())
		return false;

	return true;
}

void C_Summoner::preset()
{
	C_Enemy::preset();

	schedule(schedule_selector(C_Summoner::callPattern), 0.5f);
}

void C_Summoner::onEnter()
{
	C_Enemy::onEnter();
}

void C_Summoner::callPattern(float fDelay)
{
	C_Pattern_Library::getInstance()->circleNWay(nullptr, getPosition(), 10, 0.0f, 2.0f);
}

void C_Summoner::endPattern()
{
	C_EnemyManager::getInstance()->changeEnemy(getLinker(), E_USE_TYPE::E_NOT_USED);
	setEnabled(false);
}
