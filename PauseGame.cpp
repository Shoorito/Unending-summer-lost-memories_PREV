#include "PauseGame.h"
#include "KeyEventManager.h"

C_PausePopup* C_PausePopup::m_pInstance = nullptr;

C_PausePopup* C_PausePopup::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_PausePopup();

	if (m_pInstance && m_pInstance->init())
	{
		m_pInstance->autorelease();
		m_pInstance->preset();
	}
	else
	{
		CC_SAFE_DELETE(m_pInstance);
	}

	return m_pInstance;
}

void C_PausePopup::setEnabled(const bool isEnabled)
{
	m_isEnabled = isEnabled;

	if (m_isEnabled)
		resume();
	else
		pause();

	setVisible(m_isEnabled);
}

bool C_PausePopup::init()
{
	if (!Node::init())
		return false;

	m_pPopupImg = nullptr;
	m_isEnabled = false;

	return true;
}

void C_PausePopup::preset()
{
	m_pPopupImg = Sprite::create("PauseScene.png");

	m_pPopupImg->setPosition(494.0f, 360.0f);

	addChild(m_pPopupImg);

	scheduleUpdate();
}

void C_PausePopup::onEnter()
{
	Layer::onEnter();

	if (!m_isEnabled)
		pause();
}

void C_PausePopup::update(float fDelay)
{
	if (C_KeyEvent_Manager::getInstance()->getPushedType(149) == E_KEY_STATE::E_DOWN)
	{
		CCLOG("DOWN_TO_MENU");
	}
}
