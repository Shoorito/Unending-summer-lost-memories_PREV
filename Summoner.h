#pragma once
#include "Enemy.h"

class C_Summoner : public C_Enemy
{
public:
	static C_Summoner* create();
public:
	virtual void startPattern();
private:
	virtual bool init()    override;
	virtual void preset()  override;
	virtual void onEnter() override;
	virtual void callPattern(float fDelay);
	virtual void endPattern();
private:
	C_Summoner() {}
	virtual ~C_Summoner() {}
	C_Summoner(C_Summoner&) = delete;
	C_Summoner operator=(C_Summoner&) = delete;
};