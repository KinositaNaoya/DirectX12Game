//--------------------------------------------------------------//
//	"scene_table.cpp"											//
//		シーンテーブル											//
//													2025/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

//起動時のシーン
eSceneTable initialScene = eSceneTable::TITLE;
//現在のシーン
eSceneTable currentScene = (eSceneTable)-1;
//切り替え予約のシーン
eSceneTable reserveScene = initialScene;


//各シーンの名前
WCHAR SceneName[(int)eSceneTable::SceneMax][32] =
{
	L"Boot",
	L"null",
	L"null2",
	L"TITLE",
	L"SELECT",
	L"STAGE1",
	L"CREAR"
};

//ステージ切り替え関数(予約)
void switchScene(eSceneTable scene)
{
	reserveScene = scene;
}

//シーン切り替え関数(予約 + 初期値指定)
void switchScene(eSceneTable scene, XMVECTOR initPos)
{
	reserveScene = scene;
	vnMainFrame::setInitPostion(&initPos);
}

//現在のステージ取得(実処理)
void switchScene()
 {
	if(currentScene==reserveScene)
	{
		return;
	}
	
	vnMainFrame::terminateScene();
	
	vnScene *pScene=NULL;
	switch(reserveScene)
	{
	case Boot:			pScene = new SceneBoot();			break;
	case Null:			pScene = new SceneNullTest();		break;
	case Null2:			pScene = new SceneNullTest2();		break;
	case TITLE:			pScene = new SceneTitle();			break;
	case SELECT:		pScene = new SceneSelect();			break;
	case STAGE_1:		pScene = new SceneSTAGE_1();		break;
	case CLEAR:			pScene = new SceneGameClear();		break;
	}
	
	vnMainFrame::initializeScene(pScene);
	
	currentScene = reserveScene;
}

eSceneTable getSceneTable()
{
	return reserveScene;
}
