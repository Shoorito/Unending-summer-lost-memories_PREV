#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_PlayScene
{
public:
	static C_PlayScene* create();
	static inline C_PlayScene* getInstance() { return m_pMyPointer; }
public:
	virtual inline Scene* getMainScene()	  { return m_pMainScene; }
	virtual inline Layer* getGameFieldLayer() { return m_pGameField; }
	virtual inline Rect& getWindoowRect()	  { return m_recWinRect; }
public:
	virtual void allFieldItemPause(); // 사용에 유의!
private:
	virtual void init();
	virtual void preset();
	virtual void createBG_Scroll();
	virtual void createCharacter();
	virtual void createBoss();
	virtual void createWeapons();
	virtual void createSubWeapons();
	virtual void createLauncher();
	virtual void createEnemy();
	virtual void createSummoner();
	virtual void createDanmaku();
	virtual void createPattern();
	virtual void createDialogue();
	virtual void createKeyEvent();
	virtual void createPopup();
	virtual void createFieldEvent();
	virtual void createEnemyPattern();
	virtual void createUI();
private:
	virtual void addChilds();
	virtual void actFunc();
	virtual void callSubweapon();
private:
	static C_PlayScene* m_pMyPointer;
private:
	Layer* m_pGameField;
	Layer* m_pPopupLayer;
	Scene* m_pMainScene;
private:
	Rect m_recWinRect;
private:
	C_PlayScene() {}
	virtual ~C_PlayScene() {}
	C_PlayScene(C_PlayScene&) = delete;
	C_PlayScene operator=(C_PlayScene&) = delete;
};