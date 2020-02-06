#pragma once
#include "cocos2d.h"
#include "Enemy.h"

USING_NS_CC;

class C_Boss : public C_Enemy
{
public:
	static C_Boss* create();
	static inline C_Boss* getInstance() { return m_pInstance; }
public:
	virtual inline const bool getImmortal() { return m_isImmortal; }
	//virtual void setEnabled(const bool isEnabled) override;
	virtual void setHP(const float fHP) override;
	virtual inline void setMaxPaze(const int nPaze) { m_nMaxPaze = nPaze; }
public:
	virtual inline void setImmortal(const bool isImmortal) { m_isImmortal = true; }
	virtual inline void setPattern(const E_HP_TYPE& eType) { m_nNowHPType = (int)eType; }
	virtual void presetBoss(const E_BOSS& eType);
	virtual void startPattern();
	virtual void playFlyMotion();
private:
	virtual void pattern(float fDelay);
private:
	virtual void normalPattern01();
	virtual void normalPattern02();
	virtual void normalPattern03();
private:
	virtual void spell01Pattern01();
	virtual void spell01Pattern02();
	virtual void spell01Pattern03();
private:
	virtual void spell02Pattern01();
	virtual void spell02Pattern02();
	virtual void spell02Pattern03();
private:
	virtual void presetFuncList();
	virtual void outPattern();
	virtual void nextPattern();
	virtual void presetPaze();
	virtual void endPaze();
	virtual void bossUnImmortal(float fDelay);
private:
	virtual bool init() override;
	virtual void preset() override;
	virtual void onEnter() override;
private:
	bool m_isImmortal;
	bool m_isUpdate;
	bool m_isUsedSpell;
	bool m_arIsSpell[5]{false, true, false, true, true};
private:
	unsigned int m_nNowHPType;
	unsigned int m_nPaze;
	unsigned int m_nPattern;
	unsigned int m_nMaxPaze;
private:
	int m_nBossType;
	int m_nNowUsedSpell;
	int m_nCount;
private:
	ParticleSystemQuad* m_pBossPar;
	Sequence* m_pBossEndAct;
private:
	void(C_Boss::*m_arPatternFunc[9])();
private:
	static C_Boss* m_pInstance;
private:
	C_Boss();
	virtual ~C_Boss();
	C_Boss(C_Boss&) = delete;
	C_Boss operator=(C_Boss&) = delete;
};