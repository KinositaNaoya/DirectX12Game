

class FallLoad : public IBossGimmick
{
private:
    bool isReached;//下に到達したかどうか
    float mWaveTimer;
    int mSelected;//ボスの出現位置

public:
    bool initialize();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};