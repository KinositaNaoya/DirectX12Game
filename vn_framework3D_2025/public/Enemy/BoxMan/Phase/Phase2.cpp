#include "../../framework.h"
#include "../../framework/vn_environment.h"

Phase2::Phase2()
{
    pPhase_skyModel = new vnModel(L"data/model/", L"SkyStar.vnm");
    pPhase_skyModel->setLighting(false);
    pPhase_skyModel->setDoubleSided(true);
    pPhase_skyModel->setScale(0.002f, 0.002f, 0.002f);
    pPhase_skyModel->setPositionY(200.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase_skyModel);

    PhaseBGM = new vnSound(L"data/sound/BGM/Egoist_2.wav");
}

Phase2::~Phase2()
{
    vnMainFrame::getSceneInstance()->deleteObject(pPhase_skyModel);
    delete(PhaseBGM);
}

void Phase2::execute(BoxMan* boss, FloorCube* floor[])
{
    if (floor == nullptr || boss == nullptr)return;

    pPhase_skyModel->addRotationY(-0.002f);
    if (!PhaseBGM->isPlaying()) {
        PhaseBGM->play(true);
    }

    if (isGimmick) {
        Randnum = rand() % 7;
        switch (Randnum)
        {
        case 0:
        case 3:
        case 6:
            currentGimmick = boss->GfallEWNS;
            break;

        case 1:
        case 4:
        case 7:
            currentGimmick = boss->GfallSIDE;
            break;

        case 2:
        case 5:
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
