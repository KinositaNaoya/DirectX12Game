#pragma once
#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"


class GameEnd : public IBossGimmick
{
private:
    vnSound* GameEndSE;

public:

    bool initialize();

    GameEnd();
    ~GameEnd();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};

/*

Phase6をギミック化


    Phase0
//    pPhase_skyModel->addRotationY(0.0004f);
    スカイドーム回し忘れ
*/