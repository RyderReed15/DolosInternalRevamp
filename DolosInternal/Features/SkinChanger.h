#pragma once

#ifndef SKIN_CHANGER_H
#define SKIN_CHANGER_H

#include "../SDK/SDK.h"

namespace SkinChanger {

    struct SkinStruct {
        int iPaintKit;
        float flWear;
        int iStatTrak;
        int iSeed;
        char* szCustomName;
    };

    void Tick();


    void OverrideSkin(CBaseCombatWeapon* pWeapon, SkinStruct skinInfo);
}



#endif // !SKIN_CHANGER_H

