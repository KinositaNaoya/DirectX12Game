#pragma once

namespace Collider
{
	//衝突方向
	enum eDirection
	{
		None,
		X_Pos,
		X_Neg,
		Y_Pos,
		Y_Neg,
		Z_Pos,
		Z_Neg,
	};

	//キューブ
	struct Cube
	{
		XMVECTOR Offset = { 0.0f,0.0f,0.0f,0.0f };
		XMVECTOR Size = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		vnObject* Owner;

		//オーナー
		void setOwner(vnObject* owner);
		vnObject* getOwner()const;

		//オフセット
		void setOffset(const XMVECTOR& offset);
		void setOffset(float x, float y, float z);
		XMVECTOR getOffset();

		//サイズ
		void setSize(const XMVECTOR &size);
		void setSize(float x, float y, float z, float w);
		XMVECTOR getSize();
	};

	//重なった時重なった分だけ押し戻す関数
	eDirection PushBackOnOverlap(Cube* p1, Cube* p2);

	//重なってるかどうかの判定処理
	bool OverlapCube(Cube* p1, Cube* p2);

	//キューブの枠線を表示するデバッグ関数
	void DrawAABB(const Collider::Cube* cube, DWORD color = 0xffffffff);

}