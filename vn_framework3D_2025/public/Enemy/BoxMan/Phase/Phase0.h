#pragma once

class Phase0 : public IBossPhase
{
public:
    float t = 0.0f;

    Phase0();
    ~Phase0();
    void execute(BoxMan* boss, FloorCube* floor[]) override;
};
