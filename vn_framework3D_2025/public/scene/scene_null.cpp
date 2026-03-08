//--------------------------------------------------------------//
//	"scene_null.cpp"											//
//		テスト用シーン											//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"




bool SceneNullTest::initialize()
{

    Debug = true;



    testEnemy = new vnModel(L"data/model/primitive/", L"cube.vnm");
    registerObject(testEnemy);


    pPlayer = new Player(L"data/model/Player/", L"HumanF_Model.bone");
    registerObject(pPlayer);
    for (int i = 0; i < pPlayer->getPartsNum(); i++) {
        registerObject(pPlayer->getParts(i));
    }

    pPlayer->setPositionY(2.0f);
    pPlayer->setPositionZ(5.0f);

    
    int pCubeNum = 0;

    //地面
    pCube[pCubeNum++] = CreateFloorCube(0.0f, 0.125f, 0.0f, 20.0f, 0.5f, 20.0f);

    //障害物1
    pCube[pCubeNum++] = CreateFloorCube(-5.0f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f);

    //障害物2
    pCube[pCubeNum++] = CreateFloorCube(5.0f, 0.0f, 5.0f, 5.0f, 5.0f, 5.0f);
    
    //障害物3
    pCube[pCubeNum++] = CreateFloorCube(5.0f, 2.5f, -5.0f, 5.0f, 5.0f, 5.0f);

    //レジスターとプレイヤーセット
    for (int i = 0; i < Collid_Max_NUM; i++) {
        if (pCube[i] != NULL) {
            registerObject(pCube[i]);
        }
    }

    return true;
}

void SceneNullTest::terminate()
{
    deleteObject(testEnemy);

    for (int i = 0; i < Collid_Max_NUM; i++) {
        if(pCube[i] != NULL )deleteObject(pCube[i]);
    }
    
    for (int i = 0; i < pPlayer->getPartsNum(); i++)
    {
        deleteObject(pPlayer->getParts(i));
    }
    deleteObject(pPlayer);	//パーツを削除してから本体を削除

}

void SceneNullTest::execute()
{
    pPlayer->execute();

    //scene移行テスト用
    if (vnKeyboard::trg(DIK_6)) {
        switchScene(Null2, XMVectorSet(-1.0f, 10.0f, -10.0f, 0.0f));
    }

    

    

    //物理判定
    for (int i = 0; i < Collid_Max_NUM; i++) {
        if (pCube[i] == NULL)break;
        using namespace Collider;
        
        //接触した場合どの方向で接触したか
        eDirection dir = PushBackOnOverlap(pPlayer->getCubeCollider(), pCube[i]->getCubeCollider());

        if (dir == eDirection::Y_Pos)	//上に乗った
        {
            pPlayer->setVelocity(0.0f, 0.0f, 0.0f);
            pPlayer->setAir(false);
        }
        else if (dir == eDirection::Y_Neg)	//下からぶつかった
        {
            pPlayer->setVelocity(0.0f, 0.0f, 0.0f);
        }
    }

    

    pCube[1]->addPositionY(0.01f);
    //vnLight::setLightDir(pPlayer->getPositionX(), pPlayer->getPositionY(), pPlayer->getPositionZ());
}

void SceneNullTest::render()
{
    Collider::DrawAABB(pPlayer->getCubeCollider());//判定表示
    for (int i = 0; i < Collid_Max_NUM; i++) {
        if (pCube[i] != NULL) {
            Collider::DrawAABB(pCube[i]->getCubeCollider());
        }
    }

    {//デバッグスペース
        if (vnKeyboard::trg(DIK_0))Debug = !Debug;

        int line = 0;
        int intervalY = 16;
        vnFont::print(10.0f, line++ * intervalY, L"SwitchDebug  :DIK_0");

        if (!Debug)return;
        vnFont::print(10.0f, line++ * intervalY, L"SwitchLevel  :DIK_6");
        vnFont::print(10.0f, line++ * intervalY, L"CamX         :%f", vnCamera::getPositionX());
        vnFont::print(10.0f, line++ * intervalY, L"CamY         :%f", vnCamera::getPositionY());
        vnFont::print(10.0f, line++ * intervalY, L"CamZ         :%f", vnCamera::getPositionZ());
        vnFont::print(10.0f, line++ * intervalY, L"CamR         :%f", vnCamera::getRadius());
        vnFont::print(10.0f, line++ * intervalY, L"CamT         :%f", vnCamera::getTheta());
        vnFont::print(10.0f, line++ * intervalY, L"CamP         :%f", vnCamera::getPhi());
        vnFont::print(10.0f, line++ * intervalY, L"ColliderX         :%f", XMVectorGetX(*pPlayer->getPosition()));
        vnFont::print(10.0f, line++ * intervalY, L"ColliderY         :%f", XMVectorGetY(*pPlayer->getPosition()));
        vnFont::print(10.0f, line++ * intervalY, L"ColliderZ         :%f", XMVectorGetZ(*pPlayer->getPosition()));
        vnFont::print(10.0f, line++ * intervalY, L"AmbientB         :%f", vnLight::getAmbientB());
        vnFont::print(10.0f, line++ * intervalY, L"AmbientG         :%f", vnLight::getAmbientG());
        vnFont::print(10.0f, line++ * intervalY, L"AmbientR         :%f", vnLight::getAmbientR());
        vnFont::print(10.0f, line++ * intervalY, L"LightDirX         :%f", vnLight::getLightDirX());
        vnFont::print(10.0f, line++ * intervalY, L"LightDirY         :%f", vnLight::getLightDirY());
        vnFont::print(10.0f, line++ * intervalY, L"LightDirZ         :%f", vnLight::getLightDirZ());
        vnFont::print(10.0f, line++ * intervalY, L"LightColR         :%f", vnLight::getLightColR());
        vnFont::print(10.0f, line++ * intervalY, L"LightColG         :%f", vnLight::getLightColG());
        vnFont::print(10.0f, line++ * intervalY, L"LightColB         :%f", vnLight::getLightColB());
        vnFont::print(10.0f, line++ * intervalY, L"aaa         :%f", XMVectorGetZ(*pPlayer->getParts(0)->getPosition()));

    }

    vnScene::render();
}