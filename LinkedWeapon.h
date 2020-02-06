#pragma once
#include "PlayerWeapon.h"

class C_Linked_Weapon
{
public:
	static C_Linked_Weapon* create();
public:
	virtual inline C_PlayerWeapon* getData()  { return m_pData; }
	virtual inline C_Linked_Weapon* getPrev() { return m_pPrev; }
	virtual inline C_Linked_Weapon* getNext() { return m_pNext; }
public:
	virtual inline void setData(C_PlayerWeapon* pData)  { m_pData = pData; }
	virtual inline void setPrev(C_Linked_Weapon* pPrev) { m_pPrev = pPrev; }
	virtual inline void setNext(C_Linked_Weapon* pNext) { m_pNext = pNext; }
private:
	virtual void init();
public:
	virtual void release();
private:
	C_PlayerWeapon*  m_pData;
	C_Linked_Weapon* m_pPrev;
	C_Linked_Weapon* m_pNext;
private:
	C_Linked_Weapon() {}
	virtual ~C_Linked_Weapon() {}
	C_Linked_Weapon(C_Linked_Weapon&) = delete;
	C_Linked_Weapon operator=(C_Linked_Weapon&) = delete;
};