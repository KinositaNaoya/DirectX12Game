#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

bool PhaseCange::initialize()
{
    return false;
}

PhaseCange::PhaseCange()
{
    SkyChangeSE = new vnSound(L"data/sound/SE/グリッチ音02.wav");
    DoOnce = true;
}

PhaseCange::~PhaseCange()
{
    delete(SkyChangeSE);
}

bool PhaseCange::execute(BoxMan* boss, FloorCube* floor[])
{

    switch (mState)
    {
        //初期化
    case STATE_SELECT:
        for (int i = 0; i < 64; i++) {
            floor[i]->Init();
        }
        boss->TimeReset();
        boss->setMotionRate(0.5f);
        boss->setMotionLoop(false);
        boss->setMotion(boss->motion_Change);
        boss->setPositionX(0.0f);
        boss->setPositionZ(0.0f);
        SkyChangeSE->play(true);

        boss->currentPhase->pPhase_skyModel->setPositionY(0.0f);
        boss->pMoveEffect->setEmit(true);

        mState = STATE_MOVE;
        break;

        //音やみ待ち
    case STATE_MOVE:

        if (SkyChangeSE->isPlaying())return false;
        mState = STATE_MOTION;
        break;

        //スルー
    case STATE_MOTION:
        
        boss->pMoveEffect->setEmit(false);
        mState = STATE_EXE;
        break;

        //拡大待ち
    case STATE_EXE:

        if (boss->currentPhase->pPhase_skyModel->getScaleX() < 2.5f) {
            boss->currentPhase->pPhase_skyModel->addScale(0.01f, 0.01f, 0.01f);
        }
        else {
            boss->setMotionRate();
            boss->setMotionLoop(true);
            mState = STATE_SELECT;
            return true;
        }
        break;
    }
    return false;

}
