#pragma once

#ifndef AIMBOT_H
#define AIMBOT_H

#include "../Settings.h"
#include "RecoilControl.h"
#include "../Utils/EntityData.h"
#include "../GUI/Render.h"

#include "../SDK/SDK.h"



#define OVERAIM_REDUCTION_FACTOR    30
#define DISTANCE_REDUCTION_FACTOR   512
#define AIM_CURVE_AMOUNT            0.15f

#define MAX_DEG_PER_TICK            35


namespace Aimbot {

    struct ClosestTarget {
        Vector2D vTarget;
        float flFOV;
    };

    bool   Tick                 (CUserCmd* pCmd);

   
    Vector FindClosestTarget    (Vector vPlayerPos , Vector vViewAngles, Vector vAimPunch, int iTickCount);
    Vector CalculateOveraim     (Vector vViewAngles, Vector vTargetAngles, int iTick);
    Vector GetNewAngles         (Vector vViewAngles, Vector vTargetAngles, int iTicks);

    void DrawFOV();

    float   FOVFormula          (float flFOV, float flDistance);
    Vector  CalculateAngle      (Vector vStart, Vector vDest);

    

    
}


#endif