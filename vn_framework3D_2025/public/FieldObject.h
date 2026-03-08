#pragma once

#define Collid_Max_NUM (100)

//床用object
class FloorCube : public vnModel
{	
private:
	float mShakeTime = 0.0f;//シェイク用

public:
	XMVECTOR initVec;
	Collider::Cube cube;

	FloorCube(const WCHAR* folder, const WCHAR* file);//コンスト
	Collider::Cube* getCubeCollider();//渡す用

	void Init();
	void FloorShake(float speed = NULL);

};
//生成用関数
FloorCube* CreateFloorCube(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ);


//スタンバイエリアobject
class AreaCube : public vnModel
{
public:
	Collider::Cube cube;

	AreaCube(const WCHAR* folder, const WCHAR* file);
	Collider::Cube* getCubeCollider();//渡す用
};
//生成用関数
AreaCube* CreateStandbyArea(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ);