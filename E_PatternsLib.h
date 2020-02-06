#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_E_PatternLib
{
public:
	static C_E_PatternLib* create();
	static inline C_E_PatternLib* getInstance() { return m_pInstance; }
private:
	virtual void preset();
public:
	virtual void normalPattern01(const int nAnimType, class C_Enemy* pTarget, bool isFilp);
	virtual void normalPattern02(const int nAnimType, class C_Enemy* pTarget);
	virtual void normalPattern03(const int nAnimType, class C_Enemy* pTarget);
	virtual void middleBoss(const int nAnimType, class C_Enemy* pTarget);
	virtual void summoner(const int nAnimType, class C_Enemy* pTarget);
	virtual void unloadEnemy(Ref* pTarget);
private:
	Vec2 m_arControlPoint01[2]{ Vec2(250.0f, 400.0f), Vec2(750.0f, 400.0f) };
	Vec2 m_arControlPoint02[2]{ Vec2(750.0f, 400.0f), Vec2(250.0f, 400.0f) };
	Vec2 m_arControlEndPoint[2]{ Vec2(1000.0f, 720.0f), Vec2(0.0f, 720.0f) };
private:
	ccBezierConfig m_bezierConfig;
private:
	static C_E_PatternLib* m_pInstance;
private:
	C_E_PatternLib() {}
	virtual ~C_E_PatternLib() {}
	C_E_PatternLib(C_E_PatternLib&)			  = delete;
	C_E_PatternLib operator=(C_E_PatternLib&) = delete;
};