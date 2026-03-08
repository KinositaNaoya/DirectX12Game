//--------------------------------------------------------------//
//	"vn_mainframe.h"											//
//		メインフレーム											//
//													2025/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnMainFrame
{
private:
	static HWND hWnd;
	static vnScene* pScene;

	//scene共有変数
	static XMVECTOR InitPos;	//初期地点
	static bool isNormalLevel;


public:

	//何処でもSceneの情報を持ってこれるようにする関数
	static vnScene* getSceneInstance();
	static HWND getHWND();

	//初期地点をセット
	static void setInitPostion(const XMVECTOR *pos);

	//初期地点を取得
	static const XMVECTOR* getInitPosition();

	//ゲームの難易度関連

	static const void setGameLevel(bool b);
	static const bool getGameLevel();

	static int screenWidth;		//画面の幅
	static int screenHeight;	//画面の高さ
	static int screenCenterX;	//画面の中央(横)
	static int screenCenterY;	//画面の中央(縦)

	static float clearColor[4];	//画面のクリアカラー(RGBA:0.0～1.0)

	static void initializeScene(vnScene* p);	//シーンの初期化
	static void terminateScene();				//シーンの終了

	//初期化
	static bool initialize(void* handle, void* hInst, int width, int height);

	//終了
	static void terminate();

	//実行
	static void execute();
};
