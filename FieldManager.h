#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_Field_Manager : public Layer
{
public:
	static C_Field_Manager* create();
	static inline C_Field_Manager* getInstance() { return m_pInstance; }
public:
	virtual void startEvent();
public:
	virtual inline const bool& isNowBossPaze() { return m_isNowBossPaze; }
public:
	virtual inline void setNowMiddleBossPaze(const bool isPaze) { m_isNowMiddleBossPaze = isPaze; }
private:
	virtual bool init() override;
	virtual void preset();
public:
	virtual void createBossShowAct();
	virtual void createBossEndAct();
	virtual void callTitleEvent();
	virtual void callTalkEvent();
	virtual void callMiddleBoss();
	virtual void callBossEvent();
	virtual void callBossStartPattern();
	virtual void callBossEndEvent();
	virtual void callBossEnd();
private:
	virtual void summonEnemy01(float fDelay);
	virtual void summonEnemy02(float fDelay);
	virtual void summonEnemy03(float fDelay);
	virtual void summonEnemy04(float fDelay);
private:
	virtual void updateToEvent(float fDelay);
private:
	int m_nEventCount;
private:
	ParticleSystemQuad* m_pBossPar;
	ParticleSystemQuad* m_pBossEndPar;
	Sequence* m_pBossShowAct;
	Sequence* m_pBossEndAct;
private:
	Sprite* m_pStageTitle;
	Sprite* m_pSubStageTitle;
private:
	bool m_isNowUsedUpdate;
	bool m_isNowBossPaze;
	bool m_isNowMiddleBossPaze;
private:
	void(C_Field_Manager::*m_arEventFuncs[3])();
private:
	static C_Field_Manager* m_pInstance;
private:
	C_Field_Manager() {}
	virtual ~C_Field_Manager() {}
	C_Field_Manager(C_Field_Manager&) = delete;
	C_Field_Manager operator=(C_Field_Manager&) = delete;
};