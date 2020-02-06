#include "Danmaku.h"
#include "Character.h"
#include "PatternsLib.h"
#include "DanmakuManager.h"
#include "PlayScene.h"

C_Danmaku * C_Danmaku::create()
{
	C_Danmaku* pDanmaku(nullptr);

	pDanmaku = new(std::nothrow) C_Danmaku();

	pDanmaku->init();
	pDanmaku->preset();

	return pDanmaku;
}

void C_Danmaku::release()
{
	Sprite::release();
}

bool C_Danmaku::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	m_fDamage		= 0.0f;
	m_fAngle		= 0.0f;
	m_fSpeed		= 0.0f;
	m_nId			= 0;
	m_recColision	= Rect::ZERO;
	m_isUsingUpdate = false;
	m_pPrev			= nullptr;
	m_pNext			= nullptr;

	return true;
}

void C_Danmaku::preset()
{
	this->schedule(schedule_selector(C_Danmaku::update), 0.01f);
	this->schedule(schedule_selector(C_Danmaku::updateWithPattern), 0.01f);
}

void C_Danmaku::onEnter()
{
	bool isRunning(false);

	isRunning = this->isRunning();

	Node::onEnter();

	if (!isRunning)
		this->pause();
}

void C_Danmaku::updateColision()
{
	m_recColision.setRect
	(
		_position.x - _contentSize.width * _scaleX / 2.0f,
		_position.y - _contentSize.height * _scaleY / 2.0f,
		_contentSize.width * _scaleX,
		_contentSize.height * _scaleY
	);
}

void C_Danmaku::update(float fDelayTime)
{
	C_Character* pCharacter(nullptr);
	
	pCharacter = C_Character::getInstance();

	if (m_isUsingUpdate)
	{
		m_isUsingUpdate = false;

		updateColision();

		if (m_recColision.intersectsCircle(pCharacter->getPosition(), pCharacter->getMode()))
		{
			setEnabled(false);
			pCharacter->setHP(pCharacter->getHP() - m_fDamage);
			C_Danmaku_Manager::getInstance()->changeDanmaku(this, (E_USE_TYPE)false);

			return;
		}
		else if (!(C_PlayScene::getInstance()->getWindoowRect().intersectsRect(m_recColision)))
		{
			setEnabled(false);
			C_Danmaku_Manager::getInstance()->changeDanmaku(this, (E_USE_TYPE)false);

			return;
		}

		m_isUsingUpdate = true;
	}
}

void C_Danmaku::updateWithPattern(float fDelay)
{
	C_Pattern_Library::getInstance()->angleShot(m_fAngle, m_fSpeed, this);
}

void C_Danmaku::setEnabled(const bool isEnabled)
{
	Sprite::setVisible(isEnabled);

	stopAllActions();

	m_isUsingUpdate = isEnabled;

	if (isEnabled)
		resume();
	else
		pause();
}

void C_Danmaku::setTextureWithRect(const char * arFile, const Rect & recTarget)
{
	setTexture(arFile);
	setTextureRect(recTarget);
}

void C_Danmaku::setTextureWithRect(const std::string & strFile, const Rect & recTarget)
{
	setTexture(strFile);
	setTextureRect(recTarget);
}

void C_Danmaku::addPosition(const Vec2 & vecPosition)
{
	_position += vecPosition;

	setPosition(_position);
}

void C_Danmaku::addPosition(const float fPositionX, const float fPositionY)
{
	_position.x += fPositionX;
	_position.y += fPositionY;

	setPosition(_position);
}

void C_Danmaku::addPositionX(const float fPositionX)
{
	this->setPositionX(this->getPositionX() + fPositionX);
}

void C_Danmaku::addPositionY(const float fPositionY)
{
	this->setPositionY(this->getPositionY() + fPositionY);
}
