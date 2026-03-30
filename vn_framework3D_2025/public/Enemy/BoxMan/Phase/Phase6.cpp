#include "../../framework.h"
#include "../../framework/vn_environment.h"

Phase6::Phase6()
{
    PhaseBGM = new vnSound(L"data/sound/SE/CHIPTUNEƒmƒCƒY.wav");
    
}

Phase6::~Phase6()
{
    delete(PhaseBGM);
}

void Phase6::execute(BoxMan* boss, FloorCube* floor[])
{
    if (floor == nullptr || boss == nullptr)return;

    if (DoOnce) {
        PhaseBGM->play(true);

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
    }
    return;

}