

class FallRand : public IBossGimmick
{
private:
    int EWNS;           //東西南北の何処にボスがいくか
    float stayTimer;    //色が変わるまでの待ち時間
    int randFloor[32];  //ランダムなfloor保存用
    bool isFallRed;     //赤を落とすか落とさないか

public:
    bool initialize();
    bool execute(BoxMan* boss, FloorCube* floor[]) override;
};