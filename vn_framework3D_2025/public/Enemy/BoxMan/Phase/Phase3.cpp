#include "../../framework.h"
#include "../../framework/vn_environment.h"

Phase3::Phase3()
{
    pPhase_skyModel = new vnModel(L"data/model/", L"T_skycode.vnm");
    pPhase_skyModel->setLighting(false);
    pPhase_skyModel->setDoubleSided(true);
    pPhase_skyModel->setScale(0.002f, 0.002f, 0.002f);
    pPhase_skyModel->setPositionY(200.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase_skyModel);

    PhaseBGM = new vnSound(L"data/sound/BGM/Zero_Time_Outage.wav");
}

Phase3::~Phase3()
{
    vnMainFrame::getSceneInstance()->deleteObject(pPhase_skyModel);
    delete(PhaseBGM);
}

void Phase3::execute(BoxMan* boss, FloorCube* floor[])
{
    if (floor == nullptr || boss == nullptr)return;

    pPhase_skyModel->addRotationY(-0.0002f);
    if (!PhaseBGM->isPlaying()) {
        PhaseBGM->play(true);
    }

    if (isGimmick) {
        Randnum = rand() % 6;
        switch (Randnum)
        {
        case 0:
        case 3:
            currentGimmick = boss->GfallCROSS;
            break;
        case 1:
        case 4:
            currentGimmick = boss->GfallLOAD;
            break;
        case 2:
        case 5:
            currentGimmick = boss->GfallRAND;
            break;
        }
        
        currentGimmick->initialize();
        if (DoOnce) {

            currentGimmick = boss->GcangePHASE;
            DoOnce = false;
        }
        isGimmick = false;
    }

    

    isGimmick = currentGimmick->execute(boss, floor);
}