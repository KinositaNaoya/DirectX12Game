#pragma once

#include "../Task/TaskManager.h"

class BoxMan;
class FloorCube;
class TaskManager;


class BossPhase
{
public:
	BossPhase();
	~BossPhase();

	void ChangePhase(class BoxMan* Boss ,EnemyBase::PHASETABLE ReservePhase);

	void execute(class BoxMan* boss, FloorCube* floor[]);

	vnModel* pPhase_skyModel = nullptr;
	vnSound* PhaseBGM = nullptr;
	TaskManager* pTaskManager = nullptr;//taskマネージャー

private:

	
	

	std::wstring skyModel;	//スカイモデルの名前
	std::wstring bgm;		//BGMの名前

	std::vector<std::wstring> phaseGimmicks;//現在フェーズのギミックの名前を格納する配列(テキストから読み込む)

	std::wstring CurrentgimmickName;	//現在ギミックの名前


	void LoadPhaseData(const wchar_t* filePath);
	bool isGimmick;	//ギミック完了か否か
	bool DoOnce;	//フェーズチェンジ用

};