#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

bool FallCross::initialize()
{
    mState = STATE_SELECT;
    return false;
}

//十字に床を落とす
bool FallCross::execute(BoxMan* boss, FloorCube* floor[])
{
    int Cross[28] =
    {          3, 4,
              11,12,
              19,20,
     24,25,26,27,28,29,30,31,
     32,33,34,35,36,37,38,39,
              43,44,
              51,52,
              59,60,
    };
    
    

    switch (mState)
    {
    case BoxMan::STATE_SELECT:

        for (int i = 0; i < 64; ++i) {
            floor[i]->Init();
        }

        CurrentPos = *boss->getPosition();
        TargetPos = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
        moveT = 0.0f;

        boss->setMotion(boss->motion_Move);

        boss->pMoveEffect->setEmit(true);

        mState = STATE_MOVE;

        break;
    case BoxMan::STATE_MOVE:
        moveT += 0.03f;
        if (moveT > 1.0f) moveT = 1.0f;

        XMVECTOR pos = XMVectorLerp(CurrentPos, TargetPos, moveT);
        boss->setPosition(&pos);

		

        if (moveT >= 1.0f)
        {
            boss->pMoveEffect->setEmit(false);
            boss->setMotion(boss->motion_FallCross);
            boss->setMotionLoop(false);
            moveT = 0.0f;
            mState = STATE_MOTION;
        }

        break;
    case BoxMan::STATE_MOTION:

        moveT += 1.0f / 60.0f;

        if (moveT > 1.0f) moveT = 1.0f;


        // 徐々に赤へ変化
        for (int i = 0; i < 28; ++i)
        {
            int index = Cross[i];

            float r = 1.0f;          // 赤最大
            float g = 1.0f - moveT;      // 徐々に減少
            float b = 1.0f - moveT;      // 徐々に減少

            floor[index]->setDiffuse(r, g, b, 1.0f);
        }


        if (!boss->getMotionEnd() && moveT != 1.0f)return false;


        mState = STATE_EXE;
        break;
    case BoxMan::STATE_EXE:
    {
        isReached = true;

        for (int i = 0; i < 64; ++i)
        {
            // この床が「残す床」か判定
            bool isSafe = true;

            for (int j = 0; j < 28; ++j)
            {
                if (i == Cross[j])
                {
                    isSafe = false;
                    break;
                }
            }

            if (isSafe) continue;



            // 落とす床のみ処理
            floor[i]->addPositionY(-0.3f);

            float y = floor[i]->getPositionY();

            if (y > -30.0f)
            {
                isReached = false;
            }
            else
            {
                floor[i]->setPositionY(-30.0f);
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
