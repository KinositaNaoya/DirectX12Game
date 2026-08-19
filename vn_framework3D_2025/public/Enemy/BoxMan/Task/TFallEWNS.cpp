#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

bool FallEWNS::initialize()
{
    mState = STATE_SELECT;
    mWaveTimer = 0.0f;
    EWNS = 0;
    return false;
}

//東西南北のどれか半分を落とす攻撃
bool FallEWNS::execute(BoxMan* boss, FloorCube* floor[])
{
    switch (mState)
    {
        // ランダム抽選
    case STATE_SELECT:
        for (int i = 0; i < 64; ++i) {
        floor[i]->Init();
        fallen[i] = false;
        }

        mWaveTimer = 0.0f;
        isReached = true;

        EWNS = rand() % 4;

        CurrentPos = *boss->getPosition();

        if (EWNS == 0)TargetPos = EAST;
        if (EWNS == 1)TargetPos = WEST;
        if (EWNS == 2)TargetPos = SOUTH;
        if (EWNS == 3)TargetPos = NORTH;
        moveT = 0.0f;
        boss->setMotion(boss->motion_Move);
        boss->pMoveEffect->setEmit(true);


        mState = STATE_MOVE;
        break;

        // 移動待ち
    case STATE_MOVE:

        moveT += 0.03f;
        if (moveT > 1.0f) moveT = 1.0f;

        XMVECTOR pos = XMVectorLerp(CurrentPos, TargetPos, moveT);
        boss->setPosition(&pos);



        if (moveT >= 1.0f)
        {
            boss->pMoveEffect->setEmit(false);
            boss->setMotion(boss->motion_FallEWNS);
            boss->setMotionLoop(false);
            mState = STATE_MOTION;
        }

        break;

        // モーション待ち
    case STATE_MOTION:

        if (DoOnce) {
            for (int i = 0; i < 64; i++){

                int x = i % 8;
                int y = i / 8;

                bool shouldFall = false; //落としていいかどうかのﾌﾗｸﾞ

                // 方角判定
                if (EWNS == 0) // EAST（右半分）
                {
                    if (x >= 4) shouldFall = true;
                }
                else if (EWNS == 1) // WEST（左半分）
                {
                    if (x <= 3) shouldFall = true;
                }
                else if (EWNS == 2) // NORTH（上半分）
                {
                    if (y >= 4) shouldFall = true;
                }
                else if (EWNS == 3) // SOUTH（下半分）
                {
                    if (y <= 3) shouldFall = true;
                }

                if (!shouldFall) continue;
                
                floor[i]->getEmitterDesc()->LifeMax = 10.0f;
                floor[i]->getEmitterDesc()->LifeMin = 10.0f;
                floor[i]->getEmitter()->setEmit(true);
            }
			DoOnce = false;
        }


        if (!boss->getMotionEnd())return false;

        DoOnce = true;
        mState = STATE_EXE;
        break;

        // 床落下処理
    case STATE_EXE:

        mWaveTimer += 1.0f;

        isReached = true;

        for (int i = 0; i < 64; i++)
        {
            int x = i % 8;
            int y = i / 8;

            bool shouldFall = false; //落としていいかどうかのﾌﾗｸﾞ

            // 方角判定
            if (EWNS == 0) // EAST（右半分）
            {
                if (x >= 4) shouldFall = true;
            }
            else if (EWNS == 1) // WEST（左半分）
            {
                if (x <= 3) shouldFall = true;
            }
            else if (EWNS == 2) // NORTH（上半分）
            {
                if (y >= 4) shouldFall = true;
            }
            else if (EWNS == 3) // SOUTH（下半分）
            {
                if (y <= 3) shouldFall = true;
            }

            if (!shouldFall) continue;
            floor[i]->getEmitterDesc()->LifeMax = 30.0f;
            floor[i]->getEmitterDesc()->LifeMin = 30.0f;

            //ウェーブ化（距離で遅延）
            float delay = 0.0f;

            if (EWNS == 0) delay = (7 - x) * 5;  // 東→西へ波
            if (EWNS == 1) delay = x * 5;        // 西→東へ波
            if (EWNS == 2) delay = (7 - y) * 5;  // 北→南へ波
            if (EWNS == 3) delay = y * 5;        // 南→北へ波

            if (mWaveTimer > delay)
            {
                

                floor[i]->addPositionY(-0.3f);
                fallen[i] = true;

                if (floor[i]->getPositionY() > -30.0f)
                {
                    floor[i]->getEmitter()->setEmit(false);
                    isReached = false;
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
