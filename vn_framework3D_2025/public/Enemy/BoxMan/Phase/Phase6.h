#pragma once

class Phase6 : public IBossPhase
{
public:
    Phase6();
    ~Phase6();
    void execute(BoxMan* boss, FloorCube* floor[]) override;
};
