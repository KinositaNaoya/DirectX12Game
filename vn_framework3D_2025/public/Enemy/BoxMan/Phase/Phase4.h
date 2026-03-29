#pragma once

class Phase4 : public IBossPhase
{
public:
    Phase4();
    ~Phase4();
    void execute(BoxMan* boss, FloorCube* floor[]) override;
};
