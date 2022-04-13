#include "RecoilControl.h"

Vector RecoilControl::RecoilControl(Vector vAngles, Vector vAimPunch, bool bAimbot){
    
    if (Settings.Recoil.ControlFactor == 0) {
        return vAngles;
    }

    if (bAimbot) {
        vOldAimPunch = vAimPunch * Settings.Recoil.ControlFactor;
        return (vAngles - vOldAimPunch).ToAngles();
    }
    else {
        Vector vNewAngles = ((vAngles + vOldAimPunch) - (vAimPunch * Settings.Recoil.ControlFactor)).ToAngles();

        vOldAimPunch = vAimPunch * Settings.Recoil.ControlFactor;

        return vNewAngles;
    }

}
