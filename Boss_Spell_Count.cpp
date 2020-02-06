#include "Boss_Spell_Count.h"

C_SpellCounter * C_SpellCounter::m_pInstance = nullptr;

C_SpellCounter * C_SpellCounter::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_SpellCounter();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_SpellCounter::setSpell(const int nNumber, const bool isVisible)
{
	if ((nNumber < 0) || (nNumber > (int)m_vecSpellCount.size()))
		return;

	if (isVisible)
	{
		m_vecSpellCount[nNumber]->setOpacity(255);
		m_vecSpellCount[nNumber]->setVisible(true);
	}
	else
	{
		Spawn* pAct(m_pRemoveAct);

		m_pRemoveAct = m_pRemoveAct->clone();

		m_pRemoveAct->retain();

		m_vecSpellCount[nNumber]->runAction(pAct);
		m_nNowUsedCount--;
	}
}

void C_SpellCounter::setAlign(const float fXpos, const float fYpos)
{
	float fXAdder(0.0f);

	for (int nWidth(0); nWidth < (int)m_nCount; nWidth++)
	{
		if (m_nCount % 6 == 0)
		{
			fXAdder = 0.0f;
		}

		m_vecSpellCount[nWidth]->setPosition(65.0f + fXAdder, 651.0f + fYpos * (nWidth / 6));
	
		fXAdder += fXpos;
	}
}

void C_SpellCounter::setUsedCounter(const int nCount)
{
	m_nNowUsedCount = nCount;

	for (int nDisable(0); nDisable < m_nCount; nDisable++)
	{
		if (nDisable < m_nNowUsedCount)
			m_vecSpellCount[nDisable]->setVisible(true);
		else
			m_vecSpellCount[nDisable]->setVisible(false);
	}
}

void C_SpellCounter::setEnabled(const bool isEnabled)
{
	setVisible(isEnabled);
}

void C_SpellCounter::createSpellCount(const int nCount)
{
	Point posAnchor(0.0f, 0.5f);

	for (int nCreate(0); nCreate < nCount; nCreate++)
	{
		Sprite* pSpell(nullptr);

		pSpell = Sprite::create("SpellCount.png");

		pSpell->setPosition(-500.0f, -500.0f);
		pSpell->setAnchorPoint(posAnchor);

		addChild(pSpell);

		m_vecSpellCount.emplace_back(pSpell);

		m_nCount++;
	}

	m_nNowUsedCount = m_nCount;
}

bool C_SpellCounter::init()
{
	if (!Layer::create())
		return false;

	m_nCount		= 0;
	m_nNowUsedCount = 0;
	m_vec2Actpos	= Vec2::ZERO;
	m_pRemoveAct	= nullptr;

	return true;
}

void C_SpellCounter::preset()
{
	FadeOut* pOut(nullptr);
	MoveBy*  pMove(nullptr);
	ScaleTo* pScale(nullptr);
	Spawn*   pSpawn(nullptr);

	m_vec2Actpos.setPoint(5.0f, -5.0f);

	pOut	= FadeOut::create(0.5f);
	pMove	= MoveBy::create(0.5f, m_vec2Actpos);
	pScale  = ScaleTo::create(0.5f, 2.0f);
	pSpawn	= Spawn::create(pOut, pMove, pScale, nullptr);

	pSpawn->retain();

	m_pRemoveAct = pSpawn;

	setEnabled(false);
}
