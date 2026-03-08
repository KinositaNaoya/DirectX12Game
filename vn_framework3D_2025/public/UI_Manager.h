#pragma once

#define SPRITE_MAX_NUM (50)

class UI_Manager : public vnObject
{
private:

	vnSprite* FadeImage = nullptr;
	vnSprite* Button = nullptr;

	vnSprite* TimeBer = nullptr;
	vnSprite* TimeBerFrame = nullptr;
	vnSprite* Timeback = nullptr;

	vnSprite* Key_W = nullptr;
	vnSprite* Key_A = nullptr;
	vnSprite* Key_S = nullptr;
	vnSprite* Key_D = nullptr;
	vnSprite* Mouse = nullptr;
	vnSprite* back = nullptr;

public:
	UI_Manager();
	~UI_Manager();


	bool FadeOUT(float speed);
	bool FadeIN (float speed);

	void setButtonActive(bool on = true);

	void PhaseTimerUI(EnemyBase* enemy);

	void ShowSafeAreaUI(EnemyBase* enemy);


};