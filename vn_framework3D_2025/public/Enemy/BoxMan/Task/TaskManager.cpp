#include"TaskManager.h"


//=============タスクマネージャー関数=================

TaskManager::TaskManager()
{
    gimmickList[L"FallEWNS"]	= new FallEWNS();
	gimmickList[L"FallLOAD"]	= new FallLoad();
	gimmickList[L"FallSIDE"]	= new FallSide();
	gimmickList[L"FallCROSS"]	= new FallCross();
	gimmickList[L"FallRAND"]	= new FallRand();
	gimmickList[L"FallRING"]	= new FallRing();
	gimmickList[L"ChangePHASE"] = new PhaseCange();
	gimmickList[L"Standby"]     = new Standby();
}

TaskManager::~TaskManager()
{
    delete(gimmickList[L"FallEWNS"]);
    delete(gimmickList[L"FallLOAD"]);
    delete(gimmickList[L"FallSIDE"]);
    delete(gimmickList[L"FallCROSS"]);
    delete(gimmickList[L"FallRAND"]);
    delete(gimmickList[L"FallRING"]);
    delete(gimmickList[L"ChangePHASE"]);
    delete(gimmickList[L"Standby"]);
  
}
