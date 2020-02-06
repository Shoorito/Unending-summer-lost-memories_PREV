#include "WeaponManager.h"
#include "Character.h"
#include "KeyEventManager.h"
#include "AudioEngine.h"
#include "SubWeaponManager.h"
#include "SubWeapon.h"

using namespace experimental;

C_WeaponManager * C_WeaponManager::m_pInstance = nullptr;

C_WeaponManager * C_WeaponManager::create()
{	
	m_pInstance = new(std::nothrow) C_WeaponManager();

	m_pInstance->init();
	m_pInstance->preset();
	m_pInstance->initUpdate();

	return m_pInstance;
}

void C_WeaponManager::addWeapon(C_Linked_Weapon * pWeaponLink, E_WEAPON_TYPE eType)
{
	linkWeapon(pWeaponLink, eType);

	m_arWeaponsLayer[(int)eType]->addChild(pWeaponLink->getData()->getLayer());

	m_arWeaponCount[(int)eType]++;
}

void C_WeaponManager::removeWeapon(C_Linked_Weapon * pWeaponLink, E_WEAPON_TYPE eType)
{
	if (!m_arWeaponCount[(int)eType])
		return;

	pWeaponLink->release();

	pWeaponLink = nullptr;
}

void C_WeaponManager::changeWeapon(C_Linked_Weapon * pWeaponLink, E_WEAPON_TYPE eType)
{
	if (m_arWeaponCount[(int)eType] == 1)
		return;

	if (pWeaponLink == m_arHead[(int)eType])
	{
		m_arHead[(int)eType] = pWeaponLink->getNext();
	}

	pWeaponLink->getNext()->setPrev(pWeaponLink->getPrev());
	pWeaponLink->getPrev()->setNext(pWeaponLink->getNext());

	linkWeapon(pWeaponLink, eType);

	return;
}

void C_WeaponManager::linkWeapon(C_Linked_Weapon * pWeaponLink, E_WEAPON_TYPE eType)
{
	if (!m_arHead[(int)eType])
	{
		m_arHead[(int)eType]	= pWeaponLink;
		m_arTail[(int)eType]	= pWeaponLink;
		m_arCursor[(int)eType]	= pWeaponLink;

		pWeaponLink->setNext(pWeaponLink);
		pWeaponLink->setPrev(pWeaponLink);
	}
	else
	{
		m_arTail[(int)eType]->setNext(pWeaponLink);
		m_arHead[(int)eType]->setPrev(pWeaponLink);
		pWeaponLink->setPrev(m_arTail[(int)eType]);
		pWeaponLink->setNext(m_arHead[(int)eType]);

		m_arTail[(int)eType] = pWeaponLink;
	}
}

void C_WeaponManager::moveCursor(const int nMovepoint, E_WEAPON_TYPE eType)
{
	for (int nCursor(0); nCursor < nMovepoint; nCursor++)
	{
		m_arCursor[(int)eType] = m_arCursor[(int)eType]->getNext();
	}
}

void C_WeaponManager::init()
{
	m_isAttack = true;
}

void C_WeaponManager::preset()
{
	for (int nLayer(0); nLayer < (int)E_WEAPON_TYPE::E_MAX; nLayer++)
	{
		m_arWeaponsLayer[nLayer] = Layer::create();
	}
}

void C_WeaponManager::initUpdate()
{
	CallFunc*	pCallFunc(nullptr);
	DelayTime*	pDelay(nullptr);
	Sequence*	pSeq(nullptr);
	Repeat*		pRep(nullptr);

	pCallFunc	= CallFunc::create(CC_CALLBACK_0(C_WeaponManager::checkPushedKey, this));
	pDelay		= DelayTime::create(0.1f);
	pSeq		= Sequence::create(pCallFunc, pDelay, nullptr);
	pRep		= Repeat::create(pSeq, -1);

	m_arWeaponsLayer[0]->runAction(pRep);
}

void C_WeaponManager::checkPushedKey()
{
	E_KEY_STATE ePushed(E_KEY_STATE::E_NONE);

	ePushed = C_KeyEvent_Manager::getInstance()->getPushedType(149); // 149번은 Z_KEY

	if (ePushed == E_KEY_STATE::E_PUSH)
	{
		shootMainWeapon();
		shootSubWeapon();
		
		//AudioEngine::play2d("attack01.mp3", false, 0.05f);
	}
}

void C_WeaponManager::shootMainWeapon()
{
	if (!m_isAttack)
		return;

	Sprite* pWeapon(nullptr);
	C_Character* pCharacter(C_Character::getInstance());
	C_PlayerWeapon* pNode(nullptr);

	m_posCharacter.setPoint(pCharacter->getPositionX() - 15.0f, pCharacter->getPositionY() + 15.0f);

	for (int nWeapon(0); nWeapon < 2; nWeapon++)
	{
		pNode   = m_arCursor[(int)E_WEAPON_TYPE::E_NORMAL]->getData();
		pWeapon = pNode->getWeapon();

		pWeapon->setPosition(m_posCharacter);
		pNode->setEnabled(true);
		pNode->createMoveEvent();
		
		m_posCharacter += Point(30.0f, 0.0f);
		
		moveCursor();
	}
}

void C_WeaponManager::shootSubWeapon()
{
	if (!m_isAttack)
		return;

	C_Subweapon_Manager* pSManager(C_Subweapon_Manager::getInstance());
	C_PlayerWeapon* pNode(nullptr);
	C_SubWeapon* pSub(nullptr);

	for (int nWeapon(0); nWeapon < pSManager->getEnabledCount(); nWeapon++)
	{
		pSub  = pSManager->getData(nWeapon);
		pNode = m_arCursor[(int)E_WEAPON_TYPE::E_SUB]->getData();

		pNode->setPosition(pSub->getPositionX(), pSub->getPositionY());
		pNode->setNormalTarget(pSub->getPositionX(), pSub->getPositionY() + 1000.0f);
		pNode->setEnabled(true);

		moveCursor(1, E_WEAPON_TYPE::E_SUB);
	}
}
