#include "../../framework.h"
#include "../../framework/vn_environment.h"

BoxMan::BoxMan(const WCHAR* folder, const WCHAR* file):EnemyBase(folder,file)
{
    collide.setSize(3.0f, 6.0f, 3.0f, 0.0f);//物理判定のサイズ
    collide.setOffset(0.0f,2.5f,0.0f);
    setLighting(false);
    setPosition(0.0f, 0.0f, 15.0f);
    setScale(3.0f, 3.0f, 3.0f);
    setMotionLoop(true);
    DoOnce = true;
    isDead = false;




    //ギミック
    GfallEWNS = new FallEWNS();
    GfallLOAD = new FallLoad();
    GfallCROSS = new FallCross();
    GfallRAND = new FallRand();
    GfallSIDE = new FallSide();
    GcangePHASE = new PhaseCange();


    
    



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
    
    currentPhase = new Phase0();
    currentGimmick = nullptr;
}

BoxMan::~BoxMan()
{

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

    delete(GfallEWNS);
    delete(GfallLOAD);
    delete(GfallCROSS);
    delete(GfallRAND);
    delete(GfallSIDE);
    delete(currentPhase);
}

void BoxMan::execute(FloorCube* floor[])
{
    if (CurrentPhase != ReservePhase)
    {
        delete(currentPhase);
        currentPhase = nullptr;
        switch (ReservePhase)
        {
        case EnemyBase::STANDBY:    currentPhase = new Phase0();            break;
        case EnemyBase::PHASE_1:    currentPhase = new Phase1();            break;
        case EnemyBase::PHASE_2:    currentPhase = new Phase2();            break;
        case EnemyBase::PHASE_3:    currentPhase = new Phase3();            break;
        case EnemyBase::PHASE_4:    currentPhase = new Phase4();            break;
        case EnemyBase::FINAL:      currentPhase = new Phase5();            break;
        case EnemyBase::GAMESET:    currentPhase = new Phase6();            break;
        }
    }

    CurrentPhase = ReservePhase;
    if (currentPhase != nullptr) {
        currentPhase->execute(this, floor);//処理
    }
    
    
    vnCharacter::execute();
}

//ボス撃破演出処理
bool BoxMan::GameSet_exe()
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

bool BoxMan::getIsDead()
{
    return isDead;
}

void BoxMan::setIsDead(bool b)
{
    isDead = b;
}



//=============ギミック関数=================

IBossGimmick::IBossGimmick()
{
    initialize();
}

bool IBossGimmick::initialize()
{
    return false;
}