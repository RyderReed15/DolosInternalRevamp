#pragma once

#ifndef ESP_H
#define ESP_H
#include "../SDK/SDK.h"
#include "../Utils/ConfigManager.h"
#include "../GUI/GUI.h"

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

namespace ESP {
    inline std::map<void*, char*> g_mWeaponNames;

    void Tick();
    void GetWeaponNames();

    void DrawBoundingBox    (CBaseEntity* pEntity, D3DCOLOR cColor, int iIndex);
    void DrawOutline        (Vector4D vBounds, D3DCOLOR cColor);
    void DrawHealth         (int iHealth, Vector4D vBounds);
    void DrawArmor          (int iArmor, Vector4D vBounds);
    void DrawBones          (IClientEntity* pEntity);
    void DrawPlayerName     (Vector4D vBounds, int iIndex);
    void DrawWeaponName     (Vector4D vBounds, char* szWeaponName);
    void DrawDistance       (Vector4D vBounds, CBaseEntity* pEntity);

    bool WorldToScreen      (Vector2D& vScreen, Vector vPos);
    bool WorldToScreen      (Vector2D& vScreen, Vector vPos, VMatrix vMatrix);
   
}








#endif // !ESP_H

