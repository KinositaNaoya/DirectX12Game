#include "../../framework.h"
#include "../../framework/vn_environment.h"

Phase4::Phase4()
{
    pPhase_skyModel = new vnModel(L"data/model/", L"T_skycode.vnm");
    pPhase_skyModel->setLighting(false);
    pPhase_skyModel->setDoubleSided(true);
    pPhase_skyModel->setScale(0.002f, 0.002f, 0.002f);
    pPhase_skyModel->setPositionY(200.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase_skyModel);

    PhaseBGM = new vnSound(L"data/sound/BGM/Cyber_Hounds.wav");
    isGimmick = true;
    Randnum = 0;
}

Phase4::~Phase4()
{
    vnMainFrame::getSceneInstance()->deleteObject(pPhase_skyModel);
    delete(PhaseBGM);
}

void Phase4::execute(BoxMan* boss, FloorCube* floor[])
{
    if (floor == nullptr)return;

    pPhase_skyModel->addRotationY(-0.0002f);
    if (!PhaseBGM->isPlaying()) {
        PhaseBGM->play(true);
    }

    if (isGimmick) {
        Randnum = rand() % 5;
        switch (Randnum)
        {
        case 0:
            currentGimmick = boss->GfallLOAD;
            break;
        case 1:
            currentGimmick = boss->GfallEWNS;
            break;
        case 2:
            currentGimmick = boss->GfallSIDE;
            break;
        case 3:
        case 5:
            currentGimmick = boss->GfallRAND;
            break;
        case 4:
            currentGimmick = boss->GfallCROSS;
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