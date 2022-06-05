#pragma once

#ifndef RECOIL_CONTROL_H
#define RECOIL_CONTROL_H

#include "../SDK/SDK.h"
#include "../Utils/ConfigManager.h"
#include "../GUI/GUI.h"

namespace RecoilControl {
    Vector RecoilControl(Vector vAngles, Vector vAimPunch, bool bAimbot);

    inline Vector vOldAimPunch = { 0,0,0 };
}




#endif // !RECOILCONTROL_H
