#include "../../framework.h"
#include "../../framework/vn_environment.h"

bool SceneGameClear::initialize()
{
    ClearBGM = new vnSound(L"data/sound/BGM/電子の箱庭の救世主.wav");
    NoiseSE = new vnSound(L"data/sound/SE/グリッチ音02.wav");
    ClearBGM->play(true);
    isSwitch = false;

    //スタートボタン生成
    Button = new vnSprite(
        vnMainFrame::screenCenterX,
        vnMainFrame::screenCenterY + 250.0f,
        522.0f * 2.0f,
        48.0f * 2.0f,
        L"data/image/PuchToSpaceTitle.png");
    registerObject(Button);
    Button->setAlpha(0.0f);

    //プレイヤーの生成処理
    pPlayer = new vnCharacter(L"data/model/Player/", L"HumanF_Model.bone");
    registerObject(pPlayer);
    for (int i = 0; i < pPlayer->getPartsNum(); i++) {
        registerObject(pPlayer->getParts(i));
    }

    

    motion_dance1 = loadMotionFile(L"data/model/Player/motion/HumanF@Dance01 - Loop.mot");
    motion_dance2 = loadMotionFile(L"data/model/Player/motion/HumanF@Dance02 - Loop.mot");
    motion_dance3 = loadMotionFile(L"data/model/Player/motion/HumanF@Dance03 - Loop.mot");
    motion_dance4 = loadMotionFile(L"data/model/Player/motion/HumanF@Dance04 - Loop.mot");
    motion_dance5 = loadMotionFile(L"data/model/Player/motion/HumanF@Dance05 - Loop.mot");
    motion_dance6 = loadMotionFile(L"data/model/Player/motion/HumanF@Dance06 - Loop.mot");

    srand((unsigned int)time(nullptr));
    int randMotion = rand() % 6;
    switch (randMotion)
    {
    case 0: pPlayer->setMotion(motion_dance1); break;
    case 1: pPlayer->setMotion(motion_dance2); break;
    case 2: pPlayer->setMotion(motion_dance3); break;
    case 3: pPlayer->setMotion(motion_dance4); break;
    case 4: pPlayer->setMotion(motion_dance5); break;
    case 5: pPlayer->setMotion(motion_dance6); break;
    }

    //スカイドーム生成処理
    pSky = new vnModel(L"data/model/", L"skydome.vnm");
    pSky->setLighting(false);
    registerObject(pSky);
    pSky->setRotationX(5.0f);

    pCube = new vnModel(L"data/model/primitive/", L"cube.vnm");
    pCube->addPositionY(-1.0f);
    registerObject(pCube);

    XMVECTOR target = XMVectorAdd(*pPlayer->getPosition(), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

    vnCamera::setTarget(&target);

    //kuria
    font = new vnSprite(
        vnMainFrame::screenCenterX,
        vnMainFrame::screenCenterY - 200,
        537.0f*2,
        80.0f*2,
        L"data/image/GAMECLEAR.png");
    font->setAlpha(0.0f);
    registerObject(font);

    return true;
}

void SceneGameClear::terminate()
{

    delete(ClearBGM);
    delete(NoiseSE);
    deleteObject(Button);

    deleteMotionFile(motion_dance1);
    deleteMotionFile(motion_dance2);
    deleteMotionFile(motion_dance3);
    deleteMotionFile(motion_dance4);
    deleteMotionFile(motion_dance5);
    deleteMotionFile(motion_dance6);

    //プレイヤー
    for (int i = 0; i < pPlayer->getPartsNum(); i++)
    {
        deleteObject(pPlayer->getParts(i));
    }
    deleteObject(pPlayer);	//パーツを削除してから本体を削除

    deleteObject(pSky);
    deleteObject(pCube);
    deleteObject(font);
}

void SceneGameClear::execute()
{
    static float t = 0.0f;
    static float timer = 0.0f;
    if (!ClearBGM->isPlaying())ClearBGM->play(true);

    pSky->addRotationY(0.0002f);
    if (t < 0.8f) {
        t += 0.01;
    }
    else {
        font->setAlpha(1.0f);
        Button->setAlpha(1.0f);
        
        timer += 0.016f;
        Button->setAlpha((int)timer % 2);
        if (vnKeyboard::trg(DIK_SPACE)) {
            ShowCursor(true);
            switchScene(TITLE);
        }
    }

    XMVECTOR zero = XMVectorSet(10.0f,10.0f,10.0f,1.0f);
    XMVECTOR result = XMVectorLerp(zero, *pPlayer->getPosition(), t);
    vnCamera::setPosition(&result);

    pPlayer->addRotationY(0.005f);

    vnScene::execute();
}

