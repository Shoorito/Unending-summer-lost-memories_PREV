#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_SpellCounter : public Layer
{
public:
	static C_SpellCounter* create();
	static inline C_SpellCounter* getInstance() { return m_pInstance; }
public:
	virtual inline const int& getCount()		{ return m_nCount;  }
	virtual inline const int& getNowSpellNum()	{ return m_nNowUsedCount; }
public:
	virtual void setSpell(const int nNumber, const bool isVisible);
	virtual void setAlign(const float fXpos, const float fYpos);
	virtual void setUsedCounter(const int nCount);
	virtual void setEnabled(const bool isEnabled);
public:
	virtual void createSpellCount(const int nCount);
private:
	virtual bool init() override;
	virtual void preset();
private:
	static C_SpellCounter* m_pInstance;
private:
	std::vector<Sprite*> m_vecSpellCount{};
private:
	Vec2 m_vec2Actpos;
private:
	Spawn* m_pRemoveAct;
private:
	int m_nCount;
	int m_nNowUsedCount;
private:
	C_SpellCounter() {}
	virtual ~C_SpellCounter() {}
	C_SpellCounter(C_SpellCounter&) = delete;
	C_SpellCounter operator=(C_SpellCounter&) = delete;
};