#pragma once

class Phase2 : public IBossPhase
{
public:
    Phase2();
    ~Phase2();
    void execute(BoxMan* boss, FloorCube* floor[]) override;
};
