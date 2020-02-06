#pragma once
#include "cocos2d.h"
#include "EnumList.h"

USING_NS_CC;

class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();
public:
    virtual void initGLContextAttrs();
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
private:
	// 지원 OS에 따라 해당 테이블의 값을 변경한다.
	bool m_arOSTable[(int)E_OS::E_MAX]
	{
		false, false, false,
		true,  false, true,
		false, false, false,
		false, false, false,
		false, false
	};
private:
	Rect m_recWindow
	{
		0.0f, 0.0f, 1280.0f, 720.0f
	};
};
