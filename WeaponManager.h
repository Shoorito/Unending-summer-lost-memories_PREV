#pragma once
#include "LinkedWeapon.h"

USING_NS_CC;

enum class E_WEAPON_TYPE
{
	E_NORMAL = 0,
	E_SUB = 1,
	E_MAX = 2
};

class C_WeaponManager
{
public:
	static C_WeaponManager* create();
	static C_WeaponManager* getInstance() { return m_pInstance; }
public:
	virtual C_Linked_Weapon* getHead(E_WEAPON_TYPE eType) { return m_arHead[(int)eType]; }
	virtual C_Linked_Weapon* getCursor(E_WEAPON_TYPE eType) { return m_arCursor[(int)eType]; }
	virtual C_Linked_Weapon* getTail(E_WEAPON_TYPE eType) { return m_arTail[(int)eType]; }
	virtual const int getWeaponCount(E_WEAPON_TYPE eType) { return m_arWeaponCount[(int)eType]; }
	virtual inline Layer* getWeaponsLayer(E_WEAPON_TYPE eType) { return m_arWeaponsLayer[(int)eType]; }
public:
	virtual void addWeapon(C_Linked_Weapon* pWeaponLink, E_WEAPON_TYPE eType);
	virtual void removeWeapon(C_Linked_Weapon* pWeaponLink, E_WEAPON_TYPE eType);
	virtual void changeWeapon(C_Linked_Weapon* pWeaponLink, E_WEAPON_TYPE eType);
public:
	virtual inline void setAttackable(const bool isAble) { m_isAttack = isAble; }
private:
	virtual void init();
	virtual void preset();
	virtual void initUpdate();
	virtual void checkPushedKey();
	virtual void shootMainWeapon();
	virtual void shootSubWeapon();
	virtual void moveCursor(const int nMovepoint = 1, E_WEAPON_TYPE eType = E_WEAPON_TYPE::E_NORMAL);
private:
	virtual void linkWeapon(C_Linked_Weapon* pWeaponLink, E_WEAPON_TYPE eType);
private:
	int m_arWeaponCount[(int)E_WEAPON_TYPE::E_MAX]{};
private:
	C_Linked_Weapon* m_arHead[(int)E_WEAPON_TYPE::E_MAX]{};
	C_Linked_Weapon* m_arCursor[(int)E_WEAPON_TYPE::E_MAX]{};
	C_Linked_Weapon* m_arTail[(int)E_WEAPON_TYPE::E_MAX]{};
private:
	Layer* m_arWeaponsLayer[(int)E_WEAPON_TYPE::E_MAX]{};
private:
	Point m_posCharacter{};
private:
	bool m_isAttack;
private:
	static C_WeaponManager* m_pInstance;
private:
	C_WeaponManager() {}
	virtual ~C_WeaponManager() {}
	C_WeaponManager(C_WeaponManager&) = delete;
	C_WeaponManager operator=(C_WeaponManager&) = delete;
};