#pragma once

#ifndef AIMBOT_H
#define AIMBOT_H
#include "../SDK/SDK.h"
#include "../Utils/ConfigManager.h"
#include "../GUI/GUI.h"
#include "RecoilControl.h"

namespace Aimbot {

    bool   Tick(CUserCmd* pCmd);

   
    Vector FindClosestTarget(Vector vPlayerPos, Vector vAngles, Vector vAimPunch, int iTickCount);
    Vector CalculateOveraim(Vector vViewAngles, Vector vDest, int iTick);
    Vector GetNewAngles(Vector vViewAngles, Vector vDest, int iTicks);



    float WeightFormula(float flFOV, float flDistance, float flRotateDistance);
    float FOVFormula(float flFOV, float flDistance);
    Vector CalculateAngle(Vector vStart, Vector vDest);

    inline IClientEntity*   pTarget     = nullptr;
    inline int              iStartTick  = 0;
    inline Vector           vOveraim;

    
}


#endif