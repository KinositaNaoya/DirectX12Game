#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

bool FallRand::initialize()
{
    mState = STATE_SELECT;
    EWNS = 0;
    stayTimer = 0.0f;
    isFallRed = true;
    return false;
}

//ランダムな床を赤か緑かで落とすor残す処理
bool FallRand::execute(BoxMan* boss, FloorCube* floor[])
{
    switch (mState){
        // ランダム抽選と初期化
    case STATE_SELECT:
        //初期化
        for (int i = 0; i < 64; ++i) {
            floor[i]->setDiffuse(0.0f, 1.0f, 1.0f, 1.0f);
            floor[i]->Init();
        }
        stayTimer = 0.0f;
        moveT = 0.0f;

        //ランダム抽選
        EWNS = rand() % 4;
        if (rand() % 2 == 0) {
            isFallRed = true;
        }
        else {
            isFallRed = false;
        }

        CurrentPos = *boss->getPosition();

        if (EWNS == 0)TargetPos = EAST;
        if (EWNS == 1)TargetPos = WEST;
        if (EWNS == 2)TargetPos = SOUTH;
        if (EWNS == 3)TargetPos = NORTH;

        // 0～63を順番に入れる
        int RandomIndex[64];
        for (int i = 0; i < 64; ++i)
        {
            RandomIndex[i] = i;
        }
        // シャッフル
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::shuffle(RandomIndex, RandomIndex + 64, mt);
        }

        // 先頭30個をコピー
        for (int i = 0; i < 32; ++i)
        {
            randFloor[i] = RandomIndex[i];
        }

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
        boss->setMotion(boss->motion_FallRand_cast);
        boss->setMotionLoop(true);
        mState = STATE_MOTION;
    }
    
        break;

        // モーション待ち
    case STATE_MOTION:
    {
        stayTimer += 1.0f / 30.0f;

        float t = stayTimer / 2.0f;
        if (t > 1.0f) t = 1.0f;

        if (isFallRed) {
            // 徐々に赤へ変化
            for (int i = 0; i < 32; ++i)
            {
                int index = randFloor[i];

                float r = 1.0f;          // 赤最大
                float g = 1.0f - t;      // 徐々に減少
                float b = 1.0f - t;      // 徐々に減少

                floor[index]->setDiffuse(r, g, b, 1.0f);
            }
        }
        else
        {
            // 徐々に緑へ変化
            for (int i = 0; i < 32; ++i)
            {
                int index = randFloor[i];

                float r = 1.0f - t;          // 赤最大
                float g = 1.0f;      // 徐々に減少
                float b = 1.0f - t;      // 徐々に減少

                floor[index]->setDiffuse(r, g, b, 1.0f);
            }
        }


        if (stayTimer >= 2.0f)
        {
            boss->setMotionLoop(false);
            boss->setMotion(boss->motion_FallRand);
            mState = STATE_EXE;
        }
    }
    break;

    // 床落下処理
    case STATE_EXE:
    {
        bool allReached = true;

        for (int i = 0; i < 64; ++i)
        {
            // この床が「残す床」か判定
            bool isSafe = false;

            for (int j = 0; j < 32; ++j)
            {
                if (i == randFloor[j])
                {
                    isSafe = true;
                    break;
                }
            }

            // 残す床ならスキップ
            if (isFallRed) {
                if (!isSafe) continue;
            }
            else {
                if (isSafe) continue;
            }


            // 落とす床のみ処理
            floor[i]->addPositionY(-0.3f);

            float y = floor[i]->getPositionY();

            if (y > -30.0f)
            {
                allReached = false;
            }
            else
            {
                floor[i]->setPositionY(-30.0f);
            }
        }

        // 落とす床が全て -30 到達でステート変更
        if (allReached)
        {
            mState = STATE_SELECT;
            return true;
        }
    }
    break;

    }
    return false;
}
