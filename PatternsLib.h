#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_Danmaku;

class C_Pattern_Library
{
public:
	static C_Pattern_Library* create();
	static inline C_Pattern_Library* getInstance() { return m_pInstance; }
public:
	virtual void circleNWay(Ref* pRef, const Vec2& vecShootPosition, const int& nWayCount, const float& fAddAngle, const float& fSpeed);
	virtual void followPlayer(const Vec2 & vecShootPosition, const int& nWayCount, const float& fAddAngle);
	virtual void followAndCircleNWay(const Vec2& vecShootPosition, const int& nWayCount, const float& fAddAngle);
	virtual void summon(const Vec2& vecSummonPosition);
	virtual void aimingBoom(const Vec2& vecSummonPosition);
	virtual void aimingBullet(const Vec2 vecSummonPosition);
	virtual void changeAngleByPlayer(Ref* pDanmaku, const Rect& recColor, const float& fSpeed);
	virtual void nWayByPlayer(const Vec2& vecSummonPosition, const int& nWay, const float& fSpeed);
	virtual void repeatCircleNWay
	(
		const Vec2 & vecSummonPosition,
		const int & nWay,
		const float & fSpeed,
		const int & nRepeat,
		const float& fDelay,
		Node* pActNode
	);
private:
	virtual void rain(const Rect& recColor, C_Danmaku* pTarget, const float& fSpeed);
private:
	virtual void pauseDanmaku(Ref* pTarget);
	virtual void disableDanmaku(Ref* pTarget);
public:
	virtual void angleShot(const float& fAngle, const float& fSpeed, C_Danmaku* pTarget);
private:
	virtual void init();
	virtual void preset();
private:
	class C_Danmaku_Manager* m_pDanmakuManager;
	class C_EnemyManager*	 m_pEnemyManager;
private:
	static C_Pattern_Library* m_pInstance;
private:
	C_Pattern_Library() {}
	virtual ~C_Pattern_Library() {}
	C_Pattern_Library(C_Pattern_Library&) = delete;
	C_Pattern_Library operator=(C_Pattern_Library&) = delete;
};