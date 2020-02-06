#include "Boss.h"
#include "Boss_HP_Bar.h"
#include "BossManager.h"
#include "PatternsLib.h"
#include "BossTimer.h"
#include "AudioEngine.h"
#include "BG_Scroll.h"
#include "DanmakuManager.h"
#include "EnemyManager.h"
#include "FieldManager.h"
#include "Boss_Spell_Count.h"

using namespace experimental;

C_Boss* C_Boss::m_pInstance = nullptr;

C_Boss * C_Boss::create()
{
	if (m_pInstance)
	{
		return nullptr;
	}

	m_pInstance = new(std::nothrow) C_Boss();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}
/*
void C_Boss::setEnabled(const bool isEnabled)
{
	if (isEnabled == isWorked())
		return;

	C_Enemy::setEnabled(isEnabled);

	if (isWorked())
		resume();
	else
		pause();
}
*/
void C_Boss::presetBoss(const E_BOSS & eType)
{
	C_Boss_HP_Bar* pHPBar(nullptr);

	pHPBar = C_BossManager::getInstance()->getHPBar();

	pHPBar->setHPBar(519.0f, 150.0f);
	pHPBar->setMaxHP(5000.0f, E_HP_TYPE::E_NORMAL);
	pHPBar->setMaxHP(7000.0f, E_HP_TYPE::E_SPELL_01);
	pHPBar->setBarColor(E_HP_TYPE::E_NORMAL, Color3B::WHITE);
	pHPBar->setBarColor(E_HP_TYPE::E_SPELL_01, Color3B(244, 116, 61));

	setHP(pHPBar->getHP());
}

void C_Boss::startPattern()
{
	presetBoss(E_BOSS::E_AYEON);
	this->schedule(schedule_selector(C_Boss::pattern), 0.01f);
	C_Timer::getInstance()->startTimer(C_BossManager::getInstance()->getTimerTimes(0));
}

void C_Boss::pattern(float fDelay)
{
	if (m_isImmortal)
	{
		if (m_nCount >= 350)
		{
			m_isImmortal = false;
			m_nCount = 0;

			return;
		}

		m_nCount++;
	}
	else
	{
		if (C_Timer::getInstance()->getNowTime() <= 0)
		{
			setHP(0.0f);

			return;
		}

		(this->*m_arPatternFunc[m_nPattern])();
	}
}

void C_Boss::normalPattern01()
{
	static float fAngle(0.0f);
	static int nCount(0);

	if (fAngle >= 360.0f)
		fAngle = 0.0f;

	if(nCount % 50 == 0)
		C_Pattern_Library::getInstance()->circleNWay(nullptr, getPosition(), 12, fAngle, 1.0f);

	fAngle += 10.0f;
	nCount++;
}

void C_Boss::normalPattern02()
{
	static float fAngle(0.0f);

	if (fAngle >= 360.0f)
		fAngle = 0.0f;

	C_Pattern_Library::getInstance()->followAndCircleNWay(getPosition(), 10, fAngle);

	fAngle += 10.0f;
}

void C_Boss::normalPattern03()
{
}

void C_Boss::spell01Pattern01()
{
	static float fAngle(0.0f);

	if (fAngle >= 360.0f)
		fAngle = 0.0f;

	C_Pattern_Library::getInstance()->followPlayer(getPosition(), 20, fAngle);

	fAngle += 10.0f;
}

void C_Boss::spell01Pattern02()
{
	static int nCount(0);

	if (nCount % 300 == 0)
	{
		C_Pattern_Library::getInstance()->circleNWay(nullptr, getPosition(), 10, 0.0f, 2.5f);
	}

	if (nCount % 100 == 0)
	{
		C_Pattern_Library::getInstance()->summon(getPosition());
	}

	nCount++;
}

void C_Boss::spell01Pattern03()
{
}

void C_Boss::spell02Pattern01()
{
	static int nCount(0);

	if (nCount % 50 == 0)
	{
		C_Pattern_Library::getInstance()->aimingBoom(getPosition());
	}

	nCount++;
}

void C_Boss::spell02Pattern02()
{
	static int nCount(0);

	if (nCount % 200 == 0)
	{
		C_Pattern_Library::getInstance()->aimingBoom(getPosition());
	}

	nCount++;
}

void C_Boss::spell02Pattern03()
{
	//CC_CALLBACK_1(C_Pattern_Library::circleNWay, this);
}

void C_Boss::playFlyMotion()
{
	MoveBy* pMoveDown(nullptr);
	MoveBy* pMoveUp(nullptr);
	Sequence* pSeq(nullptr);
	Repeat* pRep(nullptr);

	pMoveDown = MoveBy::create(2.0f, Point(0.0f, -10.0f));
	pMoveUp = MoveBy::create(2.0f, Point(0.0f, 10.0f));
	pSeq = Sequence::create(pMoveDown, pMoveUp, nullptr);
	pRep = Repeat::create(pSeq, -1);
	
	this->runAction(pRep);
}

