#pragma once
#include "cocos2d.h"
#include "EnumList.h"

USING_NS_CC;

class C_Danmaku : public Sprite
{
public:
	static C_Danmaku* create();
public:
	virtual inline Sprite* getMyPointer() { return this; }
public:
	virtual inline C_Danmaku* getPrev() { return m_pPrev; }
	virtual inline C_Danmaku* getNext() { return m_pNext; }
	virtual inline const int getId()	{ return m_nId; }
	virtual inline const float getAngle() { return m_fAngle; }
public:
	virtual inline void setPrev(C_Danmaku* pDanmaku) { m_pPrev = pDanmaku; }
	virtual inline void setNext(C_Danmaku* pDanmaku) { m_pNext = pDanmaku; }
public:
	virtual inline void setSpeed(const float fSpeed) { m_fSpeed = fSpeed; }
	virtual inline void setAngle(const float fAngle) { m_fAngle = fAngle; }
	virtual inline void setId(const int nId)		 { m_nId = nId; }
public:
	virtual inline void setDamage(const float fDamage) { m_fDamage = fDamage; }
	virtual void setEnabled(const bool isEnabled);
	virtual void setTextureWithRect(const char* arFile, const Rect& recTarget);
	virtual void setTextureWithRect(const std::string& strFile, const Rect& recTarget);
public:
	virtual void addPosition(const Vec2& vecPosition);
	virtual void addPosition(const float fPositionX, const float fPositionY);
	virtual void addPositionX(const float fPositionX);
	virtual void addPositionY(const float fPositionY);
public:
	virtual void release();
private:
	virtual bool init() override;
	virtual void preset();
	virtual void onEnter() override;
	virtual void updateColision();
	virtual void update(float fDelayTime) override;
	virtual void updateWithPattern(float fDelay);
private:
	Rect m_recColision;
private:
	float m_fDamage;
	float m_fAngle;
	float m_fSpeed;
private:
	unsigned int m_nId;
private:
	bool m_isUsingUpdate;
private:
	C_Danmaku* m_pPrev;
	C_Danmaku* m_pNext;
private:
	C_Danmaku() {}
	virtual ~C_Danmaku() {}
	C_Danmaku(C_Danmaku&) = delete;
	C_Danmaku operator=(C_Danmaku&) = delete;
};