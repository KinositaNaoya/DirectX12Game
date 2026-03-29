

class FallCross : public IBossGimmick
{
private:
    bool isReached;//‰º‚É“ž’B‚µ‚½‚©‚Ç‚¤‚©

public:
    bool initialize();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};