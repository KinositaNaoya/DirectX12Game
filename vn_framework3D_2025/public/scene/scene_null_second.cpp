//--------------------------------------------------------------//
//	"scene_null.cpp"											//
//		テスト用シーン(scene移行のテストくらいにしか使わない...)//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

vnMotionData* motion_idle;
vnMotionData* motion_walk;

bool SceneNullTest2::initialize()
{
    pModel = new vnModel(L"data/model/", L"skydome.vnm");
	pModel->setLighting(false);
	pModel->setScale(1.0f, 1.0f, 1.0f);
	pModel->setPosition(1.0f, 0.0f, 0.0f);
    pModel2 = new vnModel(L"data/model/", L"T_skyCube_half_size.vnm");
    pModel2->setDoubleSided(true);
	pModel2->setLighting(false);
    pModel2->setScale(1.0f, 1.0f, 1.0f);
	pModel2->setPosition(0.0f, 0.0f, 0.0f);

    Debug = true;
    vnCamera::setPosition(vnMainFrame::getInitPosition());
    /*pSky = new vnModel(L"data/model/", L"skydome.vnm");
    pSky->setLighting(false);*/

    //registerObject(pSky);
    registerObject(pModel);
    registerObject(pModel2);




    return true;
}

void SceneNullTest2::terminate()
{
    //deleteObject(pSky);
    deleteObject(pModel);
    deleteObject(pModel2);


}

void SceneNullTest2::execute()
{
    vnCamera::FreeCamCtrl();

    //scene移行テスト用
    if (vnKeyboard::trg(DIK_6)) {
        switchScene(Null, XMVectorSet(20.0f, 10.0f, 20.0f, 0.0f));
    }

    

    vnDebugDraw::Grid(100);
    vnDebugDraw::Axis();

    vnScene::execute();

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
    }
}

void SceneNullTest2::render()
{
    vnScene::render();
}