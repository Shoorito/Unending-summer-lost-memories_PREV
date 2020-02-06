#include "PlayerWeapon.h"
#include "LinkedWeapon.h"
#include "EnemyManager.h"
#include "WeaponManager.h"
#include "FieldManager.h"
#include "Character.h"
#include "PlayScene.h"
#include "Enemy.h"

C_PlayerWeapon * C_PlayerWeapon::create(E_WEAPON_TYPE eType)
{
	C_PlayerWeapon* pWeapon(nullptr);

	pWeapon = new(std::nothrow) C_PlayerWeapon();

	pWeapon->init();
	pWeapon->setType(eType);
	pWeapon->preset();

	return pWeapon;
}

void C_PlayerWeapon::setLink(C_Linked_Weapon * pLink)
{
	m_pMyLinker = pLink;
}

void C_PlayerWeapon::setTexture(const char * arFile)
{
	m_pWeapon->setTexture(arFile);
}

void C_PlayerWeapon::setEnabled(const bool isEnabled)
{
	m_pWeaponLayer->setVisible(isEnabled);

	if (isEnabled)
	{
		m_isActEnd = false;
		m_pWeaponLayer->resume();

		if (m_eWeaponType == E_WEAPON_TYPE::E_SUB)
			m_pWeapon->resume();
	}
	else
	{
		m_isActEnd = true;
		m_pWeaponLayer->pause();

		if (m_eWeaponType == E_WEAPON_TYPE::E_SUB)
		{
			m_pWeapon->pause();
		}
		else
			m_pWeapon->stopAllActions();
	}
}

void C_PlayerWeapon::setTextureWithRect(const char * arFile, Rect recSprite)
{
	m_pWeapon->setTexture(arFile);
	m_pWeapon->setTextureRect(recSprite);
}

void C_PlayerWeapon::setNormalTarget(const float fXpos, const float fYpos)
{
	m_fNormalMoverX = fXpos;
	m_fNormalMoverY = fYpos;
}

void C_PlayerWeapon::setScale(const float fScale)
{
	m_pWeapon->setScale(fScale);
}

void C_PlayerWeapon::setPosition(const float fXpos, const float fYpos)
{
	m_fPositionX = fXpos;
	m_fPositionY = fYpos;

	m_pWeapon->setPosition(m_fPositionX, m_fPositionY);
}

void C_PlayerWeapon::setRotation(const float fRotation)
{
	m_pWeapon->setRotation(fRotation);
}

void C_PlayerWeapon::onIntersectsEnemy()
{
	if (m_isActEnd)
		return;

	int nEnemyNums(0);
	C_EnemyManager* pEManager(nullptr);

	pEManager  = C_EnemyManager::getInstance();
	nEnemyNums = pEManager->getEnemyCount(E_USE_TYPE::E_USED);

	pEManager->initCursor(E_USE_TYPE::E_USED);

	for (int nEnemy(0); nEnemy < nEnemyNums; nEnemy++)
	{
		Rect recWeapon(Rect::ZERO);
		C_Enemy* pEnemy(nullptr);

		pEnemy	  = pEManager->getEnemy(E_USE_TYPE::E_USED);
		recWeapon = m_pWeapon->getBoundingBox();

		if (pEnemy->getEnemyRect().intersectsRect(recWeapon)) // 무기와 적이 부딪히면?
		{
			int nHp(0);
		
			nHp = pEnemy->getHP();

			pEnemy->setHP(nHp - m_nAttack);

			m_isActEnd = true;

			disabledWeapon();

			return;
		}

		pEManager->moveCursor(E_USE_TYPE::E_USED);
	}
}

void C_PlayerWeapon::onIntersectsBorder()
{
	if (m_isActEnd)
		return;

	bool isBorderIn(false);

	isBorderIn = C_PlayScene::getInstance()->getWindoowRect().intersectsRect(m_pWeapon->getBoundingBox());

	if (!isBorderIn)
	{
		disabledWeapon();
	}
}

void C_PlayerWeapon::disabledWeapon()
{
	setEnabled(false);

	C_WeaponManager::getInstance()->changeWeapon(m_pMyLinker, m_eWeaponType);
}

void C_PlayerWeapon::updateSubWeapon()
{
	if (m_isUpdating || m_isActEnd)
	{
		return;
	}
	
	C_Enemy* pTarget(nullptr);
	C_EnemyManager* pEManager(C_EnemyManager::getInstance());

	m_isUpdating = true;
	
	pTarget = pEManager->getImmediateEnemy(&(C_Character::getInstance()->getPosition()));
	
	updateByGuided(pTarget);
	
	setPosition(m_fPositionX, m_fPositionY);
	setRotation(m_fShotAngle);

	m_isUpdating = false;
}

