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

    void Tick();

    void DrawBoundingBox(IClientEntity* pEntity, D3DCOLOR cColor);
    void DrawHealth(int iHealth, Vector4D vBounds);
    void DrawArmor(int iArmor, Vector4D vBounds);
    void DrawBones(IClientEntity* pEntity);

    bool WorldToScreen(Vector2D* vScreen, Vector vPos);
    bool WorldToScreen(Vector2D* vScreen, Vector vPos, VMatrix vMatrix);
   
}








#endif // !ESP_H

