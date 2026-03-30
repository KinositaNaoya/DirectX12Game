#pragma once

class IBossPhase;

//ギミック用インターフェース
class IBossGimmick
{
public:
	IBossGimmick();

	XMVECTOR EAST = XMVectorSet(23.0f, 0.0f, 0.0f, 0.0f);	//東
	XMVECTOR WEST = XMVectorSet(-23.0f, 0.0f, 0.0f, 0.0f);	//西
	XMVECTOR NORTH = XMVectorSet(0.0f, 0.0f, -23.0f, 0.0f);	//南
	XMVECTOR SOUTH = XMVectorSet(0.0f, 0.0f, 23.0f, 0.0f);	//北

	XMVECTOR CurrentPos;
	XMVECTOR TargetPos;
	float moveT;//lerp用

	enum Gimmick
	{
		STATE_SELECT,
		STATE_MOVE,
		STATE_MOTION,
		STATE_EXE,
	};
	Gimmick mState;

	virtual bool initialize() = 0;
	virtual bool execute(class BoxMan* boss, FloorCube* floor[]) = 0;
};



//ボスクラス
class BoxMan : public EnemyBase
{
public:
	

	BoxMan(const WCHAR* folder, const WCHAR* file);
	~BoxMan();

	virtual void execute(FloorCube* floor[]);

	
	bool GameSet_exe();


	enum Gimmick
	{
		STATE_SELECT,
		STATE_MOVE,
		STATE_MOTION,
		STATE_EXE,
	};
	Gimmick mState;

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


	//ギミック
	IBossGimmick* GfallEWNS;
	IBossGimmick* GfallLOAD;
	IBossGimmick* GfallCROSS;
	IBossGimmick* GfallRAND;
	IBossGimmick* GfallSIDE;
	IBossGimmick* GcangePHASE;

	IBossGimmick* currentGimmick;

	IBossPhase* currentPhase;

	bool getIsDead();
	void setIsDead(bool b);

private:

	bool isDead;

	




	//float t = 0.0f;

	
	//bool skyChangeFlag;

	
	
	bool DoOnce;//PhaseChange専用

};

