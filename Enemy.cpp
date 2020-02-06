#include "Enemy.h"
#include "Danmaku.h"
#include "EnemyManager.h"
#include "FieldManager.h"
#include "DanmakuManager.h"
#include "PatternsLib.h"
#include "Character.h"
#include "AudioEngine.h"

using namespace experimental;

C_Enemy * C_Enemy::create()
{
	C_Enemy* pEnemy(nullptr);

	pEnemy = new(std::nothrow) C_Enemy();

	pEnemy->init();
	pEnemy->preset();

	return pEnemy;
}

C_Enemy * C_Enemy::createWithAnimation(std::string* arFiles, const int nCount)
{
	if (nCount <= 0 || !arFiles)
		return nullptr;

	C_Enemy* pEnemy(nullptr);
	Animation* pAnim(nullptr);

	pEnemy = new(std::nothrow) C_Enemy();

	pEnemy->init();
	pEnemy->preset();

	pAnim = Animation::create();

	pAnim->setDelayPerUnit(0.2f);

	for (int nAnim(0); nAnim < nCount; nAnim++)
	{
		pEnemy->setAnimationFile(arFiles[nAnim]);
		pAnim->addSpriteFrameWithFile(arFiles[nAnim]);
	}

	pAnim->retain();

	pEnemy->m_arAnim.push_back(pAnim);

	return pEnemy;
}

C_Enemy * C_Enemy::createWithAnimationAtlas(const std::string & strFile, const Rect & recSize, const int & nCount)
{
	if (strFile == "" || nCount <= 0)
		return nullptr;

	C_Enemy* pEnemy(nullptr);
	Rect	 recAnim(recSize);
	Animation* pAnim(nullptr);

	pEnemy = new(std::nothrow) C_Enemy();

	pEnemy->init();
	pEnemy->preset();
	pEnemy->setAnimationFile(strFile);

	pAnim = Animation::create();

	pAnim->setDelayPerUnit(0.2f);

	for (int nAnim(0); nAnim < nCount; nAnim++)
	{
		Sprite* pSprite(nullptr);

		pSprite = Sprite::create(strFile);
		pSprite->setTextureRect(recAnim);
		
		pAnim->addSpriteFrame(pSprite->getSpriteFrame());

		recAnim.setRect
		(
			recAnim.origin.x + recSize.size.width,
			recAnim.origin.y,
			recSize.size.width,
			recSize.size.height
		);
	}

	pAnim->retain();

	pEnemy->m_arAnim.push_back(pAnim);

	return pEnemy;
}

void C_Enemy::setHP(const float fHP)
{
	if (fHP <= 0)
	{
		setEnabled(false);
		setPosition(-500.0f, -500.0f);
		C_EnemyManager::getInstance()->changeEnemy(m_pMyLinker, (E_USE_TYPE)m_isWorking);

		if (m_isMiddleBoss)
		{
			m_isMiddleBoss = false;
			C_Field_Manager::getInstance()->setNowMiddleBossPaze(false);

			AudioEngine::play2d("se_enep01.mp3", false, 0.1f);
		}

		CCLOG("Enemy is Destroy.");
		return;
	}

	std::string strLog("Remote HP is ");

	m_fHP = fHP;
	
	strLog += std::to_string(m_fHP);

	CCLOG(strLog.c_str());
}

void C_Enemy::setTextureWithRect(const char * arFile, const Rect& recSprite)
{
	setTexture(arFile);
	setTextureRect(recSprite);
}

void C_Enemy::setTextureWithRect(const std::string & strFile, const Rect& recSprite)
{
	setTexture(strFile);
	setTextureRect(recSprite);
}

void C_Enemy::setEnabled(const bool isEnabled)
{
	m_isWorking = isEnabled;

	setVisible(m_isWorking);
	
	if (m_isWorking)
	{
		m_nUpdateCount = 0;
		resume();
	}
	else
	{
		stopAllActions();
		pause();
	}
}

void C_Enemy::setAnimationFile(const std::string& strFileName, const int nType)
{
	if (nType < 0)
		return;

	if ((int)m_vecAnimFiles.size() - 1 < nType)
	{
		std::vector<std::string>* pNewVector(nullptr);

		pNewVector = new(std::nothrow) std::vector<std::string>();

		pNewVector->emplace_back(strFileName);

		m_vecAnimFiles.push_back(pNewVector);
	}
	else
	{
		m_vecAnimFiles.at(nType)->emplace_back(strFileName);
	}
}

