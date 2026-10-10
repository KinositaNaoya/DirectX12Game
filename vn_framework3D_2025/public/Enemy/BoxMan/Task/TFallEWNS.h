#pragma once

#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

class FallEWNS : public IBossGimmick
{
private:
    float mWaveTimer = 0.0f;
    int EWNS = 0;
    bool fallen[64] = { false };
    bool isReached;//‰º‚É“ž’B‚µ‚½‚©‚Ç‚¤‚©
    
public:
    bool initialize();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};