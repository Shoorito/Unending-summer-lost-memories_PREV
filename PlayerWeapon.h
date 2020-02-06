#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_Enemy;
class C_Linked_Weapon;
enum class E_WEAPON_TYPE;

// 본 노드는 서브 웨폰 사용 시에만 생성합니다.
// 노드를 따로 사용하는 이유는 불필요한 메모리양을 증가 시키지 않기 위함입니다.
struct S_SubWeaponNode
{
	void init()
	{
		fDistance = 0.0f;
		fDistanceX = 0.0f;
		fDistanceY = 0.0f;
		fDirectionAngle = 0.0f;
		fTargetAngle = 0.0f;
		fTargetXpos = 0.0f;
		fTargetYpos = 0.0f;
		fAngleDifference = 0.0f;
		fHomingAngle = 0.0f;
	}

	float fDistance			= 0.0f;
	float fDistanceX		= 0.0f;
	float fDistanceY		= 0.0f;
	float fDirectionAngle	= 0.0f;
	float fTargetAngle		= 0.0f;
	float fTargetXpos		= 0.0f;
	float fTargetYpos		= 0.0f;
	float fAngleDifference	= 0.0f;
	float fHomingAngle		= 0.0f;
};

class C_PlayerWeapon
{
public:
	static C_PlayerWeapon* create(E_WEAPON_TYPE eType);
public:
	virtual inline Layer* getLayer() { return m_pWeaponLayer; }
public:
	virtual inline void setId(const int nId) { m_nId = nId; }
	virtual inline const int getId() { return m_nId; }
	virtual inline void setType(const E_WEAPON_TYPE eType) { m_eWeaponType = eType; }
	virtual inline E_WEAPON_TYPE getType() { return m_eWeaponType; }
public:
	virtual void setScale(const float fScale);
	virtual void setPosition(const float fXpos, const float fYpos);
	virtual void setRotation(const float fRotation = 0);
	virtual void setLink(C_Linked_Weapon* pLink);
	virtual void setTexture(const char* arFile);
	virtual void setEnabled(const bool isEnabled);
	virtual void setTextureWithRect(const char* arFile, Rect recSprite);
public:
	virtual void setNormalTarget(const float fXpos, const float fYpos);
public:
	virtual inline Sprite* getWeapon() { return m_pWeapon; }
	virtual inline Vec2 getPosition()  { return m_pWeapon->getPosition(); }
public:
	virtual void onIntersectsEnemy();
	virtual void onIntersectsBorder();
	virtual void disabledWeapon();
	virtual void updateSubWeapon();
	virtual void updateByGuided(C_Enemy* pTarget);
private:
	virtual const float calcAngle(const float fAngle);
	virtual const float calcSquare(const float fNumber);
private:
	virtual void init();
	virtual void initAction();
	virtual void preset();
	virtual void createIntersectsEvent();
	virtual void createEvents();
	virtual void createHomingEvent(); // SubWeapon이 아니면 작동하지 않음.
	virtual bool isWinInner(const C_Enemy* pTarget);
public:
	virtual void createMoveEvent();
public:
	virtual void release();
private:
	E_WEAPON_TYPE m_eWeaponType;
private:
	C_Linked_Weapon* m_pMyLinker;
private:
	Layer*  m_pWeaponLayer;
private:
	Sprite* m_pWeapon;
private:
	Node* m_pNode;
private:
	bool m_isActEnd;
	bool m_isUpdating;
private:
	float m_fWeaponSpeed; // SubWeapon에서만 사용.
	float m_fPositionX;
	float m_fPositionY;
	float m_fNormalMoverX;
	float m_fNormalMoverY;
	float m_fMoverX;
	float m_fMoverY;
	float m_fShotAngle;
private:
	int m_nAttack;
	int m_nId;
private:
	C_PlayerWeapon() {}
	virtual ~C_PlayerWeapon() {}
	C_PlayerWeapon(C_PlayerWeapon&) = delete;
	C_PlayerWeapon operator=(C_PlayerWeapon&) = delete;
};