#include "SubWeapon.h"

C_SubWeapon * C_SubWeapon::create()
{
	C_SubWeapon* pWeapon(nullptr);

	pWeapon = new(std::nothrow) C_SubWeapon();
	
	pWeapon->init();
	pWeapon->preset();

	return pWeapon;
}

void C_SubWeapon::setTextureWithRect(const char * arFile, const Rect & recTarget)
{
	setTexture(arFile);
	m_pSubWeapon->setTextureRect(recTarget, false, recTarget.size);
}

void C_SubWeapon::setTextureWithRect(const std::string & arFile, const Rect & recTarget)
{
	setTexture(arFile);
	m_pSubWeapon->setTextureRect(recTarget, false, recTarget.size);
}

void C_SubWeapon::setPosition(const float fXpos, const float fYpos)
{
	m_fPositionX = fXpos;
	m_fPositionY = fYpos;

	m_pSubWeapon->setPosition(m_fPositionX, m_fPositionY);
}

void C_SubWeapon::setPosition(const Vec2 & vecPosition)
{
	m_fPositionX = vecPosition.x;
	m_fPositionY = vecPosition.y;

	m_pSubWeapon->setPosition(m_fPositionX, m_fPositionY);
}

void C_SubWeapon::addPosition(const float fXpos, const float fYpos)
{
	m_fPositionX += fXpos;
	m_fPositionY += fYpos;

	m_pSubWeapon->setPosition(m_fPositionX, m_fPositionY);
}

void C_SubWeapon::addPosition(const Vec2 & vecPosition)
{
	m_fPositionX += vecPosition.x;
	m_fPositionY += vecPosition.y;

	m_pSubWeapon->setPosition(m_fPositionX, m_fPositionY);
}

void C_SubWeapon::addPositionX(const float fXpos)
{
	m_fPositionX += fXpos;

	m_pSubWeapon->setPositionX(m_fPositionX);
}

void C_SubWeapon::addPositionY(const float fYpos)
{
	m_fPositionY += fYpos;

	m_pSubWeapon->setPositionY(m_fPositionY);
}

void C_SubWeapon::setEnabled(const bool isEnabled)
{
	m_pSubWeaponLayer->setVisible(isEnabled);

	if (isEnabled)
		m_pSubWeapon->resume();
	else
		m_pSubWeapon->pause();
}

void C_SubWeapon::init()
{
	m_pSubWeapon	  = nullptr;
	m_pSubWeaponLayer = nullptr;
}

void C_SubWeapon::initAction()
{
	CallFunc*	pActionFunc(nullptr);
	CallFunc*	pDisableFunc(nullptr);
	DelayTime*	pDelay(nullptr);
	Sequence*	pSequancer(nullptr);

	pActionFunc		= CallFunc::create(CC_CALLBACK_0(C_SubWeapon::startAction, this));
	pDisableFunc	= CallFunc::create(CC_CALLBACK_0(C_SubWeapon::presetByAction, this));
	pDelay			= DelayTime::create(0.01f);
	pSequancer		= Sequence::create(pActionFunc, pDelay, pDisableFunc, nullptr);

	m_pSubWeaponLayer->runAction(pSequancer);
}

void C_SubWeapon::preset()
{
	m_pSubWeaponLayer	= Layer::create();
	m_pSubWeapon		= Sprite::create();

	m_pSubWeaponLayer->addChild(m_pSubWeapon);

	initAction();
}

void C_SubWeapon::presetByAction()
{
	setEnabled(false);
}

void C_SubWeapon::startAction()
{
	ScaleTo*	pSizeUp(nullptr);
	ScaleTo*	pSizeDown(nullptr);
	Sequence*	pSequancer(nullptr);
	Repeat*		pRepeater(nullptr);
	RotateBy*	pRotate(nullptr);
	Spawn*		pSpawn(nullptr);
	
	pSizeUp		= ScaleTo::create(0.5f, 1.2f);
	pSizeDown	= ScaleTo::create(0.5f, 1.0f);
	pSequancer	= Sequence::create(pSizeUp, pSizeDown, nullptr);
	pRotate		= RotateBy::create(1.0f, 360.0f);
	pSpawn		= Spawn::create(pSequancer, pRotate, nullptr);
	pRepeater	= Repeat::create(pSpawn, -1);

	m_pSubWeapon->runAction(pRepeater);
}

C_SubWeapon::C_SubWeapon()
{
}

C_SubWeapon::~C_SubWeapon()
{
}
