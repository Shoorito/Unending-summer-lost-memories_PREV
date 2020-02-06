#pragma once
#include "cocos2d.h"
#include "EnumList.h"

USING_NS_CC;

class C_Danmaku;

class C_Danmaku_Manager
{
public:
	static C_Danmaku_Manager* create();
	static inline C_Danmaku_Manager* getInstance() { return m_pInstance; }
public:
	virtual inline Layer* getLayer() const { return m_pGroupLayer; }
	virtual inline const int& getUseDanmakuCount(const E_USE_TYPE& eType) const { return m_arNodeCount[(int)eType]; }
public:
	// "getDanmaku"에서는 "현재 'Cursor'의 위치"의 데이터를 반환합니다.
	// 따라서 다른 데이터를 얻기 위해서는 커서의 위치를 이동시켜야 합니다.
	virtual inline C_Danmaku* getDanmaku(const E_USE_TYPE& eType) { return m_arCursor[(int)eType]; }
	virtual void moveCursor(const int& nMoveCount = 1, const E_USE_TYPE& eType = E_USE_TYPE::E_NOT_USED);
	virtual void initCursor(const E_USE_TYPE& eType);
public:
	// INFO 아래 함수들의 기능에 대한 정보입니다.
	// addEnemy는 게임의 적들을 맨 처음 "생성"할 때만 사용합니다.
	// addEnemy에 추가되는 노드들은 모두 "Not Used"상태로 추가됩니다.
	// 사용을 원할 경우 별도로 상태를 "Used"로 변화시켜주어야 합니다.
	// pushEnemy는 적 노드를 "Tail"자리에 배치합니다.
	// popDanmaku는 두 타입으로 나뉩니다.
	// popDanmaku의 "꺼낼 노드를 입력하는 타입"에서는 해당 노드를 이용해 바로 빼냅니다.
	// 나머지 유형은 "지정하지 않는 타입"으로 "Tail"에 위치한 노드를 빼냅니다.
	// popDanmaku는 노드를 제거하지 않으므로 제거하기 위해선 "removeDanmaku"를 사용해야 합니다.
	// changeEnemy는 입력된 노드를 입력한 타입의 리스트로 이동시킵니다.
	// removeEnemy는 입력된 노드를 완전히 제거합니다. 따라서 게임 도중 사용은 추천하지 않습니다.
	virtual void addDanmaku(C_Danmaku* pDanmaku);
	virtual void pushDanmaku(C_Danmaku* pDanmaku, const E_USE_TYPE& eType);
	virtual C_Danmaku* popDanmaku(const E_USE_TYPE& eType);
	virtual C_Danmaku* popDanmaku(C_Danmaku* pDanmaku, const E_USE_TYPE& eType);
	virtual void changeDanmaku(C_Danmaku* pDanmaku, const E_USE_TYPE& eType);
	virtual void removeDanmaku(C_Danmaku* pDanmaku, const E_USE_TYPE& eType);
	virtual void allDisabledDanmaku();
public:
	// 해당 함수는 현재 커서가 위치한 데이터(탄막)의 "Rect"정보를 변경합니다.
	// 추가 적인 생성을 막기위해 제한된 레퍼런스 데이터 형태로 값을 받습니다.
	// 마찬가지로 다른 데이터의 값을 변경하고 싶은 경우 "moveCursor"기능을 이용하셔야 합니다.
	virtual void changeDanmakuRect(const Rect& recDanmaku, const E_USE_TYPE& eType);
private:
	virtual void init();
	virtual void preset();
private:
	C_Danmaku* m_arHead[(int)E_USE_TYPE::E_MAX]{};
	C_Danmaku* m_arTail[(int)E_USE_TYPE::E_MAX]{};
	C_Danmaku* m_arCursor[(int)E_USE_TYPE::E_MAX]{};
private:
	int m_arNodeCount[(int)E_USE_TYPE::E_MAX]{};
private:
	Layer* m_pGroupLayer;
private:
	static C_Danmaku_Manager* m_pInstance;
private:
	C_Danmaku_Manager() {}
	virtual ~C_Danmaku_Manager() {}
	C_Danmaku_Manager(C_Danmaku_Manager&) = delete;
	C_Danmaku_Manager operator=(C_Danmaku_Manager&) = delete;
};