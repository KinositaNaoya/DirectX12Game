#include "../framework.h"
#include "../framework/vn_environment.h"

BoseEnemy1::BoseEnemy1(const WCHAR* folder, const WCHAR* file):EnemyBase(folder,file)
{
    collide.setSize(3.0f, 6.0f, 3.0f, 0.0f);//物理判定のサイズ
    collide.setOffset(0.0f,2.5f,0.0f);
    setLighting(false);
    setPosition(0.0f, 0.0f, 15.0f);
    setScale(3.0f, 3.0f, 3.0f);
    setMotionLoop(true);
    DoOnce = true;

    SkyChangeSE = new vnSound(L"data/sound/SE/グリッチ音02.wav");
    skyChangeFlag = false;


    //===ステージチェンジ用変数===
    CurrentPos = XMVectorZero();
    TargetPos = XMVectorZero();
    moveT = 0.0f;
    
    Randnum = 0;
    isGimmick = true;


    //===Phase1用変数===

    Phase1_BGM = new vnSound(L"data/sound/BGM/塵の雨と硝煙.wav");
    pPhase1_sky = new vnModel(L"data/model/", L"skydome.vnm");
    pPhase1_sky->setLighting(false);
    pPhase1_sky->setScale(5.0f, 5.0f, 5.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase1_sky);


    //===Phase2用変数===

    Phase2_BGM = new vnSound(L"data/sound/BGM/Egoist_2.wav");
    pPhase2_sky = new vnModel(L"data/model/", L"SkyStar.vnm");
    pPhase2_sky->setLighting(false);
    pPhase2_sky->setDoubleSided(true);
    pPhase2_sky->setScale(0.002f, 0.002f, 0.002f);
    pPhase2_sky->setPositionY(200.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase2_sky);

    //===Phase3用変数===

    Phase3_BGM = new vnSound(L"data/sound/BGM/Zero_Time_Outage.wav");
    pPhase3_sky = new vnModel(L"data/model/", L"T_skycode.vnm");
    pPhase3_sky->setLighting(false);
    pPhase3_sky->setDoubleSided(true);
    pPhase3_sky->setScale(0.002f, 0.002f, 0.002f);
    pPhase3_sky->setPositionY(200.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase3_sky);

    //===Phase4用変数===

    Phase4_BGM = new vnSound(L"data/sound/BGM/Cyber_Hounds.wav");
    pPhase4_sky = new vnModel(L"data/model/", L"T_skycode.vnm");
    pPhase4_sky->setLighting(false);
    pPhase4_sky->setDoubleSided(true);
    pPhase4_sky->setScale(0.002f, 0.002f, 0.002f);
    pPhase4_sky->setPositionY(200.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase4_sky);

    //===Phase5用変数===

    Phase5_BGM = new vnSound(L"data/sound/BGM/Kurba.wav");
    pPhase5_sky = new vnModel(L"data/model/", L"T_skycube.vnm");
    pPhase5_sky->setLighting(false);
    pPhase5_sky->setDoubleSided(true);
    pPhase5_sky->setScale(0.002f, 0.002f, 0.002f);
    pPhase5_sky->setPositionY(200.0f);
    vnMainFrame::getSceneInstance()->registerObject(pPhase5_sky);
    



    //===共有モーション===
    motion_idle =   loadMotionFile(L"data/model/Enemy1/motion/Idle.mot");
    motion_dead =   loadMotionFile(L"data/model/Enemy1/motion/Death.mot");
    motion_Change = loadMotionFile(L"data/model/Enemy1/motion/StageChange.mot");
    motion_Move =   loadMotionFile(L"data/model/Enemy1/motion/Move.mot");

    //===ギミック用モーション===
    motion_FallLoad =  loadMotionFile(L"data/model/Enemy1/motion/FallLoad.mot");
    motion_FallEWNS =  loadMotionFile(L"data/model/Enemy1/motion/FallEWNS.mot");

    motion_FallGrand = loadMotionFile(L"data/model/Enemy1/motion/GrandFall.mot");

    motion_FallRand_cast = loadMotionFile(L"data/model/Enemy1/motion/RandFall_Cast.mot");
    motion_FallRand = loadMotionFile(L"data/model/Enemy1/motion/RandFall.mot");

    motion_FallCross = loadMotionFile(L"data/model/Enemy1/motion/FallCross.mot");

    setMotion(motion_idle);
}

