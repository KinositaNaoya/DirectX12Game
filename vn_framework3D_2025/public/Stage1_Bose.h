#pragma once




class BoseEnemy1 : public EnemyBase
{
public:
	

	BoseEnemy1(const WCHAR* folder, const WCHAR* file);
	~BoseEnemy1();

	virtual void execute();


	

	void Phase_1_exe(FloorCube* floor[]);
	void Phase_2_exe(FloorCube* floor[]);
	void Phase_3_exe(FloorCube* floor[]);
	void Phase_4_exe(FloorCube* floor[]);
	void FINAL_exe(FloorCube* floor[]);
	bool GameSet_exe();
	void PhaseChange_exe(FloorCube* floor[]);

	enum Gimmick
	{
		STATE_SELECT,
		STATE_MOVE,
		STATE_MOTION,
		STATE_EXE,
	};
	Gimmick mState;
	void setGimmickState(Gimmick state);

private:

	XMVECTOR EAST = XMVectorSet(23.0f, 0.0f, 0.0f, 0.0f);	//東
	XMVECTOR WEST = XMVectorSet(-23.0f, 0.0f, 0.0f, 0.0f);	//西
	XMVECTOR NORTH = XMVectorSet(0.0f, 0.0f, -23.0f, 0.0f);	//南
	XMVECTOR SOUTH = XMVectorSet(0.0f, 0.0f, 23.0f, 0.0f);	//北

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

	//ギミック制御用
	int Randnum = 0;
	bool isGimmick = true;

	// STATE_MOVE用変数
	XMVECTOR CurrentPos;
	XMVECTOR TargetPos;
	float moveT;//lerp用





	float t = 0.0f;

	vnSound* SkyChangeSE;
	bool skyChangeFlag;

	vnSound* Phase1_BGM;
	vnSound* Phase2_BGM;
	vnSound* Phase3_BGM;
	vnSound* Phase4_BGM;
	vnSound* Phase5_BGM;
	
	vnModel* pPhase1_sky;
	vnModel* pPhase2_sky;
	vnModel* pPhase3_sky;
	vnModel* pPhase4_sky;
	vnModel* pPhase5_sky;

	
	
	bool DoOnce;//PhaseChange専用
	

	//ギミックの処理関数
	bool FallLoad(FloorCube* floor[]);
	bool FallEWNS(FloorCube* floor[]);
	bool FallSide(FloorCube* floor[]);
	bool FallRand(FloorCube* floor[]);
	bool FallCross(FloorCube* floor[]);

};
