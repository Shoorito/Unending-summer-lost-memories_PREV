#include "EnemyManager.h"
#include "LinkedEnemy.h"
#include "Enemy.h"
#include "Summoner.h"

C_EnemyManager* C_EnemyManager::m_pInstance = nullptr;

C_EnemyManager * C_EnemyManager::create()
{
	if (m_pInstance)
		return m_pInstance;

	m_pInstance = new(std::nothrow) C_EnemyManager();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

C_Enemy * C_EnemyManager::getEnemy(const E_USE_TYPE& eType)
{
	return m_arCursor[(int)eType]->getData();
}

// 종단 노드를 넘어가면 "head"노드로 가기 때문에 사용에 주의가 필요합니다.

C_Enemy * C_EnemyManager::checkEnemy(const E_USE_TYPE & eType, const E_ENEMY_TYPE & eEType, C_Enemy * pPrev)
{
	while (m_arCursor[(int)eType]->getData()->getType() != eEType)
	{
		if (m_arCursor[(int)eType]->getData() == pPrev)
				return pPrev;

		moveCursor(eType);
	}

	return m_arCursor[(int)eType]->getData();
}

void C_EnemyManager::moveCursor(const E_USE_TYPE& eType, const int nMoveCount)
{
	if (!m_arCursor[(int)eType])
		return;

	for (int nMove(0); nMove < nMoveCount; nMove++)
	{
		m_arCursor[(int)eType] = m_arCursor[(int)eType]->getNext();
	}
}

void C_EnemyManager::initCursor(const E_USE_TYPE& eType)
{
	m_arCursor[(int)eType] = m_arHead[(int)eType];
}

void C_EnemyManager::addEnemy(C_Linked_Enemy* pEnemy)
{
	pushEnemy(pEnemy, E_USE_TYPE::E_NOT_USED);

	m_pEnemiesLayer->addChild(pEnemy->getData());
}

void C_EnemyManager::pushEnemy(C_Linked_Enemy * pEnemy, const E_USE_TYPE& eType)
{
	if (!m_arHead[(int)eType])
	{
		m_arHead[(int)eType]	= pEnemy;
		m_arTail[(int)eType]	= pEnemy;
		m_arCursor[(int)eType]	= pEnemy;

		pEnemy->setNext(pEnemy);
		pEnemy->setPrev(pEnemy);
	}
	else
	{
		m_arTail[(int)eType]->setNext(pEnemy);
		m_arHead[(int)eType]->setPrev(pEnemy);
		pEnemy->setPrev(m_arTail[(int)eType]);
		pEnemy->setNext(m_arHead[(int)eType]);

		m_arTail[(int)eType] = pEnemy;
	}

	if (eType == E_USE_TYPE::E_USED && pEnemy->getData()->getType() == E_ENEMY_TYPE::E_SUMMONER)
		m_nSummonNode++;

	m_arNodeCount[(int)eType]++;
}

C_Linked_Enemy* C_EnemyManager::popEnemy(C_Linked_Enemy * pEnemy, const E_USE_TYPE& eType)
{
	if (!m_arNodeCount[(int)eType])
		return nullptr;

	if (m_arNodeCount[(int)eType] < 2)
	{
		m_arHead[(int)eType]		= nullptr;
		m_arTail[(int)eType]		= nullptr;
		m_arCursor[(int)eType]		= nullptr;
		m_arNodeCount[(int)eType]	= 0;
		pEnemy->setPrev(nullptr);
		pEnemy->setNext(nullptr);

		return pEnemy;
	}

	if (pEnemy == m_arCursor[(int)eType])
	{
		moveCursor(eType, 1);
	}

	if (m_arHead[(int)eType] == pEnemy)
		m_arHead[(int)eType] = pEnemy->getNext();
	if (m_arTail[(int)eType] == pEnemy)
		m_arTail[(int)eType] = pEnemy->getPrev();

	pEnemy->getPrev()->setNext(pEnemy->getNext());
	pEnemy->getNext()->setPrev(pEnemy->getPrev());

	pEnemy->setNext(nullptr);
	pEnemy->setPrev(nullptr);

	if (eType == E_USE_TYPE::E_USED && pEnemy->getData()->getType() == E_ENEMY_TYPE::E_SUMMONER)
		m_nSummonNode--;

	m_arNodeCount[(int)eType]--;

	return pEnemy;
}

void C_EnemyManager::changeEnemy(C_Linked_Enemy * pEnemy, const E_USE_TYPE& eType)
{
	bool arSwap[(int)E_USE_TYPE::E_MAX]{ true, false };

	if (!pEnemy)
		return;
	
	popEnemy(pEnemy, (E_USE_TYPE)arSwap[(int)eType]);
	pushEnemy(pEnemy, eType);
}

void C_EnemyManager::removeEnemy(C_Linked_Enemy * pEnemy, const E_USE_TYPE& eType)
{
	if (!pEnemy)
		return;

	C_Linked_Enemy* pRemoveNode(nullptr);

	pRemoveNode = popEnemy(pEnemy, eType);

	pRemoveNode->release();
}

void C_EnemyManager::disabledAllEnemy(const E_ENEMY_TYPE & eType)
{
	if (!m_arNodeCount[(int)E_USE_TYPE::E_USED])
		return;

	initCursor(E_USE_TYPE::E_USED);

	while (m_nSummonNode)
	{
		C_Enemy* pSummon(nullptr);

		pSummon = m_arCursor[(int)E_USE_TYPE::E_USED]->getData();

		if (pSummon->getType() == eType)
		{
			m_arCursor[(int)E_USE_TYPE::E_USED]->getData()->setEnabled(false);
			changeEnemy(m_arCursor[(int)E_USE_TYPE::E_USED], E_USE_TYPE::E_NOT_USED);
		}
		else
		{
			moveCursor(E_USE_TYPE::E_USED);
		}
	}
}

C_Enemy * C_EnemyManager::getImmediateEnemy(const Vec2 * pCharacterPos)
{
	C_Enemy* pResult(nullptr);
	C_Enemy* pData(nullptr);
	float fMin(9999.0f);

	if (!m_arHead[(int)E_USE_TYPE::E_USED])
		return nullptr;

	initCursor(E_USE_TYPE::E_USED);

	for (int nEnemy(0); nEnemy < m_arNodeCount[(int)E_USE_TYPE::E_USED]; nEnemy++)
	{
		float fNowData(0.0f);

		pData	  = m_arCursor[(int)E_USE_TYPE::E_USED]->getData();
		fNowData += fabsf(pData->getPositionX() - pCharacterPos->x);
		fNowData += fabsf(pData->getPositionY() - pCharacterPos->y);

		if (fNowData < fMin)
		{
			fMin	= fNowData;
			pResult = pData;
		}

		moveCursor(E_USE_TYPE::E_USED);
	}

	return pResult;
}

void C_EnemyManager::init()
{
	m_pEnemiesLayer = nullptr;
	m_nSummonNode = 0;
}

void C_EnemyManager::preset()
{
	m_pEnemiesLayer = Layer::create();
}