BoseEnemy1::~BoseEnemy1()
{
    
    delete(SkyChangeSE);
    delete(Phase1_BGM);
    delete(Phase2_BGM);
    delete(Phase3_BGM);
    delete(Phase4_BGM);
    delete(Phase5_BGM);
    vnMainFrame::getSceneInstance()->deleteObject(pPhase5_sky);
    vnMainFrame::getSceneInstance()->deleteObject(pPhase4_sky);
    vnMainFrame::getSceneInstance()->deleteObject(pPhase3_sky);
    vnMainFrame::getSceneInstance()->deleteObject(pPhase2_sky);
    vnMainFrame::getSceneInstance()->deleteObject(pPhase1_sky);

    deleteMotionFile(motion_idle);
    deleteMotionFile(motion_dead);
    deleteMotionFile(motion_Change);
    deleteMotionFile(motion_Move);
    deleteMotionFile(motion_FallLoad);
    deleteMotionFile(motion_FallEWNS);
    deleteMotionFile(motion_FallGrand);
    deleteMotionFile(motion_FallRand_cast);
    deleteMotionFile(motion_FallRand);
    deleteMotionFile(motion_FallCross);

}

void BoseEnemy1::execute()
{
    

    if (phase == STANDBY) {
        //モーション的なの
        t += 0.05f;  // スピード調整
        float y = sinf(t) * 0.1f; // 振れ幅2.0f
        setPositionY(y - 1.5f);
        setMotion(motion_idle);
        pPhase1_sky->addRotationY(0.0004f);
    }
    
    vnCharacter::execute();
}

void BoseEnemy1::Phase_1_exe(FloorCube* floor[])
{
    if (!Phase1_BGM->isPlaying()) Phase1_BGM->play(true);//BGM
    pPhase1_sky->addRotationY(0.0004f);

    if (floor == nullptr)return;

    if (getEnemyTimer() > 54.0f) {
        PhaseChange_exe(floor);
        return;
    }
    
    DoOnce = true;//ここじゃないとダメ

    

    if (isGimmick) {
        Randnum = rand() % 6;
        isGimmick = false;
    }

    switch (Randnum)
    {
    case 0:case 3:
        isGimmick = FallEWNS(floor);
        break;
    default:
        isGimmick = FallLoad(floor);
        break;
    }
}

void BoseEnemy1::Phase_2_exe(FloorCube* floor[])
{
    if (!Phase1_BGM->isStopped())Phase1_BGM->stop();
    if (!Phase2_BGM->isPlaying()) Phase2_BGM->play(true);
    pPhase2_sky->addRotationY(-0.002f);
    if (floor == nullptr)return;

    if (getEnemyTimer() > 114.0f) {
        PhaseChange_exe(floor);
        return;
    }
    
    DoOnce = true;//ここじゃないとダメ

    

    if (isGimmick) {
        Randnum = rand() % 7;
        isGimmick = false;
    }

    switch (Randnum)
    {
    case 0:
    case 3:
    case 6:
        isGimmick = FallEWNS(floor);
        break;

    case 1:
    case 4:
    case 7:
        isGimmick = FallSide(floor);
        break;

    case 2:
    case 5:
        isGimmick = FallCross(floor);
        break;
    }
}

