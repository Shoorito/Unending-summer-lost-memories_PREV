#include "PlayScene.h"
#include "Character.h"
#include "KeyEventManager.h"
#include "WeaponManager.h"
#include "EnemyManager.h"
#include "LinkedEnemy.h"
#include "Enemy.h"
#include "SubWeapon.h"
#include "SubWeaponManager.h"
#include "DanmakuManager.h"
#include "FieldManager.h"
#include "Danmaku.h"
#include "PatternsLib.h"
#include "PlayerUI.h"
#include "BG_Scroll.h"
#include "BossManager.h"
#include "TalkManager.h"
#include "Summoner.h"
#include "AudioEngine.h"
#include "E_PatternsLib.h"
#include "PauseGame.h"
#include "ResourceList.h"

using namespace experimental;

C_PlayScene* C_PlayScene::m_pMyPointer = nullptr;

C_PlayScene * C_PlayScene::create()
{
	if (m_pMyPointer)
	{
		CCLOG("The Scene is already...");

		return nullptr;
	}

	m_pMyPointer = new(std::nothrow) C_PlayScene();

	m_pMyPointer->init();
	m_pMyPointer->preset();

	return m_pMyPointer;
}

void C_PlayScene::allFieldItemPause()
{
	C_WeaponManager*	 pWManager(nullptr);
	C_EnemyManager*		 pEManager(nullptr);
	C_Subweapon_Manager* pSManager(nullptr);
	C_Danmaku_Manager*	 pDManager(nullptr);
	C_Player_UI*		 pUI(nullptr);

	pWManager = C_WeaponManager::getInstance();
	pEManager = C_EnemyManager::getInstance();
	pSManager = C_Subweapon_Manager::getInstance();
	pDManager = C_Danmaku_Manager::getInstance();
	pUI = C_Player_UI::getInstance();

	C_Scroll_BG::getInstance()->pause();
	pSManager->pauseWeapons();
	pWManager->getWeaponsLayer(E_WEAPON_TYPE::E_NORMAL)->pause();
	pWManager->getWeaponsLayer(E_WEAPON_TYPE::E_SUB)->pause();
	C_Character::getInstance()->getCharacter()->pause();
	pDManager->getLayer()->pause();
	C_BossManager::getInstance()->pause();
	pEManager->getLayer()->pause();
	C_TalkManager::getInstance()->pause();
	C_Field_Manager::getInstance()->pause();
	pUI->pause();
	C_KeyEvent_Manager::getInstance();
}

void C_PlayScene::init()
{
	m_pMainScene  = nullptr;
	m_pGameField  = nullptr;
	m_pPopupLayer = nullptr;
	m_recWinRect  = Rect::ZERO;
}

void C_PlayScene::preset()
{
	AudioEngine::lazyInit();

	m_pMainScene  = Scene::create();
	m_pGameField  = Layer::create();
	m_pPopupLayer = Layer::create();

	m_recWinRect.setRect(60.0f, 27.0f, 867.0f, 666.0f);

	createKeyEvent();
	createBG_Scroll();
	createDanmaku();
	createCharacter();
	createWeapons();
	createSubWeapons();
	createEnemy();
	createSummoner();
	createLauncher();
	createUI();
	createDialogue();
	createBoss();
	createPattern();
	createEnemyPattern();
	createFieldEvent();
	createPopup();

	addChilds();
	actFunc();

	AudioEngine::play2d("Theme_01.mp3", true, 0.2f);
}

void C_PlayScene::createBG_Scroll()
{
	C_Scroll_BG::create();
}

void C_PlayScene::createCharacter()
{
	C_Character* pCharacter(nullptr);

	pCharacter = C_Character::create();

	pCharacter->setTextureWithRect(g_arChracterResList[0], Rect(0.0f, 0.0f, 288.0f, 540.0f));
	pCharacter->setPosition(640.0f, 360.0f);
	pCharacter->setScale(0.125f);
	pCharacter->setEnabled(true);
}

void C_PlayScene::createBoss()
{
	C_BossManager* pBoss(nullptr);

	pBoss = C_BossManager::create();

	pBoss->setEnabled(false);
	//C_EnemyManager::getInstance()->changeEnemy(pBoss->getBoss()->getLinker(), E_USE_TYPE::E_USED);
	//pBoss->getBoss()->startPattern();
}

