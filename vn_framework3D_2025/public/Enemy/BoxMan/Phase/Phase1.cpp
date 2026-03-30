#include "../../framework.h"
#include "../../framework/vn_environment.h"

Phase1::Phase1()
{
    pPhase_skyModel = new vnModel(L"data/model/", L"skydome.vnm");
    pPhase_skyModel->setLighting(false);
    pPhase_skyModel->setScale(5.0f, 5.0f, 5.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase_skyModel);

    PhaseBGM = new vnSound(L"data/sound/BGM/o‚Ì‰J‚ÆÉ‰Œ.wav");
}

Phase1::~Phase1()
{
    vnMainFrame::getSceneInstance()->deleteObject(pPhase_skyModel);
    delete(PhaseBGM);
}

void Phase1::execute(BoxMan* boss, FloorCube* floor[])
{
    if (floor == nullptr || boss == nullptr)return;

    if (!PhaseBGM->isPlaying()) {
        PhaseBGM->play(true);
    }

    if (isGimmick) {
        

        Randnum = rand() % 6;
        switch (Randnum)
        {
        case 0:case 3:
            currentGimmick = boss->GfallEWNS;
            break;
        default:
            currentGimmick = boss->GfallLOAD;
            break;
        }
        currentGimmick->initialize();
        isGimmick = false;
    }

    

    isGimmick = currentGimmick->execute(boss, floor);
}