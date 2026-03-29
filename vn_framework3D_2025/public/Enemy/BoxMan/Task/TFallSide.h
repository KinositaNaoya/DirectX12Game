

class FallSide : public IBossGimmick
{
private:
    bool isReached;//下に到達したかどうか
    float mWaveTimer;
    int EWNS;//ボスの出現位置

public:
    bool initialize();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};