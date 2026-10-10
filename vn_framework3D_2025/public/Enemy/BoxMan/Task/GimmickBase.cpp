#include "GimmickBase.h"

//=============ÉMÉ~ÉbÉNä÷êî=================

IBossGimmick::IBossGimmick()
{
    DoOnce = true;
    initialize();
}

bool IBossGimmick::initialize()
{
    return false;
}