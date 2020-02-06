#include "Character.h"
#include "PlayScene.h"
#include "KeyEventManager.h"

C_Character* C_Character::m_pInstance = nullptr;

C_Character * C_Character::create()
{
	m_pInstance = new(std::nothrow) C_Character();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

bool C_Character::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	m_fSpeed		= 0.5f;   // 캐릭터가 한 번 갱신마다 움직이는 거리
	m_fMinBorderX	= 0.0f;
	m_fMinBorderY	= 0.0f;
	m_fMaxBorderX	= 0.0f;
	m_fMaxBorderY	= 0.0f;
	m_fPlayerHP		= 0.0f; // 플레이어 체력
	m_isEnabled		= false;
	m_isPushedShift = false;
	m_pSlowMode		= nullptr;
	m_pSlowModeSpiner = nullptr;

	for (int nInput(0); nInput < 4; nInput++)
	{
		m_arKeyPushed[nInput] = false;
	}

	return true;
}

void C_Character::initMoveUpdate()
{
	this->schedule(schedule_selector(C_Character::checkPushedMoveKey), g_fCharacterUpdateDelay);
}

void C_Character::preset()
{
	m_arMoveFunc[0]	= &C_Character::moveLeft;
	m_arMoveFunc[1]	= &C_Character::moveRight;
	m_arMoveFunc[2]	= &C_Character::moveUp;
	m_arMoveFunc[3]	= &C_Character::moveDown;

	m_pSlowMode = Sprite::create("SlowMode_Colision.png");
	m_pSlowModeSpiner = Sprite::create("slowMode.png");

	m_pSlowMode->setPosition(getPosition());
	m_pSlowMode->setVisible(false);

	m_pSlowModeSpiner->setPosition(getPosition());
	m_pSlowModeSpiner->setVisible(false);
	m_pSlowModeSpiner->setScale(0.5f);

	presetBorder();
	initMoveUpdate();

	C_PlayScene::getInstance()->getMainScene()->addChild(m_pSlowMode, 2);
	C_PlayScene::getInstance()->getMainScene()->addChild(m_pSlowModeSpiner, 2);
}

void C_Character::presetBorder()
{
	m_fMinBorderX = 60.0f;
	m_fMinBorderY = 27.0f;
	m_fMaxBorderX = 927.0f;
	m_fMaxBorderY = 693.0f;
}

void C_Character::addSpinerAngle()
{
	if(m_pSlowModeSpiner->getRotation() > 360.0f)
		m_pSlowModeSpiner->setRotation(0.0f);

	m_pSlowModeSpiner->setRotation(m_pSlowModeSpiner->getRotation() + 0.2f);
}

void C_Character::onEnter()
{
	Node::onEnter();

	if (!m_isEnabled)
		pause();
}

void C_Character::checkSpeedDown(bool isPushed_Shift)
{
	m_isPushedShift = isPushed_Shift;

	m_pSlowMode->setVisible(m_isPushedShift);
	m_pSlowMode->setPosition(getPosition());
	m_pSlowModeSpiner->setVisible(m_isPushedShift);
	m_pSlowModeSpiner->setOpacity(200.0f);
	m_pSlowModeSpiner->setPosition(getPosition());
	addSpinerAngle();

	m_fSpeed = m_arShiftPushed[m_isPushedShift];
}

void C_Character::moveLeft()
{
	if (_position.x <= m_fMinBorderX)
	{
		return;
	}

	_position.x -= m_fSpeed;

	move();
}

void C_Character::moveRight()
{
	if (_position.x >= m_fMaxBorderX)
	{
		return;
	}

	_position.x += m_fSpeed;

	move();
}

void C_Character::moveUp()
{
	if (_position.y >= m_fMaxBorderY)
	{
		return;
	}

	_position.y	+= m_fSpeed;
	
	move();
}

void C_Character::moveDown()
{
	if (_position.y <= m_fMinBorderY)
	{
		return;
	}

	_position.y -= m_fSpeed;

	move();
}

void C_Character::move()
{
	_transformUpdated			= true;
	_transformDirty				= true;
	_inverseDirty				= true;
	_usingNormalizedPosition	= false;
}

void C_Character::checkPushedMoveKey(float fDelay)
{
	C_KeyEvent_Manager* pManager(nullptr);

	pManager = C_KeyEvent_Manager::getInstance();

	for (int nKey(0); nKey < 4; nKey++)
	{
		if (pManager->getPushedType(nKey + 26) == E_KEY_STATE::E_PUSH)
		{
			(this->*m_arMoveFunc[nKey])();
		}
	}

	checkSpeedDown(pManager->getPushedType(12) == E_KEY_STATE::E_PUSH);
}

void C_Character::setEnabled(const bool isSwitch)
{
	m_isEnabled = isSwitch;

	setVisible(m_isEnabled);

	if (m_isEnabled)
		resume();
	else
		pause();
}

void C_Character::setTextureWithRect(const char * arFile, const Rect& recSprite)
{
	setTexture(arFile);
	setTextureRect(recSprite);
}

void C_Character::setMoveBorder(const float fMinX, const float fMinY, const float fMaxX, const float fMaxY)
{
	m_fMinBorderX = fMinX;
	m_fMinBorderY = fMinY;
	m_fMaxBorderX = fMaxX;
	m_fMaxBorderY = fMaxY;
}

void C_Character::setScale(const float fScale)
{
	Size szCharacter(getContentSize());
	float fWidth(0.0f);
	float fHeight(0.0f);

	fWidth	= szCharacter.width / 2.0f * fScale;
	fHeight = szCharacter.height / 2.0f * fScale;
	
	setMoveBorder
	(
		m_fMinBorderX + fWidth,
		m_fMinBorderY + fHeight,
		m_fMaxBorderX - fWidth,
		m_fMaxBorderY - fHeight
	);
	
	Sprite::setScale(fScale);
}

void C_Character::setSpeed(const float fSpeed)
{
	m_fSpeed = fSpeed;
}

void C_Character::setHP(const float fHP)
{
	m_fPlayerHP = fHP;
}
