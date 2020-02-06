#include "LinkedEnemy.h"
#include "Enemy.h"

C_Linked_Enemy * C_Linked_Enemy::create()
{
	C_Linked_Enemy* pEnemy(nullptr);

	pEnemy = new(std::nothrow) C_Linked_Enemy();

	pEnemy->init();

	return pEnemy;
}

void C_Linked_Enemy::release()
{
}

void C_Linked_Enemy::init()
{
	m_pData = nullptr;
	m_pNext = nullptr;
	m_pPrev = nullptr;
}
