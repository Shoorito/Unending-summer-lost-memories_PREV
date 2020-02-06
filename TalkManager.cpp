#include "TalkManager.h"
#include "TalkNode.h"
#include "KeyEventManager.h"
#include "FieldManager.h"
#include "BossManager.h"
#include "AudioEngine.h"
#include "PauseGame.h"
#include "PlayScene.h"

using namespace experimental;

C_TalkManager * C_TalkManager::m_pInstance = nullptr;

C_TalkManager * C_TalkManager::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_TalkManager();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_TalkManager::setEnabled(const bool isEnabled)
{
	m_isEnabled = isEnabled;

	setVisible(m_isEnabled);

	if (m_isEnabled)
		resume();
	else
		pause();
}

void C_TalkManager::callTalkEvent()
{
	m_arPlayerSCG[0]->setVisible(true);

	runAction(m_pFirstAct);
}

void C_TalkManager::callEndTalkEvent()
{
	m_nPageNumber  = 0;
	m_nEventCount  = 0;
	m_nFirstCall   = 0;
	m_isNowReading = false;

	changePlayer(1);
	turnChange(false);
	presetEndTalk();
	setEnabled(true);
	nextPage();
}

bool C_TalkManager::init()
{
	if (!Layer::init())
		return false;

	m_pBossSCG			= nullptr;
	m_pBossSCG_Changed	= nullptr;
	m_pTextBox			= nullptr;
	m_pTalk				= nullptr;
	m_pPrevSCG			= nullptr;
	m_pFirstAct			= nullptr;
	m_pBossShowPar		= nullptr;
	m_isNowPushed		= false;
	m_isReveal			= false;
	m_isNowReading		= false;
	m_isPreFight		= true;
	m_isEnabled			= false;
	m_nPageNumber		= 0;
	m_nReadCount		= 0;
	m_nFirstCall		= 0;
	m_nEventCount		= 0;

	return true;
}

void C_TalkManager::preset()
{
	Point posInitAnchor(0.0f, 0.0f);

	createSprites();
	createTalkNodes();
	presetStartTalk();

	m_pTalk		   = Label::createWithTTF("", "fonts/NotoSansCJKkr-Bold.otf", 25);
	m_pBossShowPar = ParticleSystemQuad::create("ShowBoss.plist");

	m_arFirstTarget[0] = m_pTextBox;
	m_arFirstTarget[1] = m_arPlayerSCG[0];
	m_pPrevSCG		   = m_arPlayerSCG[0];
	m_arUpdateFunc[0]  = &C_TalkManager::updateToEndFight;
	m_arUpdateFunc[1]  = &C_TalkManager::updateToPreFight;

	m_pPrevSCG->setOpacity(0);
	m_pBossSCG->setOpacity(0);
	m_pBossSCG_Changed->setOpacity(0);
	
	m_pTextBox->setScale(0.01f);

	m_pBossSCG->setVisible(false);
	m_pBossShowPar->setVisible(false);

	m_pBossSCG->setAnchorPoint(Point(1.0f, 0.0f));
	m_pBossSCG_Changed->setAnchorPoint(Point(1.0f, 0.0f));
	m_pTalk->setAnchorPoint(Point(0.0f, 1.0f));
	m_pTextBox->setAnchorPoint(Point(0.0f, 1.0f));

	m_pTextBox->setPosition(83.0f, 197.0f);
	m_pTalk->setPosition(95.0f, 193.0f);
	m_pBossShowPar->setPosition(494.0f, 520.0f);

	for (int nSCG(0); nSCG < 6; nSCG++)
	{
		m_arPlayerSCG[nSCG]->setPosition(34.0f, 1.0f);
		m_arPlayerSCG[nSCG]->setAnchorPoint(posInitAnchor);
		m_arPlayerSCG[nSCG]->setVisible(false);
	}

	createFirstEvent();
	addChilds();

	schedule(schedule_selector(C_TalkManager::update), 0.016f);
}

void C_TalkManager::onEnter()
{
	Node::onEnter();

	if (!m_isEnabled)
		pause();
}