void BoseEnemy1::Phase_3_exe(FloorCube* floor[])
{
    if (!Phase2_BGM->isStopped())Phase2_BGM->stop();
    if (!Phase3_BGM->isPlaying()) Phase3_BGM->play(true);
    pPhase3_sky->addRotationY(-0.002f);

    if (floor == nullptr)return;

    if (getEnemyTimer() > 174.0f) {
        PhaseChange_exe(floor);
        return;
    }
    DoOnce = true;

    if (isGimmick) {
        Randnum = rand() % 5;
        isGimmick = false;
    }

    switch (Randnum)
    {
    case 0:
    case 3:
        isGimmick = FallCross(floor);
        break;
    case 1:
    case 4:
        isGimmick = FallLoad(floor);
        break;
    case 2:
    case 5:
        isGimmick = FallRand(floor);
        break;
    }

}

void BoseEnemy1::Phase_4_exe(FloorCube* floor[])
{
    if (!Phase3_BGM->isStopped())Phase3_BGM->stop();
    if (!Phase4_BGM->isPlaying()) Phase4_BGM->play(true);
    pPhase4_sky->addRotationY(-0.002f);
    if (floor == nullptr)return;

    if (getEnemyTimer() > 234.0f) {
        PhaseChange_exe(floor);
        return;
    }
    DoOnce = true;

    if (isGimmick) {
        Randnum = rand() % 5;
        isGimmick = false;
    }

    switch (Randnum)
    {
    case 0:
        isGimmick = FallLoad(floor);
        break;
    case 1:
        isGimmick = FallEWNS(floor);
        break;
    case 2:
        isGimmick = FallSide(floor);
        break;
    case 3:
    case 5:
        isGimmick = FallRand(floor);
        break;
    case 4:
        isGimmick = FallCross(floor);
        break;

    }
}

void BoseEnemy1::FINAL_exe(FloorCube* floor[])
{
    if (!Phase4_BGM->isStopped())Phase4_BGM->stop();
    if (!Phase5_BGM->isPlaying()) Phase5_BGM->play(true);
    pPhase5_sky->addRotationY(-0.002f);
    if (floor == nullptr)return;

    DoOnce = true;

    if (isGimmick) {
        Randnum = rand() % 5;
        isGimmick = false;
        setMotionRate(1.5f);
    }

    switch (Randnum)
    {
    case 0:
        isGimmick = FallLoad(floor);
        break;
    case 1:
        isGimmick = FallEWNS(floor);
        break;
    case 2:
        isGimmick = FallSide(floor);
        break;
    case 3:
    case 5:
        isGimmick = FallRand(floor);
        break;
    case 4:
        isGimmick = FallCross(floor);
        break;

    }

}

//ボス撃破演出処理
bool BoseEnemy1::GameSet_exe()
{
    if (DoOnce){
        TimeReset();
        setMotionRate(0.15f);
        setMotionLoop(false);
        setMotion(motion_dead);
        DoOnce = false;
    }

    vnCamera::setTarget(getPosition());
    static XMVECTOR enemyPos = *getPosition();

    enemyPos = XMVectorAdd(enemyPos, XMVectorSet(0.0f, 0.1f, 0.0f, 1.0f));
    static XMVECTOR cameraPos = *vnCamera::getPosition();
    static float time = 0.0f;
    if (time < 0.8f)time += 0.01f;

    XMVECTOR result = XMVectorLerp(cameraPos, enemyPos, time);
    vnCamera::setPosition(&result);

    
    if (getMotionEnd()) {
        return true;
    }
    return false;
}

