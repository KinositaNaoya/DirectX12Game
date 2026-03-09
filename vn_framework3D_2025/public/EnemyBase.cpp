#include "../framework.h"
#include "../framework/vn_environment.h"

EnemyBase::EnemyBase(const WCHAR* folder, const WCHAR* file):vnCharacter(folder, file)
{
    srand((unsigned int)time(NULL));
    collide.setOwner(this);
    Time = 0.0f;
    elapsedTime = 0.0f;
    Second = 0.0f;
    if (vnMainFrame::getGameLevel() == true) {
        LoadLevel();
    }
}

EnemyBase::~EnemyBase()
{
    SaveLevel();
}

void EnemyBase::TimeUpdate(){
    
    // デバッグ加速
    //if (vnKeyboard::on(DIK_P)) Time += 1.0f;


    Time += 1.0f / 60.0f;

    // 1秒経過
    if (Time >= 1.0f)
    {
        Time -= 1.0f;
        elapsedTime++;
        Second++;
    }
    // 60秒で分へ
    if (Second >= 60.0f)
    {
        Second = 0.0f;
        Minutes++;
    }

    // 1秒経過まではStandby
    if (elapsedTime == 0)
    {
        phase = STANDBY;
        return;
    }

    UpdatePhase();
}

void EnemyBase::setSkipPhase(PHASETABLE Phase)
{
    switch (Phase)
    {
    case EnemyBase::STANDBY:
        elapsedTime = 0;
        break;
    case EnemyBase::PHASE_1:
        elapsedTime = 0;
        break;
    case EnemyBase::PHASE_2:
        elapsedTime = 55.0f;
        break;
    case EnemyBase::PHASE_3:
        elapsedTime = 115.0f;
        break;
    case EnemyBase::PHASE_4:
        elapsedTime = 175.0f;
        break;
    case EnemyBase::FINAL:
        elapsedTime = 235.0f;
        break;
    case EnemyBase::GAMESET:
        elapsedTime = 300.0f;
        break;
    }
}

void EnemyBase::SaveLevel()
{

    std::ofstream file("data/save/SavePhase.txt");

    if (!file.is_open()) return;

    file << (int)phase;
    file.close();
}

void EnemyBase::LoadLevel()
{
    
    std::ifstream file("data/save/SavePhase.txt");

    if (!file.is_open()) return;

    int phaseNumber;

    file >> phaseNumber;

    setSkipPhase((PHASETABLE)phaseNumber);
    

    file.close();

}

void EnemyBase::UpdatePhase()
{
    if (elapsedTime < 1.0f)
    {
        phase = STANDBY;
    }
    else if (elapsedTime < 60.0f)
    {
        phase = PHASE_1;
    }
    else if (elapsedTime < 120.0f)
    {
        phase = PHASE_2;
    }
    else if (elapsedTime < 180.0f)
    {
        phase = PHASE_3;
    }
    else if (elapsedTime < 240.0f)
    {
        phase = PHASE_4;
    }
    else if (elapsedTime < 300.0f)
    {
        phase = FINAL;
    }
    else
    {
        elapsedTime = 300.0f;
        phase = GAMESET;
    }
}

float EnemyBase::getScond()
{
    return Second;
}

float EnemyBase::getMinutes()
{
    return Minutes;
}

int EnemyBase::getEnemyTimer()
{
    return elapsedTime;
}

Collider::Cube* EnemyBase::getCubeCollider()
{
    return &collide;
}

//フェーズの時間を0～1で返す
float EnemyBase::getEnemyPhaseRate(){
    float phaseTime = fmod(elapsedTime, phaseLength);

    return phaseTime / phaseLength;
}