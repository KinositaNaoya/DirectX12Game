#pragma once


class SceneTitle : public vnScene
{
public:
	//‰Šú‰»
	bool initialize();
	//I—¹
	void terminate();

	//ˆ—
	void execute();
	//•`‰æ
	void render();

private:

	vnSound* TitleBGM;
	vnSound* ButtonSE;
	vnSound* FadeOutSE;

	

	UI_Manager* Manager;


	vnModel* Cube;
	vnBillboard* TitelObject;
	bool isStart = false;

	float addVel;

	void TitleGimmick();




};