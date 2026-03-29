#include "../../framework.h"
#include "../../framework/vn_environment.h"

bool FallSide::initialize()
{
    mState = STATE_SELECT;
    mWaveTimer = 0.0f;
    EWNS = 0;
    return false;
}
//サイドを落とす攻撃
bool FallSide::execute(BoxMan* boss, FloorCube* floor[])
{
    switch (mState){
        // ランダム抽選
    case STATE_SELECT:
        for (int i = 0; i < 64; ++i) {
            floor[i]->Init();
        }

        EWNS = rand() % 4;

        CurrentPos = *boss->getPosition();


        if (EWNS == 0)TargetPos = EAST;
        if (EWNS == 1)TargetPos = WEST;
        if (EWNS == 2)TargetPos = SOUTH;
        if (EWNS == 3)TargetPos = NORTH;
        moveT = 0.0f;

        boss->setMotion(boss->motion_Move);


        mState = STATE_MOVE;
        break;

    case STATE_MOVE:

        moveT += 0.03f;
        if (moveT > 1.0f) moveT = 1.0f;

        XMVECTOR pos = XMVectorLerp(CurrentPos, TargetPos, moveT);
        boss->setPosition(&pos);

        if (moveT >= 1.0f)
        {
            boss->setMotion(boss->motion_FallGrand);
            boss->setMotionLoop(false);
            mState = STATE_MOTION;
        }

        break;

        // モーション待ち
    case STATE_MOTION:


        for (int i = 0; i < 64; i++)
        {
            int row = i / 8;
            int col = i % 8;

            // 下3段 or 上3段23
            if (EWNS == 1 || EWNS == 0)
            {
                if (row <= 2 || row >= 5) {
                    floor[i]->FloorShake();
                }
            }
            else
            {
                if (col <= 2 || col >= 5) {
                    floor[i]->FloorShake();
                }
            }
        }

        if (!boss->getMotionEnd())return false;

        mState = STATE_EXE;
        break;


        // 床落下処理
    case STATE_EXE:

        isReached = true;

        for (int i = 0; i < 64; i++)
        {
            int row = i / 8;
            int col = i % 8;


            if (EWNS == 0 || EWNS == 1)
            {
                if (row <= 2 || row >= 5) {
                    floor[i]->addPositionY(-0.3f);
                    floor[i]->setRotation(0.0f, 0.0f, 0.0f);
                    if (floor[i]->getPositionY() > -30.0f)isReached = false;
                }
            }
            else
            {
                if (col <= 2 || col >= 5) {
                    floor[i]->addPositionY(-0.3f);
                    floor[i]->setRotation(0.0f, 0.0f, 0.0f);
                    if (floor[i]->getPositionY() > -30.0f)isReached = false;
                }
            }

        }
        if (isReached)
        {
            mState = STATE_SELECT;
            return true;
        }
        break;
    }
    return false;
}
