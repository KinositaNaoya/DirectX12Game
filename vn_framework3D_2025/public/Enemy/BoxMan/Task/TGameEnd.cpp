#include "TGameEnd.h"

bool GameEnd::initialize()
{
	DoOnce = true;
	return true;
}

GameEnd::GameEnd()
{
	GameEndSE = new vnSound(L"data/sound/SE/CHIPTUNEÉmÉCÉY.wav");
	DoOnce = true;
}

GameEnd::~GameEnd()
{
	delete(GameEndSE);
}

bool GameEnd::execute(BoxMan* boss, FloorCube* floor[])
{
	
	    if (DoOnce) {
	
	        for (int i = 0; i < 64; ++i) {
	            floor[i]->Init();
	        }
	        boss->TimeReset();
	        boss->setMotionRate(0.15f);
	        boss->setMotionLoop(false);
	        boss->setMotion(boss->motion_dead);
	        DoOnce = false;
	    }
	
	    vnCamera::setTarget(boss->getPosition());
	    static XMVECTOR enemyPos = *boss->getPosition();
	
	    enemyPos = XMVectorAdd(enemyPos, XMVectorSet(0.0f, 0.1f, 0.0f, 1.0f));
	    static XMVECTOR cameraPos = *vnCamera::getPosition();
	    static float time = 0.0f;
	    if (time < 0.8f)time += 0.01f;
	
	    XMVECTOR result = XMVectorLerp(cameraPos, enemyPos, time);
	    vnCamera::setPosition(&result);
	
	
	    if (boss->getMotionEnd()) {
	        boss->setIsDead(true);
			return true;
	    }
	return false;
}
