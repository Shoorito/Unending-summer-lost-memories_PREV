#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_SubWeapon;

class C_Subweapon_Manager
{
public:
	static C_Subweapon_Manager* create();
	static inline C_Subweapon_Manager* getInstance() { return m_pInstance; }
public:
	virtual inline Layer* getLaunchersLayer() { return m_pLaunchersLayer; }
	virtual inline C_SubWeapon* getData(const int nDatapos) { return m_arSubLaunchers[nDatapos]; }
	virtual inline const int getCount() { return m_nMaxCount; }
	virtual inline const int getEnabledCount() { return m_nLauncherCount; }
public:
	// 서브 웨폰의 특성 상 처음부터 정해진 수를 최대치로 가지기 때문에
	// 기존의 "연결 리스트"들과 다르게 배열로 관리합니다.
	// 해당 함수는 생성 단계에서 단 한 번만 호출해야 하며 이후 부르게 될 경우
	// 이전에 생성 된 객체들을 모두 제거하고 다시 생성합니다.
	virtual void createLaunchersArray(const int nCount = 0);
public:
	virtual void setData(C_SubWeapon* pData, const int nDatapos);
	virtual void setEnabled(const int nDatapos, const bool isEnabled);
	virtual void pauseWeapons();
private:
	virtual void init();
	virtual void preset();
	virtual void createUpdateActions();
	virtual void update(float dt);
private:
	C_SubWeapon** m_arSubLaunchers;
private:
	std::function<void(float)> m_funcUpdater{};
private:
	Scheduler* m_pScheduler;
private:
	int m_nLauncherCount;
	int m_nMaxCount;
private:
	bool m_isEvenCount;
	bool* m_arEnabledList;
private:
	Layer* m_pLaunchersLayer;
private:
	static C_Subweapon_Manager* m_pInstance;
};