void C_PlayerWeapon::updateByGuided(C_Enemy* pTarget)
{
	if (!pTarget || isWinInner(pTarget))
	{
		m_fMoverX = m_fNormalMoverX;
		m_fMoverY = m_fNormalMoverY;
	}
	else
	{
		m_fMoverX = pTarget->getPositionX();
		m_fMoverY = pTarget->getPositionY();
	}
	
	m_fShotAngle = calcAngle(atan2f(m_fPositionY - m_fMoverY, m_fMoverX - m_fPositionX));

	m_fPositionX += cosf(-m_fShotAngle * 3.141592 / 180.0f) * m_fWeaponSpeed;
	m_fPositionY += sinf(-m_fShotAngle * 3.141592 / 180.0f) * m_fWeaponSpeed;
}

const float C_PlayerWeapon::calcAngle(const float fAngle)
{
	return fAngle * 180.0f / 3.141592f;
}

const float C_PlayerWeapon::calcSquare(const float fNumber)
{
	return fNumber * fNumber;
}

void C_PlayerWeapon::init()
{
	m_pWeapon		= nullptr;
	m_pWeaponLayer	= nullptr;
	m_nAttack		= 10;
	m_fWeaponSpeed  = 20.0f;
	m_fPositionX	= 0.0f;
	m_fPositionY	= 0.0f;
	m_fMoverX		= 0.0f;
	m_fMoverY		= 0.0f;
	m_fNormalMoverX = 0.0f;
	m_fNormalMoverY = 0.0f;
	m_fShotAngle	= 0.0f;
	m_isActEnd		= true;
	m_isUpdating	= false;
}

void C_PlayerWeapon::initAction()
{
	m_pWeaponLayer->setVisible(false);
	m_pWeaponLayer->pause();
	m_pWeapon->pause();
}

void C_PlayerWeapon::preset()
{
	if (m_eWeaponType == E_WEAPON_TYPE::E_SUB)
	{

	}

	m_pWeapon		= Sprite::create();
	m_pWeaponLayer	= Layer::create();
	m_pNode			= Node::create();

	m_pWeapon->setOpacity(255);
	m_pWeaponLayer->addChild(m_pNode);
	m_pWeaponLayer->addChild(m_pWeapon);

	createEvents();
}

void C_PlayerWeapon::createIntersectsEvent()
{
	CallFunc*	pFunc01(nullptr);
	CallFunc*	pFunc02(nullptr);
	DelayTime*	pDelay(nullptr);
	Sequence*	pSeq(nullptr);
	Repeat*		pRep(nullptr);

	pFunc01 = CallFunc::create(CC_CALLBACK_0(C_PlayerWeapon::onIntersectsEnemy, this));
	pFunc02	= CallFunc::create(CC_CALLBACK_0(C_PlayerWeapon::onIntersectsBorder, this));
	pDelay	= DelayTime::create(0.01f);
	pSeq	= Sequence::create(pFunc01, pFunc02, pDelay, nullptr);
	pRep	= Repeat::create(pSeq, -1);

	m_pWeaponLayer->runAction(pRep);	
}

void C_PlayerWeapon::createEvents()
{
	DelayTime*	pDelay(nullptr);
	CallFunc*	pFunc01(nullptr);
	CallFunc*	pFunc02(nullptr);
	CallFunc*	pFunc03(nullptr);
	Sequence*	pSeq(nullptr);

	pFunc01 = CallFunc::create(CC_CALLBACK_0(C_PlayerWeapon::createIntersectsEvent, this));
	pFunc02 = CallFunc::create(CC_CALLBACK_0(C_PlayerWeapon::createHomingEvent, this));
	pFunc03 = CallFunc::create(CC_CALLBACK_0(C_PlayerWeapon::initAction, this));
	pDelay = DelayTime::create(0.1f);
	pSeq = Sequence::create(pFunc01, pFunc02, pDelay, pFunc03, nullptr);

	m_pNode->runAction(pSeq);
}

void C_PlayerWeapon::createHomingEvent()
{
	if (m_eWeaponType != E_WEAPON_TYPE::E_SUB)
		return;

	CallFunc*	pUpdateFunc(nullptr);
	DelayTime*	pDelay(nullptr);
	Sequence*	pSequance(nullptr);
	Repeat*		pRepeat(nullptr);

	pUpdateFunc = CallFunc::create(CC_CALLBACK_0(C_PlayerWeapon::updateSubWeapon, this));
	pDelay		= DelayTime::create(0.01f);
	pSequance	= Sequence::create(pUpdateFunc, pDelay, nullptr);
	pRepeat		= Repeat::create(pSequance, -1);

	m_pWeapon->runAction(pRepeat);
}

bool C_PlayerWeapon::isWinInner(const C_Enemy* pTarget)
{
	if (C_PlayScene::getInstance()->getWindoowRect().intersectsRect(pTarget->getBoundingBox()))
		return false;

	return true;
}

void C_PlayerWeapon::createMoveEvent()
{
	MoveBy* pMove(nullptr);

	pMove = MoveBy::create(0.2f, Vec2(0.0f, 800.0f));

	m_pWeapon->runAction(pMove);
}

void C_PlayerWeapon::release()
{
	Scene* pMainScene(nullptr);

	pMainScene = C_PlayScene::getInstance()->getMainScene();
	
	pMainScene->removeChild(m_pWeaponLayer);

	init();

	delete this;
}
