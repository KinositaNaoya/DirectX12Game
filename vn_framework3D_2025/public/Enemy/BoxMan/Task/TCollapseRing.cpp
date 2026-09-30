#include "../../../../framework.h"
#include "../../../../framework/vn_environment.h"

bool FallRing::initialize()
{
    mState = STATE_SELECT;
    CurrentRing = 0;
	isFAST = true;
    return false;
}


bool FallRing::execute(BoxMan* boss, FloorCube* floor[])
{
    switch (mState)
    {
    case BoxMan::STATE_SELECT:
        isFAST = true;
        CurrentRing = 0;
        for (int i = 0; i < 64; ++i)
        {
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

        if (moveT > 1.0f)
            moveT = 1.0f;

        XMVECTOR pos =
            XMVectorLerp(CurrentPos, TargetPos, moveT);

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

        if (isFAST) {
            moveT += 1.0f / 45.0f;
        }
        else
        {
            moveT += 1.0f / 10.0f;
        }
        

        if (moveT > 1.0f)
            moveT = 1.0f;


        //================================
        // 現在落とすリングを赤くする
        //================================

        for (int i = 0; i < 64; ++i)
        {
            int x = i % 8;
            int y = i / 8;

            int ring = min(
                min(x, 7 - x),
                min(y, 7 - y)
            );

            // 現在のリング以外は無視
            if (ring != CurrentRing)
                continue;


            float r = 1.0f;
            float g = 1.0f - moveT;
            float b = 1.0f - moveT;

            floor[i]->setDiffuse(
                r, g, b, 1.0f
            );
        }


        if (moveT != 1.0f)
            return false;


        moveT = 0.0f;

        mState = STATE_EXE;

        break;


    case BoxMan::STATE_EXE:
    {
        isReached = true;


        //================================
        // 現在のリングを落とす
        //================================

        for (int i = 0; i < 64; ++i)
        {
            int x = i % 8;
            int y = i / 8;

            int ring = min(
                min(x, 7 - x),
                min(y, 7 - y)
            );


            // 現在のリング以外は処理しない
            if (ring != CurrentRing)
                continue;


            floor[i]->addPositionY(-0.5f);

            float posY = floor[i]->getPositionY();

            if (posY > -30.0f)
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
            // 次のリングへ
            CurrentRing++;

            // 中央4マスまで来たら終了
            if (CurrentRing >= 3)
            {
                mState = STATE_SELECT;
                return true;
            }

            // 次のリングの赤色処理へ
            moveT = 0.0f;
            mState = STATE_MOTION;
			isFAST = false;
        }
    }
    break;
    }

    return false;
}