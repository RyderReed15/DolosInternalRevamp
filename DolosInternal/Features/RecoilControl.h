#pragma once

#ifndef RECOIL_CONTROL_H
#define RECOIL_CONTROL_H

#include "../SDK/SDK.h"
#include "../Settings.h"
#include "../GUI/GUI.h"

namespace RecoilControl {

    Vector  RecoilControl(Vector vAngles, Vector vAimPunch, bool bAimbot); 
    void    UpdateOldPunch(Vector vAimPunch);
}




#endif // !RECOILCONTROL_H
