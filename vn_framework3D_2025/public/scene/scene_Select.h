//--------------------------------------------------------------//
//	"scene_null.h"												//
//		テスト用シーン											//
//--------------------------------------------------------------//

#pragma once
class UI_Button : public vnSprite
{
public:
	UI_Button(float x, float y, float width, float height, const WCHAR* texture_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);

	void execute();
	
	bool isMouseOver();
	
};


class SceneSelect : public vnScene
{
private:
	UI_Button* NormalButton;
	UI_Button* HardButton;
	vnSprite* NormalImage;
	vnSprite* HardImage;
	

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

