#pragma once

#ifndef ENGINE_PREDICTION_H
#define ENGINE_PREDICTION_H

#include "../SDK/SDK.h"

namespace EnginePrediction {

    int     Begin   (IClientEntity* pLocalPlayer, CUserCmd* pCmd);
    void    End     (IClientEntity* pLocalPlayer);

    inline  float   g_flOldTime, g_flOldFrameTime;
}



#endif // !ENGINE_PREDICTION_H
