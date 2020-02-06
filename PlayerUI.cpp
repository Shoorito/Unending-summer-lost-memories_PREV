#include "PlayerUI.h"
#include "ProgressBar.h"

C_Player_UI* C_Player_UI::m_pInstance = nullptr;

C_Player_UI * C_Player_UI::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_Player_UI();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

bool C_Player_UI::init()
{
	if (!Layer::init())
		return false;

	m_pUI_BG	= nullptr;
	m_pUI_Title = nullptr;

	return true;
}

void C_Player_UI::loadHighScore()
{
	int nDigit(0);
	FileUtils* pFile(nullptr);
	std::string strFileData("");
	std::string strTempScore("");

	pFile		= FileUtils::getInstance();
	strFileData = pFile->getStringFromFile(pFile->getWritablePath() + "Save.txt");

	if (strFileData == "")
	{
		initFile();

		strTempScore = "000001000000";
	}
	else
	{
		for (int nText(10); nText < (int)strFileData.size(); nText++)
		{
			strTempScore += strFileData.at(nText);
			nDigit++;
		}

		for (int nText(nDigit); nText < 12; nText++)
		{
			m_strFileName += '0';
		}
	}

	m_strFileName += strTempScore;

	m_pHighScore->setString(m_strFileName);
}

void C_Player_UI::initFile()
{
	FileUtils* pFileManager(nullptr);
	std::string strWriteFile("");

	strWriteFile = "HighScore 1000000";
	pFileManager = FileUtils::getInstance();

	pFileManager->writeStringToFile(strWriteFile, pFileManager->getWritablePath() + "Save.txt");
}

void C_Player_UI::preset()
{
	m_pUI_BG		= Sprite::create("UI/UI_BG.png");
	m_pUI_Title		= Sprite::create("UI/Title.png");
	m_pUI_HighScore = Sprite::create("UI/HIGH_SCORE.png");
	m_pUI_Score		= Sprite::create("UI/SCORE.png");
	m_pUI_HP		= Sprite::create("UI/PLAYER_HP.png");
	m_pUI_MP		= Sprite::create("UI/PLAYER_MP.png");
	m_pUI_Power		= Sprite::create("UI/POWER.png");
	m_pHighScore	= Label::createWithBMFont("BMFonts/NotoSansKR_Bold_50px.fnt", "");
	m_pScore		= Label::createWithBMFont("BMFonts/NotoSansKR_Bold_50px.fnt", "000000000000");

	loadHighScore();

	presetGazeUI();
	presetAnchorPoint();
	presetPosition();
	presetAddChild();

	m_pHighScore->setScale(0.8f);
	m_pScore->setScale(0.8f);
}

void C_Player_UI::presetAnchorPoint()
{
	Point posAnchor(0.0f, 0.5f);

	m_pUI_HighScore->setAnchorPoint(posAnchor);
	m_pUI_Score->setAnchorPoint(posAnchor);
	m_pUI_HP->setAnchorPoint(posAnchor);
	m_pUI_MP->setAnchorPoint(posAnchor);
	m_pUI_Power->setAnchorPoint(posAnchor);
	m_pHighScore->setAnchorPoint(posAnchor);
	m_pScore->setAnchorPoint(posAnchor);
}

void C_Player_UI::presetPosition()
{
	m_pUI_BG->setPosition(640.0f, 360.0f);
	m_pUI_Title->setPosition(1104.0f, 202.0f);
	m_pUI_HighScore->setPosition(936.0f, 645.0f);
	m_pUI_Score->setPosition(936.0f, 595.0f);
	m_pUI_HP->setPosition(936.0f, 520.0f);
	m_pUI_MP->setPosition(936.0f, 472.0f);
	m_pUI_Power->setPosition(936.0f, 424.0f);

	m_pHP_Gaze->setPosition(1058.0f, 520.0f);
	m_pMP_Gaze->setPosition(1058.0f, 472.0f);
	m_pPower_Gaze->setPosition(1058.0f, 424.0f);

	m_pHighScore->setPosition(1075.0f, 647.0f);
	m_pScore->setPosition(1075.0f, 597.0f);
}

void C_Player_UI::presetGazeUI()
{
	Rect recGaze(0.0f, 0.0f, 217.0f, 33.0f);

	m_pHP_Gaze = C_ProgressBar::create();
	m_pMP_Gaze = C_ProgressBar::create();
	m_pPower_Gaze = C_ProgressBar::create();
	
	m_pHP_Gaze->setBackground("UI/Progress_BG.png");
	m_pMP_Gaze->setBackground("UI/Progress_BG.png");
	m_pPower_Gaze->setBackground("UI/Progress_BG.png");
	m_pHP_Gaze->setBorder("UI/Progress_Border.png");
	m_pMP_Gaze->setBorder("UI/Progress_Border.png");
	m_pPower_Gaze->setBorder("UI/Progress_Border.png");

	m_pHP_Gaze->setProgressed(Color3B(255, 0, 0), recGaze);
	m_pMP_Gaze->setProgressed(Color3B(0, 72, 255), recGaze);
	m_pPower_Gaze->setProgressed(Color3B(183, 0, 255), recGaze);

	m_pHP_Gaze->setProgressMeter(50.0f);
	m_pMP_Gaze->setProgressMeter(75.0f);
	m_pPower_Gaze->setProgressMeter(100.0f);
}

void C_Player_UI::presetAddChild()
{
	addChild(m_pUI_BG);
	addChild(m_pUI_Title);
	addChild(m_pUI_HighScore);
	addChild(m_pUI_Score);
	addChild(m_pUI_HP);
	addChild(m_pUI_MP);
	addChild(m_pUI_Power);
	addChild(m_pHP_Gaze);
	addChild(m_pMP_Gaze);
	addChild(m_pPower_Gaze);
	addChild(m_pHighScore);
	addChild(m_pScore);
}
