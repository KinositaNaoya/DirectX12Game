#pragma once

#include "../../../framework.h"
#include "../../../framework/vn_environment.h"

#include"../EnemyBase.h"
#include"Phase/BossPhase.h"

class BossPhase;



//ボスクラス
class BoxMan : public EnemyBase
{
public:
	

	BoxMan(const WCHAR* folder, const WCHAR* file);
	~BoxMan();

	virtual void execute(FloorCube* floor[]);

	
	bool GameSet_exe();

	vnMotionData* motion_idle;
	vnMotionData* motion_dead;
	vnMotionData* motion_Move;
	vnMotionData* motion_Change;

	vnMotionData* motion_FallLoad;
	vnMotionData* motion_FallEWNS;
	vnMotionData* motion_FallGrand;
	vnMotionData* motion_FallRand_cast;
	vnMotionData* motion_FallRand;
	vnMotionData* motion_FallCross;

	vnEmitter* pMoveEffect;
	vnEmitter* pChargeEffect;
	ImpactActor* pMoveImpact;


	bool getIsDead();
	void setIsDead(bool b);

	BossPhase* PhaseManager;//フェーズ管理オブジェクト

private:

	bool isDead;

	XMVECTOR BossBackPos = XMVectorSet(0.0f,0.0f,0.0f,0.0f);


	
	
	
	bool DoOnce;//PhaseChange専用

};

