#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_PausePopup : public Layer
{
public:
	static C_PausePopup* create();
	static inline C_PausePopup* getInstance() { return m_pInstance; }
public:
	virtual void setEnabled(const bool isEnabled);
private:
	virtual bool init() override;
	virtual void preset();
	virtual void onEnter() override;
	virtual void update(float fDelay) override;
private:
	Sprite* m_pPopupImg;
private:
	bool m_isEnabled;
private:
	static C_PausePopup* m_pInstance;
private:
	C_PausePopup() {}
	virtual ~C_PausePopup() {}
	C_PausePopup(C_PausePopup&) = delete;
	C_PausePopup operator=(C_PausePopup&) = delete;
};