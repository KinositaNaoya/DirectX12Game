#include "../../framework.h"
#include "../../framework/vn_environment.h"


bool SceneSTAGE_1::initialize()
{
    Debug = false;
    StageClear = false;

    ShowCursor(false);

    //エネミーの生成処理
    pEnemy = new BoseEnemy1(L"data/model/Enemy1/", L"HumanF_Model.bone");
    registerObject(pEnemy);
    for (int i = 0; i < pEnemy->getPartsNum(); i++) {
        registerObject(pEnemy->getParts(i));
        pEnemy->getParts(i)->setLighting(false);
    }

    //プレイヤーの生成処理
    pPlayer = new Player(L"data/model/Player/", L"HumanF_Model.bone");
    registerObject(pPlayer);
    for (int i = 0; i < pPlayer->getPartsNum(); i++) {
        registerObject(pPlayer->getParts(i));
    }

    //
    pShadow = new vnModel(L"data/model/Player/", L"shadow.vnm");
    pShadow->setTransparent(true);
    pShadow->setPositionY(-2.4f);

    //ステージの床の生成処理
    int pCubeNum = 0;
    float CubeSize = 5.0f;
    int FieldNum = 8;
    float FieldOffsetX = 17.5f;
    float FieldOffsetZ = 17.5f;
    for (int i = 0; i < FieldNum; i++) {
        for (int j = 0; j < FieldNum; j++) {
            pFloor[pCubeNum++] = CreateFloorCube(
                (float)j * CubeSize - FieldOffsetX,
                -5.0f,
                (float)i * CubeSize - FieldOffsetZ,
                CubeSize,
                CubeSize,
                CubeSize);
        }
    }
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


    //pCubeのregister処理
    for (int i = 0; i < Collid_Max_NUM; i++) {
        if (pFloor[i] != NULL) {
            registerObject(pFloor[i]);
        }
    }


    StandbyArea = CreateStandbyArea(0.0f, -2.5f, -15.0f, 5.0f, 4.0f, 5.0f);
    registerObject(StandbyArea);
    registerObject(pShadow);

    Manager = new UI_Manager();

    //pEnemy->setSkipPhase(EnemyBase::PHASE_2);

    return true;
}

//終了処理
void SceneSTAGE_1::terminate()
{
    delete(Manager);

    //エネミー
    for (int i = 0; i < pEnemy->getPartsNum(); i++)
    {
        deleteObject(pEnemy->getParts(i));
    }
    deleteObject(pEnemy);	//パーツを削除してから本体を削除

    //プレイヤー
    for (int i = 0; i < pPlayer->getPartsNum(); i++)
    {
        deleteObject(pPlayer->getParts(i));
    }
    deleteObject(pPlayer);	//パーツを削除してから本体を削除
    deleteObject(pShadow);
    
    //フロア
    for (int i = 0; i < Collid_Max_NUM; i++) {
        if (pFloor[i] != NULL)deleteObject(pFloor[i]);
    }

    deleteObject(StandbyArea);
}