void BoseEnemy1::PhaseChange_exe(FloorCube* floor[])
{
    if (floor == nullptr)return;

    if (DoOnce) {
        setGimmickState(STATE_SELECT); //ギミックの状態遷移をreset
        isGimmick = true;
        setMotionRate(0.5f);
        setMotionLoop(false);
        setMotion(motion_Change);
        setPositionX(0.0f);
        setPositionZ(0.0f);
        for (int i = 0; i < 64; i++)
        {
            floor[i]->Init();
        }
        DoOnce = false;
        SkyChangeSE->play(true);

        
        
        if (phase == EnemyBase::PHASE_1)pPhase2_sky->setPositionY(0.0f);
        if (phase == EnemyBase::PHASE_2)pPhase3_sky->setPositionY(0.0f);
        if (phase == EnemyBase::PHASE_3)pPhase4_sky->setPositionY(0.0f);
        if (phase == EnemyBase::PHASE_4)pPhase5_sky->setPositionY(0.0f);
    }

    if (SkyChangeSE->isPlaying())return;

    switch (phase)
    {
    case EnemyBase::STANDBY:
        break;
    case EnemyBase::PHASE_1:
        //今回の終了処理
        if (getMotionEnd()) {
            pPhase1_sky->setPositionY(-500.0f);
            pPhase1_sky->setScale(0.0001f, 0.0001f, 0.0001f);
            setMotionRate(1.0f);
        }
        pPhase2_sky->addRotationY(-0.002f);
        if (pPhase2_sky->getScaleX() < 3.0f)pPhase2_sky->addScale(0.01f, 0.01f, 0.01f);
        break;

    case EnemyBase::PHASE_2:
        //今回の終了処理
        if (getMotionEnd()) {
            pPhase2_sky->setPositionY(-500.0f);
            pPhase2_sky->setScale(0.0001f, 0.0001f, 0.0001f);
            setMotionRate(1.0f);
        }
        //次回の初回処理
        pPhase3_sky->addRotationY(-0.0002f);
        if (pPhase3_sky->getScaleX() < 2.0f)pPhase3_sky->addScale(0.01f, 0.01f, 0.01f);
        break;

    case EnemyBase::PHASE_3:
        //今回の終了処理
        if (getMotionEnd()) {
            pPhase3_sky->setPositionY(-500.0f);
            pPhase3_sky->setScale(0.0001f, 0.0001f, 0.0001f);
            setMotionRate(1.0f);
        }
        //次回の初回処理
        pPhase4_sky->addRotationY(-0.0002f);
        if (pPhase4_sky->getScaleX() < 2.0f)pPhase4_sky->addScale(0.01f, 0.01f, 0.01f);
        break;

    case EnemyBase::PHASE_4:
        //今回の終了処理
        if (getMotionEnd()) {
            pPhase4_sky->setPositionY(-500.0f);
            pPhase4_sky->setScale(0.0001f, 0.0001f, 0.0001f);
            setMotionRate(1.0f);
        }
        //次回の初回処理
        pPhase5_sky->addRotationY(-0.0002f);
        if (pPhase5_sky->getScaleX() < 2.0f)pPhase5_sky->addScale(0.01f, 0.01f, 0.01f);
        break;

    case EnemyBase::FINAL:
        break;

    case EnemyBase::GAMESET:
        break;
    }
}

//=============ギミック関数=================

//ギミックステートを操作
void BoseEnemy1::setGimmickState(Gimmick state)
{
    mState = state;
}

