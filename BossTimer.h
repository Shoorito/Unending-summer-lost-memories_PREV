#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_Timer : public Layer
{
public:
	static C_Timer* create();
	static C_Timer* getInstance() { return m_pInstance; }
public:
	virtual inline const int getNowTime() { return m_nCount; }
	virtual inline const bool isRunningTimer() { return m_isRunningTimer; }
	virtual inline Label* getLabel() { return m_pText; }
public:
	virtual void startTimer(const unsigned int nTime);
	virtual void stopTimer();
	virtual void setTimer(const unsigned int nTime);
	virtual void setEnabled(const bool isEnabled);
	virtual void setPosition(const float fXpos, const float fYpos) override;
private:
	virtual bool init() override;
	virtual void preset();
	virtual void onEnter();
	virtual void count(float fDelay);
private:
	unsigned int m_nCount;
	bool m_isRunningTimer;
	Label* m_pText;
private:
	static C_Timer* m_pInstance;
private:
	C_Timer() {}
	virtual ~C_Timer() {}
	C_Timer(C_Timer&) = delete;
	C_Timer operator=(C_Timer&) = delete;
};