#pragma once
#include "cocos2d.h"

USING_NS_CC;

const float g_fCharacterUpdateDelay = 0.001f;

class C_Character : public Sprite
{
public:
	static C_Character* create();
	static C_Character* getInstance() { return m_pInstance; }
public:
	virtual inline const float& getHP() const	{ return m_fPlayerHP; }
	virtual inline Sprite* getCharacter()		{ return this; }
	virtual inline const float& getMode() const { return m_arSpeedMode[m_isPushedShift]; }
public:
	virtual void setEnabled(const bool isSwitch);
public:
	virtual void setTextureWithRect(const char* arFile, const Rect& recSprite);
	virtual void setMoveBorder(const float fMinX, const float fMinY, const float fMaxX, const float fMaxY);
	virtual void setScale(const float fScale);
	virtual void setSpeed(const float fSpeed);
	virtual void setHP(const float fHP);
private:
	virtual bool init();
	virtual void initMoveUpdate();
	virtual void preset();
	virtual void presetBorder();
	virtual void addSpinerAngle();
	virtual void onEnter();
private:
	virtual void moveLeft();
	virtual void moveRight();
	virtual void moveUp();
	virtual void moveDown();
	virtual void move();
private:
	virtual void checkPushedMoveKey(float fDelay);
	virtual void checkSpeedDown(bool isPushed_Shift);
private:
	void(C_Character::*m_arMoveFunc[4])() {};
private:
	bool m_arKeyPushed[4]{};
	bool m_isPushedShift;
	bool m_isEnabled;
private:
	float m_arShiftPushed[2]{ 0.5f, 0.2f };
	float m_arSpeedMode[2]	{ 18.0f, 1.0f };
	float m_fMinBorderX;
	float m_fMinBorderY;
	float m_fMaxBorderX;
	float m_fMaxBorderY;
private:
	float m_fPlayerHP;
	float m_fSpeed;
private:
	Sprite* m_pSlowMode;
	Sprite* m_pSlowModeSpiner;
private:
	static C_Character* m_pInstance;
private:
	C_Character() {}
	virtual ~C_Character() {}
	C_Character(C_Character&) = delete; // 추후 바뀔 수도 있음.
	C_Character operator=(C_Character&) = delete; // 추후 바뀔 수도 있음.
};