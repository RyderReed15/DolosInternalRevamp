#pragma once

#ifndef TRIGGERBOT_H
#define TRIGGERBOT_H

#include "../SDK/SDK.h"
#include "../Utils/ConfigManager.h"
#include "RecoilControl.h"

namespace Triggerbot {

    void Tick(CUserCmd* pCmd);
}




#endif // !TRIGGERBOT_H
