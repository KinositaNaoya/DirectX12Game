#pragma once

#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

#include "GimmickBase.h"

#include"TCollapseRing.h"
#include"TFallCross.h"
#include"TFallEWNS.h"
#include"TFallLoad.h"
#include"TFallRand.h"
#include"TFallSide.h"
#include"TPhaseCange.h"
#include"TStandby.h"


//タスクマネージャークラス
class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	std::unordered_map<std::wstring, IBossGimmick*> gimmickList;

private:

};