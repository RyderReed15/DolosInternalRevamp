#pragma once

#ifndef SKIN_CHANGER_H
#define SKIN_CHANGER_H

#include "../SDK/SDK.h"

namespace SkinChanger {

    struct SkinStruct {
        int     nItemDefIndex;
        int     iPaintKit;
        float   flWear;
        int     iStatTrak;
        int     iSeed;
        int     iQuality;
        char    szCustomName[32];
    };

    

    //void Tick();
    void PreTick();
    void PostTick();

    void ChangeItemIndex    (CBaseCombatWeapon* pWeapon, int nItemDefIndex);
    void OverrideSkin       (CBaseCombatWeapon* pWeapon, SkinStruct* pSkinInfo);
    void OverrideModel      (CBaseCombatWeapon* pWeapon, int nItemDefinitionIndex);
    int  GetModelIndexFromId(int iWeaponIndex);
}


#include "../Utils/ConfigManager.h"

#endif // !SKIN_CHANGER_H

