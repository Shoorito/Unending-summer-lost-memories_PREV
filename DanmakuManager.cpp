#include "DanmakuManager.h"
#include "Danmaku.h"

C_Danmaku_Manager* C_Danmaku_Manager::m_pInstance = nullptr;

C_Danmaku_Manager* C_Danmaku_Manager::create()
{
	if (m_pInstance)
	{
		return nullptr;
	}

	m_pInstance = new(std::nothrow) C_Danmaku_Manager();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_Danmaku_Manager::moveCursor(const int& nMoveCount, const E_USE_TYPE& eType)
{
	if (!m_arCursor[(int)eType])
		return;

	for (int nMove(0); nMove < nMoveCount; nMove++)
	{
		m_arCursor[(int)eType] = m_arCursor[(int)eType]->getNext();
	}
}

void C_Danmaku_Manager::initCursor(const E_USE_TYPE& eType)
{
	m_arCursor[(int)eType] = m_arHead[(int)eType];
}

void C_Danmaku_Manager::addDanmaku(C_Danmaku * pDanmaku)
{
	pushDanmaku(pDanmaku, E_USE_TYPE::E_NOT_USED);

	m_pGroupLayer->addChild(pDanmaku);
}

void C_Danmaku_Manager::pushDanmaku(C_Danmaku * pDanmaku, const E_USE_TYPE& eType)
{
	if (!m_arHead[(int)eType])
	{
		m_arHead[(int)eType] = pDanmaku;
		m_arTail[(int)eType] = pDanmaku;
		m_arCursor[(int)eType] = pDanmaku;

		pDanmaku->setNext(pDanmaku);
		pDanmaku->setPrev(pDanmaku);
	}
	else
	{
		m_arTail[(int)eType]->setNext(pDanmaku);
		m_arHead[(int)eType]->setPrev(pDanmaku);
		pDanmaku->setPrev(m_arTail[(int)eType]);
		pDanmaku->setNext(m_arHead[(int)eType]);

		m_arTail[(int)eType] = pDanmaku;
	}

	m_arNodeCount[(int)eType]++;
}

C_Danmaku * C_Danmaku_Manager::popDanmaku(const E_USE_TYPE& eType)
{
	if (!m_arNodeCount[(int)eType])
		return nullptr;

	C_Danmaku* pResult(m_arCursor[(int)eType]);

	if (m_arNodeCount[(int)eType] == 1)
	{
		m_arHead[(int)eType]		= nullptr;
		m_arTail[(int)eType]		= nullptr;
		m_arCursor[(int)eType]		= nullptr;
		m_arNodeCount[(int)eType]	= 0;
		pResult->setPrev(nullptr);
		pResult->setNext(nullptr);
		
		return pResult;
	}

	if (m_arTail[(int)eType] == pResult)
		m_arTail[(int)eType] = pResult->getPrev();
	
	if (m_arHead[(int)eType] == pResult)
		m_arHead[(int)eType] = pResult->getNext();

	moveCursor(1, eType);
	
	pResult->getPrev()->setNext(pResult->getNext());
	pResult->getNext()->setPrev(pResult->getPrev());
	pResult->setNext(nullptr);
	pResult->setPrev(nullptr);

	m_arNodeCount[(int)eType]--;

	return pResult;
}

C_Danmaku * C_Danmaku_Manager::popDanmaku(C_Danmaku * pDanmaku, const E_USE_TYPE & eType)
{
	if (!m_arNodeCount[(int)eType])
		return nullptr;

	C_Danmaku* pPopDanmaku(pDanmaku);

	if (m_arNodeCount[(int)eType] == 1)
	{
		m_arHead[(int)eType]		= nullptr;
		m_arTail[(int)eType]		= nullptr;
		m_arCursor[(int)eType]		= nullptr;
		m_arNodeCount[(int)eType]	= 0;
		pPopDanmaku->setNext(nullptr);
		pPopDanmaku->setPrev(nullptr);

		return pPopDanmaku;
	}

	if (m_arCursor[(int)eType] == pPopDanmaku)
	{
		moveCursor(1, eType);
	}

	if (pPopDanmaku == m_arTail[(int)eType])
		m_arTail[(int)eType] = pPopDanmaku->getPrev();

	if (pPopDanmaku == m_arHead[(int)eType])
		m_arHead[(int)eType] = pPopDanmaku->getNext();

	pPopDanmaku->getPrev()->setNext(pPopDanmaku->getNext());
	pPopDanmaku->getNext()->setPrev(pPopDanmaku->getPrev());

	m_arNodeCount[(int)eType]--;

	return pPopDanmaku;
}

void C_Danmaku_Manager::changeDanmaku(C_Danmaku * pDanmaku, const E_USE_TYPE& eType)
{
	E_USE_TYPE arSwap[(int)E_USE_TYPE::E_MAX]{ E_USE_TYPE::E_USED, E_USE_TYPE::E_NOT_USED };
	std::string strLog("Change_Turn_");

	if (!pDanmaku)
		return;

	strLog += "ID : " + std::to_string(pDanmaku->getId());
	strLog += "TYPE : " + std::to_string((int)eType);

	CCLOG(strLog.c_str());

	popDanmaku(pDanmaku, arSwap[(int)eType]);
	pushDanmaku(pDanmaku, eType);
}

void C_Danmaku_Manager::removeDanmaku(C_Danmaku * pDanmaku, const E_USE_TYPE& eType)
{
	if (!pDanmaku)
		return;

	pDanmaku = popDanmaku(pDanmaku, eType);

	pDanmaku->release();
}

void C_Danmaku_Manager::allDisabledDanmaku()
{
	if (!m_arNodeCount[(int)E_USE_TYPE::E_USED])
		return;

	initCursor(E_USE_TYPE::E_USED);

	for (int nDisabled(0); m_arCursor[(int)E_USE_TYPE::E_USED]; nDisabled++)
	{
		m_arCursor[(int)E_USE_TYPE::E_USED]->setEnabled(false);
		changeDanmaku(m_arCursor[(int)E_USE_TYPE::E_USED], E_USE_TYPE::E_NOT_USED);
	}
}

void C_Danmaku_Manager::changeDanmakuRect(const Rect & recDanmaku, const E_USE_TYPE& eType)
{
	m_arCursor[(int)eType]->setTextureRect(recDanmaku);
}

void C_Danmaku_Manager::init()
{
	m_pGroupLayer = nullptr;
}

void C_Danmaku_Manager::preset()
{
	m_pGroupLayer = Layer::create();
}
