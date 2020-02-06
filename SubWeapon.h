#pragma once
#include "cocos2d.h"

USING_NS_CC;

class C_SubWeapon
{
public:
	static C_SubWeapon* create();
public:
	virtual inline Layer* getLayer() { return m_pSubWeaponLayer; }
	virtual inline const float getPositionX() { return m_fPositionX; }
	virtual inline const float getPositionY() { return m_fPositionY; }
public:
	virtual inline void pause() { m_pSubWeapon->pause(); }
public:
	virtual inline void setTexture(const char* arFile) { m_pSubWeapon->setTexture(arFile); }
	virtual inline void setTexture(const std::string& strFile) { m_pSubWeapon->setTexture(strFile); }
	virtual void setTextureWithRect(const char* arFile, const Rect& recTarget);
	virtual void setTextureWithRect(const std::string& arFile, const Rect& recTarget);
public:
	virtual void setPosition(const float fXpos, const float fYpos);
	virtual void setPosition(const Vec2& vecPosition);
	virtual void addPosition(const float fXpos, const float fYpos);
	virtual void addPosition(const Vec2& vecPosition);
	virtual void addPositionX(const float fXpos);
	virtual void addPositionY(const float fYpos);
public:
	virtual void setEnabled(const bool isEnabled);
private:
	virtual void init();
	virtual void initAction();
	virtual void preset();
	virtual void presetByAction();
	virtual void startAction();
private:
	Sprite* m_pSubWeapon;
	Layer* m_pSubWeaponLayer;
private:
	float m_fPositionX;
	float m_fPositionY;
private:
	C_SubWeapon();
	virtual ~C_SubWeapon();
	C_SubWeapon(C_SubWeapon&) = delete;
	C_SubWeapon operator=(C_SubWeapon&) = delete;
};