#include "TStandby.h"

bool Standby::initialize()
{
	mState = STATE_SELECT;
	t = 0.0f;
	y = 0.0f;
	return true;
}

Standby::Standby()
{
}

Standby::~Standby()
{
}

bool Standby::execute(BoxMan* boss, FloorCube* floor[])
{
	switch (mState)
	{
	case IBossGimmick::STATE_SELECT:

		//モーション的なの
		t += 0.05f;  // スピード調整
		y = sinf(t) * 0.1f; // 振れ幅2.0f
		boss->setPositionY(y - 1.5f);
		boss->setMotion(boss->motion_idle);


		break;
	case IBossGimmick::STATE_MOVE:
		break;
	case IBossGimmick::STATE_MOTION:
		break;
	case IBossGimmick::STATE_EXE:
		break;
	default:
		break;
	}

	return false;
}
