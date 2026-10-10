#pragma once
#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"


class Standby : public IBossGimmick
{
private:
    vnSound* SkyChangeSE;
    float t = 0.0f;
	float y = 0.0f;

public:

    bool initialize();

    Standby();
    ~Standby();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};
