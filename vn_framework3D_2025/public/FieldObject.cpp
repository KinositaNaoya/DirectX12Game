#include "../framework.h"
#include "../framework/vn_environment.h"

//コンストラクタ
FloorCube::FloorCube(const WCHAR* folder, const WCHAR* file) :vnModel(folder, file)
{
	mShakeTime = 0.0f;
	cube.setOwner(this);

	EmitterDesc.LifeMax = 30.0f;
	EmitterDesc.LifeMin = 30.0f;
	EmitterDesc.SizeMax = 0.5f;
	EmitterDesc.SizeMin = 0.1f;
	EmitterDesc.ColorMax = XMVectorSet(1.0f, 0.9f, 0.2f, 1.0f);
	EmitterDesc.ColorMin = XMVectorSet(1.0f, 0.5f, 0.0f, 0.8f);

	EmitterDesc.SpeedMin = XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f);
	EmitterDesc.SpeedMax = XMVectorSet(1.0f, 5.0f, 1.0f, 0.0f);

	pFloorEmitter = new vnEmitter(&EmitterDesc);
	pFloorEmitter->setEmit(false);

	vnMainFrame::getSceneInstance()->registerObject(pFloorEmitter);

}

FloorCube::~FloorCube()
{
	vnMainFrame::getSceneInstance()->deleteObject(pFloorEmitter);
}

void FloorCube::execute()
{
	pFloorEmitter->setDesc(&EmitterDesc);

	pFloorEmitter->setPosition(this->getPosition());
	pFloorEmitter->execute();
}

vnEmitter::stEmitterDesc* FloorCube::getEmitterDesc()
{
	return &EmitterDesc;
}

vnEmitter* FloorCube::getEmitter()
{
	return pFloorEmitter;
}

//取得
Collider::Cube* FloorCube::getCubeCollider()
{
	return &cube;
}

//初期地点に帰ってくる関数
void FloorCube::Init()
{
	this->setPosition(&this->initVec);
	this->setDiffuse(0.0f, 1.0f, 1.0f, 1.0f);
	this->setAmbient(0.0f, 0.0f, 0.0f, 1.0f);
	this->setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
	this->setRotation(0.0f, 0.0f, 0.0f);
	this->getEmitter()->setEmit(true);
}


//フロアを揺らす処理
void FloorCube::FloorShake(float Time){

	if (Time == NULL) {
		mShakeTime += 0.1f;
	}
	else
	{
		mShakeTime = Time;
	}
	

	float shake = sinf(mShakeTime * 1.0f) * 0.2f;
	this->setRotation(0.0f, shake, shake * 0.7);
}

//床用オブジェクトの生成(キューブ)
FloorCube* CreateFloorCube(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ)
{

	FloorCube* ret = new FloorCube(L"data/model/primitive/", L"cube.vnm");
	ret->setAmbient	(0.0f, 0.0f, 0.0f, 1.0f);	//default:(0.0f, 0.0f, 0.0f, 1.0f)
	ret->setDiffuse	(0.0f, 1.0f, 1.0f, 1.0f);	//default:(1.0f, 1.0f, 1.0f, 1.0f)
	ret->setSpecular(0.0f, 0.0f, 0.0f, 1.0f);	//default:(0.0f, 0.0f, 0.0f, 1.0f)
	ret->setTransparent(false);
	ret->setLighting(true);
	ret->setScale(sizeX * 0.5f, sizeY * 0.5f, sizeZ * 0.5f);
	ret->setPosition(posX, posY, posZ);
	ret->initVec = XMVectorSet(posX,posY,posZ,1.0f);
	ret->cube.setSize(sizeX, sizeY, sizeZ, 0.0f);
	return ret;
}

//スタンバイオブジェクト生成関数
AreaCube* CreateStandbyArea(float posX, float posY, float posZ, float sizeX, float sizeY, float sizeZ)
{
	AreaCube* ret = new AreaCube(L"data/model/primitive/", L"StandbyArea.vnm");
	ret->setTransparent(true);

	ret->setLighting(false);
	ret->getCubeCollider()->setOffset(0.0f,2.0f,0.0f);
	ret->setScale(sizeX, sizeY, sizeZ);
	ret->setPosition(posX, posY, posZ);
	ret->cube.setSize(sizeX, sizeY, sizeZ, 0.0f);
	return ret;
}

AreaCube::AreaCube(const WCHAR* folder, const WCHAR* file) :vnModel(folder, file)
{
	cube.setOwner(this);
}

Collider::Cube* AreaCube::getCubeCollider()
{
	return &cube;
}
