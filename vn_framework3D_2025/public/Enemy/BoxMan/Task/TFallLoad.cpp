#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

bool FallLoad::initialize()
{
    mState = STATE_SELECT;
    isReached = false;
    mWaveTimer = 0.0f;
    mSelected = 0;
    return false;
}

//エネミーから対岸までの直線を落とす攻撃
bool FallLoad::execute(BoxMan* boss, FloorCube* floor[])
{

    //エネミーの出現位置
    int edgeIndex[] = {
         1, 2, 3, 4, 5, 6,
         8,15,16,23,24,31,
        32,39,40,47,48,55,
        60,61,62,57,58,59
    };

    int edgeCount = sizeof(edgeIndex) / sizeof(int);
    

    switch (mState){
        // ランダム抽選
    case STATE_SELECT:
        for (int i = 0; i < 64; ++i) {
            floor[i]->Init();
        }

		DoOnce = true;
        isReached = true;
        mWaveTimer = 0.0f;

        mSelected = edgeIndex[rand() % edgeCount];

        CurrentPos = *boss->getPosition();

        TargetPos = XMVectorSet(
            floor[mSelected]->getPositionX(),
            boss->getPositionY(),
            floor[mSelected]->getPositionZ(),
            1.0f
        );
        boss->setMotion(boss->motion_Move);

        boss->pMoveEffect->setEmit(true);

        moveT = 0.0f;

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
            boss->setMotion(boss->motion_FallLoad);
            boss->setMotionLoop(false);
            mState = STATE_MOTION;
        }

        break;

        // モーション待機
    case STATE_MOTION:
    {
        if (DoOnce) {
            int bossX = mSelected % 8;
            int bossY = mSelected / 8;

            for (int i = 0; i < 8; i++) {

                for (int offset = -1; offset <= 1; offset++)//幅三つ分
                {
                    int index = -1;

                    if (bossY == 0)
                    {
                        int x = bossX + offset;
                        if (x < 0 || x > 7) continue;
                        index = i * 8 + x;
                    }
                    else if (bossY == 7)
                    {
                        int x = bossX + offset;
                        if (x < 0 || x > 7) continue;
                        index = (7 - i) * 8 + x;
                    }
                    else if (bossX == 0)
                    {
                        int y = bossY + offset;
                        if (y < 0 || y > 7) continue;
                        index = y * 8 + i;
                    }
                    else
                    {
                        int y = bossY + offset;
                        if (y < 0 || y > 7) continue;
                        index = y * 8 + (7 - i);
                    }

                    floor[index]->getEmitterDesc()->LifeMax = 10.0f;
                    floor[index]->getEmitterDesc()->LifeMin = 10.0f;
                    floor[index]->getEmitter()->setEmit(true);
                }
            }

			DoOnce = false;
        }

        if (!boss->getMotionEnd())return false;

		DoOnce = true;

        mState = STATE_EXE;
    }
        break;

        // 床落下処理
    case STATE_EXE:
    {
        mWaveTimer += 1.0f;

        int bossX = mSelected % 8;
        int bossY = mSelected / 8;

        isReached = true;

        for (int i = 0; i < 8; i++) {

            for (int offset = -1; offset <= 1; offset++)//幅三つ分
            {
                int index = -1;

                if (bossY == 0)
                {
                    int x = bossX + offset;
                    if (x < 0 || x > 7) continue;
                    index = i * 8 + x;
                }
                else if (bossY == 7)
                {
                    int x = bossX + offset;
                    if (x < 0 || x > 7) continue;
                    index = (7 - i) * 8 + x;
                }
                else if (bossX == 0)
                {
                    int y = bossY + offset;
                    if (y < 0 || y > 7) continue;
                    index = y * 8 + i;
                }
                else
                {
                    int y = bossY + offset;
                    if (y < 0 || y > 7) continue;
                    index = y * 8 + (7 - i);
                }

                if (mWaveTimer > i * 5)
                {
                    floor[index]->addPositionY(-0.3f);

                    floor[index]->getEmitterDesc()->LifeMax = 30.0f;
                    floor[index]->getEmitterDesc()->LifeMin = 30.0f;
                    

                    if (floor[index]->getPositionY() > -30.0f){
                        isReached = false;
                    }
                    else{
                        floor[index]->getEmitter()->setEmit(false);
                        floor[index]->setPositionY(-100.0f);
                    }
                }
            }
        }

        if (isReached)
        {
            mState = STATE_SELECT;
            return true;
        }
    }
    break;
    }
    return false;
}

