#include "ProgressBar.h"
#include "PlayScene.h"

C_ProgressBar * C_ProgressBar::create()
{
	C_ProgressBar* pProgress(nullptr);

	pProgress = new(std::nothrow) C_ProgressBar();

	pProgress->init();
	pProgress->preset();
	
	return pProgress;
}

void C_ProgressBar::setBackground(const std::string & strFile)
{
	m_pBackground->setTexture(strFile);
}

void C_ProgressBar::setBorder(const std::string & strFile)
{
	m_pBorder->setTexture(strFile);
}

// 해당 함수를 처음으로 사용한다면 "Background"리소스를 꼭 지정해야 합니다.
void C_ProgressBar::setProgressed(const std::string& strFile)
{
	float fXSize(0.0f);

	fXSize = m_pProgressBar->getContentSize().width;

	if (fXSize <= 0.0f)
	{
		fXSize = m_pBackground->getContentSize().width / 2.0f;

		m_pProgressBar->setPosition(-fXSize, 0.0f);
	}

	m_pProgressBar->setTexture(strFile);

	m_szProgressed = m_pProgressBar->getContentSize();
}

// 해당 함수를 처음으로 사용한다면 "Background"리소스를 꼭 지정해야 합니다.
void C_ProgressBar::setProgressed(const Color3B colorPreset, const Rect& recRect)
{
	float fXSize(0.0f);

	fXSize = m_pProgressBar->getContentSize().width;

	if (fXSize <= 0.0f)
	{
		fXSize = m_pBackground->getContentSize().width / 2.0f;

		m_pProgressBar->setPosition(-fXSize + 4.0f, 0.0f);
	}

	m_pProgressBar->setColor(colorPreset);
	m_pProgressBar->setTextureRect(recRect);

	m_szProgressed = m_pProgressBar->getContentSize();
}

void C_ProgressBar::setProgressedCapInsets(const float fWidth, const float fHeight)
{
	Size szTarget(0.0f, 0.0f);

	szTarget = m_pBackground->getContentSize();

	m_pBackground->setCapInsets(Rect(fWidth, fHeight, szTarget.width - fWidth * 2.0f, szTarget.height - fHeight * 2.0f));

	szTarget = m_pBorder->getContentSize();

	m_pBorder->setCapInsets(Rect(fWidth, fHeight, szTarget.width - fWidth * 2.0f, szTarget.height - fHeight * 2.0f));
}

void C_ProgressBar::setProgressMeter(const float fProgress)
{
	if (fProgress > 100.0f || fProgress < 0.0f)
	{
		return;
	}

	Size szTarget(Size::ZERO);
	float fProgressd(0.0f);

	fProgressd = m_szProgressed.width / 100.0f;

	szTarget.setSize(fProgressd * fProgress, m_szProgressed.height);

	m_pProgressBar->setContentSize(szTarget);
}

void C_ProgressBar::setPosition(const float fXpos, const float fYpos)
{
	m_pBackground->setPosition(fXpos, fYpos);
	m_pBorder->setPosition(fXpos, fYpos);
	m_pProgressBar->setPosition(fXpos, fYpos);
}

void C_ProgressBar::setPosition(const Vec2 & vecPosition)
{
	m_pBackground->setPosition(vecPosition);
	m_pBorder->setPosition(vecPosition);
	m_pProgressBar->setPosition(vecPosition);
}

void C_ProgressBar::setItemContentSize(const float fWidth, const float fHeight)
{
	m_pBackground->setContentSize(Size(fWidth, fHeight));
	m_pBorder->setContentSize(Size(fWidth, fHeight));
	m_pProgressBar->setContentSize(Size(fWidth, fHeight));
}

void C_ProgressBar::setItemContentSize(const Size& szPosition)
{
	m_pBackground->setContentSize(szPosition);
	m_pBorder->setContentSize(szPosition);
	m_pProgressBar->setContentSize(szPosition);
}

bool C_ProgressBar::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_pBackground  = nullptr;
	m_pBorder      = nullptr;
	m_pProgressBar = nullptr;
	m_szProgressed = Size::ZERO;
	m_fProgress	   = 0.0f;

	return true;
}

void C_ProgressBar::preset()
{
	Scene* pMainScene(nullptr);
	Vec2   vecAnchor(0.0f, 0.5f);

	m_pBackground	= Scale9Sprite::create();
	m_pBorder		= Scale9Sprite::create();
	m_pProgressBar	= Scale9Sprite::create();

	m_pBackground->setAnchorPoint(vecAnchor);
	m_pProgressBar->setAnchorPoint(vecAnchor);
	m_pBorder->setAnchorPoint(vecAnchor);

	addChild(m_pBackground);
	addChild(m_pProgressBar);
	addChild(m_pBorder);
}

void C_ProgressBar::release()
{
	this->release();
}

C_ProgressBar::C_ProgressBar()
{
}

C_ProgressBar::~C_ProgressBar()
{
}
