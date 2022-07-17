#include "RecoilControl.h"

Vector vOldAimPunch = { 0,0,0 };


//Removes the expected recoil to the view angles
Vector RecoilControl::RecoilControl(Vector vAngles, Vector vAimPunch, bool bAimbot){
    
    if (Settings.Recoil.ControlFactor == 0 || !Settings.Recoil.Enabled) {
        return vAngles;
    }

    if (bAimbot) {
        return (vAngles - vAimPunch * Settings.Recoil.ControlFactor).ToAngles();
    }
    else {
        Vector vNewAngles = ((vAngles + vOldAimPunch) - (vAimPunch * Settings.Recoil.ControlFactor)).ToAngles();

        UpdateOldPunch(vAimPunch);

        return vNewAngles;
    }
}

void RecoilControl::UpdateOldPunch(Vector vAimPunch){
    vOldAimPunch = vAimPunch * Settings.Recoil.ControlFactor;
}
