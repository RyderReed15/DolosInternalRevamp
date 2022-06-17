#include "Misc.h"

void Misc::Bhop(CUserCmd* pCmd, int iFlags) {
    pCmd->iButtons &= pCmd->iButtons & IN_JUMP && !(iFlags & 1) && Settings.Misc.Bhop ? ~IN_JUMP : UINT_MAX;
}