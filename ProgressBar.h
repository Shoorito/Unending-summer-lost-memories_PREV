#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

// INFO! 이 클래스는 "진행바" 기능입니다.
// 주로 로딩, 체력과 같은 것에 쓰입니다.
// 또한 "진행"이라는 특성 상 모든 Node형 맴버가 "0.0f, 1.0f"의 앵커 포인트를 가지고 있습니다.

class C_ProgressBar : public Layer
{
public:
	static C_ProgressBar* create();
public:
	virtual void release();
public:
	virtual void setBackground(const std::string& strFile);
	virtual void setBorder(const std::string& strFile);
	virtual void setProgressed(const std::string& strFile);
public:
	virtual void setProgressed(const Color3B colorPreset, const Rect& recRect);
public:
	virtual void setProgressedCapInsets(const float fWidth, const float fHeight);
	virtual void setProgressMeter(const float fProgress);
public:
	virtual void setPosition(const float fXpos, const float fYpos) override;
	virtual void setPosition(const Vec2& vecPosition) override;
	virtual void setItemContentSize(const float fWidth, const float fHeight);
	virtual void setItemContentSize(const Size& szPosition);
private:
	virtual bool init();
	virtual void preset();
private:
	ui::Scale9Sprite* m_pBackground;
	ui::Scale9Sprite* m_pBorder;
	ui::Scale9Sprite* m_pProgressBar;
private:
	float m_fProgress;
private:
	Size m_szProgressed;
private:
	C_ProgressBar();
	virtual ~C_ProgressBar();
	C_ProgressBar(C_ProgressBar&) = delete;
	C_ProgressBar operator=(C_ProgressBar&) = delete;
};