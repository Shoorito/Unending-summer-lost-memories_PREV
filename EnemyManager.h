#pragma once
#include "cocos2d.h"
#include "EnumList.h"

USING_NS_CC;

class C_Enemy;
class C_Linked_Enemy;

class C_EnemyManager
{
public:
	static C_EnemyManager* create();
	static inline C_EnemyManager* getInstance() { return m_pInstance; }
public:
	virtual inline const int getEnemyCount(const E_USE_TYPE& eType) { return m_arNodeCount[(int)eType]; }
	virtual inline Layer* getLayer() { return m_pEnemiesLayer; }
public:
	// INFO 이 함수들은 아직 수정을 거치고 있습니다.
	// 적에 대한 정보는 "양방향 연결 리스트"로 관리 됩니다.
	// 한 번 타입에 따른 적의 정보를 얻을 때 마다 필요시 커서를 수동으로 이동시켜줘야 합니다.
	// "getEnemy"는 현재 커서가 위치한 노드의 데이터를 반환하니 참고 바랍니다.
	// 2019-10-04日 송명원
	virtual C_Enemy* getEnemy(const E_USE_TYPE& eType);

	// INFO 본 함수는 getEnemy함수와 비슷한 역할을 합니다.
	// 그러나 값을 이미 찾았으나 다른 값으로 갱신 할 필요가 있을 때 이 함수를 사용합니다.
	// 따라서 원하는 값이 없을 경우 pPrev 인수로 넣은 값이 재 반환되며
	// 원하는 값을 찾은 경우에만 찾은 값을 리턴합니다.
	virtual C_Enemy* checkEnemy(const E_USE_TYPE& eType, const E_ENEMY_TYPE& eEType, C_Enemy* pPrev);
	virtual inline C_Linked_Enemy* getLEnemy(const E_USE_TYPE& eType) { return m_arCursor[(int)eType]; }
	virtual void moveCursor(const E_USE_TYPE& eType, const int nMoveCount = 1);
	virtual void initCursor(const E_USE_TYPE& eType);
public:
	// INFO 아래 함수들의 기능에 대한 정보입니다.
	// addEnemy는 게임의 적들을 맨 처음 "생성"할 때만 사용합니다.
	// addEnemy에 추가되는 노드들은 모두 "Not Used"상태로 추가됩니다.
	// 사용을 원할 경우 별도로 상태를 "Used"로 변화시켜주어야 합니다.
	// pushEnemy는 적 노드를 "Tail"자리에 배치합니다.
	// popEnemy는 가장 마지막에 위치한 노드를 빼냅니다.
	// popEnemy는 노드를 제거하지 않으므로 제거하기 위해선 removeEnemy를 사용해야 합니다.
	// changeEnemy는 입력된 노드를 입력한 타입의 리스트로 이동시킵니다.
	// removeEnemy는 입력된 노드를 완전히 제거합니다. 따라서 게임 도중 사용은 추천하지 않습니다.
	virtual void addEnemy(C_Linked_Enemy* pEnemy);
	virtual void pushEnemy(C_Linked_Enemy* pEnemy, const E_USE_TYPE& eType);
	virtual C_Linked_Enemy* popEnemy(C_Linked_Enemy* pEnemy, const E_USE_TYPE& eType);
	virtual void changeEnemy(C_Linked_Enemy* pEnemy, const E_USE_TYPE& eType);
	virtual void removeEnemy(C_Linked_Enemy* pEnemy, const E_USE_TYPE& eType);
public:
	virtual void disabledAllEnemy(const E_ENEMY_TYPE& eType);
public:
	virtual C_Enemy* getImmediateEnemy(const Vec2* pCharacterPos);
private:
	virtual void init();
	virtual void preset();
private:
	C_Linked_Enemy* m_arHead[(int)E_USE_TYPE::E_MAX]{};
	C_Linked_Enemy* m_arTail[(int)E_USE_TYPE::E_MAX]{};
	C_Linked_Enemy* m_arCursor[(int)E_USE_TYPE::E_MAX]{};
private:
	int m_arNodeCount[(int)E_USE_TYPE::E_MAX]{};
	int m_nSummonNode;
private:
	Layer* m_pEnemiesLayer;
private:
	static C_EnemyManager* m_pInstance;
private:
	C_EnemyManager() {}
	virtual ~C_EnemyManager() {}
	C_EnemyManager(C_EnemyManager&) = delete;
	C_EnemyManager operator=(C_EnemyManager&) = delete;
};