void C_TalkManager::presetStartTalk()
{
	m_arTalkNodes[0]->setAll(u8"정말이지..", 0, 1);
	m_arTalkNodes[1]->setAll(u8"원래부터 주변이 판타지 투성이라는 건 알고 있었지만..", 0, 1);
	m_arTalkNodes[2]->setAll(u8"이정도로 심했던 적은 없었을 텐데요.", 0, 2);
	m_arTalkNodes[3]->setAll(u8"대체 무슨 일이 벌어지고 있는 걸까요?", 0, 2);
	m_arTalkNodes[4]->setAll(u8"꽤나 재밌는 상황이 벌어졌는걸?", 1, 0);
	m_arTalkNodes[5]->setAll(u8"(..언니?!)", 3, 3);
	m_arTalkNodes[6]->setAll(u8"왜 이런 곳에 계신거죠?", 0, 1);
	m_arTalkNodes[7]->setAll(u8"산책 겸 잠시 돌아다니고 있었어.", 1, 0);
	m_arTalkNodes[8]->setAll(u8"도경이도 데리고 오고 싶었지만..", 1, 0);
	m_arTalkNodes[9]->setAll(u8"좀 더 안정을 취해야 할 것 같아서 말이야.", 1, 0);
	m_arTalkNodes[10]->setAll(u8"..그렇군요.", 0, 1);
	m_arTalkNodes[11]->setAll(u8"역시나 했는데 그런거군요.", 0, 2);
	m_arTalkNodes[12]->setAll(u8"당신, 언니가 아니군요.", 0, 1);
	m_arTalkNodes[13]->setAll(u8"..응? 그게 무슨 소리야?", 1, 1);
	m_arTalkNodes[14]->setAll(u8"갑자기 그런 농담을 해도..", 1, 1);
	m_arTalkNodes[15]->setAll(u8"안타깝게, 이래뵈도 무당이거든요.", 0, 1);
	m_arTalkNodes[16]->setAll(u8"당신과 같은 \"의태\"정도는 쉽게 간파 할 수 있죠~", 0, 5);
	m_arTalkNodes[17]->setAll(u8"..흠흠", 2, 0);
	m_arTalkNodes[18]->setAll(u8"대단하군", 2, 0);
	m_arTalkNodes[19]->setAll(u8"참으로 기대를 저버리지 않는 처자야!", 2, 0);
	m_arTalkNodes[20]->setAll(u8"칭찬은 고맙지만, 의문만 많아지네요.", 0, 0);
	m_arTalkNodes[21]->setAll(u8"당신.. 대체 누구죠?", 0, 1);
	m_arTalkNodes[22]->setAll(u8"왜 언니 모습으로.. 아니 그 전에 당신은..", 0, 1);
	m_arTalkNodes[23]->setAll(u8"참으로 성리가 급한 처자로구나..", 2, 0);
	m_arTalkNodes[24]->setAll(u8"아! 이건 어떤가?", 2, 0);
	m_arTalkNodes[25]->setAll(u8"...?", 0, 1);
	m_arTalkNodes[26]->setAll(u8"나와 싸워서 이기면 원하는 정보를 주지.\n어떤가?", 2, 0);
	m_arTalkNodes[27]->setAll(u8"...좋아요, 받아들이죠.", 0, 1);
	m_arTalkNodes[28]->setAll(u8"그대신.", 0, 1);
	m_arTalkNodes[29]->setAll(u8"뼈 한 두 곳 부러져도 저는 모릅니다!", 0, 6);
}

