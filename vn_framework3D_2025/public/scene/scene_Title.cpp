#include "../../framework.h"
#include "../../framework/vn_environment.h"




bool SceneTitle::initialize()
{
    


    TitleBGM = new vnSound(L"data/sound/BGM/アンドロイドの涙.wav");
    TitleBGM->play(true);
    TitleBGM->setVolume(2.0f);
    
    ButtonSE = new vnSound(L"data/sound/SE/ゲームのボタン音.wav");
    FadeOutSE = new vnSound(L"data/sound/SE/サウンドロゴ01_1.wav");

    


    //タイトル画面オブジェクト生成
    TitelObject = new vnBillboard(
        23.0f, 
        13.0f,
        L"data/image/Title.png");
    registerObject(TitelObject);

    //キューブ生成
    Cube = new vnModel(L"data/model/primitive/", L"cube.vnm");
    Cube->setPositionZ(-5.0f);
    Cube->setDoubleSided(true);
    registerObject(Cube);

    //カメラセッティング
    vnCamera::setPosition(0.0f,0.0f,-11.0f);
    vnCamera::setTarget(TitelObject->getPosition());
    vnCamera::setFovY(1.0f);

    addVel = 0.0f;
    Manager = new UI_Manager();
    return true;
}

void SceneTitle::terminate()
{
    deleteObject(TitelObject);
    deleteObject(Cube);
    delete(TitleBGM);
    delete(ButtonSE);
    delete(FadeOutSE);
    delete(Manager);
}

void SceneTitle::execute()
{


    //とりあえずスペースでscene移行
    if (vnKeyboard::trg(DIK_SPACE)){
        if(!ButtonSE->isPlaying()) ButtonSE->play(true);
        TitleBGM->stop();

        Cube->setLighting(false);
        Cube->setDiffuse(0.1f,0.1f,0.1f,1.0f);
        isStart = true;
    }
    
    
    if (!TitleBGM->isPlaying() && !isStart)TitleBGM->play(true);

    if (!isStart)TitleGimmick();
}

void SceneTitle::render()
{
    Manager->setButtonActive(!isStart);

    if (isStart) {
        if (ButtonSE->isStopped() && !FadeOutSE->isPlaying()) {
            FadeOutSE->play(true);
        }


        //scene移行実行時の処理
        Cube->setLighting(false);
        addVel += 0.0002f;
        vnCamera::addPositionZ(addVel);
        TitelObject->addPositionZ(addVel * 2);


        if (Manager->FadeIN(addVel * 0.2)) {
            switchScene(SELECT);
        }
    }

    vnScene::render();

}

//タイトルギミックの処理関数
void SceneTitle::TitleGimmick()
{
    // 1. マウス座標取得

    float mx = (float)vnMouse::getX();//マウスのX座標
    float my = (float)vnMouse::getY();//マウスのY座標
    
    // 現在のクライアントサイズを取得

    RECT rc;
    GetClientRect(vnMainFrame::getHWND(), &rc);
    float width = (float)(rc.right - rc.left);  //画面の幅
    float height = (float)(rc.bottom - rc.top); //画面の縦
    
    // 行列取得
    XMMATRIX view = *vnCamera::getView();       //View行列
    XMMATRIX projection = *vnCamera::getProj(); //Proj行列
    XMMATRIX identity = XMMatrixIdentity();     //null行列
    
    // カメラから見て一番近いマウスのポイント
    XMVECTOR nearPoint = XMVector3Unproject(
        XMVectorSet(mx, my, 0.0f, 1.0f),
        0.0f, 0.0f,
        width, height,
        0.0f, 1.0f,
        projection,
        view,
        identity);
    
    // カメラから見て一番遠いマウスのポイント
    XMVECTOR farPoint = XMVector3Unproject(
        XMVectorSet(mx, my, 1.0f, 1.0f),
        0.0f, 0.0f,
        width, height,
        0.0f, 1.0f,
        projection,
        view,
        identity);
    
    // レイ生成
    XMVECTOR RayPos = nearPoint;
    XMVECTOR RayDir = XMVector3Normalize(farPoint - nearPoint);
    
    // ローカル空間へ変換
    XMMATRIX world = *Cube->getWorldMatrix();
    XMMATRIX invWorld = XMMatrixInverse(nullptr, world);
    
    XMVECTOR localRayPos = XMVector3TransformCoord(RayPos, invWorld);
    XMVECTOR localRayDir = XMVector3TransformNormal(RayDir, invWorld);
    localRayDir = XMVector3Normalize(localRayDir);
    
    // Ray vs AABB（Slab法）
    float half = 1.0f;
    float tmin = 0.0f;
    float tmax = 10000.0f;
    
    bool hit = true;
    
    for (int i = 0; i < 3; i++)
    {
        float rayPos = XMVectorGetByIndex(localRayPos, i);
        float rayDir = XMVectorGetByIndex(localRayDir, i);
    
        float boxMin = -half;
        float boxMax = half;
    
        if (fabs(rayDir) > 0.0001f)
        {
            float ood = 1.0f / rayDir;
            float t1 = (boxMin - rayPos) * ood;
            float t2 = (boxMax - rayPos) * ood;
    
            if (t1 > t2) std::swap(t1, t2);
    
            tmin = max(tmin, t1);
            tmax = min(tmax, t2);
    
            if (tmin > tmax)
            {
                hit = false;
                break;
            }
        }
    }
    
    // 回転処理
    
    static float velocityX = 0.0f;
    static float velocityY = 0.0f;
    
    static float prevMouseX = mx;
    static float prevMouseY = my;
    static float autoTime = 0.0f;
    
    float deltaX = mx - prevMouseX;
    float deltaY = my - prevMouseY;
    
    prevMouseX = mx;
    prevMouseY = my;
    
    float sensitivity = -0.0025f;
    float damping = 0.95f;
    
    if (hit && vnMouse::onL())
    {
        velocityX += deltaY * sensitivity;
        velocityY += deltaX * sensitivity;
    }
    else
    {
        autoTime += 0.016f;
        float base = 0.01f;
    
        float rx = base + sinf(autoTime * 1.3f) * 0.005f;
        float ry = base + sinf(autoTime * 0.7f) * 0.004f;
        float rz = base + sinf(autoTime * 1.9f) * 0.006f;
    
        Cube->addRotation(rx, ry, rz);
    }
    
    Cube->addRotation(velocityX, velocityY, 0.0f);
    
    velocityX *= damping;
    velocityY *= damping;
}