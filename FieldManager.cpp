#include "FieldManager.h"
#include "TalkManager.h"
#include "BossManager.h"
#include "EnemyManager.h"
#include "AudioEngine.h"
#include "LinkedEnemy.h"
#include "E_PatternsLib.h"
#include "Enemy.h"

using namespace experimental;

C_Field_Manager* C_Field_Manager::m_pInstance = nullptr;

C_Field_Manager* C_Field_Manager::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_Field_Manager();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_Field_Manager::startEvent()
{
	/*
	(this->*m_arEventFuncs[m_nEventCount])();

	m_nEventCount++;
	*/

	schedule(schedule_selector(C_Field_Manager::updateToEvent), 0.1f);
}

bool C_Field_Manager::init()
{
	if (!Layer::init())
		return false;

	m_nEventCount	 = 0;
	m_pBossPar		 = nullptr;
	m_pBossShowAct	 = nullptr;
	m_pBossEndPar	 = nullptr;
	m_pBossEndAct	 = nullptr;
	m_pStageTitle	 = nullptr;
	m_pSubStageTitle = nullptr;
	m_isNowUsedUpdate = false;
	m_isNowBossPaze   = false;
	m_isNowMiddleBossPaze = false;

	return true;
}

void C_Field_Manager::preset()
{
	m_arEventFuncs[0] = &C_Field_Manager::callTalkEvent;
	m_arEventFuncs[1] = &C_Field_Manager::callBossEvent;
	m_arEventFuncs[2] = &C_Field_Manager::callBossEndEvent;

	m_pBossPar		 = ParticleSystemQuad::create("SpellAttack.plist");
	m_pBossEndPar	 = ParticleSystemQuad::create("BossEnd.plist");
	m_pStageTitle	 = Sprite::create("Stage_Title/Title.png");
	m_pSubStageTitle = Sprite::create("Stage_Title/SubTitle.png");

	m_pBossPar->setPosition(494.0f, 520.0f);
	m_pBossEndPar->setPosition(494.0f, 520.0f);
	m_pStageTitle->setPosition(494.0f, 454.0f);
	m_pSubStageTitle->setPosition(494.0f, 396.0f);

	m_pBossPar->setVisible(false);
	m_pBossEndPar->setVisible(false);
	
	m_pStageTitle->setOpacity(0);
	m_pSubStageTitle->setOpacity(0);

	addChild(m_pBossPar);
	addChild(m_pBossEndPar);
	addChild(m_pStageTitle);
	addChild(m_pSubStageTitle);
	createBossShowAct();
	createBossEndAct();
}

void C_Field_Manager::createBossShowAct()
{
	DelayTime* pDelay(nullptr);
	CallFunc* pCallBoss(nullptr);

	pDelay			= DelayTime::create(2.0f);
	pCallBoss		= CallFunc::create(CC_CALLBACK_0(C_Field_Manager::callBossStartPattern, this));
	m_pBossShowAct	= Sequence::create(pDelay, pCallBoss, nullptr);

	m_pBossShowAct->retain();
}

void C_Field_Manager::createBossEndAct()
{
	DelayTime* pDelay(nullptr);
	CallFunc*  pCallBoss(nullptr);

	pDelay = DelayTime::create(2.0f);
	pCallBoss = CallFunc::create(CC_CALLBACK_0(C_Field_Manager::callBossEnd, this));
	m_pBossEndAct = Sequence::create(pDelay, pCallBoss, nullptr);

	m_pBossEndAct->retain();
}

void C_Field_Manager::callTitleEvent()
{
	FadeIn*  pFadeInTitle(nullptr);
	FadeOut* pFadeOutSubTitle(nullptr);
	DelayTime* pTitleDelay(nullptr);
	DelayTime* pSubDelay(nullptr);

	pFadeInTitle	 = FadeIn::create(1.0f);
	pFadeOutSubTitle = FadeOut::create(1.0f);
	pTitleDelay		 = DelayTime::create(1.0f);
	pSubDelay		 = DelayTime::create(2.0f);

	m_pStageTitle->runAction(Sequence::create(pFadeInTitle, pTitleDelay, pSubDelay->clone(), pFadeOutSubTitle->clone(), nullptr));
	m_pSubStageTitle->runAction(Sequence::create(pTitleDelay, pFadeInTitle, pSubDelay, pFadeOutSubTitle, nullptr));
}

void C_Field_Manager::callTalkEvent()
{
	C_TalkManager::getInstance()->setEnabled(true);
	C_TalkManager::getInstance()->callTalkEvent();

	m_isNowBossPaze = true;
}

void C_Field_Manager::callMiddleBoss()
{
	C_EnemyManager* pManager(nullptr);
	C_Enemy* pEnemy(nullptr);

	pManager = C_EnemyManager::getInstance();
	pEnemy = pManager->checkEnemy(E_USE_TYPE::E_NOT_USED, E_ENEMY_TYPE::E_NORMAL, nullptr);
	pEnemy->setHP(1000.0f);
	pEnemy->setEnabled(true);
	pEnemy->setMiddleBoss(true);
	pEnemy->setPosition(494.0f, 740.0f);

	pManager->changeEnemy(pEnemy->getLinker(), E_USE_TYPE::E_USED);

	C_E_PatternLib::getInstance()->middleBoss(3, pEnemy);
}

void C_Field_Manager::callBossEvent()
{
	C_BossManager* pBoss(nullptr);

	pBoss = C_BossManager::getInstance();

	pBoss->setEnabled(true);

	m_pBossPar->setVisible(true);
	m_pBossPar->start();

	runAction(m_pBossShowAct);

	AudioEngine::play2d("se_ch02.mp3", false, 0.2f);
}

