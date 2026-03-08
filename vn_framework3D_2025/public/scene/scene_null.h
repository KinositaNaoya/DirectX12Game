//--------------------------------------------------------------//
//	"scene_null.h"												//
//		テスト用シーン											//
//--------------------------------------------------------------//

#pragma once


class SceneNullTest : public vnScene
{
private:

	Player* pPlayer;
	
	bool Debug = false;

	
	vnModel* testEnemy;

public:


	FloorCube* pCube[Collid_Max_NUM];
	int pCubeNum = 0;

	//初期化
	bool initialize();
	//終了
	void terminate();

	//処理
	void execute();
	//描画
	void render();


};