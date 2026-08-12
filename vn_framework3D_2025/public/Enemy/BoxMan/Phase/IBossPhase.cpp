#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

IBossPhase::IBossPhase()
{
	PhaseBGM = nullptr;
	pPhase_skyModel = nullptr;
	isGimmick = true;
	Randnum = 0;
	DoOnce = true;

	currentGimmick = nullptr;
}

IBossPhase::~IBossPhase()
{
}