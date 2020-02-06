#include "E_PatternsLib.h"
#include "EnemyManager.h"
#include "LinkedEnemy.h"
#include "Enemy.h"

C_E_PatternLib* C_E_PatternLib::m_pInstance = nullptr;

C_E_PatternLib* C_E_PatternLib::create()
{
	if (m_pInstance)
		return nullptr;

	m_pInstance = new(std::nothrow) C_E_PatternLib();

	m_pInstance->preset();

	return m_pInstance;
}

void C_E_PatternLib::preset()
{

}

void C_E_PatternLib::normalPattern01(const int nAnimType, C_Enemy * pTarget, bool isFilp)
{
	BezierTo*   pMoveAct(nullptr);
	CallFuncN*  pUnload(nullptr);
	Sequence*   pSequance(nullptr);

	m_bezierConfig.controlPoint_1.setPoint(m_arControlPoint01[isFilp].x, m_arControlPoint01[isFilp].y);
	m_bezierConfig.controlPoint_2.setPoint(m_arControlPoint02[isFilp].x, m_arControlPoint02[isFilp].y);
	m_bezierConfig.endPosition.setPoint(m_arControlEndPoint[isFilp].x, m_arControlEndPoint[isFilp].y);

	pMoveAct  = BezierTo::create(5.0f, m_bezierConfig);
	pUnload   = CallFuncN::create(CC_CALLBACK_1(C_E_PatternLib::unloadEnemy, this));
	pSequance = Sequence::create(pMoveAct, pUnload, nullptr);

	pTarget->setPatternType(0);
	pTarget->startAnimation(nAnimType);
	pTarget->runAction(pSequance);
}

void C_E_PatternLib::normalPattern02(const int nAnimType, C_Enemy * pTarget)
{
	MoveBy*		pMove01(nullptr);
	DelayTime*	pDelay(nullptr);
	CallFuncN*  pUnload(nullptr);
	MoveTo*		pMove02(nullptr);
	Sequence*   pSequance(nullptr);

	pMove01 = MoveBy::create(3.0f, Vec2(0.0f, -360.0f));
	pMove02 = MoveTo::create(3.0f, Vec2(pTarget->getPositionX(), -20.0f));
	pUnload = CallFuncN::create(CC_CALLBACK_1(C_E_PatternLib::unloadEnemy, this));
	pDelay  = DelayTime::create(1.0f);
	pSequance = Sequence::create(pMove01, pDelay, pMove02, pUnload, nullptr);

	pTarget->setPatternType(1);
	pTarget->startAnimation(nAnimType);
	pTarget->runAction(pSequance);
}

void C_E_PatternLib::normalPattern03(const int nAnimType, C_Enemy * pTarget)
{
	MoveTo*		pMove01(nullptr);
	MoveTo*		pMove02(nullptr);
	DelayTime*  pDelay(nullptr);
	CallFuncN*  pUnload(nullptr);
	Sequence*   pSeqAct(nullptr);

	pMove01 = MoveTo::create(0.5f, Vec2(pTarget->getPositionX(), 512.0f));
	pMove02 = MoveTo::create(3.5f, Vec2(pTarget->getPositionX(), -20.0f));
	pDelay  = DelayTime::create(2.0f);
	pUnload = CallFuncN::create(CC_CALLBACK_1(C_E_PatternLib::unloadEnemy, this));
	pSeqAct = Sequence::create(pMove01, pDelay, pMove02, pUnload, nullptr);

	pTarget->setPatternType(2);
	pTarget->startAnimation(nAnimType);
	pTarget->runAction(pSeqAct);
}

void C_E_PatternLib::middleBoss(const int nAnimType, C_Enemy * pTarget)
{
	MoveTo* pMove(nullptr);
	EaseOut* pMoveAct(nullptr);

	pMove = MoveTo::create(0.25f, Vec2(494.0f, 453.0f));
	pMoveAct = EaseOut::create(pMove, 2.0f);

	pTarget->setPatternType(3);
	pTarget->startAnimation(nAnimType);
	pTarget->runAction(pMoveAct);
}

void C_E_PatternLib::summoner(const int nAnimType, C_Enemy * pTarget)
{
	MoveTo*		pMove(nullptr);
	CallFuncN*	pFunc(nullptr);
	Sequence*	pSeq(nullptr);
	Spawn*		pSpawn(nullptr);
	Animation*	pAnim(nullptr);
	Animate*	pAnimate(nullptr);
	Repeat*		pRepeat(nullptr);

	pMove	 = MoveTo::create(2.0f, Vec2(pTarget->getPositionX(), -20.0f));
	pFunc	 = CallFuncN::create(CC_CALLBACK_1(C_E_PatternLib::unloadEnemy, this));
	pSeq	 = Sequence::create(pMove, pFunc, nullptr);
	pAnim	 = pTarget->getAnim(nAnimType);
	pAnimate = Animate::create(pAnim);
	pRepeat  = Repeat::create(pAnimate, -1);
	pSpawn	 = Spawn::create(pSeq, pRepeat, nullptr);

	pTarget->setAnimation(pAnim->clone(), nAnimType);
	pTarget->runAction(pSpawn);
}

void C_E_PatternLib::unloadEnemy(Ref * pTarget)
{
	C_Enemy* pEnemy(nullptr);

	pEnemy = (C_Enemy*)pTarget;

	pEnemy->setEnabled(false);

	C_EnemyManager::getInstance()->changeEnemy(pEnemy->getLinker(), E_USE_TYPE::E_NOT_USED);
}
