#pragma once

#ifndef SKIN_CHANGER_H
#define SKIN_CHANGER_H

#include "../SDK/SDK.h"
#include "Json--/JsonParser.h"
#include "../Utils/LocalizationManager.h"
#include <map>

#define ITEMS_PATH  "\\csgo\\scripts\\items\\items_game.txt"
#define CDN_PATH    "\\csgo\\scripts\\items\\items_game_cdn.txt"


namespace SkinChanger {

    struct SkinStruct {
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
    void OverrideSkin           (CBaseCombatWeapon* pWeapon, SkinStruct* pSkinInfo);
    void OverrideModel          (CBaseCombatWeapon* pWeapon, int nItemDefinitionIndex);

    void UpdateIndices          (void);

    bool InitializeModels       (JsonObject* pItems);

    bool InitializeSkins        (JsonObject* pItems, std::string szPath);

    bool InitializeSkinChanger  (void);


    inline std::map<int, std::map<int, std::string>> g_mWeapSkins;
    inline std::map<int, std::string> g_mWeapNames;
}


#include "../Utils/ConfigManager.h"

#endif // !SKIN_CHANGER_H

