//--------------------------------------------------------------//
//	"scene_null.cpp"											//
//		テスト用シーン											//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

UI_Button::UI_Button(float x, float y, float width, float height, const WCHAR* texture_file, float left_u, float right_u, float top_v, float bottom_v):vnSprite( x,  y,  width,  height,   texture_file,  left_u,  right_u,  top_v,  bottom_v)
{
}

void UI_Button::execute()
{
    float target = 1.0f;

    if (isMouseOver())
    {
        target = 1.5f;
    }

    scaleX += (target - scaleX) * 0.2f;
    scaleY += (target - scaleY) * 0.2f;
}

//ボタン内にマウスが入っているかどうか
bool UI_Button::isMouseOver()
{
    float mouseX = (float)vnMouse::getX();
    float mouseY = (float)vnMouse::getY();


    return (
        mouseX >= posX - sizeX / 2 &&
        mouseX <= posX + sizeX / 2 &&
        mouseY >= posY - sizeY / 2 &&
        mouseY <= posY + sizeY / 2
        );
}