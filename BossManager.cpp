#include "BossManager.h"
#include "EnemyManager.h"
#include "LinkedEnemy.h"
#include "Boss_HP_Bar.h"
#include "BossTimer.h"
#include "Boss_Spell_Count.h"
#include "AudioEngine.h"

using namespace experimental;

C_BossManager* C_BossManager::m_pInstance = nullptr;

C_BossManager * C_BossManager::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_BossManager();
	
	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_BossManager::setEnabled(const bool isEnabled)
{
	m_isEnabled = isEnabled;

	setVisible(m_isEnabled);
	m_pBoss->setEnabled(m_isEnabled);
	m_pHpBar->setEnabled(m_isEnabled);
	m_pTimer->setEnabled(m_isEnabled);
}

void C_BossManager::setVisibleToMageCircle(const bool isEnabled)
{
	if (isEnabled)
		createActByCircle();

	setVisible(isEnabled);
}

void C_BossManager::callSpellCard()
{
	DelayTime* pDelay(nullptr);
	MoveTo* pMove01(nullptr);
	MoveTo* pMove02(nullptr);
	MoveTo* pMove03(nullptr);
	Hide*	pHide(nullptr);

	pMove01 = MoveTo::create(0.5f, Vec2(599.0f, 361.0f));
	pMove02 = MoveTo::create(1.5f, Vec2(589.0f, 351.0f));
	pMove03 = MoveTo::create(0.5f, Vec2(-250.0f, -200.0f));
	pDelay  = DelayTime::create(1.0f);
	pHide	= Hide::create();

	m_pBossSCG->setPosition(1304.0f, 661.0f);
	m_pBossSCG->setVisible(true);
	m_pBossSCG->runAction(Sequence::create(pDelay, pMove01,pMove02, pMove03, pHide, nullptr));
}

void C_BossManager::callSpellCardTitle(const int nNumber)
{
	Spawn* pSpawn(nullptr);
	MoveTo* pMove(nullptr);
	FadeIn* pFadeIn(nullptr);
	ScaleTo* pScaleTo(nullptr);
	DelayTime* pDelay(nullptr);
	DelayTime* pDelayFirst(nullptr);
	CallFunc*  pCallSound(nullptr);

	pMove	 = MoveTo::create(0.5f, Vec2(922.0f, 642.0f));
	pDelay	 = DelayTime::create(1.5f);
	pDelayFirst = DelayTime::create(1.0f);
	pFadeIn  = FadeIn::create(0.25f);
	pScaleTo = ScaleTo::create(0.25f, 1.0f);
	pCallSound = CallFunc::create(CC_CALLBACK_0(C_BossManager::playSpellSound, this));
	pSpawn	 = Spawn::create(pDelay, pFadeIn, pScaleTo, pCallSound, nullptr);

	m_pSpells->setTextureRect(m_arSpellCards[nNumber]);
	m_pSpells->setPosition(922.0f, 142.0f);
	m_pSpells->setScale(4.0f);
	m_pSpells->setOpacity(0);
	m_pSpells->setVisible(true);
	m_pSpells->runAction(Sequence::create(pDelayFirst, pSpawn, pMove, nullptr));
}

void C_BossManager::outSpellCardTitle(const int nNumber)
{
	m_pSpells->setVisible(false);
	m_pSpells->setOpacity(0);
	m_pSpells->setPosition(-500.0f, -500.0f);
}

bool C_BossManager::init()
{
	if (!Layer::init())
		return false;

	m_pBoss  = nullptr;
	m_pHpBar = nullptr;
	m_pTimer = nullptr;
	m_pBossSCG = nullptr;

	return true;
}

void C_BossManager::preset()
{
	m_pName		= Sprite::create("Boss_Title.png");
	m_pCircle	= Sprite::create("Magic_Circle.png");
	m_pBossSCG	= Sprite::create("SCG/Ayeon_SCG.png");
	m_pSpells	= Sprite::create("SpellCards.png");

	m_pName->setAnchorPoint(Point(0.0f, 0.5f));
	m_pSpells->setAnchorPoint(Point(1.0f, 0.5f));

	m_pName->setPosition(65.0f, 672.5f);
	m_pBossSCG->setPosition(1104.0f, 661.0f);

	m_pBossSCG->setVisible(false);
	m_pSpells->setVisible(false);

	m_pCircle->setScale(0.01f);

	createHpBar();
	createTimer();
	createBoss();
	//createActByCircle();
	createSpellCounter();
	presetSpellCards();
	addChilds();

	schedule(schedule_selector(C_BossManager::updateCircle), 0.001f);
}

void C_BossManager::presetSpellCards()
{
	m_arSpellCards[0].setRect(0.0f, 0.0f, 225.0f, 22.0f);
	m_arSpellCards[1].setRect(0.0f, 23.0f, 225.0f, 22.0f);
	m_arSpellCards[2].setRect(0.0f, 46.0f, 225.0f, 22.0f);
}

void C_BossManager::createBoss()
{
	C_EnemyManager* pManager(C_EnemyManager::getInstance());
	C_Linked_Enemy* pNode(nullptr);

	m_pBoss = C_Boss::create();
	pNode = C_Linked_Enemy::create();

	pNode->setData(m_pBoss);
	pManager->addEnemy(pNode);

	m_pBoss->setLinker(pNode);
	m_pBoss->setType(E_ENEMY_TYPE::E_BOSS);
	m_pBoss->setTexture("Boss.png");
	m_pBoss->setEnabled(false);
}

void C_BossManager::createHpBar()
{
	m_pHpBar = C_Boss_HP_Bar::create();

	m_pHpBar->setEnabled(false);
}

void C_BossManager::createTimer()
{
	m_pTimer = C_Timer::create();

	m_pTimer->setEnabled(false);
}

void C_BossManager::createSpellCounter()
{
	m_pSpellCounter = C_SpellCounter::create();

	m_pSpellCounter->createSpellCount(5);
	m_pSpellCounter->setAlign(21.0f, 0.0f);
	m_pSpellCounter->setEnabled(true);
	m_pSpellCounter->setUsedCounter(1);
}

void C_BossManager::createActByCircle()
{
	ScaleTo* pScaleDown(nullptr);
	ScaleTo* pScaleUp(nullptr);
	Sequence* pSeq(nullptr);
	Repeat* pRep(nullptr);

	pScaleDown = ScaleTo::create(1.0f, 0.5f);
	pScaleUp   = ScaleTo::create(1.0f, 1.5f);
	pSeq = Sequence::create(pScaleUp, pScaleDown, nullptr);
	pRep = Repeat::create(pSeq, -1);

	m_pCircle->runAction(pRep);
}

void C_BossManager::playSpellSound()
{
	AudioEngine::play2d("se_cat00.mp3", false, 0.05f);
}

void C_BossManager::addChilds()
{
	addChild(m_pCircle);
	addChild(m_pHpBar);
	addChild(m_pTimer);
	addChild(m_pName);
	addChild(m_pBossSCG);
	addChild(m_pSpells);
	addChild(m_pSpellCounter);
}

void C_BossManager::updateCircle(float fDelay)
{
	static float fAngle(0.0f);

	if (fAngle >= 360.0f)
		fAngle = 0.0f;

	m_pCircle->setRotation(fAngle);
	m_pCircle->setPosition(m_pBoss->getPosition());

	fAngle += 0.3f;
}
