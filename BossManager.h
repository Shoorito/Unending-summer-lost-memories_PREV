#pragma once
#include "cocos2d.h"
#include "Boss.h"

USING_NS_CC;

class C_BossManager : public Layer
{
public:
	static C_BossManager* create();
	static C_BossManager* getInstance() { return m_pInstance; }
public:
	virtual inline class C_Boss_HP_Bar* getHPBar() { return m_pHpBar; }
	virtual inline const float& getTimerTimes(const int nNumber) { return m_arStage01BossTimer[nNumber]; }
	virtual inline C_Boss* getBoss() { return m_pBoss; }
public:
	virtual void setEnabled(const bool isEnabled);
	virtual void setVisibleToMageCircle(const bool isEnabled);
public:
	virtual void callSpellCard();
	virtual void callSpellCardTitle(const int nNumber);
	virtual void outSpellCardTitle(const int nNumber);
private:
	virtual bool init();
	virtual void preset();
	virtual void presetSpellCards();
	virtual void createBoss();
	virtual void createHpBar();
	virtual void createTimer();
	virtual void createSpellCounter();
	virtual void createActByCircle();
	virtual void playSpellSound();
	virtual void addChilds();
private:
	virtual void updateCircle(float fDelay);
private:
	class C_Boss*			m_pBoss;
	class C_Timer*			m_pTimer;
	class C_Boss_HP_Bar*	m_pHpBar;
	class C_SpellCounter*   m_pSpellCounter;
private:
	Sprite* m_pName;
	Sprite* m_pCircle;
	Sprite* m_pBossSCG;
	Sprite* m_pSpells;
private:
	Rect m_arSpellCards[3]{};
	float m_arStage01BossTimer[5]{ 50.0f, 60.0f, 55.0f, 50.0f, 60.0f };
private:
	bool m_isEnabled;
private:
	static C_BossManager* m_pInstance;
private:
	C_BossManager() {}
	virtual ~C_BossManager() {}
	C_BossManager(C_BossManager&) = delete;
	C_BossManager operator=(C_BossManager&) = delete;
};