void C_Enemy::setAnimationWithRect(const std::string & strFile, const Rect & recSize, const int & nCount, const int& nType)
{
	Rect recAnim(recSize);
	Animation* pAnim(nullptr);

	setAnimationFile(strFile, nType);
	
	pAnim = Animation::create();
	
	pAnim->setDelayPerUnit(0.2f);

	for (int nAnim(0); nAnim < nCount; nAnim++)
	{
		Sprite* pSprite(nullptr);

		pSprite = Sprite::create(strFile);
		pSprite->setTextureRect(recAnim);

		pAnim->addSpriteFrame(pSprite->getSpriteFrame());

		recAnim.setRect
		(
			recAnim.origin.x + recSize.size.width,
			recAnim.origin.y,
			recSize.size.width,
			recSize.size.height
		);
	}

	pAnim->retain();
	m_arAnim.push_back(pAnim);
}

bool C_Enemy::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	m_pMyLinker		= nullptr;
	m_pActNode		= nullptr;
	m_isWorking		= false;
	m_isNowUsedUpdate = false;
	m_isMiddleBoss  = false;
	m_fHP			= 0.0f;
	m_nId			= 0;
	m_nUpdateCount  = 0;

	return true;
}

void C_Enemy::preset()
{
	m_arPatternList[0] = &C_Enemy::pattern01;
	m_arPatternList[1] = &C_Enemy::pattern02;
	m_arPatternList[2] = &C_Enemy::pattern03;
	m_arPatternList[3] = &C_Enemy::pattern04;
	m_arPatternList[4] = &C_Enemy::pattern05;
	m_arPatternList[5] = &C_Enemy::patternSummoner;

	m_pActNode = Node::create();

	schedule(schedule_selector(C_Enemy::patternByNormal), 0.01f);

	addChild(m_pActNode);

	//setVisible(false);
}

void C_Enemy::onEnter()
{
	bool isRunning(false);

	isRunning = this->isRunning();

	Node::onEnter();

	if (!isRunning && !m_isWorking)
		pause();
}

void C_Enemy::patternByNormal(float fDelay)
{
	if (m_isNowUsedUpdate)
		return;

	m_isNowUsedUpdate = true;

	if (m_nUpdateCount >= 1000000000)
		m_nUpdateCount = 0;

	(this->*m_arPatternList[m_nEnemyType])(m_nUpdateCount);

	m_nUpdateCount++;
	m_isNowUsedUpdate = false;
}

void C_Enemy::pattern01(const int nCount)
{
	if (nCount % 40 != 0)
		return;

	C_Pattern_Library::getInstance()->aimingBullet(getPosition());
}

void C_Enemy::pattern02(const int nCount)
{
	if (nCount % 200 != 0)
		return;

	C_Pattern_Library::getInstance()->nWayByPlayer(getPosition(), 5, 2.0f);
}

void C_Enemy::pattern03(const int nCount)
{
	if (nCount % 200 != 0)
		return;

	C_Pattern_Library::getInstance()->repeatCircleNWay(getPosition(), 8, 2.5f, 5, 0.1f, m_pActNode);
}

void C_Enemy::pattern04(const int nCount)
{
	static float fAngle(0.0f);

	if (nCount % 40 != 0)
		return;

	if (fAngle >= 360.0f)
		fAngle = 0.0f;

	C_Pattern_Library::getInstance()->circleNWay(nullptr, getPosition(), 15, fAngle, 3.0f);

	if (nCount % 200 == 0)
	{
		C_Pattern_Library::getInstance()->aimingBullet(getPosition());
	}

	fAngle += 10.0f;
}

void C_Enemy::pattern05(const int nCount)
{

}

void C_Enemy::patternSummoner(const int nCount)
{
	if (nCount % 50 != 0)
		return;

	C_Pattern_Library::getInstance()->circleNWay(nullptr, getPosition(), 10, 0.0f, 2.0f);
}

void C_Enemy::startAnimation(const int nType)
{
	if (!(int)m_vecAnimFiles.size() || (int)m_vecAnimFiles.size() < nType || nType < 0)
		return;

	Animate* pAnimate(nullptr);
	Repeat* pRepeat(nullptr);
	
	pAnimate		= Animate::create(m_arAnim[nType]);
	pRepeat			= Repeat::create(pAnimate, -1);
	m_arAnim[nType] = m_arAnim[nType]->clone();
	
	m_arAnim[nType]->retain();
	//this->setTexture(m_vecAnimFiles[nType]->at(0));
	runAction(pRepeat);
}
