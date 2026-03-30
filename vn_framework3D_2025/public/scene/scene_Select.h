#pragma once


class SceneSelect : public vnScene
{
private:
	UI_Button* NormalButton;
	UI_Button* HardButton;
	vnSprite* NormalImage;
	vnSprite* HardImage;
	

public:


	//‰Šú‰»
	bool initialize();
	//I—¹
	void terminate();

	//ˆ—
	void execute();
	//•`‰æ
	void render();


};

