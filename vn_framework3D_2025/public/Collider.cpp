#include "../framework.h"
#include "../framework/vn_environment.h"

//設定
void Collider::Cube::setOwner(vnObject* owner)
{
	Owner = owner;
}

//取得
vnObject* Collider::Cube::getOwner()const
{
	return Owner;
}

//設定
void Collider::Cube::setOffset(const XMVECTOR& offset)
{
	Offset = offset;
}

void Collider::Cube::setOffset(float x, float y, float z)
{
	Offset = XMVectorSet(x, y, z, 0.0f);
}

//取得
XMVECTOR Collider::Cube::getOffset()
{
	return Offset;
}

//設定
void Collider::Cube::setSize(const XMVECTOR &size)
{
	Size = size;
}

void Collider::Cube::setSize(float x, float y, float z, float w)
{
	Size = XMVectorSet(x, y, z, w);
}

//取得
XMVECTOR Collider::Cube::getSize()
{
	return Size;
}

//重なった時重なった分だけ押し戻す関数(AABB)
Collider::eDirection Collider::PushBackOnOverlap(Cube* p1, Cube* p2)
{
	eDirection ret = eDirection::None;

	if (p1->getOwner() == NULL || p2->getOwner() == NULL)return ret;

	//二つのオブジェクトの間の距離
	XMVECTOR range = XMVectorAdd(p1->getSize() * 0.5f, p2->getSize() * 0.5f);
	float rangex = XMVectorGetX(range);//当たったかどうかの二間の距離X
	float rangey = XMVectorGetY(range);//当たったかどうかの二間の距離Y
	float rangez = XMVectorGetZ(range);//当たったかどうかの二間の距離Z

	//AABB中心のワールド座標の変数化
	XMVECTOR wPos1 = XMVectorAdd(*p1->getOwner()->getPosition(), p1->getOffset());
	XMVECTOR wPos2 = XMVectorAdd(*p2->getOwner()->getPosition(), p2->getOffset());

	//中心座標の差分(絶対値)
	//XMVECTOR dif = XMVectorSubtract(p1->getCenter(), p2->getCenter());
	XMVECTOR dif = XMVectorSubtract(wPos1, wPos2);
	dif = XMVectorAbs(dif);//この関数で絶対値化する

	float difx = XMVectorGetX(dif);//現在の場所基準の二間の距離X
	float dify = XMVectorGetY(dif);//現在の場所基準の二間の距離Y
	float difz = XMVectorGetZ(dif);//現在の場所基準の二間の距離Z

	//衝突判定
	if (difx < rangex && dify < rangey && difz < rangez)
	{
		//重なっている部分の長さ(最少の要素の方向に押し戻す)
		float sx = rangex - difx;
		float sy = rangey - dify;
		float sz = rangez - difz;
		if (sx < sy)
		{
			if (sx < sz)
			{	//xが最少
				if (XMVectorGetX(wPos1) < XMVectorGetX(wPos2))
				{
					p1->getOwner()->addPositionX(-sx);
					ret = eDirection::X_Neg;
				}
				else
				{
					p1->getOwner()->addPositionX(sx);
					ret = eDirection::X_Pos;
				}
			}
			else
			{	//zが最少
				if (XMVectorGetZ(wPos1) < XMVectorGetZ(wPos2))
				{
					p1->getOwner()->addPositionZ(-sz);
					ret = eDirection::Z_Neg;
				}
				else
				{
					p1->getOwner()->addPositionZ(sz);
					ret = eDirection::Z_Pos;
				}
			}
		}
		else
		{
			if (sy < sz)
			{	//yが最少
				if (XMVectorGetY(wPos1) < XMVectorGetY(wPos2))
				{
					p1->getOwner()->addPositionY(-sy);
					ret = eDirection::Y_Neg;
				}
				else
				{
					p1->getOwner()->addPositionY(sy);
					ret = eDirection::Y_Pos;
				}
			}
			else
			{	//zが最少
				if (XMVectorGetZ(wPos1) < XMVectorGetZ(wPos2))
				{
					p1->getOwner()->addPositionZ(-sz);
					ret = eDirection::Z_Neg;
				}
				else
				{
					p1->getOwner()->addPositionZ(sz);
					ret = eDirection::Z_Pos;
				}
			}
		}
	}

	return ret;
}

//重なってるかどうか
bool Collider::OverlapCube(Cube* p1, Cube* p2)
{
	if (p1->getOwner() == NULL || p2->getOwner() == NULL)return false;

	//二つのオブジェクトの間の距離
	XMVECTOR range = XMVectorAdd(p1->getSize() * 0.5f, p2->getSize() * 0.5f);
	float rangex = XMVectorGetX(range);//当たったかどうかの二間の距離X
	float rangey = XMVectorGetY(range);//当たったかどうかの二間の距離Y
	float rangez = XMVectorGetZ(range);//当たったかどうかの二間の距離Z

	//AABB中心のワールド座標の変数化
	XMVECTOR wPos1 = XMVectorAdd(*p1->getOwner()->getPosition(), p1->getOffset());
	XMVECTOR wPos2 = XMVectorAdd(*p2->getOwner()->getPosition(), p2->getOffset());

	//中心座標の差分(絶対値)
	XMVECTOR dif = XMVectorSubtract(wPos1, wPos2);
	dif = XMVectorAbs(dif);//この関数で絶対値化する

	float difx = XMVectorGetX(dif);//現在の場所基準の二間の距離X
	float dify = XMVectorGetY(dif);//現在の場所基準の二間の距離Y
	float difz = XMVectorGetZ(dif);//現在の場所基準の二間の距離Z

	if (difx < rangex && dify < rangey && difz < rangez) {
		return true;
	}
	return false;
}

//キューブの枠線を表示するデバッグ関数
void Collider::DrawAABB(const Collider::Cube* cube, DWORD color)
{
	if (cube == NULL)return;

	//変数化
	XMVECTOR wPos = XMVectorAdd(*cube->getOwner()->getPosition(), cube->Offset);

	XMVECTOR half = XMVectorScale(cube->Size, 0.5f);
	XMVECTOR min = XMVectorSubtract(wPos, half);
	XMVECTOR max = XMVectorAdd(wPos, half);

	XMFLOAT3 vmin, vmax;
	XMStoreFloat3(&vmin, min);
	XMStoreFloat3(&vmax, max);

	float x0 = vmin.x;
	float y0 = vmin.y;
	float z0 = vmin.z;

	float x1 = vmax.x;
	float y1 = vmax.y;
	float z1 = vmax.z;

	// 底面
	vnDebugDraw::Line(x0, y0, z0, x1, y0, z0, color);
	vnDebugDraw::Line(x1, y0, z0, x1, y0, z1, color);
	vnDebugDraw::Line(x1, y0, z1, x0, y0, z1, color);
	vnDebugDraw::Line(x0, y0, z1, x0, y0, z0, color);

	// 上面
	vnDebugDraw::Line(x0, y1, z0, x1, y1, z0, color);
	vnDebugDraw::Line(x1, y1, z0, x1, y1, z1, color);
	vnDebugDraw::Line(x1, y1, z1, x0, y1, z1, color);
	vnDebugDraw::Line(x0, y1, z1, x0, y1, z0, color);

	// 縦線
	vnDebugDraw::Line(x0, y0, z0, x0, y1, z0, color);
	vnDebugDraw::Line(x1, y0, z0, x1, y1, z0, color);
	vnDebugDraw::Line(x1, y0, z1, x1, y1, z1, color);
	vnDebugDraw::Line(x0, y0, z1, x0, y1, z1, color);
}
