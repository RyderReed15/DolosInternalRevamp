#pragma once

#ifndef SKIN_CHANGER_H
#define SKIN_CHANGER_H

#include "../SDK/SDK.h"

namespace SkinChanger {

    struct SkinStruct {
        int     iPaintKit;
        float   flWear;
        int     iStatTrak;
        int     iSeed;
        int     iQuality;
        char    szCustomName[32];
    };

    void Tick();


    void OverrideSkin(CBaseCombatWeapon* pWeapon, SkinStruct* pSkinInfo);
}


#include "../Utils/ConfigManager.h"

#endif // !SKIN_CHANGER_H

