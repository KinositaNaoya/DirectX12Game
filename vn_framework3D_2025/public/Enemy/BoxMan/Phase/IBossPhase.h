#pragma once

#include "../Task/TPhaseCange.h"
#include "../Task/TFallLoad.h"
#include "../Task/TFallCross.h"
#include "../Task/TFallEWNS.h"
#include "../Task/TFallRand.h"
#include "../Task/TFallSide.h"


//フェーズ用インターフェース
class IBossPhase
{
public:
	bool isGimmick;//ギミック完了か否か
	int Randnum;	//ランダム用
	IBossGimmick* currentGimmick;
	vnSound* PhaseBGM;
	vnModel* pPhase_skyModel;
	bool DoOnce;
	

	IBossPhase();
	virtual ~IBossPhase();
	virtual void execute(class BoxMan* boss, FloorCube* floor[]) = 0;
};