void C_TalkManager::presetEndTalk()
{
	m_arTalkNodes[0]->setAll(u8"역시 대단한 실력이구나.", 2, 0);
	m_arTalkNodes[1]->setAll(u8"이 정도 힘은 있어야 언니와 그 \"바보\"분을 지킬 수 있거든요.", 0, 5);
	m_arTalkNodes[2]->setAll(u8"그래 좋다, 약속대로 알려주도록 하지.", 2, 0);
	m_arTalkNodes[3]->setAll(u8"나는 평소처럼 이 주변을 거닐고 있었다네", 2, 0);
	m_arTalkNodes[4]->setAll(u8"그런데 갑자기 \"검은 안개\"가 날 뒤덮더군.", 2, 0);
	m_arTalkNodes[5]->setAll(u8"(그러고보니.. 아까 주변에 그런 안개가 있었지..)", 3, 1);
	m_arTalkNodes[6]->setAll(u8"그리고 이 처자의 모습으로 변하게 된 것이네", 2, 0);
	m_arTalkNodes[7]->setAll(u8"오랜만에 젋은 몸이 되니 나도 모르게 들떴던 모양이네", 2, 0);
	m_arTalkNodes[8]->setAll(u8"그렇군요, 근데 그럼 그 의태는..", 0, 1);
	m_arTalkNodes[9]->setAll(u8"걱정말게, 처자의 공격으로 검은 안개의 영향이 사라졌으니", 2, 0);
	m_arTalkNodes[10]->setAll(u8"이제 \"원래 있어야 할 모습\"으로 돌아가게 되겠지..", 2, 0);
	m_arTalkNodes[11]->setAll(u8"그렇군요..", 0, 1);
	m_arTalkNodes[12]->setAll(u8"자, 그럼 이제 슬슬 시간이네\n오랜만에 즐거웠다네", 2, 0);
	m_arTalkNodes[13]->setAll(u8"잠깐만요! 아직 당신이 누군지..", 0, 3);
	m_arTalkNodes[14]->setAll(u8"사라졌네요.. ", 0, 1);
	m_arTalkNodes[15]->setAll(u8"멋대로 나타났다가 사라지다니\n대체 누구였던걸까요?", 0, 1);
	m_arTalkNodes[16]->setAll(u8"일단, \"검은안개\"라고 했었죠.", 0, 1);
	m_arTalkNodes[17]->setAll(u8"안개가 심한 곳으로 가면 실마리를 얻을 수 있을지도 모르겠네요.", 0, 1);
}

void C_TalkManager::createSprites()
{
	m_pTextBox			= Sprite::create("TextBox.png");
	m_pBossSCG			= Sprite::create("SCG/Boss_Normal.png");
	m_pBossSCG_Changed	= Sprite::create("SCG/Boss_Change.png");

	for (int nSCG(0); nSCG < 6; nSCG++)
	{
		m_arPlayerSCG[nSCG] = Sprite::create("SCG/Player_0" + std::to_string(nSCG + 1) + ".png");
	}
}

void C_TalkManager::createTalkNodes()
{
	for (int nTalk(0); nTalk < 50; nTalk++)
	{
		m_arTalkNodes[nTalk] = C_TalkNode::create();
	}
}

void C_TalkManager::addChilds()
{
	addChild(m_pBossShowPar);
	addChild(m_pBossSCG);
	addChild(m_pBossSCG_Changed);

	for (int nSCG(0); nSCG < 6; nSCG++)
	{
		addChild(m_arPlayerSCG[nSCG]);
	}

	addChild(m_pTextBox);
	addChild(m_pTalk);
}

void C_TalkManager::nextPage()
{
	if (m_isNowReading)
		return;

	C_TalkNode* pNode(m_arTalkNodes[m_nPageNumber]);
	CallFuncN*	 pFunc(nullptr);
	DelayTime*	 pDelay(nullptr);
	Sequence*	 pSeq(nullptr);

	m_nReadCount = 0;
	m_isNowReading = true;
	m_strDialogue.clear();
	m_pTalk->setString(" ");

	changePlayer(pNode->getType());
	turnChange(pNode->getTalkType());

	pFunc	= CallFuncN::create(CC_CALLBACK_1(C_TalkManager::writeText, this, pNode->getDialogue()));
	pDelay	= DelayTime::create(0.01f);
	pSeq	= Sequence::create(pFunc, pDelay, nullptr);

	m_pTalk->enableOutline(m_arOutline[pNode->getTalkType()], 2);
	m_pTalk->runAction(Repeat::create(pSeq, (unsigned int)pNode->getDialogue().size()));

	m_nPageNumber++;
}

