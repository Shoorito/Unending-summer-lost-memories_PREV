#include "KeyEventManager.h"

C_KeyEvent_Manager* C_KeyEvent_Manager::m_pInstance = nullptr;

C_KeyEvent_Manager * C_KeyEvent_Manager::create()
{
	if (m_pInstance)
	{
		return m_pInstance;
	}

	m_pInstance = new(std::nothrow) C_KeyEvent_Manager();

	if (m_pInstance && m_pInstance->init())
	{
		m_pInstance->autorelease();
		m_pInstance->preset();
	}
	else
		CC_SAFE_DELETE(m_pInstance);

	return m_pInstance;
}

void C_KeyEvent_Manager::setEnabled(const bool isEnabled)
{
	m_isEnabled = isEnabled;

	m_pKeyListener->setEnabled(m_isEnabled);

	if (m_isEnabled)
		resume();
	else
		pause();
}

bool C_KeyEvent_Manager::init()
{
	if (!Node::init()) return false;

	m_pKeyListener = nullptr;
	m_isEnabled    = false;

	for (int nKey(0); nKey < 166; nKey++)
	{
		m_arKeyPast[nKey]  = false;
		m_arKeyNow[nKey]   = false;
		m_arKeyState[nKey] = E_KEY_STATE::E_NONE;
	}

	m_arKeyTypes[0][0] = E_KEY_STATE::E_NONE;
	m_arKeyTypes[0][1] = E_KEY_STATE::E_DOWN;
	m_arKeyTypes[1][0] = E_KEY_STATE::E_UP;
	m_arKeyTypes[1][1] = E_KEY_STATE::E_PUSH;

	return true;
}

void C_KeyEvent_Manager::preset()
{
	createKeyEvent();
	scheduleUpdate();
}

void C_KeyEvent_Manager::onEnter()
{
	Node::onEnter();

	if (!m_isEnabled)
		pause();
}

void C_KeyEvent_Manager::createKeyEvent()
{
	m_pKeyListener = EventListenerKeyboard::create();
	
	m_pKeyListener->onKeyPressed = CC_CALLBACK_2(C_KeyEvent_Manager::onKeyPressed, this);
	m_pKeyListener->onKeyReleased = CC_CALLBACK_2(C_KeyEvent_Manager::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pKeyListener, this);
}

void C_KeyEvent_Manager::update(float fDelay)
{
	for (int nKey(0); nKey < 166; nKey++)
	{
		m_arKeyState[nKey] = m_arKeyTypes[m_arKeyPast[nKey]][m_arKeyNow[nKey]];
		m_arKeyPast[nKey]  = m_arKeyNow[nKey];
	}
}

void C_KeyEvent_Manager::onKeyPressed(const EventKeyboard::KeyCode eKey, Event * pNotUsed)
{
	m_arKeyNow[(int)eKey] = true;
}

void C_KeyEvent_Manager::onKeyReleased(const EventKeyboard::KeyCode eKey, Event * pNotUsed)
{
	m_arKeyNow[(int)eKey] = false;
}
