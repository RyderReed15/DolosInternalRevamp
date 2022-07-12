#pragma once

#ifndef ESP_H
#define ESP_H
#include "../SDK/SDK.h"
#include "../Utils/ConfigManager.h"
#include "../Utils/LocalizationManager.h"
#include "../GUI/GUI.h"

#define NAME_PREFIX_LEN     1

#define INCH_TO_METER       0.0254f

enum BONES {
    GROUND = 1, 
    HEIGHT, 
    PELVIS,
    SPINE1, 
    SPINE2, 
    CHEST, 
    NECK, 
    HEAD, 
    NECK2
};

struct EntityData {
    D3DCOLOR cColor = 0;
    Vector4D vSize = { 0,0,0,0 };
    Vector vPos = { 0,0,0 };
    int iHealth = 0;
    int iArmor = 0;
    int iDistance = 0;
    union {
        char szName[128];
        wchar_t wszName[128];
    };
   
    bool bPlayer = false;
    bool bEnemy = false;
    bool bDeleted = true;
    std::vector<Vector4D> vBones;

};

namespace ESP {

    void Tick();

    void GetData            (void);
    void GetEntityInfo      (CBaseEntity* pEntity, D3DCOLOR cColor, bool bPlayer, int iIndex);
    void GetBones           (IClientEntity* pEntity, EntityData* pEntityData);


    void DrawOutline        (Vector4D vBounds, D3DCOLOR cColor);
    void DrawHealth         (int iHealth, Vector4D vBounds);
    void DrawArmor          (int iArmor, Vector4D vBounds);
    void DrawBones          (std::vector<Vector4D> vBones, D3DCOLOR cColor);
    void DrawPlayerName     (Vector4D vBounds, const char* szPlayerName);
    void DrawWeaponName     (Vector4D vBounds, const wchar_t* szWeaponName);
    void DrawDistance       (Vector4D vBounds, int iDistance);

    void DrawElements       (void);

    bool WorldToScreen      (Vector2D& vScreen, Vector vPos);
    bool WorldToScreen      (Vector2D& vScreen, Vector vPos, VMatrix vMatrix);
   

    inline                  std::unordered_map<int, EntityData> g_mEntityData;;
}








#endif // !ESP_H

