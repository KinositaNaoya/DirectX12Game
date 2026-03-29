

class PhaseCange : public IBossGimmick
{
private:
    vnSound* SkyChangeSE;
    bool DoOnce;

public:
    bool initialize();

    PhaseCange();
    ~PhaseCange();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};