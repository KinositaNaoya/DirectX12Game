#include "BossPhase.h"

BossPhase::BossPhase()
{
	pTaskManager = new TaskManager();
    LoadPhaseData(L"data/Phase/Phase0.txt");
	isGimmick = true;

}

BossPhase::~BossPhase()
{

	vnMainFrame::getSceneInstance()->deleteObject(pPhase_skyModel);
	delete(PhaseBGM);
}

//フェーズの変更
void BossPhase::ChangePhase(class BoxMan* Boss, EnemyBase::PHASETABLE ReservePhase)
{

    if (Boss->CurrentPhase != ReservePhase) {

        switch (ReservePhase)
        {
        case EnemyBase::STANDBY:    LoadPhaseData(L"data/Phase/Phase0.txt");    break;
        case EnemyBase::PHASE_1:    LoadPhaseData(L"data/Phase/Phase1.txt");    break;
        case EnemyBase::PHASE_2:    LoadPhaseData(L"data/Phase/Phase2.txt");    break;
        case EnemyBase::PHASE_3:    LoadPhaseData(L"data/Phase/Phase3.txt");    break;
        case EnemyBase::PHASE_4:    LoadPhaseData(L"data/Phase/Phase4.txt");    break;
        case EnemyBase::FINAL:      LoadPhaseData(L"data/Phase/Phase5.txt");    break;
        case EnemyBase::GAMESET:    LoadPhaseData(L"data/Phase/Phase6.txt");    break;
        }
        DoOnce = true;
    }
    Boss->CurrentPhase = ReservePhase;
}

//ギミック処理関数
void BossPhase::execute(BoxMan* boss, FloorCube* floor[]) {

    if (floor == nullptr || boss == nullptr)
        return;

    // BGM再生
    if (PhaseBGM != nullptr && !PhaseBGM->isPlaying())
    {
        PhaseBGM->play(true);
    }

    //フェーズチェンジ処理
    if (DoOnce) {
        if(boss->CurrentPhase != EnemyBase::PHASE_1){
            CurrentgimmickName = L"ChangePHASE";
            isGimmick = false;
            DoOnce = false;
		}
    }

    // ギミックを選ぶ
    if (isGimmick)
    {
        if (!phaseGimmicks.empty())
        {


			// ギミック数を基準にランダムで選ぶ
            int index = rand() % static_cast<int>(phaseGimmicks.size());

            // TEXTから選ばれたギミック名
            CurrentgimmickName = phaseGimmicks[index];

            // ギミックリストから探す
            auto it = pTaskManager->gimmickList.find(CurrentgimmickName);

            // 見つかった場合
            if (it != pTaskManager->gimmickList.end())
            {
                it->second->initialize();
                
                // ギミック実行中
                isGimmick = false;
            }
        }
    }
    else // ギミックを実行
    {
        auto it = pTaskManager->gimmickList.find(CurrentgimmickName);

        if (it != pTaskManager->gimmickList.end())
        {
            //===ギミック処理区間===

			isGimmick = it->second->execute(boss, floor);// ギミックが完了したらtrueを返す
            for (int i = 0; i < 64; i++) { //FloorCubeの数回す
                floor[i]->execute();
            }
        }
    }
}

//textからフェーズの情報を読み込む関数
void BossPhase::LoadPhaseData(const wchar_t* filePath)
{
    isGimmick = true;

    // 前のフェーズのギミック情報を消す
    phaseGimmicks.clear();

    // 前のギミック名を消す
    CurrentgimmickName.clear();


	std::wifstream file(filePath);  //全文

    if (!file) {
        MessageBox(NULL, L"PhaseFile読み込みエラー", L"エラー", MB_OK | MB_ICONERROR);
		return;
    }

    std::wstring line;              //一行

	// ファイルを一行ずつ読み込む
    while (std::getline(file, line))
    {
        // SkyModel
        if (line.find(L"SkyModel{") == 0)
        {
            size_t start = line.find(L'{') + 1;
            size_t end = line.find(L'}');
            std::wstring modelName = line.substr(start, end - start);

            // 前のモデルを解放(nullptrなら何もしない)
            if (pPhase_skyModel != nullptr) {
                vnMainFrame::getSceneInstance()->deleteObject(pPhase_skyModel);
                pPhase_skyModel = nullptr;
            }

            // 空なら表示しない: 新しいモデルは作らず、次の行へ
            if (modelName.empty()) {
                pPhase_skyModel->setRenderEnable(false);
                continue;
            }

            pPhase_skyModel = new vnModel(L"data/model/", modelName.c_str());
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
            

            // 前のBGMを止めて解放(nullptrなら何もしない)
            if (PhaseBGM != nullptr) {
                PhaseBGM->stop();
                delete PhaseBGM;
                PhaseBGM = nullptr;
            }

            // 空なら無音: 新しいBGMは作らず、次の行へ
            if (bgmName.empty()) {
                continue;
            }

            std::wstring bgmPath = L"data/sound/BGM/" + bgmName;
            PhaseBGM = new vnSound(bgmPath.c_str());
        }

		// Gimmick
        if (line.find(L"Gimmick{") == 0)
        {
            // 前のフェーズの設定を消す
            phaseGimmicks.clear();

            // Gimmick{ の次の行から読み込む
            while (std::getline(file, line))
            {
                // Gimmickの終了
                if (line.find(L"}") == 0)
                {
                    break;
                }

                // { と } を探す
                size_t start = line.find(L'{');
                size_t end = line.find(L'}');

                if (start == std::wstring::npos ||
                    end == std::wstring::npos)
                {
                    continue;
                }

                // [0]{FallEWNS} の { } の中身
                std::wstring gimmickName =
                    line.substr(start + 1, end - start - 1);

                // 空でなければ追加
                if (!gimmickName.empty())
                {
                    phaseGimmicks.push_back(gimmickName);
                }
            }
        }
    }
}
