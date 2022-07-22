#pragma once

#ifndef SKIN_CHANGER_H
#define SKIN_CHANGER_H

#include "Json--/JsonParser.h"
#include "../Utils/EntityData.h"


#include "../SDK/SDK.h"

#include <map>

#define ITEMS_PATH  "\\csgo\\scripts\\items\\items_game.txt"
#define CDN_PATH    "\\csgo\\scripts\\items\\items_game_cdn.txt"


namespace SkinChanger {

    struct SkinInfo {
        int     nItemId;
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

    void ChangeItemIndex        (CBaseCombatWeapon* pWeapon, int nItemDefIndex);
    void OverrideSkin           (CBaseCombatWeapon* pWeapon, SkinInfo* pSkinInfo);
    void OverrideModel          (CBaseCombatWeapon* pWeapon, int nItemDefinitionIndex);

    void UpdateIndices          (void);
    void UpdateStatTrak         (IGameEvent* pEvent);
    void OverrideKillIcon       (IGameEvent* pEvent);

    void ForceWeaponUpdate      (CBaseCombatWeapon* pWeapon);

    bool InitializeModels       (JsonObject* pItems);

    bool InitializeSkins        (JsonObject* pItems, std::string szPath);

    bool InitializeSkinChanger  (void);



    inline std::map<int, std::map<int, std::wstring>> g_mWeapSkins;
    inline std::map<int, std::wstring> g_mWeapNames;
}



#include "../Settings.h"

#endif // !SKIN_CHANGER_H

