#pragma once

class Phase5 : public IBossPhase
{
public:
    Phase5();
    ~Phase5();
    void execute(BoxMan* boss, FloorCube* floor[]) override;
};