//エネミーから対岸までの直線を落とす攻撃
bool BoseEnemy1::FallLoad(FloorCube* floor[])
{
    /* : ステージ表
    56,57,58,59,60,61,62,63,
    48,49,50,51,52,53,54,55,
    40,41,42,43,44,45,46,47,
    32,33,34,35,36,37,38,39,
    24,25,26,27,28,29,30,31,
    16,17,18,19,20,21,22,23,
    08,09,10,11,12,13,14,15,
    00,01,02,03,04,05,06,07,
    */

    //エネミーの出現位置
    int edgeIndex[] = {
         1, 2, 3, 4, 5, 6,
         8,15,16,23,24,31,
        32,39,40,47,48,55,
        60,61,62,57,58,59
    };

    int edgeCount = sizeof(edgeIndex) / sizeof(int);
    static int mSelected = 0;
    static float mWaveTimer = 0.0f;
    static bool reached = true;//床が下まで到達したかどうか

    switch (mState)
    {
       // ランダム抽選
    case STATE_SELECT:

        reached = true;
        mWaveTimer = 0.0f;

        mSelected = edgeIndex[rand() % edgeCount];

        CurrentPos = *this->getPosition();

        TargetPos = XMVectorSet(
            floor[mSelected]->getPositionX(),
            this->getPositionY(),
            floor[mSelected]->getPositionZ(),
            1.0f
        );
        setMotion(motion_Move);

        moveT = 0.0f;

        mState = STATE_MOVE;
        break;

        // 移動待ち
    case STATE_MOVE:

        moveT += 0.03f;
        if (moveT > 1.0f) moveT = 1.0f;

        XMVECTOR pos = XMVectorLerp(CurrentPos, TargetPos, moveT);
        this->setPosition(&pos);

        if (moveT >= 1.0f)
        {
            setMotion(motion_FallLoad);
            setMotionLoop(false);
            mState = STATE_MOTION;
        }

        break;

        // モーション待機
    case STATE_MOTION:
        if (!getMotionEnd())return false;

        mState = STATE_EXE;
        break;

        // 床落下処理
    case STATE_EXE:
        {
            mWaveTimer += 1.0f;

            int bossX = mSelected % 8;
            int bossY = mSelected / 8;

            reached = true;

            for (int i = 0; i < 8; i++) {

                for (int offset = -1; offset <= 1; offset++) // ←3列
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

                        if (floor[index]->getPositionY() > -30.0f)
                        {
                            reached = false;
                        }
                        else
                        {
                            floor[index]->setPositionY(-100.0f);
                        }
                    }
                }
            }

            if (reached)
            {
                for (int i = 0; i < 64; i++){
                    floor[i]->Init();
                }
                mState = STATE_SELECT;
                return true;
            }
        }
    break;
    }
    return false;
}

//東西南北のどれか半分を落とす攻撃
bool BoseEnemy1::FallEWNS(FloorCube* floor[])
{
    static float mWaveTimer = 0.0f;
    static int EWNS = 0;
    static bool fallen[64] = { false };
    static bool reached = true;
    static int randNum = 0;

    switch (mState)
    {
        // ランダム抽選
    case STATE_SELECT:
        
        mWaveTimer = 0.0f;
        reached = true;

        EWNS = rand() % 4;
        if (EWNS == randNum)EWNS = rand() % 4;

        CurrentPos = *this->getPosition();

        if (EWNS == 0)TargetPos = EAST;
        if (EWNS == 1)TargetPos = WEST;
        if (EWNS == 2)TargetPos = SOUTH;
        if (EWNS == 3)TargetPos = NORTH;
        moveT = 0.0f;
        setMotion(motion_Move);


        mState = STATE_MOVE;
        break;

        // 移動待ち
    case STATE_MOVE:

        moveT += 0.03f;
        if (moveT > 1.0f) moveT = 1.0f;

        XMVECTOR pos = XMVectorLerp(CurrentPos, TargetPos, moveT);
        this->setPosition(&pos);

        if (moveT >= 1.0f)
        {
            setMotion(motion_FallEWNS);
            setMotionLoop(false);
            mState = STATE_MOTION;
        }

        break;

        // モーション待ち
    case STATE_MOTION:
        if (!getMotionEnd())return false;

        mState = STATE_EXE;
        break;

        // 床落下処理
    case STATE_EXE:
    
        mWaveTimer += 1.0f;

        reached = true;

        for (int i = 0; i < 64; i++)
        {
            int x = i % 8;
            int y = i / 8;

            bool shouldFall = false;

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
                    reached = false;
                }
            }
        }

        if (reached)
        {
            for (int i = 0; i < 64; i++)
            {
                if (fallen[i])
                {
                    floor[i]->Init();
                    fallen[i] = false;//次回のためにクリア
                }
            }

            mWaveTimer = 0.0f;

            setMotion(motion_idle);
            setMotionLoop(true);
            TimeReset();

            mState = STATE_SELECT;
            return true;
        }

        break;
    }
    return false;
}

