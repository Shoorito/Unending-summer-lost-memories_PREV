#include "Boss_HP_Bar.h"
#include "Boss.h"

C_Boss_HP_Bar * C_Boss_HP_Bar::create()
{
	C_Boss_HP_Bar* pHPBar(nullptr);

	pHPBar = new(std::nothrow) C_Boss_HP_Bar();

	if (!pHPBar->init())
	{
		CCLOG(u8"생성 중 문제가 발생했습니다. \n문제 발생 위치 : C_Boss_HP_Bar");

		return nullptr;
	}
	
	pHPBar->preset();
		
	return pHPBar;
}

const float C_Boss_HP_Bar::getAllHP() const
{
	float fHP(0.0f);

	for (int nHP(0); nHP < (int)E_HP_TYPE::E_MAX; nHP++)
	{
		fHP += m_arMaxHP[nHP];
	}

	return fHP;
}

void C_Boss_HP_Bar::setEnabled(const bool isEnabled)
{
	setVisible(isEnabled);
}

void C_Boss_HP_Bar::setHP(float fHP)
{
	Size szBar(Size::ZERO);

	m_arHP[m_nNowpos] = fHP;

	if (m_arHP[m_nNowpos] <= 0.0f)
	{
		m_arHP[m_nNowpos] = 0.0f;
		m_arBoss_HP[m_nNowpos]->setContentSize(Size::ZERO);
		m_nNowpos++;

		return;
	}
	
	szBar.setSize(m_arHP[m_nNowpos] * m_arHPUnit[m_nNowpos], 17.0f);

	m_arBoss_HP[m_nNowpos]->setContentSize(szBar);
}

void C_Boss_HP_Bar::setHPBar(const float fXSize)
{
	if (fXSize != m_pBG->getContentSize().width - 6.0f)
	{
		CCLOG(u8"최대 값 수치에 맞지 않습니다!\n위치 : setHPBar(const float fXSize)");
		return;
	}

	m_nNowpos  = 0;
	m_eBarType = E_HP_BAR_TYPE::E_NORMAL;

	m_arBarXSize[(int)m_eBarType] = fXSize;

	m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]->setTextureRect(Rect(0.0f, 0.0f, fXSize, 17.0f));
	m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]->setPosition(213.0f, 672.5f);
}

void C_Boss_HP_Bar::setHPBar(const float fXSize01, const float fXSize02)
{
	if ((fXSize01 + fXSize02) != m_pBG->getContentSize().width - 6.0f)
	{
		CCLOG(u8"최대 값 수치에 맞지 않습니다!\n위치 : setHPBar(const float fXSize)");
		return;
	}

	m_nNowpos  = 0;
	m_eBarType = E_HP_BAR_TYPE::E_SPELL;

	m_arBarXSize[(int)m_eBarType - 1] = fXSize01;
	m_arBarXSize[(int)m_eBarType]	  = fXSize02;

	m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]->setTextureRect(Rect(0.0f, 0.0f, fXSize01, 17.0f));
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_01]->setTextureRect(Rect(0.0f, 0.0f, fXSize02, 17.0f));
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_01]->setPosition(213.0f, 672.5f);
	m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]->setPosition(213.0f + fXSize02, 672.5f);
}

void C_Boss_HP_Bar::setHPBar(const float fXSize01, const float fXSize02, const float fXSize03)
{
	if ((fXSize01 + fXSize02 + fXSize03) != m_pBG->getContentSize().width - 6.0f)
	{
		CCLOG(u8"최대 값 수치에 맞지 않습니다!\n위치 : setHPBar(const float fXSize)");
		return;
	}

	m_nNowpos  = 0;
	m_eBarType = E_HP_BAR_TYPE::E_LASTSPELL;

	m_arBarXSize[(int)m_eBarType - 2] = fXSize01;
	m_arBarXSize[(int)m_eBarType - 1] = fXSize02;
	m_arBarXSize[(int)m_eBarType]	  = fXSize03;

	m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]->setTextureRect(Rect(0.0f, 0.0f, fXSize01, 17.0f));
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_01]->setTextureRect(Rect(0.0f, 0.0f, fXSize02, 17.0f));
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_02]->setTextureRect(Rect(0.0f, 0.0f, fXSize03, 17.0f));
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_02]->setPosition(213.0f, 672.5f);
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_01]->setPosition(213.0f + fXSize03, 672.5f);
	m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]->setPosition(213.0f + fXSize02 + fXSize03, 672.5f);
}

void C_Boss_HP_Bar::setMaxHP(const float fMaxHP, const E_HP_TYPE& eType)
{
	m_arMaxHP[(int)eType]	= fMaxHP;
	m_arHP[(int)eType]		= fMaxHP;
	m_arHPUnit[(int)eType]	= m_arBarXSize[(int)eType] / m_arMaxHP[(int)eType];
}

bool C_Boss_HP_Bar::init()
{
	if (!Layer::init())
		return false;

	m_pBG		= nullptr;
	m_pBorder	= nullptr;
	m_nNowpos   = 0;
	m_eBarType  = E_HP_BAR_TYPE::E_NORMAL;

	return true;
}

void C_Boss_HP_Bar::preset()
{
	m_pBG									= Sprite::create("UI/Boss_HP_BG.png");
	m_pBorder								= Sprite::create("UI/Boss_HP_Border.png");
	m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]   = Scale9Sprite::create();
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_01] = Scale9Sprite::create();
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_02] = Scale9Sprite::create();

	presetBar();

	addChild(m_pBG);
	addChild(m_pBorder);
	addChild(m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]);
	addChild(m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_01]);
	addChild(m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_02]);
}

void C_Boss_HP_Bar::presetBar()
{
	m_pBG->setAnchorPoint(Point(0.0f, 0.5f));
	m_pBorder->setAnchorPoint(Point(0.0f, 0.5f));
	m_arBoss_HP[(int)E_HP_TYPE::E_NORMAL]->setAnchorPoint(Point(0.0f, 0.5f));
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_01]->setAnchorPoint(Point(0.0f, 0.5f));
	m_arBoss_HP[(int)E_HP_TYPE::E_SPELL_02]->setAnchorPoint(Point(0.0f, 0.5f));

	m_pBG->setPosition(210.0f, 672.5f);
	m_pBorder->setPosition(210.0f, 672.5f);
}