void SceneSTAGE_1::execute()
{
    pShadow->setPositionX(pPlayer->getPositionX());
    pShadow->setPositionZ(pPlayer->getPositionZ());


    
    //敵が死んでいたらプレイヤーの操作を切る
    if (!StageClear)pPlayer->execute();
   
    pEnemy->execute();
    float dx = pPlayer->getPositionX() - pEnemy->getPositionX();
    float dz = pPlayer->getPositionZ() - pEnemy->getPositionZ();
    float angle = atan2f(dx, dz);
    pEnemy->setRotationY(angle);

    //物理判定
    {
        for (int i = 0; i < Collid_Max_NUM; i++) {
            if (pFloor[i] == NULL)break;
            using namespace Collider;
            //接触判定処理
            eDirection dir = PushBackOnOverlap(
                pPlayer->getCubeCollider(),
                pFloor[i]->getCubeCollider());

            if (dir == eDirection::Y_Pos) {
                //上に乗った
                pPlayer->setVelocity(0.0f, 0.0f, 0.0f);
                pPlayer->setAir(false);
            }
            else if (dir == eDirection::Y_Neg) {
                //下からぶつかった
                pPlayer->setVelocity(0.0f, 0.0f, 0.0f);
            }
        }
        //プレイヤーとエネミー
        PushBackOnOverlap(
            pPlayer->getCubeCollider(),
            pEnemy->getCubeCollider());
    }

    //エリア内にいるときはここから先コードは通せない...
    if (Collider::OverlapCube(pPlayer->getCubeCollider(),StandbyArea->getCubeCollider()))return;

    Manager->ShowSafeAreaUI(pEnemy);

    StandbyArea->setPositionY(-500.0f);


    pEnemy->TimeUpdate();
    
    //ボスの状態遷移
    switch (pEnemy->getPhase()){
    case BoseEnemy1::PHASETABLE::PHASE_1:
        pEnemy->Phase_1_exe(pFloor);//エネミーのフェーズ処理
        break;

    case BoseEnemy1::PHASETABLE::PHASE_2:
        pEnemy->Phase_2_exe(pFloor);//エネミーのフェーズ処理
        break;

    case BoseEnemy1::PHASETABLE::PHASE_3:
        pEnemy->Phase_3_exe(pFloor);//エネミーのフェーズ処理
        break;

    case BoseEnemy1::PHASETABLE::PHASE_4:
        pEnemy->Phase_4_exe(pFloor);//エネミーのフェーズ処理
        break;

    case BoseEnemy1::PHASETABLE::FINAL:
        pEnemy->FINAL_exe(pFloor);//エネミーのフェーズ処理
        break;

    case BoseEnemy1::PHASETABLE::GAMESET:
        StageClear = true;

        if (pEnemy->GameSet_exe()) {
            switchScene(CLEAR);
        }
        break;
    }
 
    //死んだときの処理
    if (pPlayer->getIsDead()) {
        if (Manager->FadeIN(0.05f)) {
            //ステージ初期化
            this->terminate();
            this->initialize();
        }
    }
}

void SceneSTAGE_1::render()
{
    vnScene::render();
    


    //フェイドアウト処理
    if (!pPlayer->getIsDead()) {
        Manager->FadeOUT(0.005f);
    }

    Manager->PhaseTimerUI(pEnemy);
    


    {//デバッグスペース
        if (vnKeyboard::trg(DIK_0))Debug = !Debug;

        

        int line = 0;
        int intervalY = 16;
        //vnFont::print(10.0f, line++ * intervalY, L"SwitchDebug  :DIK_0");


        if (!Debug)return;
        Collider::DrawAABB(pPlayer->getCubeCollider());//判定表示
        Collider::DrawAABB(pEnemy->getCubeCollider());//判定表示
        Collider::DrawAABB(StandbyArea->getCubeCollider());
        for (int i = 0; i < Collid_Max_NUM; i++) {
            if (pFloor[i] != NULL) {
                Collider::DrawAABB(pFloor[i]->getCubeCollider());
            }
        }

        //vnFont::print(10.0f, line++ * intervalY, L"%02d:%02d", (int)pEnemy->getMinutes(), (int)pEnemy->getScond());
        vnFont::print(10.0f, line++ * intervalY, L"%d/60s",(int)pEnemy->getEnemyTimer() % 60);
        vnFont::print(10.0f, line++ * intervalY, L"%02d/秒", (int)pEnemy->getEnemyTimer());
        vnFont::print(10.0f, line++ * intervalY, L"SwitchLevel  :DIK_6");
        vnFont::print(10.0f, line++ * intervalY, L"ColliderX         :%f", XMVectorGetX(*pPlayer->getPosition()));
        vnFont::print(10.0f, line++ * intervalY, L"ColliderY         :%f", XMVectorGetY(*pPlayer->getPosition()));
        vnFont::print(10.0f, line++ * intervalY, L"ColliderZ         :%f", XMVectorGetZ(*pPlayer->getPosition()));
        vnFont::print(10.0f, line++ * intervalY, L"EnemyX         :%f", pEnemy->getParts(0)->getPositionX());
        vnFont::print(10.0f, line++ * intervalY, L"EnemyY         :%f", pEnemy->getParts(0)->getPositionY());
        vnFont::print(10.0f, line++ * intervalY, L"EnemyZ         :%f", pEnemy->getParts(0)->getPositionZ());
        
    }

    
}