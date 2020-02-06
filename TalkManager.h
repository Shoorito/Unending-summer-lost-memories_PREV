#pragma once
#include "cocos2d.h"
#include "TalkNode.h"

USING_NS_CC;

class C_TalkManager : public Layer
{
public:
	static C_TalkManager* create();
	static inline C_TalkManager* getInstance() { return m_pInstance; }
public:
	virtual void setEnabled(const bool isEnabled);
	virtual void callTalkEvent();
	virtual void callEndTalkEvent();
private:
	virtual bool init();
	virtual void preset();
	virtual void onEnter() override;
	virtual void presetStartTalk();
	virtual void presetEndTalk();
	virtual void createSprites();
	virtual void createTalkNodes();
	virtual void addChilds();
	virtual void nextPage();
	virtual void createFirstEvent();
	virtual void writeText(Ref* pTarget, std::string& strText);
private:
	virtual void resumeScheduler();
	virtual void callFirstEvent();
	virtual void callBossSummonEffect();
	virtual void showBoss();
	virtual void showBossUI();
	virtual void changeBoss();
	virtual void changePlayer(const int& nType);
	virtual void turnChange(const bool& isType);
private:
	virtual void updateToPreFight();
	virtual void updateToEndFight();
private:
	virtual void update(float fDelay) override;
private:
	Sprite* m_pBossSCG;
	Sprite* m_pBossSCG_Changed;
	Sprite* m_arPlayerSCG[6]{};
	Sprite* m_pPrevSCG;
	Sprite* m_pTextBox;
private:
	Sprite* m_arFirstTarget[2]{};
private:
	Label* m_pTalk;
private:
	Color4B m_arOutline[4]
	{ 
		Color4B(209, 91, 51, 255),
		Color4B(234, 20, 25, 255),
		Color4B(5, 26, 154, 255),
		Color4B(128, 128, 128, 255)
	};
private:
	Vec2 m_arPMovepoint[2]{ Vec2(34.0f, 1.0f), Vec2(59.0f, 26.0f) };
private:
	FiniteTimeAction* m_arFirstAct[2]{};
	Sequence* m_pFirstAct;
private:
	ParticleSystemQuad* m_pBossShowPar;
private:
	void(C_TalkManager::*m_arUpdateFunc[2])();
private:
	C_TalkNode* m_arTalkNodes[50];
private:
	bool m_isNowPushed;
	bool m_isReveal;
	bool m_isNowReading;
	bool m_isPreFight;
	bool m_isEnabled;
private:
	int m_nPageNumber;
	int m_nReadCount;
	int m_nEventCount;
	int m_nFirstCall;
private:
	std::string m_strDialogue{};
private:
	static C_TalkManager* m_pInstance;
};