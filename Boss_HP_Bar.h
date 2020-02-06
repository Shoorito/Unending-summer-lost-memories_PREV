#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "EnumList.h"

USING_NS_CC;
USING_NS_CC_EXT;

class C_Boss_HP_Bar : public Layer
{
public:
	static C_Boss_HP_Bar* create();
public:
	virtual const float& getHP() const { return m_arHP[m_nNowpos]; }
	virtual const float& getMaxHP(E_HP_TYPE& eType) const { return m_arMaxHP[(int)eType]; }
	virtual const float getAllHP() const;
	virtual inline const E_HP_BAR_TYPE& getHPBarType() const { return m_eBarType; }
	virtual inline const int& getNowpos() const { return m_nNowpos; }
public:
	virtual void setEnabled(const bool isEnabled);
	// 2019-10-26日 기능 변경 → setHP(const float, const E_HP_TYPE&) >> setHP(const float)
	// 기존의 HP기능에서 일일히 타입에 따른 HP를 줄이는 것보다
	// "현재 HP바 타입"에 따라 소모 또는 증가한 HP에 상대적으로 HP바를 감소시키는 것이
	// 보다 유연하다고 판단했습니다.
	virtual void setHP(float fHP);
public:
	// INFO 체력바 설정에 대한 설명입니다.
	// "setHPBar"는 실시간 갱신 용 함수가 아닙니다.
	// "setHPBar"기능은 "다른 타입의 HP바"를 보다 쉽게 작성하기 위해 작성한 "편의 함수"이며
	// 실제 HP가 아닌 "HPBar"의 크기만을 변화시켜줍니다.
	// 이는 실제 HP바의 크기와 HP 수치가 일치 않게끔 설계하였기 때문입니다.
	// 각각의 HP바의 "Max"수치를 지정하고 싶다면 "setMaxHP" 함수를 참조하십시오.

	// 나뉘지 않은 타입의 보스 HP바
	virtual void setHPBar(const float fXSize);
	// Normal, Spell로 나눈 타입의 HP바
	virtual void setHPBar(const float fXSize01, const float fXSize02);
	// Normal, Spell, LastSpell로 나눈 타입의 HP바
	virtual void setHPBar(const float fXSize01, const float fXSize02, const float fXSize03);
public:
	virtual void setMaxHP(const float fMaxHP, const E_HP_TYPE& eType);
	virtual inline void setBarColor(const E_HP_TYPE& eType, const Color3B& colorType) 
	{
		m_arBoss_HP[(int)eType]->setColor(colorType);
	}
private:
	virtual bool init();
	virtual void preset();
	virtual void presetBar();
private:
	Sprite* m_pBorder;
	Sprite* m_pBG;
	ui::Scale9Sprite* m_arBoss_HP[(int)E_HP_TYPE::E_MAX]{};
private:
	float m_arHP[(int)E_HP_TYPE::E_MAX]{};
	float m_arHPUnit[(int)E_HP_TYPE::E_MAX]{};
	float m_arMaxHP[(int)E_HP_TYPE::E_MAX]{};
	float m_arBarXSize[(int)E_HP_TYPE::E_MAX]{};
private:
	int m_nNowpos;
private:
	E_HP_BAR_TYPE m_eBarType;
private:
	C_Boss_HP_Bar() {}
 	virtual ~C_Boss_HP_Bar() {}
	C_Boss_HP_Bar(C_Boss_HP_Bar&) = delete;
	C_Boss_HP_Bar operator=(C_Boss_HP_Bar&) = delete;
};