//--------------------------------------------------------------//
//	"scene_null.h"												//
//		テスト用シーン											//
//--------------------------------------------------------------//

#pragma once


class SceneNullTest2 : public vnScene
{
private:

	vnModel* pSky;		//空
	bool Debug = false;

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