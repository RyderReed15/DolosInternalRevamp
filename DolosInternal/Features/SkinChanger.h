#pragma once

#ifndef SKIN_CHANGER_H
#define SKIN_CHANGER_H

#include "../SDK/SDK.h"
#include "Json--/JsonParser.h"

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

    struct GameSkin {
        std::string szName;
        int         iIndex;
    };
    

    //void Tick();
    void PreTick();
    void PostTick();

    void ChangeItemIndex        (CBaseCombatWeapon* pWeapon, int nItemDefIndex);
    void OverrideSkin           (CBaseCombatWeapon* pWeapon, SkinStruct* pSkinInfo);
    void OverrideModel          (CBaseCombatWeapon* pWeapon, int nItemDefinitionIndex);

    bool InitializeModels       (JsonObject* pItems);

    bool InitializeSkins        (JsonObject* pItems, std::string szPath);

    bool InitializeSkinChanger  (void);
}


#include "../Utils/ConfigManager.h"

#endif // !SKIN_CHANGER_H

