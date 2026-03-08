#include "../framework.h"
#include "../framework/vn_environment.h"

//初期化
UI_Manager::UI_Manager()
{
	IDWriteTextFormat* bigFont = vnFont::create(L"Meiryo", 40);
	vnFont::setTextFormat(bigFont);
	

	// フェイドアウトorインUI生成
	FadeImage = new vnSprite(
		vnMainFrame::screenCenterX,
		vnMainFrame::screenCenterY,
		vnMainFrame::screenWidth,
		vnMainFrame::screenHeight,
		L"data/image/Black.png");


	switch (getSceneTable())
	{	
	case TITLE:
		FadeImage->setAlpha(0.0f);

		// スタートボタン
		Button = new vnSprite(
			vnMainFrame::screenCenterX,
			vnMainFrame::screenCenterY + 250.0f,
			516,
			46,
			L"data/image/STARTButton.png");
		vnMainFrame::getSceneInstance()->registerObject(Button);
			
		break;
	case STAGE_1:

		Timeback = new vnSprite(
			vnMainFrame::screenCenterX,
			0.0f,
			550,
			150,
			L"data/image/Black.png");
		Timeback->setAlpha(0.5f);
		vnMainFrame::getSceneInstance()->registerObject(Timeback);

		//タイムバー
		TimeBer = new vnSprite(
			vnMainFrame::screenCenterX,
			vnMainFrame::screenCenterY - 300.0f,
			516,
			23,
			L"data/image/TimeBar.png",
			0.0f,0.5f);
		vnMainFrame::getSceneInstance()->registerObject(TimeBer);

		//タイムバーのフレーム
		TimeBerFrame = new vnSprite(
			vnMainFrame::screenCenterX,
			vnMainFrame::screenCenterY - 300.0f,
			516,
			23,
			L"data/image/TimeFrame.png");
		vnMainFrame::getSceneInstance()->registerObject(TimeBerFrame);



		//チュートリアル表示用
		back = new vnSprite(
			64.0f * 3,
			vnMainFrame::screenCenterY / 2,
			64.0f * 6,
			64.0f * 3,
			L"data/image/Black.png");
		back->setAlpha(0.5f);
		Key_W = new vnSprite(
			-64.0f,
			-32.0f,
			64.0f,
			64.0f,
			L"data/image/Key_W.png");
		Key_A = new vnSprite(
			-64.0f * 2,
			32.0f,
			64.0f,
			64.0f,
			L"data/image/Key_A.png");
		Key_S = new vnSprite(
			-64.0f,
			32.0f,
			64.0f,
			64.0f,
			L"data/image/Key_S.png");
		Key_D = new vnSprite(
			0.0f,
			32.0f,
			64.0f,
			64.0f,
			L"data/image/Key_D.png");
		Mouse = new vnSprite(
			64.0f * 2,
			0.0f,
			64.0f * 2,
			64.0f * 2,
			L"data/image/mouse.png");

		Key_W->setParent(back);
		Key_A->setParent(back);
		Key_S->setParent(back);
		Key_D->setParent(back);
		Mouse->setParent(back);

		vnMainFrame::getSceneInstance()->registerObject(back);
		vnMainFrame::getSceneInstance()->registerObject(Key_W);
		vnMainFrame::getSceneInstance()->registerObject(Key_A);
		vnMainFrame::getSceneInstance()->registerObject(Key_S);
		vnMainFrame::getSceneInstance()->registerObject(Key_D);
		vnMainFrame::getSceneInstance()->registerObject(Mouse);

		break;
	}

	//一番前に表示したいからレジスターはここ
	vnMainFrame::getSceneInstance()->registerObject(FadeImage);
}

//終了処理
UI_Manager::~UI_Manager()
{

	vnMainFrame::getSceneInstance()->deleteObject(FadeImage);
	
	switch (getSceneTable())
	{
	case TITLE:
		// スタートボタン
		vnMainFrame::getSceneInstance()->deleteObject(Button);

		break;
	case STAGE_1:

		vnMainFrame::getSceneInstance()->deleteObject(Timeback);
		vnMainFrame::getSceneInstance()->deleteObject(TimeBer);
		vnMainFrame::getSceneInstance()->deleteObject(TimeBerFrame);
		vnMainFrame::getSceneInstance()->deleteObject(back);
		vnMainFrame::getSceneInstance()->deleteObject(Key_W);
		vnMainFrame::getSceneInstance()->deleteObject(Key_A);
		vnMainFrame::getSceneInstance()->deleteObject(Key_S);
		vnMainFrame::getSceneInstance()->deleteObject(Key_D);
		vnMainFrame::getSceneInstance()->deleteObject(Mouse);

		break;
	}
}


bool UI_Manager::FadeOUT(float speed)
{
	if (FadeImage == NULL)return false;

	if (FadeImage->getAlpha() <= 0.0f)return true;

	FadeImage->addAlpha(-speed);

	return false;
}

bool UI_Manager::FadeIN(float speed)
{
	if (FadeImage == NULL)return false;

	if (FadeImage->getAlpha() >= 1.0f)return true;

	FadeImage->addAlpha(speed);

	return false;
}


void UI_Manager::setButtonActive(bool on)
{
	if (Button == NULL)return;

	if (!on) {
		Button->setAlpha(0.0f);
		return;
	}

	static float timer = 1.0f;
	timer += 0.016f;

	Button->setAlpha((int)timer % 2);
}

//
void UI_Manager::PhaseTimerUI(EnemyBase* enemy)
{

	float rate = enemy->getEnemyPhaseRate();

	float startU = 0.0f + 0.5f * rate;
	float endU = 0.5f + 0.5f * rate;

	TimeBer->setUVRange(startU, endU);
	TimeBer->setAlpha(1.0f);
	TimeBerFrame->setAlpha(1.0f);

	EnemyBase::PHASETABLE Phase = enemy->getPhase();
	vnFont::setColor(0xffc0c0c0);
	switch (Phase)
	{
	case EnemyBase::STANDBY:
		TimeBer->setAlpha(0.0f);
		TimeBerFrame->setAlpha(0.0f);
		vnFont::print(530.0f, 10.0f, L"STANDBY");
		break;
	case EnemyBase::PHASE_1:
		vnFont::print(400.0f, 0.0f, L"PHASE 1");
		break;
	case EnemyBase::PHASE_2:
		vnFont::print(400.0f, 0.0f, L"PHASE 2");
		break;
	case EnemyBase::PHASE_3:
		vnFont::print(400.0f, 0.0f, L"PHASE 3");
		break;
	case EnemyBase::PHASE_4:
		vnFont::print(400.0f, 0.0f, L"PHASE 4");
		break;
	case EnemyBase::FINAL:
		vnFont::print(400.0f, 0.0f, L"FINAL");
		break;
	case EnemyBase::GAMESET:
		break;
	}
	vnFont::setColor(0xffffffff);
	
}

void UI_Manager::ShowSafeAreaUI(EnemyBase* enemy)
{
	if (enemy->getPhase() == EnemyBase::STANDBY)return;
	if(back->posX >= -200.0f)back->posX -= 2.0f;
	
}


