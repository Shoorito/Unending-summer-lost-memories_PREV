#include "LinkedWeapon.h"
#include "WeaponManager.h"

C_Linked_Weapon * C_Linked_Weapon::create()
{
	C_Linked_Weapon* pLinked(nullptr);

	pLinked = new(std::nothrow) C_Linked_Weapon();

	pLinked->init();

	return pLinked;
}

void C_Linked_Weapon::init()
{
	m_pData = nullptr;
	m_pNext = nullptr;
	m_pPrev = nullptr;
}

void C_Linked_Weapon::release()
{
	C_WeaponManager* pManager(nullptr);

	pManager = C_WeaponManager::getInstance();

	m_pPrev->m_pNext = m_pNext;
	m_pNext->m_pPrev = m_pPrev;

	m_pData->release();

	init();

	delete this;

	return;
}
