#include "../../framework.h"
#include "../../framework/vn_environment.h"

EnemyBase::EnemyBase(const WCHAR* folder, const WCHAR* file):vnCharacter(folder, file)
{
    srand((unsigned int)time(NULL));
    collide.setOwner(this);
    Time = 0.0f;
    elapsedTime = 0.0f;
    Second = 0.0f;
    CurrentPhase = STANDBY;
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
        CurrentPhase = STANDBY;
        return;
    }

    UpdatePhase();
}

//フェーズをスキップする関数
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
        elapsedTime = 60.0f;
        break;
    case EnemyBase::PHASE_3:
        elapsedTime = 120.0f;
        break;
    case EnemyBase::PHASE_4:
        elapsedTime = 180.0f;
        break;
    case EnemyBase::FINAL:
        elapsedTime = 240.0f;
        break;
    case EnemyBase::GAMESET:
        elapsedTime = 300.0f;
        break;
    }
}

//フェーズレベルをセーブ
void EnemyBase::SaveLevel()
{

    std::ofstream file("data/save/SavePhase.txt");

    if (!file.is_open()) return;

    file << (int)CurrentPhase;
    file.close();
}

//フェーズレベルをロード
void EnemyBase::LoadLevel()
{
    
    std::ifstream file("data/save/SavePhase.txt");

    if (!file.is_open()) return;

    int phaseNumber;

    file >> phaseNumber;

    setSkipPhase((PHASETABLE)phaseNumber);
    

    file.close();

}

//エネミー時間に応じてフェーズを更新
void EnemyBase::UpdatePhase()
{
    if (elapsedTime < 1.0f)
    {
        ReservePhase = STANDBY;
    }
    else if (elapsedTime < 60.0f)
    {
        ReservePhase = PHASE_1;
    }
    else if (elapsedTime < 120.0f)
    {
        ReservePhase = PHASE_2;
    }
    else if (elapsedTime < 180.0f)
    {
        ReservePhase = PHASE_3;
    }
    else if (elapsedTime < 240.0f)
    {
        ReservePhase = PHASE_4;
    }
    else if (elapsedTime < 300.0f)
    {
        ReservePhase = FINAL;
    }
    else
    {
        elapsedTime = 300.0f;
        ReservePhase = GAMESET;
    }
}

//秒を取得
float EnemyBase::getScond()
{
    return Second;
}

//分を取得
float EnemyBase::getMinutes()
{
    return Minutes;
}

//総秒数を取得
int EnemyBase::getEnemyTimer()
{
    return elapsedTime;
}

//コライダーの取得
Collider::Cube* EnemyBase::getCubeCollider()
{
    return &collide;
}

//フェーズの時間(60秒)を0～1で返す
float EnemyBase::getEnemyPhaseRate(){
    float phaseTime = fmod(elapsedTime, phaseLength);

    return phaseTime / phaseLength;
}