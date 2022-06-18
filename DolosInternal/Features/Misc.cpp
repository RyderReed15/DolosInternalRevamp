#include "Misc.h"

void Misc::Bhop(CUserCmd* pCmd, int iFlags) {
    //Checks if players is current not on the ground and will set the jump button to false in the air only allowing the press through on landing
    pCmd->iButtons &= pCmd->iButtons & IN_JUMP && !(iFlags & 1) && Settings.Misc.Bhop ? ~IN_JUMP : UINT_MAX;
}