#include "BossPhase.h"


BossPhase::BossPhase()
{
    LoadPhaseData(L"data/Phase/Phase0");
}

BossPhase::~BossPhase()
{
	delete(pPhase_skyModel);
	delete(PhaseBGM);
	delete(currentGimmick);
}

void BossPhase::ChangePhase(EnemyBase::PHASETABLE CurrentPhase)
{
    switch (CurrentPhase)
    {
    case EnemyBase::STANDBY:
        LoadPhaseData(L"data/Phase/Phase0");
        break;
    case EnemyBase::PHASE_1:
        LoadPhaseData(L"data/Phase/Phase1");
        break;
    case EnemyBase::PHASE_2:
        break;
    case EnemyBase::PHASE_3:
        break;
    case EnemyBase::PHASE_4:
        break;
    case EnemyBase::FINAL:
        break;
    case EnemyBase::GAMESET:
        break;
    default:
        break;
    }

}
void BossPhase::execute(BoxMan* boss, FloorCube* floor[]) {

}

//textからフェーズの情報を読み込む関数
void BossPhase::LoadPhaseData(const wchar_t* filePath)
{


	std::wifstream file(filePath);  //全文
    if (!file)
        return;

    std::wstring line;              //一行

	// ファイルを一行ずつ読み込む
    while (std::getline(file, line))
    {
        // SkyModel
        if (line.find(L"SkyModel{") == 0)
        {
            size_t start = line.find(L'{') + 1;
            size_t end = line.find(L'}');

            //モデル名
            std::wstring modelName = line.substr(start, end - start);

            // モデル名が空の場合の処理
            if(modelName.empty()){
                pPhase_skyModel->setRenderEnable(true);//モデル非表示
                break;
			}
            //新しいモデル上書き
            pPhase_skyModel = new vnModel(
                L"data/model/",
                modelName.c_str()
            );

            pPhase_skyModel->setLighting(false);
            pPhase_skyModel->setScale(5.0f, 5.0f, 5.0f);
            vnMainFrame::getSceneInstance()->registerObject(pPhase_skyModel);
        }

        // BGM
        if (line.find(L"BGM{") == 0)
        {
            size_t start = line.find(L'{') + 1;
            size_t end = line.find(L'}');
			std::wstring bgmName = line.substr(start, end - start);
            std::wstring bgmPath = L"data/sound/BGM/" + bgmName;

			//BGM名が空の場合の処理
            if (bgmName.empty()) {
                PhaseBGM->stop();
                break;
            }

            PhaseBGM = new vnSound(bgmPath.c_str());
        }

		// Gimmick
        
    }
}

//つづき
//ギミックのとこと
//フェーズまとめるのと～