void C_TalkManager::createFirstEvent()
{
	ScaleTo* pXScaleUp(nullptr);
	ScaleTo* pYScaleUp(nullptr);
	FadeIn*  pFadeInToBody(nullptr);
	MoveTo*	 pMoveToBody(nullptr);
	CallFunc* pCallActs(nullptr);
	CallFunc* pCallPage(nullptr);
	DelayTime* pDelay(nullptr);
	
	pXScaleUp		= ScaleTo::create(1.0f, 1.0f, 0.01f);
	pYScaleUp		= ScaleTo::create(1.0f, 1.0f, 1.0f);
	pFadeInToBody	= FadeIn::create(0.5f);
	pMoveToBody		= MoveTo::create(0.5f, Vec2(59.0f, 26.0f));
	pCallActs		= CallFunc::create(CC_CALLBACK_0(C_TalkManager::callFirstEvent, this));
	pCallPage		= CallFunc::create(CC_CALLBACK_0(C_TalkManager::nextPage, this));
	pDelay			= DelayTime::create(2.0f);

	m_arFirstAct[0] = Sequence::create(pXScaleUp, pYScaleUp, nullptr);
	m_arFirstAct[1] = Spawn::create(pMoveToBody, pFadeInToBody, nullptr);
	m_pFirstAct		= Sequence::create(pCallActs, pDelay, pCallActs, pCallPage, nullptr);
	
	m_arFirstAct[0]->retain();
	m_arFirstAct[1]->retain();
	m_pFirstAct->retain();
}

void C_TalkManager::writeText(Ref * pTarget, std::string& strText)
{
	Label* pLabel(nullptr);

	pLabel = (Label*)pTarget;

	m_strDialogue += strText[m_nReadCount];

	pLabel->setString(m_strDialogue);

	m_nReadCount++;

	if ((int)strText.size() == m_nReadCount)
		m_isNowReading = false;
}

void C_TalkManager::resumeScheduler()
{
	_scheduler->resumeTarget(this);
}

void C_TalkManager::callFirstEvent()
{
	m_arFirstTarget[m_nFirstCall]->runAction(m_arFirstAct[m_nFirstCall]);

	m_nFirstCall++;
}

void C_TalkManager::callBossSummonEffect()
{
	m_pBossShowPar->setVisible(true);
	m_pBossShowPar->start();
	AudioEngine::play2d("se_ch02.mp3", false, 0.2f);
}

void C_TalkManager::showBoss()
{
	MoveTo*		pMove(nullptr);
	FadeIn*		pFadeIn(nullptr);
	CallFunc*	pParticle(nullptr);
	CallFunc*	pShowBoss(nullptr);
	CallFunc*	pResume(nullptr);
	DelayTime*	pDelay(nullptr);
	Spawn*		pShowAct(nullptr);

	pMove	  = MoveTo::create(0.5f, Vec2(913.0f, 25.0f));
	pFadeIn   = FadeIn::create(0.5f);
	pParticle = CallFunc::create(CC_CALLBACK_0(C_TalkManager::callBossSummonEffect, this));
	pShowBoss = CallFunc::create(CC_CALLBACK_0(C_TalkManager::showBossUI, this));
	pResume   = CallFunc::create(CC_CALLBACK_0(C_TalkManager::resumeScheduler, this));
	pDelay	  = DelayTime::create(3.0f);
	pShowAct  = Spawn::create(pFadeIn, pMove, nullptr);

	_scheduler->pauseTarget(this);
	m_pBossSCG->setVisible(true);
	m_pBossSCG->setOpacity(0);
	m_pBossSCG->setPosition(938.0f, 0.0f);
	m_pBossSCG->runAction(Sequence::create(pParticle, pDelay, pShowBoss, pShowAct, pResume, nullptr));

	m_nEventCount++;
}

