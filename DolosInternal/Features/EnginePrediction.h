#pragma once

#ifndef ENGINE_PREDICTION_H
#define ENGINE_PREDICTION_H

#include "../SDK/SDK.h"

namespace EnginePrediction {

    int     Begin   (IClientEntity* pLocalPlayer, CUserCmd* pCmd);
    void    End     (IClientEntity* pLocalPlayer);

}



#endif // !ENGINE_PREDICTION_H