void C_PlayScene::createWeapons()
{
	C_WeaponManager* pWManager(nullptr);

	pWManager = C_WeaponManager::create();

	for (int nWeapon(0); nWeapon < g_nWeaponCount; nWeapon++)
	{
		C_PlayerWeapon*  pWeapon(nullptr);
		C_Linked_Weapon* pWLink(nullptr);
		
		pWeapon = C_PlayerWeapon::create(E_WEAPON_TYPE::E_NORMAL);
		pWLink  = C_Linked_Weapon::create();

		pWeapon->setTexture("bullet.png");
		pWeapon->setPosition(-500.0f, -500.0f);
		pWeapon->setId(nWeapon + 1);

		pWLink->setData(pWeapon);
		pWeapon->setLink(pWLink);
		pWManager->addWeapon(pWLink, E_WEAPON_TYPE::E_NORMAL);
	}
}

void C_PlayScene::createSubWeapons()
{
	C_WeaponManager* pWManager(nullptr);

	pWManager = C_WeaponManager::getInstance();

	for (int nSubWeapon(0); nSubWeapon < g_nSubWeaponCount; nSubWeapon++)
	{
		C_PlayerWeapon*  pWeapon(nullptr);
		C_Linked_Weapon* pWLink(nullptr);

		pWeapon = C_PlayerWeapon::create(E_WEAPON_TYPE::E_SUB);
		pWLink  = C_Linked_Weapon::create();

		pWeapon->setTexture("Subweapon_02.png");
		pWeapon->setPosition(-500.0f, -500.0f);
		pWeapon->setId(nSubWeapon + 1);

		pWLink->setData(pWeapon);
		pWeapon->setLink(pWLink);
		pWManager->addWeapon(pWLink, E_WEAPON_TYPE::E_SUB);
	}
}

void C_PlayScene::createLauncher()
{
	C_Subweapon_Manager* pManager(nullptr);

	pManager = C_Subweapon_Manager::create();
	
	pManager->createLaunchersArray(g_nLauncherCount);

	for (int nLauncher(0); nLauncher < g_nLauncherCount; nLauncher++)
	{
		C_SubWeapon* pSub(nullptr);

		pSub = C_SubWeapon::create();

		pSub->setTexture("Launcher_1.png");
		
		pManager->setData(pSub, nLauncher);
	}
}

void C_PlayScene::createEnemy()
{
	C_EnemyManager* pManager(nullptr);

	pManager = C_EnemyManager::create();

	for (int nEnemy(0); nEnemy < 50; nEnemy++)
	{
		C_Enemy* pEnemy(nullptr);
		C_Linked_Enemy* pNode(nullptr);

		pEnemy = C_Enemy::createWithAnimationAtlas("Enemy/Enemy_Fairy.png", Rect(Vec2::ZERO, Size(60.0f, 54.0f)), 5);
		pNode = C_Linked_Enemy::create();

		pNode->setData(pEnemy);
		pManager->addEnemy(pNode);

		pEnemy->setEnabled(false);
		pEnemy->setPosition(150.0f + (200.0f * nEnemy), 500.0f);
		pEnemy->setHP(3000.0f);
		pEnemy->setLinker(pNode);
		pEnemy->setId(nEnemy + 1);
		pEnemy->setType(E_ENEMY_TYPE::E_NORMAL);
		pEnemy->setAnimationWithRect("Enemy/Enemy_Fairy.png", Rect(Vec2(0.0f, 54.0f), Size(60.0f, 54.0f)), 5, 1);
		pEnemy->setAnimationWithRect("Enemy/Enemy_Fairy.png", Rect(Vec2(0.0f, 108.0f), Size(60.0f, 54.0f)), 5, 2);
		pEnemy->setAnimationWithRect("Enemy/middleBoss.png",  Rect(Vec2::ZERO, Size(100.0f, 87.0f)), 5, 3);
		//pEnemy->startAnimation(0);
	}
	
}

void C_PlayScene::createSummoner()
{
	/*
	C_EnemyManager* pManager(nullptr);

	pManager = C_EnemyManager::getInstance();

	for (int nEnemy(0); nEnemy < 50; nEnemy++)
	{
		C_Summoner* pEnemy(nullptr);
		C_Linked_Enemy* pNode(nullptr);

		pEnemy = C_Summoner::create();
		pNode  = C_Linked_Enemy::create();

		pNode->setData(pEnemy);
		pManager->addEnemy(pNode);

		pEnemy->setEnabled(false);
		pEnemy->setAnimationWithRect("Enemy/Enemy_Fairy.png", Rect(Vec2(0.0f, 54.0f), Size(60.0f, 54.0f)), 5, 0);
		pEnemy->setPosition(-500.0f, -500.0f);
		pEnemy->setHP(100.0f);
		pEnemy->setLinker(pNode);
		pEnemy->setId(nEnemy + 1);
		pEnemy->setType(E_ENEMY_TYPE::E_SUMMONER);
	}
	*/
}

