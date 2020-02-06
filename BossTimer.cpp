#include "BossTimer.h"
#include "AudioEngine.h"

using namespace experimental;

C_Timer* C_Timer::m_pInstance = nullptr;

C_Timer * C_Timer::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_Timer();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_Timer::startTimer(const unsigned int nTime)
{
	setTimer(nTime);
	setEnabled(true);
}

void C_Timer::stopTimer()
{
	m_pText->enableOutline(Color4B(0, 0, 0, 0));

	setEnabled(false);
	setTimer(0);
}

void C_Timer::setTimer(const unsigned int nTime)
{
	std::string strTime("");

	m_nCount = nTime;

	if(m_nCount < 10)
		strTime += '0';

	if (m_nCount < 10 && m_nCount > 0)
	{
		if (m_nCount <= 5)
		{
			m_pText->enableOutline(Color4B(255, 0, 0, 255), 2);
			AudioEngine::play2d("se_time_under_05.mp3", false, 0.1f);
		}
		else
		{
			m_pText->enableOutline(Color4B(244, 178, 35, 255), 2);
			AudioEngine::play2d("se_time_under_10.mp3", false, 0.1f);
		}
	}

	strTime += std::to_string(m_nCount);

	m_pText->setString(strTime);
}

void C_Timer::setEnabled(const bool isEnabled)
{
	m_isRunningTimer = isEnabled;

	setVisible(m_isRunningTimer);

	if (m_isRunningTimer)
		resume();
	else
		pause();
}

void C_Timer::setPosition(const float fXpos, const float fYpos)
{
	m_pText->setPosition(fXpos, fYpos);
}

bool C_Timer::init()
{
	if (!Layer::init())
		return false;

	m_nCount = 0;
	m_isRunningTimer = false;
	m_pText = nullptr;

	return true;
}

void C_Timer::preset()
{
	m_pText = Label::createWithTTF("00", "fonts/NotoSansCJKkr-Bold.otf", 30.0f);

	m_pText->setAnchorPoint(Point(0.0f, 0.5f));
	m_pText->setPosition(888.0f, 674.0f);

	addChild(m_pText);

	schedule(schedule_selector(C_Timer::count), 1.0f);
}

void C_Timer::onEnter()
{
	Node::onEnter();

	if (!m_isRunningTimer)
		pause();
}

void C_Timer::count(float fDelay)
{
	if (m_nCount < 2)
	{
		stopTimer();

		return;
	}

	m_nCount--;
	setTimer(m_nCount);
}
