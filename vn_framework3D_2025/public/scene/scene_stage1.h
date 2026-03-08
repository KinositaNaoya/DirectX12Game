#pragma once


class SceneSTAGE_1 : public vnScene
{
private:
	UI_Manager* Manager;

	Player* pPlayer;
	vnModel* pShadow;

	BoseEnemy1* pEnemy;

	FloorCube* pFloor[Collid_Max_NUM];
	AreaCube* StandbyArea;//このエリア内にいると敵の行動すべてが止まる
	
	int pCubeNum = 0;	//fieldcube用の変数
	bool Debug = false;	//デバッグを表示するかどうか
	bool StageClear;	//ステージをクリアしたかどうか


public:

	//初期化
	bool initialize();
	//終了
	void terminate();

	//処理
	void execute();
	//描画
	void render();
};