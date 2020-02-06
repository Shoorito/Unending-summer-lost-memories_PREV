#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_Scroll_BG : public Layer
{
public:
	static C_Scroll_BG* create();
	static C_Scroll_BG* getInstance() { return m_pInstance; }
public:
	virtual void callSpellCard(); // 확장된 게임에서 사용 할 시 반드시 변경할 것.
	virtual void outSpellCard();
	virtual void pause() override;
private:
	virtual bool init();
	virtual void preset();
	virtual void presetPosition();
	virtual void startMoveBG();
private:
	Sprite* m_arScrollImg[3]{};
	Sprite* m_pCloud;
	Sprite* m_pSpellCard; // 확장된 게임에서 사용 할 시 배열로 선언 할 것.
private:
	static C_Scroll_BG* m_pInstance;
private:
	C_Scroll_BG() {}
	virtual ~C_Scroll_BG() {}
	C_Scroll_BG(C_Scroll_BG&) = delete;
	C_Scroll_BG operator=(C_Scroll_BG&) = delete;
};