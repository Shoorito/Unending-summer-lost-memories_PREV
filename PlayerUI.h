#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_ProgressBar;

class C_Player_UI : public Layer
{
public:
	static C_Player_UI* create();
	static C_Player_UI* getInstance() { return m_pInstance; }
private:
	virtual bool init() override;
	virtual void preset();
	virtual void presetAnchorPoint();
	virtual void presetPosition();
	virtual void presetGazeUI();
	virtual void presetAddChild();
private:
	virtual void loadHighScore();
	virtual void initFile();
private:
	Sprite* m_pUI_BG;
	Sprite* m_pUI_Title;
	Sprite* m_pUI_HighScore;
	Sprite* m_pUI_Score;
	Sprite* m_pUI_HP;
	Sprite* m_pUI_MP;
	Sprite* m_pUI_Power;
private:
	C_ProgressBar* m_pHP_Gaze;
	C_ProgressBar* m_pMP_Gaze;
	C_ProgressBar* m_pPower_Gaze;
private:
	Label* m_pScore;
	Label* m_pHighScore;
private:
	std::string m_strFileName{};
private:
	static C_Player_UI* m_pInstance;
private:
	C_Player_UI() {}
	virtual ~C_Player_UI() {}
	C_Player_UI(C_Player_UI&)			= delete;
	C_Player_UI operator=(C_Player_UI&) = delete;
};