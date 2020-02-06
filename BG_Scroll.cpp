#include "BG_Scroll.h"

C_Scroll_BG * C_Scroll_BG::m_pInstance = nullptr;

C_Scroll_BG * C_Scroll_BG::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_Scroll_BG();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_Scroll_BG::callSpellCard()
{
	FadeIn* pFadeIn(nullptr);
	DelayTime* pDelay(nullptr);
	
	m_pSpellCard->setVisible(true);

	pFadeIn = FadeIn::create(1.0f);
	pDelay = DelayTime::create(1.0f);

	m_pSpellCard->runAction(Sequence::create(pDelay, pFadeIn, nullptr));
}

void C_Scroll_BG::outSpellCard()
{
	m_pSpellCard->setVisible(false);
	m_pSpellCard->setOpacity(0);
}

void C_Scroll_BG::pause()
{
	Layer::pause();

	m_arScrollImg[0]->stopAllActions();
	m_arScrollImg[1]->stopAllActions();
	m_arScrollImg[2]->stopAllActions();
}

bool C_Scroll_BG::init()
{
	if (!Layer::init())
		return false;

	m_pCloud	 = nullptr;
	m_pSpellCard = nullptr;

	return true;
}

void C_Scroll_BG::preset()
{
	for (int nBG(0); nBG < 3; nBG++)
	{
		m_arScrollImg[nBG] = Sprite::create("Field_Texture.png");

		addChild(m_arScrollImg[nBG]);
	}

	//m_pCloud = Sprite::create("cloud.png");
	m_pSpellCard = Sprite::create("Boss_Spell_BG.png");

	//addChild(m_pCloud);
	addChild(m_pSpellCard);

	m_pSpellCard->setOpacity(0);
	m_pSpellCard->setVisible(false);

	presetPosition();
	startMoveBG();
}

void C_Scroll_BG::presetPosition()
{
	//m_pCloud->setPosition(494.0f, 360.0f);
	m_pSpellCard->setPosition(494.0f, 360.0f);
	m_arScrollImg[0]->setPosition(494.0f, 360.0f); 
	m_arScrollImg[1]->setPosition(494.0f, 1029.0f);
	m_arScrollImg[2]->setPosition(494.0f, 1698.0f);
}

void C_Scroll_BG::startMoveBG()
{
	Vec2 vecMove(0.0f, -666.0f);
	Vec2 vecPlace(494.0f, 1692.0f);

	for (int nAct(0); nAct < 3; nAct++)
	{
		MoveBy* pOnceMove(nullptr);
		MoveBy* pMove(nullptr);
		Place* pPlace(nullptr);
		Sequence* pSeq(nullptr);
		Sequence* pSeqRepeat(nullptr);
		Repeat* pRep(nullptr);

		pOnceMove	= MoveBy::create(2.0f * (float)(nAct + 1), vecMove * (nAct + 1));
		pMove		= MoveBy::create(6.0f, vecMove * 3);
		pPlace		= Place::create(vecPlace);
		pSeqRepeat	= Sequence::create(pPlace, pMove, nullptr);
		pRep		= Repeat::create(pSeqRepeat, -1);
		pSeq		= Sequence::create(pOnceMove, pRep, nullptr);

		m_arScrollImg[nAct]->runAction(pSeq);
	}
}