void C_Boss::presetFuncList()
{
	m_arPatternFunc[0] = &C_Boss::normalPattern01;
	m_arPatternFunc[1] = &C_Boss::spell01Pattern01;
	m_arPatternFunc[2] = &C_Boss::normalPattern02;
	m_arPatternFunc[3] = &C_Boss::spell01Pattern02;
	m_arPatternFunc[4] = &C_Boss::spell02Pattern01;
}

void C_Boss::outPattern()
{
	if (m_isUsedSpell)
	{
		C_Scroll_BG::getInstance()->outSpellCard();
		C_BossManager::getInstance()->outSpellCardTitle(m_nNowUsedSpell);
	}
	
	AudioEngine::play2d("se_tan00.mp3", false, 0.1f);
}

void C_Boss::nextPattern()
{
	if (m_arIsSpell[m_nPattern + 1])
	{
		C_Scroll_BG::getInstance()->callSpellCard();
		C_BossManager::getInstance()->callSpellCard();
		C_BossManager::getInstance()->callSpellCardTitle(m_nNowUsedSpell);

		m_isUsedSpell = true;
		m_nNowUsedSpell++;
	}
	
	C_Timer::getInstance()->stopTimer();
	C_Timer::getInstance()->startTimer(C_BossManager::getInstance()->getTimerTimes(m_nPattern + 1));
}

void C_Boss::presetPaze()
{
	C_Boss_HP_Bar* pHPBar(nullptr);
	C_SpellCounter* pSpellCounter(nullptr);

	pHPBar = C_BossManager::getInstance()->getHPBar();

	pHPBar->setHPBar(419.0f, 160.0f, 90.0f);
	pHPBar->setMaxHP(6000.0f, E_HP_TYPE::E_NORMAL);
	pHPBar->setMaxHP(10000.0f, E_HP_TYPE::E_SPELL_01);
	pHPBar->setMaxHP(12000.0f, E_HP_TYPE::E_SPELL_02);
	pHPBar->setBarColor(E_HP_TYPE::E_NORMAL, Color3B::WHITE);
	pHPBar->setBarColor(E_HP_TYPE::E_SPELL_01, Color3B(244, 116, 61));
	pHPBar->setBarColor(E_HP_TYPE::E_SPELL_02, Color3B(226, 30, 63));
	
	pSpellCounter = C_SpellCounter::getInstance();

	pSpellCounter->setSpell(pSpellCounter->getNowSpellNum() - 1, false);

	C_Enemy::setHP(pHPBar->getHP());
}

void C_Boss::endPaze()
{
	C_Scroll_BG::getInstance()->outSpellCard();

	AudioEngine::play2d("se_enep01.mp3", false, 0.1f);
}

void C_Boss::bossUnImmortal(float fDelay)
{
	m_isImmortal = false;
}

bool C_Boss::init()
{
	if (!C_Enemy::init())
		return false;

	m_isImmortal = false;
	m_isUpdate	 = false;
	m_isUsedSpell = false;
	m_nPaze		 = 0;
	m_nNowHPType = 0;
	m_nPattern	 = 0;
	m_nMaxPaze	 = 1;
	m_nCount	 = 0;
	m_nBossType  = 0;
	m_nNowUsedSpell = 0;

	return true;
}

void C_Boss::preset()
{
	//C_Enemy::preset();

	setPosition(494.0f, 520.0f);

	presetFuncList();
}

void C_Boss::onEnter()
{
	Node::onEnter();

	if (!isWorked())
		pause();
}

void C_Boss::setHP(const float fHP)
{
	if (m_isUpdate || m_isImmortal)
		return;

	C_Boss_HP_Bar* pHpBar(nullptr);

	m_isUpdate	= true;
	pHpBar		= C_BossManager::getInstance()->getHPBar();

	if (fHP <= 0.0f)
	{
		pHpBar->setHP(0.0f);

		C_EnemyManager::getInstance()->disabledAllEnemy(E_ENEMY_TYPE::E_SUMMONER);
		C_Danmaku_Manager::getInstance()->allDisabledDanmaku();

		m_nNowHPType++;

		if (m_nNowHPType == ((int)pHpBar->getHPBarType() + 1))
		{
			if (m_nPaze == m_nMaxPaze)
			{
				m_isImmortal = true;
				endPaze();
				//C_Enemy::setHP(0.1f);
				m_fHP = 0.0f;
				C_EnemyManager::getInstance()->changeEnemy(m_pMyLinker, E_USE_TYPE::E_NOT_USED);
				//C_BossManager::getInstance()->setEnabled(false);
				C_Field_Manager::getInstance()->callBossEndEvent();

				return;
			}

			presetPaze();

			m_nNowHPType = 0;
			m_isImmortal = true;
			m_nPaze++;
		}
		else
		{
			C_Enemy::setHP(pHpBar->getMaxHP((E_HP_TYPE&)m_nNowHPType));
		}

		outPattern();
		nextPattern();

		m_nPattern++;

		m_isImmortal = true;
		m_isUpdate	 = false;

		return;
	}
	else
	{
		C_Enemy::setHP(fHP);
	}

	pHpBar->setHP(fHP);

	m_isUpdate = false;
}

C_Boss::C_Boss()
{
}

C_Boss::~C_Boss()
{
}