//サイドを落とす攻撃
bool BoseEnemy1::FallSide(FloorCube* floor[])
{
    
    static int EWNS = 0;
    static bool reached = true;
    static int randNum = 0;

    

    switch (mState)
    {
        // ランダム抽選
    case STATE_SELECT:


        EWNS = rand() % 4;
        if (EWNS == randNum)EWNS = rand() % 4;
        
        CurrentPos = *this->getPosition();


        if (EWNS == 0)TargetPos = EAST;
        if (EWNS == 1)TargetPos = WEST;
        if (EWNS == 2)TargetPos = SOUTH;
        if (EWNS == 3)TargetPos = NORTH;
        moveT = 0.0f;

        setMotion(motion_Move);


        mState = STATE_MOVE;
        break;

    case STATE_MOVE:

        moveT += 0.03f;
        if (moveT > 1.0f) moveT = 1.0f;

        XMVECTOR pos = XMVectorLerp(CurrentPos, TargetPos, moveT);
        this->setPosition(&pos);

        if (moveT >= 1.0f)
        {
            setMotion(motion_FallGrand);
            setMotionLoop(false);
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

        if (!getMotionEnd())return false;

        mState = STATE_EXE;
        break;


        // 床落下処理
    case STATE_EXE:

        reached = true;

        for (int i = 0; i < 64; i++)
        {
            int row = i / 8;
            int col = i % 8;

            
            if (EWNS == 0 || EWNS == 1)
            {
                if (row <= 2 || row >= 5) {
                    floor[i]->addPositionY(-0.3f);
                    floor[i]->setRotation(0.0f, 0.0f, 0.0f);
                    if (floor[i]->getPositionY() > -30.0f)reached = false;
                }
            }
            else
            {
                if (col <= 2 || col >= 5) {
                    floor[i]->addPositionY(-0.3f);
                    floor[i]->setRotation(0.0f,0.0f,0.0f);
                    if (floor[i]->getPositionY() > -30.0f)reached = false;
                }
            }
            
        }
        if (reached)
        {
            for (int i = 0; i < 64; i++)
            {
                int row = i / 8;
                int col = i % 8;

                if (EWNS == 0 || EWNS == 1)
                {
                    if (row <= 2 || row >= 5) {
                        floor[i]->Init();
                    }
                }
                else
                {
                    if (col <= 2 || col >= 5) {
                        floor[i]->Init();
                    }
                }
            }

            reached = true;
            setMotion(motion_idle);
            setMotionLoop(true);
            TimeReset();
            mState = STATE_SELECT;
            return true;
        }
        break;
    }
    return false;
}

//ランダムな床を赤か緑かで落とすor残す処理
bool BoseEnemy1::FallRand(FloorCube* floor[])
{
    /* : ステージ表
    56,57,58,59,60,61,62,63,
    48,49,50,51,52,53,54,55,
    40,41,42,43,44,45,46,47,
    32,33,34,35,36,37,38,39,
    24,25,26,27,28,29,30,31,
    16,17,18,19,20,21,22,23,
    08,09,10,11,12,13,14,15,
    00,01,02,03,04,05,06,07,
    */

    static int randFloor[32];

    static int EWNS = 0;
    static int randNum = 0;

    static float stayTimer = 0.0f;

    static bool FallRed = true;


    switch (mState)
    {
        // ランダム抽選
    case STATE_SELECT:
        stayTimer = 0.0f;

        EWNS = rand() % 4;
        if (EWNS == randNum)EWNS = rand() % 4;
        if (rand() % 2 == 0) {
            FallRed = true;
        }
        else{
            FallRed = false;
        }

        CurrentPos = *this->getPosition();

        if (EWNS == 0)TargetPos = EAST;
        if (EWNS == 1)TargetPos = WEST;
        if (EWNS == 2)TargetPos = SOUTH;
        if (EWNS == 3)TargetPos = NORTH;
        moveT = 0.0f;

        int RandomIndex[64];

        // 0～63を順番に入れる
        for (int i = 0; i < 64; ++i)
        {
            RandomIndex[i] = i;
        }

        
        {// シャッフル
        std::random_device rd;
        std::mt19937 mt(rd());
        std::shuffle(RandomIndex, RandomIndex + 64, mt);
        }

        // 先頭30個をコピー
        for (int i = 0; i < 32; ++i)
        {
            randFloor[i] = RandomIndex[i];
        }

        setMotion(motion_Move);

        mState = STATE_MOVE;
        break;

        // 移動待ち
    case STATE_MOVE:

        moveT += 0.03f;
        if (moveT > 1.0f) moveT = 1.0f;

        XMVECTOR pos = XMVectorLerp(CurrentPos, TargetPos, moveT);
        this->setPosition(&pos);

        if (moveT >= 1.0f)
        {
            setMotion(motion_FallRand_cast);
            setMotionLoop(true);
            mState = STATE_MOTION;
        }

        break;

        // モーション待ち
    case STATE_MOTION:
        {
            stayTimer += 1.0f / 30.0f;

            float t = stayTimer / 2.0f;
            if (t > 1.0f) t = 1.0f;
            
            if (FallRed) {
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
                setMotionLoop(false);
                setMotion(motion_FallRand);
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
                if (FallRed) {
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
                for (int i = 0; i < 64; ++i)
                {
                    bool isSafe = false;

                    for (int j = 0; j < 32; ++j)
                    {
                        if (i == randFloor[j])
                        {
                            isSafe = true;
                            break;
                        }
                    }

                    if (FallRed) {
                        if (isSafe) floor[i]->Init();
                    }
                    else {
                        if (!isSafe) floor[i]->Init();
                    }
                    floor[i]->setDiffuse(0.0f, 1.0f, 1.0f, 1.0f);
                }
                mState = STATE_SELECT;
                return true;
            }
        }
    break;

    }
    return false;
}

//十字に床を落とす
bool BoseEnemy1::FallCross(FloorCube* floor[])
{
    /* : ステージ表
    56,57,58,59,60,61,62,63,
    48,49,50,51,52,53,54,55,
    40,41,42,43,44,45,46,47,
    32,33,34,35,36,37,38,39,
    24,25,26,27,28,29,30,31,
    16,17,18,19,20,21,22,23,
    08,09,10,11,12,13,14,15,
    00,01,02,03,04,05,06,07,
    */
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
    case BoseEnemy1::STATE_SELECT:

        CurrentPos = *this->getPosition();
        TargetPos = XMVectorSet(0.0f,0.0f,0.0f,1.0f);
        moveT = 0.0f;

        setMotion(motion_Move);

        mState = STATE_MOVE;

        break;
    case BoseEnemy1::STATE_MOVE:
        moveT += 0.03f;
        if (moveT > 1.0f) moveT = 1.0f;

        XMVECTOR pos = XMVectorLerp(CurrentPos, TargetPos, moveT);
        this->setPosition(&pos);

        if (moveT >= 1.0f)
        {
            setMotion(motion_FallCross);
            setMotionLoop(false);
            moveT = 0.0f;
            mState = STATE_MOTION;
        }

        break;
    case BoseEnemy1::STATE_MOTION:

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


        if (!getMotionEnd() && moveT != 1.0f)return false;


        mState = STATE_EXE;
        break;
    case BoseEnemy1::STATE_EXE:
        {
            bool allReached = true;

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
                    floor[i]->Init();

                }
                mState = STATE_SELECT;
                return true;
            }
        }
        break;
    }

    return false;
}
