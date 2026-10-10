#pragma once
#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"
#include "GimmickBase.h"

class FallCross : public IBossGimmick
{
private:
    bool isReached;//‰º‚É“ž’B‚µ‚½‚©‚Ç‚¤‚©
	bool RandCloss;

public:
    bool initialize();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};