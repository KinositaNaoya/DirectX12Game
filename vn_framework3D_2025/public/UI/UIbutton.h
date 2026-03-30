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