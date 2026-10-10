#pragma once
#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"


class FallRing : public IBossGimmick
{
private:
    bool isReached;//‰º‚É“ž’B‚µ‚½‚©‚Ç‚¤‚©
    int CurrentRing = 0;
	bool isFAST = true;

public:
    bool initialize();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};