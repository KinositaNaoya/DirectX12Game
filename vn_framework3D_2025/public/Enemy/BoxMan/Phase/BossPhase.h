#pragma once

#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"
#include <string>

class BossPhase
{
public:
	BossPhase();
	~BossPhase();

	void ChangePhase(EnemyBase::PHASETABLE CurrentPhase);

	void execute(class BoxMan* boss, FloorCube* floor[]);

private:
	IBossGimmick* currentGimmick;
	vnSound* PhaseBGM;
	vnModel* pPhase_skyModel;

	std::wstring skyModel;
	std::wstring bgm;

	//BossÇ…Ç†ÇÈÇÃÇ≈å„Ç≈è¡ÇªÇ§ÇÀÅB
	enum class GimickID
	{
		FallEWNS,
		FallLOAD,
		FallCROSS,
		FallRAND,
		FallSIDE,
		FallRING,
		ChangePHASE,
	};

	void LoadPhaseData(const wchar_t* filePath);

};