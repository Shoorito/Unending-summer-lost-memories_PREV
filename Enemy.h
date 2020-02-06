#pragma once
#include "cocos2d.h"
#include "EnumList.h"

USING_NS_CC;

class C_Linked_Enemy;

class C_Enemy : public Sprite
{
public:
	static C_Enemy* create();
	static C_Enemy* createWithAnimation(std::string* arFiles, const int nCount);
	// 애니메이션을 가진 적을 만들 때 사용하는 유형입니다.
	// @param "arFiles" 애니메이션 파일들의 정보를 가져옵니다.
	// @param "nCount" 파일들의 개수를 가져옵니다.(반드시 지정해 주어야 합니다.)
	static C_Enemy* createWithAnimationAtlas
	(
		const std::string& strFile, const Rect& recSize, const int& nCount
	);
	// 애니메이션 형태의 적을 "Atlas"스타일로 만들 때 사용하는 유형입니다.
	// @param "strFile" 사용 파일 명(.png 파일만 지원)
	// @param "recSize" 애니메이션 스프라이트 파츠의 크기
	// @param "nCount"  파츠의 개수
public:
	virtual inline const bool&  isWorked()		 { return m_isWorking; }
	virtual inline const Rect&	getEnemyRect()	 { return getBoundingBox(); }
	virtual inline const float& getHP()			 { return m_fHP; }
	virtual inline const int&	getId()			 { return m_nId; }
	virtual inline const int& getPatternType()	 { return m_nEnemyType; }
	virtual inline const E_ENEMY_TYPE getType()  { return m_eType; }
	virtual inline C_Linked_Enemy* getLinker()	 { return m_pMyLinker; }
	virtual inline Animation* getAnim(const int nType) { return m_arAnim[nType]; }
public:
	virtual inline void setId(const int nId)				{ m_nId = nId; }
	virtual inline void setType(E_ENEMY_TYPE eType)   { m_eType = eType; }
	virtual inline void setLinker(C_Linked_Enemy* pLinker)	{ m_pMyLinker = pLinker; }
	virtual inline void setPatternType(const int nType) { m_nEnemyType = nType; }
	virtual inline void setMiddleBoss(const bool isBoss) { m_isMiddleBoss = isBoss; }
	virtual inline void setAnimation(Animation* pAnim, const int nType)
	{
		pAnim->retain();
		m_arAnim[nType] = pAnim; 
	}
public:
	virtual void setHP(const float fHP);
	// 입력된 값을 적의 HP로 설정해 줍니다.
	// @param "fHP" 적용되는 체력 값.
	virtual void setEnabled(const bool isEnabled);
	virtual void setAnimationFile(const std::string& strFileName, const int nType = 0);
	virtual void setAnimationWithRect
	(
		const std::string& strFile, const Rect& recSize, const int& nCount, const int& nType
	);
	// "setAnimationFile"함수는 애니메이션 사용 파일에 대한 정보를 알기 쉽도록 만들어진 함수입니다.
	// @param "strFileName"은 파일의 이름을 넣으면 됩니다.
	// @param "nType"은 애니메이션 타입으로 다른 애니메이션을 추가 할 때 사용합니다.
	virtual void setTextureWithRect(const char* arFile, const Rect& recSprite);
	virtual void setTextureWithRect(const std::string& strFile, const Rect& recSprite);
public:
	virtual void startAnimation(const int nType);
protected:
	virtual bool init() override;
	virtual void preset();
	virtual void onEnter() override;
private:
	virtual void patternByNormal(float fDelay);
private:
	virtual void pattern01(const int);
	virtual void pattern02(const int);
	virtual void pattern03(const int);
	virtual void pattern04(const int);
	virtual void pattern05(const int);
	virtual void patternSummoner(const int nCount);
private:
	void(C_Enemy::*m_arPatternList[6])(const int);
protected:
	C_Linked_Enemy* m_pMyLinker;
private:
	std::vector<std::vector<std::string>*> m_vecAnimFiles{};
private:
	std::vector<Animation*> m_arAnim{};
private:
	E_ENEMY_TYPE m_eType;
private:
	Node* m_pActNode;
protected:
	float m_fHP;
	int	  m_nId;
	int	  m_nUpdateCount;
	int   m_nEnemyType;
	bool  m_isWorking;
	bool  m_isNowUsedUpdate;
	bool  m_isMiddleBoss;
protected:
	C_Enemy() {}
	virtual ~C_Enemy() {}
private:
	C_Enemy(C_Enemy&) = delete;
	C_Enemy operator=(C_Enemy&) = delete;
};