void C_PlayScene::createDanmaku()
{
	C_Danmaku_Manager* pDManager(nullptr);

	pDManager = C_Danmaku_Manager::create();

	for (int nDanmaku(0); nDanmaku < g_nDanmakuCount; nDanmaku++)
	{
		C_Danmaku* pDanamku(nullptr);
		
		pDanamku = C_Danmaku::create();

		pDanamku->setEnabled(false);
		pDanamku->setTextureWithRect(g_strDanmaku, Rect(4.0f, 0.0f, 56.0f, 56.0f));
		pDanamku->setPosition(-500.0f, -500.0f);
		pDanamku->setId(nDanmaku + 1);
		pDManager->addDanmaku(pDanamku);
	}
}

void C_PlayScene::createPattern()
{
	C_Pattern_Library::create();
}

void C_PlayScene::createDialogue()
{
	C_TalkManager* pTalker(nullptr);

	pTalker = C_TalkManager::create();

	pTalker->setEnabled(false);
}

void C_PlayScene::createKeyEvent()
{
	C_KeyEvent_Manager* pManager(nullptr);

	pManager = C_KeyEvent_Manager::create();
	
	pManager->setEnabled(true);
}

void C_PlayScene::createPopup()
{
	C_PausePopup* pPopup(nullptr);

	pPopup = C_PausePopup::create();

	pPopup->setEnabled(false);
}

void C_PlayScene::createFieldEvent()
{
	C_Field_Manager* pField(nullptr);

	pField = C_Field_Manager::create();
	
	pField->startEvent();
}

void C_PlayScene::createEnemyPattern()
{
	C_E_PatternLib::create();
}

void C_PlayScene::createUI()
{
	C_Player_UI::create();
}

void C_PlayScene::addChilds()
{
	C_WeaponManager*	 pWManager(nullptr);
	C_EnemyManager*		 pEManager(nullptr);
	C_Subweapon_Manager* pSManager(nullptr);
	C_Danmaku_Manager*	 pDManager(nullptr);
	C_Player_UI*		 pUI(nullptr);

	pWManager = C_WeaponManager::getInstance();
	pEManager = C_EnemyManager::getInstance();
	pSManager = C_Subweapon_Manager::getInstance();
	pDManager = C_Danmaku_Manager::getInstance();
	pUI		  = C_Player_UI::getInstance();

	m_pGameField->addChild(C_Scroll_BG::getInstance());
	m_pGameField->addChild(pSManager->getLaunchersLayer());
	m_pGameField->addChild(pWManager->getWeaponsLayer(E_WEAPON_TYPE::E_NORMAL));
	m_pGameField->addChild(pWManager->getWeaponsLayer(E_WEAPON_TYPE::E_SUB));
	m_pGameField->addChild(C_Character::getInstance());
	m_pGameField->addChild(pDManager->getLayer());
	m_pGameField->addChild(C_BossManager::getInstance());
	m_pGameField->addChild(pEManager->getLayer());
	m_pGameField->addChild(C_TalkManager::getInstance());
	m_pGameField->addChild(C_Field_Manager::getInstance());
	m_pGameField->addChild(pUI, 2);
	m_pGameField->addChild(C_KeyEvent_Manager::getInstance());
	m_pPopupLayer->addChild(C_PausePopup::getInstance());

	m_pMainScene->addChild(m_pGameField);
	m_pMainScene->addChild(m_pPopupLayer);
}

void C_PlayScene::actFunc()
{
	CallFunc* pFunc(nullptr);
	DelayTime* pDelay(nullptr);
	Sequence* pSequancer(nullptr);

	pFunc = CallFunc::create(CC_CALLBACK_0(C_PlayScene::callSubweapon, this));
	pDelay = DelayTime::create(0.01f);
	pSequancer = Sequence::create(pDelay, pFunc, nullptr);

	m_pMainScene->runAction(pSequancer);
}

void C_PlayScene::callSubweapon()
{	
	C_Subweapon_Manager::getInstance()->setEnabled(0, true);
	C_Subweapon_Manager::getInstance()->setEnabled(1, true);
	C_Subweapon_Manager::getInstance()->setEnabled(2, true);
	C_Subweapon_Manager::getInstance()->setEnabled(3, true);
}