void C_TalkManager::showBossUI()
{
	//C_BossManager::getInstance()->getBoss()->setImmortal(true);
	C_BossManager::getInstance()->getBoss()->setEnabled(true);
	C_BossManager::getInstance()->getBoss()->playFlyMotion();
}

void C_TalkManager::changeBoss()
{
	FadeOut* pFadeOut(nullptr);
	FadeIn* pFadeIn(nullptr);

	pFadeOut = FadeOut::create(1.0f);
	pFadeIn  = FadeIn::create(1.0f);

	m_pBossSCG->runAction(pFadeOut);
	m_pBossSCG_Changed->setPosition(927.0f, 25.0f);
	m_pBossSCG_Changed->setOpacity(0);
	m_pBossSCG_Changed->runAction(pFadeIn);

	m_nEventCount++;
}

void C_TalkManager::changePlayer(const int& nType)
{
	if (nType - 1 < 0)
		return;

	m_pPrevSCG->setVisible(false);

	m_arPlayerSCG[nType - 1]->setPosition(m_pPrevSCG->getPosition());

	m_pPrevSCG = m_arPlayerSCG[nType - 1];

	m_pPrevSCG->setVisible(true);
}

void C_TalkManager::turnChange(const bool& isType)
{
	if (m_nPageNumber < 4 && m_isPreFight)
		return;

	Sprite* arSwap[2]{ m_pBossSCG, m_pBossSCG_Changed };
	int arOpacity[2] { 128, 255 };
	Vec2 arBMovepoint[2]{ Vec2(938.0f, 0.0f), Vec2(913.0f + (14.0f * m_isReveal), 25.0f) };
	MoveTo* pPMove(nullptr);
	MoveTo* pBMove(nullptr);

	pPMove = MoveTo::create(0.5f, m_arPMovepoint[!isType]);
	pBMove = MoveTo::create(0.5f, arBMovepoint[isType]);

	m_pPrevSCG->stopAllActions();
	arSwap[m_isReveal]->stopAllActions();

	m_pPrevSCG->setOpacity(arOpacity[!isType]);
	arSwap[m_isReveal]->setOpacity(arOpacity[isType]);

	m_pPrevSCG->runAction(pPMove);
	arSwap[m_isReveal]->runAction(pBMove);
}

void C_TalkManager::updateToPreFight()
{
	if (m_nPageNumber >= g_nTalkCount)
	{
		m_isPreFight = false;

		setEnabled(false);
		C_Field_Manager::getInstance()->callBossEvent();

		return;
	}

	if (m_nPageNumber + m_nEventCount == 5)
	{
		showBoss();
		return;
	}

	if (m_nPageNumber == 18)
		m_isReveal = true;

	if (m_nPageNumber + m_nEventCount == 19)
	{
		changeBoss();
		return;
	}

	if (m_nPageNumber == g_nTalkCount - 1)
	{
		AudioEngine::stopAll();
		AudioEngine::play2d("Theme_02.mp3", true, 0.2f);
		C_BossManager::getInstance()->setVisibleToMageCircle(true);
	}

	nextPage();
}

void C_TalkManager::updateToEndFight()
{
	if (m_nPageNumber >= g_nEndTalkCount)
	{
		setEnabled(false);
		
		C_PlayScene::getInstance()->allFieldItemPause();
		C_PausePopup::getInstance()->setEnabled(true);

		return;
	}

	if (m_nPageNumber == 13)
	{
		m_pBossSCG_Changed->setVisible(false);
	}

	nextPage();
}

void C_TalkManager::update(float fDelay)
{
	if (m_isNowReading)
		return;

	if (C_KeyEvent_Manager::getInstance()->getPushedType(149) == E_KEY_STATE::E_DOWN)
	{
		m_isNowPushed = true;

		(this->*m_arUpdateFunc[m_isPreFight])();

		return;
	}

	/*
	if (C_KeyEvent_Manager::getInstance()->getPushedType(149) == E_KEY_STATE::E_PUSH)
	{
		m_isNowPushed = false;
		
		return;
	}
	*/
}
