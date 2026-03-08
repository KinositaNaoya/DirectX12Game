//--------------------------------------------------------------//
//	"scene_null.cpp"											//
//		テスト用シーン											//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"




bool SceneSelect::initialize()
{
    ShowCursor(true);

    NormalImage = new vnSprite(
        vnMainFrame::screenCenterX,
        vnMainFrame::screenCenterY,
        vnMainFrame::screenWidth,
        vnMainFrame::screenHeight,
        L"data/image/NormalImage.png");
    registerObject(NormalImage);

    HardImage = new vnSprite(
        vnMainFrame::screenCenterX,
        vnMainFrame::screenCenterY,
        vnMainFrame::screenWidth,
        vnMainFrame::screenHeight,
        L"data/image/HardImage.png");
    registerObject(HardImage);
    HardImage->setAlpha(0.0f);
    
    NormalButton = new UI_Button(
        vnMainFrame::screenCenterX - 200.0f,
        vnMainFrame::screenCenterY + 250.0f,
        200,
        50,
        L"data/image/NormalButton.png");
    registerObject(NormalButton);

    HardButton = new UI_Button(
        vnMainFrame::screenCenterX + 200.0f,
        vnMainFrame::screenCenterY + 250.0f,
        200,
        50,
        L"data/image/HardButton.png");
    registerObject(HardButton);
    return true;
}

void SceneSelect::terminate()
{
    deleteObject(NormalImage);
    deleteObject(HardImage);
    deleteObject(NormalButton);
    deleteObject(HardButton);
}

void SceneSelect::execute()
{
    NormalButton->execute();
    HardButton->execute();

    static float normalAlpha = 0.0f;
    static float hardAlpha = 0.0f;

    float normalTarget = NormalButton->isMouseOver() ? 1.0f : 0.0f;
    float hardTarget = HardButton->isMouseOver() ? 1.0f : 0.0f;

    normalAlpha += (normalTarget - normalAlpha) * 0.15f;
    hardAlpha += (hardTarget - hardAlpha) * 0.15f;

    NormalImage->setAlpha(normalAlpha);
    HardImage->setAlpha(hardAlpha);

    if (NormalButton->isMouseOver() && vnMouse::trg(vnMouse::LT))
    {
        ShowCursor(false);
        vnMainFrame::setGameLevel(true);
        switchScene(STAGE_1);
    }

    if (HardButton->isMouseOver() && vnMouse::trg(vnMouse::LT))
    {
        ShowCursor(false);
        vnMainFrame::setGameLevel(false);
        switchScene(STAGE_1);
    }
    
}

void SceneSelect::render()
{
    vnScene::render();
}



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