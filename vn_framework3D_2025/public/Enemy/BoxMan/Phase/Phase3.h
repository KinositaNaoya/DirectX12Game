#pragma once

class Phase3 : public IBossPhase
{
public:
    Phase3();
    ~Phase3();
    void execute(BoxMan* boss, FloorCube* floor[]) override;
};
