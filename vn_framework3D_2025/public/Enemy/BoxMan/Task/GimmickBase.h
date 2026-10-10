#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class BoxMan;
class FloorCube;

//ギミック用親クラス
class IBossGimmick
{
public:
	IBossGimmick();
	virtual ~IBossGimmick() = default;

	//東西南北の位置
	XMVECTOR EAST = XMVectorSet(23.0f, 0.0f, 0.0f, 0.0f);	//東
	XMVECTOR WEST = XMVectorSet(-23.0f, 0.0f, 0.0f, 0.0f);	//西
	XMVECTOR NORTH = XMVectorSet(0.0f, 0.0f, -23.0f, 0.0f);	//南
	XMVECTOR SOUTH = XMVectorSet(0.0f, 0.0f, 23.0f, 0.0f);	//北

	XMVECTOR CurrentPos;
	XMVECTOR TargetPos;
	float moveT;//lerp用
	bool DoOnce;//一度だけ実行する処理用

	enum Gimmick
	{
		STATE_SELECT,
		STATE_MOVE,
		STATE_MOTION,
		STATE_EXE,
	};
	Gimmick mState;

	virtual bool initialize() = 0;
	virtual bool execute(BoxMan* boss, FloorCube* floor[]) = 0;

};