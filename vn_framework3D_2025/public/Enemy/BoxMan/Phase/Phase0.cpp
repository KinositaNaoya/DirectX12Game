#include "../../framework.h"
#include "../../framework/vn_environment.h"

Phase0::Phase0()
{
    pPhase_skyModel = new vnModel(L"data/model/", L"skydome.vnm");
    pPhase_skyModel->setLighting(false);
    pPhase_skyModel->setScale(5.0f, 5.0f, 5.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase_skyModel);
}

Phase0::~Phase0()
{
    vnMainFrame::getSceneInstance()->deleteObject(pPhase_skyModel);
}

void Phase0::execute(BoxMan* boss, FloorCube* floor[])
{
    if (floor == nullptr || boss == nullptr)return;

    //モーション的なの
    t += 0.05f;  // スピード調整
    float y = sinf(t) * 0.1f; // 振れ幅2.0f
    boss->setPositionY(y - 1.5f);
    boss->setMotion(boss->motion_idle);
    pPhase_skyModel->addRotationY(0.0004f);
}