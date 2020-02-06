#include "PatternsLib.h"
#include "Danmaku.h"
#include "DanmakuManager.h"
#include "EnemyManager.h"
#include "Summoner.h"
#include "Character.h"
#include "E_PatternsLib.h"
#include "AudioEngine.h"

using namespace experimental;

C_Pattern_Library* C_Pattern_Library::m_pInstance = nullptr;

C_Pattern_Library * C_Pattern_Library::create()
{
	if (m_pInstance)
	{
		return nullptr;
	}

	m_pInstance = new(std::nothrow) C_Pattern_Library();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_Pattern_Library::circleNWay(Ref* pRef, const Vec2 & vecShootPosition, const int& nWayCount, const float& fAddAngle, const float& fSpeed)
{
	for (int nWay(0); nWay < nWayCount; nWay++)
	{
		C_Danmaku* pTarget(nullptr);

		pTarget = m_pDanmakuManager->popDanmaku(E_USE_TYPE::E_NOT_USED);
		m_pDanmakuManager->pushDanmaku(pTarget, E_USE_TYPE::E_USED);

		pTarget->setPosition(vecShootPosition);
		pTarget->setTextureRect(Rect(1.0f, 56.0f, 62.0f, 67.0f));
		pTarget->setSpeed(fSpeed);
		pTarget->setScale(0.5f);
		pTarget->setAngle((360.0f / (float)nWayCount) * (float)nWay + fAddAngle);
		pTarget->setEnabled(true);
	}

	AudioEngine::play2d("se_tan00.mp3", false, 0.01f);
}

void C_Pattern_Library::followPlayer(const Vec2 & vecShootPosition, const int& nWayCount, const float& fAddAngle)
{
	static int nSpeedCount(0);

	if (nSpeedCount % 80 == 0)
	{
		for (int nWay(0); nWay < nWayCount; nWay++)
		{
			C_Danmaku* pTarget(nullptr);
			CallFuncN* pFunc(nullptr);
			DelayTime* pDelay(nullptr);

			pTarget = m_pDanmakuManager->popDanmaku(E_USE_TYPE::E_NOT_USED);
			pFunc   = CallFuncN::create(CC_CALLBACK_1(C_Pattern_Library::changeAngleByPlayer, this, Rect(249.0f, 56.0f, 62.0f, 67.0f), random(1, 6)));
			pDelay  = DelayTime::create(2.0f);
			m_pDanmakuManager->pushDanmaku(pTarget, E_USE_TYPE::E_USED);

			pTarget->setPosition(vecShootPosition);
			pTarget->setTextureRect(Rect(1.0f, 56.0f, 62.0f, 67.0f));
			pTarget->setSpeed(0.5f);
			pTarget->setScale(0.5f);
			pTarget->setAngle((360.0f / (float)nWayCount) * (float)nWay + fAddAngle);
			pTarget->setEnabled(true);
			pTarget->runAction(Sequence::create(pDelay, pFunc, nullptr));
		}

		//AudioEngine::play2d("se_tan00.mp3", false, 0.01f);
	}

	nSpeedCount++;
}

void C_Pattern_Library::followAndCircleNWay(const Vec2 & vecShootPosition, const int & nWayCount, const float & fAddAngle)
{
	static int nSpeedCount(0);

	if (nSpeedCount % 100 == 0)
	{
		C_Danmaku* pTarget(nullptr);

		pTarget = m_pDanmakuManager->popDanmaku(E_USE_TYPE::E_NOT_USED);
		m_pDanmakuManager->pushDanmaku(pTarget, E_USE_TYPE::E_USED);

		circleNWay(pTarget, vecShootPosition, nWayCount, fAddAngle, 1.0f);
	}

	if (nSpeedCount % 10 == 0)
	{
		C_Danmaku* pTarget(nullptr);

		pTarget = m_pDanmakuManager->popDanmaku(E_USE_TYPE::E_NOT_USED);
		m_pDanmakuManager->pushDanmaku(pTarget, E_USE_TYPE::E_USED);

		pTarget->setPosition(vecShootPosition.x + random(-400, 400), vecShootPosition.y);
		pTarget->setScale(0.5f);
		pTarget->setEnabled(true);
		
		rain(Rect(249.0f, 56.0f, 62.0f, 67.0f), pTarget, 3.0f);

		AudioEngine::play2d("se_tan00.mp3", false, 0.01f);
		//changeAngleByPlayer(pTarget, Rect(249.0f, 56.0f, 62.0f, 67.0f), random(1, 3));
	}

	nSpeedCount++;
}

void C_Pattern_Library::summon(const Vec2 & vecSummonPosition)
{
	C_Enemy* pEnemy(nullptr);

	pEnemy = m_pEnemyManager->getEnemy(E_USE_TYPE::E_NOT_USED);

	pEnemy->setType(E_ENEMY_TYPE::E_SUMMONER);

	m_pEnemyManager->changeEnemy(pEnemy->getLinker(), E_USE_TYPE::E_USED);

	pEnemy->setPosition(vecSummonPosition.x + random(-400, 400), vecSummonPosition.y);
	pEnemy->setPatternType(5);
	pEnemy->setHP(300.0f);
	pEnemy->setEnabled(true);
	
	C_E_PatternLib::getInstance()->summoner(3, pEnemy);
}

void C_Pattern_Library::aimingBoom(const Vec2 & vecSummonPosition)
{
	C_Danmaku* pTarget(nullptr);
	DelayTime* pDelay(nullptr);
	CallFuncN* pFunc(nullptr);

	pTarget = m_pDanmakuManager->popDanmaku(E_USE_TYPE::E_NOT_USED);
	m_pDanmakuManager->pushDanmaku(pTarget, E_USE_TYPE::E_USED);

	pTarget->setPosition(vecSummonPosition);
	pTarget->setEnabled(true);
	pTarget->setScale(0.5f);

	changeAngleByPlayer(pTarget, Rect(4.0f, 0.0f, 56.0f, 56.0f), 2.0f);

	pDelay = DelayTime::create(1.5f);
	pFunc  = CallFuncN::create(CC_CALLBACK_1(C_Pattern_Library::pauseDanmaku, this));

	pTarget->runAction(Sequence::create(pDelay, pFunc, nullptr));
	AudioEngine::play2d("se_boon01.mp3", false, 0.1f);
}

void C_Pattern_Library::aimingBullet(const Vec2 vecSummonPosition)
{
	C_Danmaku* pTarget(nullptr);

	pTarget = m_pDanmakuManager->popDanmaku(E_USE_TYPE::E_NOT_USED);
	m_pDanmakuManager->pushDanmaku(pTarget, E_USE_TYPE::E_USED);

	pTarget->setPosition(vecSummonPosition);
	pTarget->setScale(0.5f);
	pTarget->setEnabled(true);

	changeAngleByPlayer(pTarget, Rect(4.0f + 56.0f * 4, 0.0f, 56.0f, 56.0f), 2.0f);

	AudioEngine::play2d("se_tan00.mp3", false, 0.01f);
}

void C_Pattern_Library::changeAngleByPlayer(Ref* pDanmaku, const Rect& recColor, const float& fSpeed)
{
	float fXMover(0.0f);
	float fYMover(0.0f);
	C_Danmaku* pTarget(nullptr);

	pTarget = (C_Danmaku*)pDanmaku;
	fXMover = C_Character::getInstance()->getPositionX();
	fYMover = C_Character::getInstance()->getPositionY();

	pTarget->setTextureRect(recColor);
	pTarget->setSpeed(fSpeed);
	pTarget->setAngle(atan2f(pTarget->getPositionY() - fYMover, fXMover - pTarget->getPositionX()) * 180.0f / 3.141592f);
}

void C_Pattern_Library::nWayByPlayer(const Vec2 & vecSummonPosition, const int & nWay, const float& fSpeed)
{
	C_Danmaku* pTarget(nullptr);
	float fAdder(0.0f);
	float fAddAngle(0.0f);

	fAdder = 90.0f / (float)nWay;

	for (int nWayCount(0); nWayCount < nWay; nWayCount++)
	{
		pTarget = m_pDanmakuManager->popDanmaku(E_USE_TYPE::E_NOT_USED);
		m_pDanmakuManager->pushDanmaku(pTarget, E_USE_TYPE::E_USED);

		pTarget->setPosition(vecSummonPosition);
		pTarget->setScale(0.5f);
		pTarget->setEnabled(true);

		changeAngleByPlayer(pTarget, Rect(60.0f, 0.0f, 56.0f, 56.0f), fSpeed);

		pTarget->setAngle(pTarget->getAngle() + fAddAngle - 45.0f);

		fAddAngle += fAdder;
	}
	
	AudioEngine::play2d("se_tan00.mp3", false, 0.01f);
}

void C_Pattern_Library::repeatCircleNWay(const Vec2 & vecSummonPosition, const int & nWay, const float & fSpeed, const int & nRepeat, const float& fDelay, Node* pActNode)
{
	CallFuncN*	pRepeatFunc(nullptr);
	DelayTime*	pDelay(nullptr);
	Sequence*	pSeqAct(nullptr);
	Repeat*		pRepeat(nullptr);

	pRepeatFunc = CallFuncN::create(CC_CALLBACK_1(C_Pattern_Library::circleNWay, this, vecSummonPosition, nWay, 0.0f, fSpeed));
	pDelay		= DelayTime::create(fDelay);
	pSeqAct		= Sequence::create(pRepeatFunc, pDelay, nullptr);
	pRepeat		= Repeat::create(pSeqAct, nRepeat);

	pActNode->runAction(pRepeat);
}

void C_Pattern_Library::rain(const Rect& recColor, C_Danmaku* pTarget, const float& fSpeed)
{
	pTarget->setTextureRect(recColor);
	pTarget->setAngle(90.0f);
	pTarget->setSpeed(fSpeed);
}

void C_Pattern_Library::pauseDanmaku(Ref * pTarget)
{
	ScaleTo*	pScaleUp(nullptr);
	CallFuncN*	pFunc01(nullptr);
	CallFuncN*	pFunc02(nullptr);
	DelayTime*	pDelay(nullptr);
	Sequence*	pSeq(nullptr);
	Repeat*		pRep(nullptr);
	C_Danmaku*  pDanmaku(nullptr);

	pDanmaku = (C_Danmaku*)pTarget;

	pDanmaku->setSpeed(0.0f);

	pScaleUp = ScaleTo::create(0.5f, 5.0f);
	pFunc01  = CallFuncN::create(CC_CALLBACK_1(C_Pattern_Library::circleNWay, this, pDanmaku->getPosition(), 10, 0.0f, 2.0f));
	pFunc02  = CallFuncN::create(CC_CALLBACK_1(C_Pattern_Library::disableDanmaku, this));
	pDelay	 = DelayTime::create(0.2f);
	pSeq	 = Sequence::create(pFunc01, pDelay, nullptr);
	pRep	 = Repeat::create(pSeq, 1);

	pDanmaku->runAction(Sequence::create(pScaleUp, pRep, pFunc02, nullptr));
}

void C_Pattern_Library::disableDanmaku(Ref* pTarget)
{
	C_Danmaku* pDanmaku(nullptr);

	pDanmaku = (C_Danmaku*)pTarget;

	pDanmaku->setEnabled(false);
	C_Danmaku_Manager::getInstance()->changeDanmaku(pDanmaku, (E_USE_TYPE)false);
}

void C_Pattern_Library::angleShot(const float& fAngle, const float& fSpeed, C_Danmaku * pTarget)
{
	pTarget->addPositionX(cosf(-fAngle * 3.141592 / 180.0f) * fSpeed);
	pTarget->addPositionY(sinf(-fAngle * 3.141592 / 180.0f) * fSpeed);
	pTarget->setRotation(fAngle - 90.0f);
}

void C_Pattern_Library::init()
{
	m_pDanmakuManager = nullptr;
	m_pEnemyManager	  = nullptr;
}

void C_Pattern_Library::preset()
{
	m_pDanmakuManager = C_Danmaku_Manager::getInstance();
	m_pEnemyManager   = C_EnemyManager::getInstance();
}
