//--------------------------------------------------------------//
//	"scene_null.h"												//
//		テスト用シーン											//
//--------------------------------------------------------------//

#pragma once


class SceneGameClear : public vnScene
{
private:
	vnSound* ClearBGM;
	vnSound* NoiseSE;

	vnCharacter* pPlayer;
	vnModel* pSky;
	vnModel* pCube;
	vnSprite* font;
	vnSprite* Button;

	vnMotionData* motion_dance1;
	vnMotionData* motion_dance2;
	vnMotionData* motion_dance3;
	vnMotionData* motion_dance4;
	vnMotionData* motion_dance5;
	vnMotionData* motion_dance6;

	bool isSwitch;

public:
	//初期化
	bool initialize();
	//終了
	void terminate();

	//処理
	void execute();
	


};