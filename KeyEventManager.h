#pragma once
#include "cocos2d.h"

USING_NS_CC;

enum class E_KEY_STATE
{
	E_NONE,
	E_DOWN,
	E_PUSH,
	E_UP
};

class C_KeyEvent_Manager : public Node
{
public:
	static C_KeyEvent_Manager* create();
	static C_KeyEvent_Manager* getInstance() { return m_pInstance; }
public:
	virtual const E_KEY_STATE& getPushedType(const int nKey) { return m_arKeyState[nKey]; }
public:
	virtual void setEnabled(const bool isEnabled);
private:
	virtual bool init() override;
	virtual void preset();
	virtual void onEnter() override;
	virtual void createKeyEvent();
	virtual void update(float fDelay) override;
private:
	virtual void onKeyPressed(const EventKeyboard::KeyCode eKey, Event* pNotUsed);
	virtual void onKeyReleased(const EventKeyboard::KeyCode eKey, Event* pNotUsed);
private:
	bool m_arKeyPast[166]{};
	bool m_arKeyNow[166]{};
private:
	E_KEY_STATE m_arKeyState[166]{};
	E_KEY_STATE m_arKeyTypes[2][2]{};
private:
	bool m_isEnabled;
private:
	EventListenerKeyboard* m_pKeyListener;
private:
	static C_KeyEvent_Manager* m_pInstance;
private:
	C_KeyEvent_Manager() {}
	virtual ~C_KeyEvent_Manager() {}
	C_KeyEvent_Manager(C_KeyEvent_Manager&) = delete;
	C_KeyEvent_Manager operator=(C_KeyEvent_Manager&) = delete;
};