void C_Field_Manager::callBossStartPattern()
{
	C_BossManager* pBoss(nullptr);

	pBoss = C_BossManager::getInstance();

	pBoss->getBoss()->startPattern();
	C_EnemyManager::getInstance()->changeEnemy(pBoss->getBoss()->getLinker(), E_USE_TYPE::E_USED);
}

void C_Field_Manager::callBossEndEvent()
{
	runAction(m_pBossEndAct);
}

void C_Field_Manager::callBossEnd()
{
	m_pBossEndPar->setVisible(true);
	m_pBossEndPar->start();

	C_BossManager::getInstance()->setEnabled(false);
	C_BossManager::getInstance()->getBoss()->setImmortal(false);
	C_TalkManager::getInstance()->callEndTalkEvent();

	AudioEngine::play2d("se_enep01.mp3", false, 0.1f);
}

void C_Field_Manager::summonEnemy01(float fDelay)
{
	static int nTypeCount(0);
	C_EnemyManager* pManager(nullptr);
	C_Enemy* pEnemy(nullptr);

	if (nTypeCount > 12)
		nTypeCount = 0;

	pManager = C_EnemyManager::getInstance();

	pEnemy = pManager->checkEnemy(E_USE_TYPE::E_NOT_USED, E_ENEMY_TYPE::E_NORMAL, nullptr);

	pEnemy->setHP(200.0f);
	pEnemy->setEnabled(true);
	pEnemy->setPosition(10.0f, 720.0f);

	pManager->changeEnemy(pEnemy->getLinker(), E_USE_TYPE::E_USED);

	C_E_PatternLib::getInstance()->normalPattern01(nTypeCount > 10, pEnemy, false);

	nTypeCount++;
}

void C_Field_Manager::summonEnemy02(float fDelay)
{
	static int nTypeCount(0);
	C_EnemyManager* pManager(nullptr);
	C_Enemy* pEnemy(nullptr);

	if (nTypeCount > 12)
		nTypeCount = 0;

	pManager = C_EnemyManager::getInstance();
	pEnemy = pManager->checkEnemy(E_USE_TYPE::E_NOT_USED, E_ENEMY_TYPE::E_NORMAL, nullptr);
	pEnemy->setHP(200.0f);
	pEnemy->setEnabled(true);
	pEnemy->setPosition(1000.0f, 720.0f);

	pManager->changeEnemy(pEnemy->getLinker(), E_USE_TYPE::E_USED);

	C_E_PatternLib::getInstance()->normalPattern01(nTypeCount > 10, pEnemy, true);

	nTypeCount++;
}

void C_Field_Manager::summonEnemy03(float fDelay)
{
	C_EnemyManager* pManager(nullptr);
	C_Enemy* pEnemy(nullptr);

	pManager = C_EnemyManager::getInstance();
	pEnemy = pManager->checkEnemy(E_USE_TYPE::E_NOT_USED, E_ENEMY_TYPE::E_NORMAL, nullptr);

	pEnemy->setHP(500.0f);
	pEnemy->setPosition(random(-200.0f, 200.0f) + 494.0f, 720.0f);
	pEnemy->setEnabled(true);

	pManager->changeEnemy(pEnemy->getLinker(), E_USE_TYPE::E_USED);

	C_E_PatternLib::getInstance()->normalPattern02(2, pEnemy);
}

void C_Field_Manager::summonEnemy04(float fDelay)
{
	C_EnemyManager* pManager(nullptr);
	C_Enemy* pEnemy(nullptr);

	pManager = C_EnemyManager::getInstance();

	for (int nEnemy(0); nEnemy < 2; nEnemy++)
	{
		pEnemy = pManager->checkEnemy(E_USE_TYPE::E_NOT_USED, E_ENEMY_TYPE::E_NORMAL, nullptr);

		pEnemy->setHP(1000.0f);
		pEnemy->setPosition(285.0f + (nEnemy * 418.0f), 720.0f);
		pEnemy->setEnabled(true);

		pManager->changeEnemy(pEnemy->getLinker(), E_USE_TYPE::E_USED);

		C_E_PatternLib::getInstance()->normalPattern03(2, pEnemy);
	}
}

void C_Field_Manager::updateToEvent(float fDelay)
{
	if (m_isNowUsedUpdate || m_isNowMiddleBossPaze || m_isNowBossPaze)
		return;

	m_isNowUsedUpdate = true;
	/*
	if (m_nEventCount == 30)
	{
		schedule(schedule_selector(C_Field_Manager::summonEnemy01), 0.5f, 12, 0.5f);
	}

	if (m_nEventCount == 80)
	{
		schedule(schedule_selector(C_Field_Manager::summonEnemy02), 0.5f, 12, 0.5f);
	}

	if (m_nEventCount == 140)
		callTitleEvent();

	if (m_nEventCount == 210)
		schedule(schedule_selector(C_Field_Manager::summonEnemy03), 1.5f, 14, 0.0f);

	if (m_nEventCount == 450)
	{
		schedule(schedule_selector(C_Field_Manager::summonEnemy01), 0.5f, 12, 0.5f);
		schedule(schedule_selector(C_Field_Manager::summonEnemy02), 0.5f, 12, 0.5f);
	}

	if (m_nEventCount == 550)
	{
		schedule(schedule_selector(C_Field_Manager::summonEnemy04), 3.0f, 2, 0.0f);
	}

	if (m_nEventCount == 700)
	{
		m_isNowMiddleBossPaze = true;

		callMiddleBoss();
	}
	*/
	if (m_nEventCount == 10)
	{
		callTalkEvent();
		//_scheduler->pauseAllTargets();
	}

	m_nEventCount++;

	m_isNowUsedUpdate = false;
}
