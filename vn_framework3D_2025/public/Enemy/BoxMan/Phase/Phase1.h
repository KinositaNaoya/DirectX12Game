#pragma once

class Phase1 : public IBossPhase
{
private:

public:
    Phase1();
    ~Phase1();
    void execute(BoxMan* boss, FloorCube* floor[]) override;
};
