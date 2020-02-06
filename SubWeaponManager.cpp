#include "SubWeaponManager.h"
#include "SubWeapon.h"
#include "Character.h"

C_Subweapon_Manager* C_Subweapon_Manager::m_pInstance = nullptr;

C_Subweapon_Manager * C_Subweapon_Manager::create()
{
	if (m_pInstance)
	{
		return nullptr;
	}

	m_pInstance = new(std::nothrow) C_Subweapon_Manager();

	m_pInstance->init();
	m_pInstance->preset();

	return m_pInstance;
}

void C_Subweapon_Manager::createLaunchersArray(const int nCount)
{
	if (!nCount)
		return;

	m_nMaxCount		 = nCount;
	m_arSubLaunchers = new(std::nothrow) C_SubWeapon*[m_nMaxCount]{};
	m_arEnabledList  = new(std::nothrow) bool[m_nMaxCount] {};

	for (int nLauncher(0); nLauncher < (int)m_nMaxCount; nLauncher++)
	{
		//m_arSubLaunchers[nLauncher] = C_SubWeapon::create();
		m_arEnabledList[nLauncher]  = false;
	}
}

void C_Subweapon_Manager::setData(C_SubWeapon * pData, const int nDatapos)
{
	if (nDatapos > (int)m_nMaxCount || nDatapos < 0)
	{
		CCLOG("Wrong Datapos Number.");
		return;
	}

	m_arSubLaunchers[nDatapos] = pData;

	m_pLaunchersLayer->addChild(pData->getLayer());
}

void C_Subweapon_Manager::setEnabled(const int nDatapos, const bool isEnabled)
{
	if (nDatapos > (int)m_nMaxCount || nDatapos < 0)
	{
		CCLOG("Wrong Datapos Number.");
		return;
	}

	if (m_arEnabledList[nDatapos] == isEnabled)
		return;

	if (isEnabled)
	{
		m_arEnabledList[nDatapos] = true;
		m_nLauncherCount++;
	}
	else
	{
		m_arEnabledList[nDatapos] = false;
		m_nLauncherCount--;
	}

	m_arSubLaunchers[nDatapos]->setEnabled(isEnabled);
}

void C_Subweapon_Manager::pauseWeapons()
{
	for (int nCount(0); nCount < m_nMaxCount; nCount++)
	{
		m_arSubLaunchers[nCount]->pause();
	}
}

void C_Subweapon_Manager::init()
{
	m_arSubLaunchers	= nullptr;
	m_pLaunchersLayer	= nullptr;
	m_pScheduler		= nullptr;
	m_nLauncherCount	= 0;
	m_nMaxCount			= 0;
}

void C_Subweapon_Manager::preset()
{
	m_pLaunchersLayer = Layer::create();

	m_pScheduler = m_pLaunchersLayer->getScheduler();
	
	createUpdateActions();
}

void C_Subweapon_Manager::createUpdateActions()
{
	m_funcUpdater = CC_CALLBACK_1(C_Subweapon_Manager::update, this);

	m_pScheduler->schedule(m_funcUpdater, m_pLaunchersLayer, 0.01f, true, "callSubweaponUpdate");
}

void C_Subweapon_Manager::update(float dt)
{
	if (m_nLauncherCount < 1)
	{
		return;
	}

	int nMax(0);

	m_isEvenCount = (m_nLauncherCount % 2 == 0);

	nMax = m_nLauncherCount - !m_isEvenCount;
	
	if (nMax > 1)
	{
		nMax /= 2;

		for (int nLeft(0); nLeft < nMax; nLeft++)
		{
			C_SubWeapon* pSub(nullptr);

			for (int nRight(0); nRight < 2; nRight++)
			{
				pSub = m_arSubLaunchers[nLeft * 2 + nRight];

				pSub->setPosition(C_Character::getInstance()->getPosition());
				pSub->addPositionX(-40.0f * (nLeft + 1));
				pSub->addPositionX((80.0f * nRight) * (nRight + nLeft));
				pSub->addPositionY(30.0f + (-30.0f * nLeft));
			}
		}
	}

	if(!m_isEvenCount)
	{
		m_arSubLaunchers[m_nLauncherCount - 1]->setPosition(C_Character::getInstance()->getPosition());
		m_arSubLaunchers[m_nLauncherCount - 1]->addPositionY(50.0f);
	}

	//CCLOG("CALL UPDATE FUNCTION");
}
