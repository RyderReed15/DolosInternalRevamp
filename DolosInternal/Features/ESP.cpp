#include "ESP.h"

std::unordered_map<int, EntityData> g_mEntityData;

//Use a game thread to get and store data for all displayable entities
void ESP::GetData() {
    if (g_pLocalPlayer == 0) return;
    for (int i = 0; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {

        if (g_mEntityData.count(i)) g_mEntityData[i].bDeleted = true;

        CBaseEntity* pEntity = g_pClientEntityList->GetClientEntity(i);

        if (!pEntity || !(pEntity->IsWeapon() || pEntity->IsPlayer())) continue;

        if (pEntity->IsWeapon() && Settings.Visuals.Weapons.Enabled && pEntity->GetOwner() == -1) {
            
                GetEntityInfo(pEntity, Settings.Visuals.Weapons.Color, false, i);
        }
       else if (pEntity->SanityCheck() && pEntity != g_pLocalPlayer) {
            bool bSameTeam = pEntity->GetTeam() == g_pLocalPlayer->GetTeam();

            if (!bSameTeam && Settings.Visuals.Enemy.ShowTeam) {

                GetEntityInfo(pEntity, Settings.Visuals.Enemy.TeamColor, true, i);
            }
            else if (bSameTeam && Settings.Visuals.Friendly.ShowTeam) {

                GetEntityInfo(pEntity, Settings.Visuals.Friendly.TeamColor, true, i);
            }
        }
    }
}

//Render data and a render thread
void ESP::Tick() {
    if (g_pLocalPlayer == 0) return;
    g_pRender->Begin();
    DrawElements();
    g_pRender->End();
}


// Finds bounding boxes, names, health, armor, location and more info - stored in g_mEntityData[iIndex]
void ESP::GetEntityInfo(CBaseEntity* pEntity, D3DCOLOR cColor, bool bPlayer, int iIndex) {
    Vector vMax, vMin;
    if (!pEntity) return;


    //Find bounding boxes for weapons and model dimensions for players
    if (!bPlayer) {
        ICollideable* pCollide = pEntity->GetCollideable();
        if (!pCollide) return;
        vMax = pCollide->OBBMaxs();
        vMin = pCollide->OBBMins();
    }
    else {
        model_t* pModel = pEntity->GetModel();

        if (!pModel) return;

        studiohdr_t* pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);

        if (!pStudioHdr) return;

        vMax = pStudioHdr->hull_max;
        vMin = pStudioHdr->hull_min;
    }

    //Outerbounds of the hitboxes
    VMatrix mTranspose(pEntity->GetCoordinateFrame());
    Vector vBounds[] = {
        Vector(vMin.x, vMin.y, vMin.z),//      .6 -----.5
        Vector(vMin.x, vMax.y, vMin.z),//    .` |    .` |
        Vector(vMax.x, vMax.y, vMin.z),//   7 ----- 4	|
        Vector(vMax.x, vMin.y, vMin.z),//	|  .0 - | -.1
        Vector(vMax.x, vMax.y, vMax.z),//	|.`		|.`
        Vector(vMin.x, vMax.y, vMax.z),//	3 ----- 2
        Vector(vMin.x, vMin.y, vMax.z),
        Vector(vMax.x, vMin.y, vMax.z),
    };

    Vector vTransformed[8];
    for (int i = 0; i < 8; i++) {
        vTransformed[i] = mTranspose.VMul4x3(vBounds[i]);
    }

    Vector2D vScreen;
    WorldToScreen(vScreen, vTransformed[0]);
    Vector4D vSize = { vScreen.x, vScreen.y, vScreen.x, vScreen.y };
    //Most extreme coordinates
    for (int i = 1; i < 8; i++) {
       
        if (WorldToScreen(vScreen, vTransformed[i])) {
            if (vSize.x > vScreen.x) vSize.x = vScreen.x;
            if (vSize.y > vScreen.y) vSize.y = vScreen.y;
            if (vSize.z < vScreen.x) vSize.z = vScreen.x;
            if (vSize.w < vScreen.y) vSize.w = vScreen.y;
        }
    }
    
    g_mEntityData[iIndex].bDeleted     = false;
    g_mEntityData[iIndex].vSize        = vSize;
    g_mEntityData[iIndex].cColor       = cColor;
    g_mEntityData[iIndex].iDistance    = static_cast<int>((pEntity->GetVecOrigin() - g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer())->GetVecOrigin()).Magnitude() * INCH_TO_METER);
    g_mEntityData[iIndex].bPlayer      = bPlayer;
    if (bPlayer) {

        g_mEntityData[iIndex].bEnemy   = pEntity->GetTeam() != g_pLocalPlayer->GetTeam();
        g_mEntityData[iIndex].iHealth  = pEntity->GetHealth();
        g_mEntityData[iIndex].iArmor   = pEntity->GetArmor();

        player_info_t playerInfo; g_pEngineClient->GetPlayerInfo(iIndex, &playerInfo);
        
        strcpy_s(g_mEntityData[iIndex].szName, 128, playerInfo.szName);

        GetBones(pEntity, &g_mEntityData[iIndex]);

    }
    else {
        strcpy_s(g_mEntityData[iIndex].szName, 128, GetLocalizedString(pEntity->GetWeaponData()->szHudName).c_str());
    } 

    
}

void ESP::DrawOutline(Vector4D vBounds, D3DCOLOR cColor){
    float flQuarterTop = (vBounds.z - vBounds.x) * .25f;
    float flQuarterSide = (vBounds.w - vBounds.y) * .25f;
    g_pRender->DrawLine({ vBounds.x, vBounds.y }, { vBounds.x + flQuarterTop, vBounds.y }, cColor);
    g_pRender->DrawLine({ vBounds.z, vBounds.y }, { vBounds.z - flQuarterTop, vBounds.y }, cColor);


    g_pRender->DrawLine({ vBounds.z, vBounds.y }, { vBounds.z, vBounds.y + flQuarterSide }, cColor);
    g_pRender->DrawLine({ vBounds.z, vBounds.w }, { vBounds.z, vBounds.w - flQuarterSide }, cColor);

    g_pRender->DrawLine({ vBounds.x, vBounds.w }, { vBounds.x + flQuarterTop, vBounds.w }, cColor);
    g_pRender->DrawLine({ vBounds.z, vBounds.w }, { vBounds.z - flQuarterTop, vBounds.w }, cColor);

    g_pRender->DrawLine({ vBounds.x, vBounds.y }, { vBounds.x, vBounds.y + flQuarterSide }, cColor);
    g_pRender->DrawLine({ vBounds.x, vBounds.w }, { vBounds.x, vBounds.w - flQuarterSide }, cColor);
}

//Gets location of important bones in a player model
void ESP::GetBones(IClientEntity* pEntity, EntityData* pEntityData) {
    pEntityData->vBones.clear();
    if (!pEntity || !pEntity->GetModel()) return;

    studiohdr_t* pStudioHdr = g_pModelInfo->GetStudiomodel(pEntity->GetModel());

    if (!pStudioHdr) return;

    Vector vChest   = pEntity->GetBonePos(BONES::CHEST);
    Vector vNeck    = pEntity->GetBonePos(BONES::NECK);

    //Loop through bones finding only those with a hitbox and eliminating uneeded ones.
    //This will form a skeleton using head, feet, knees, elbows, chest, and arms
    Vector vAttach  = (vChest + vNeck) / 2;
    for (int i = 0; i < pStudioHdr->numbones; i++) {
        mstudiobone_t* pBone = pStudioHdr->GetBone(i);
        if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && pBone->parent != -1) {
            
            Vector vChild   = pEntity->GetBonePos(i);
            Vector vParent  = pEntity->GetBonePos(pBone->parent);

            Vector vDeltaChild = vChild - vAttach;
            Vector vDeltaParent = vParent - vAttach;
            if (i == 7) {
                vChild = vAttach;
            }else if (i != 6) {
                if (vChild.Distance(vAttach) < 5) continue; // Eliminate close together bones - magic number
                if (vParent.Distance(vAttach) < 5) vParent = vAttach;
            }
            
            Vector2D vScreenChild, vScreenParent;
            WorldToScreen(vScreenChild, vChild);
            WorldToScreen(vScreenParent, vParent);
            pEntityData->vBones.push_back({ vScreenChild.x, vScreenChild.y, vScreenParent.x, vScreenParent.y });
            
        }
    }
}

void ESP::DrawPlayerName(Vector4D vBounds, const char* szPlayerName){
 

    D3DXVECTOR2 vSize = g_pRender->GetStringSize(g_pWeaponFont, szPlayerName);
    g_pRender->DrawString({ (vBounds.x + vBounds.z - vSize.x) / 2 , vBounds.y - vSize.y }, WHITE, g_pWeaponFont, szPlayerName);
    
}

void ESP::DrawWeaponName(Vector4D vBounds, const char* szWeaponName){
   
    if (szWeaponName) {
        D3DXVECTOR2 vSize = g_pRender->GetStringSize(g_pWeaponFont, szWeaponName);
        g_pRender->DrawString({ (vBounds.x + vBounds.z - vSize.x) / 2 , vBounds.y - vSize.y }, WHITE, g_pWeaponFont, szWeaponName);

    } 
    
}

void ESP::DrawDistance(Vector4D vBounds, int iDistance){
   

    char szDistance[33];  _itoa_s(iDistance, szDistance, 10);
    D3DXVECTOR2 vSize = g_pRender->GetStringSize(g_pWeaponFont, szDistance);
    g_pRender->DrawString({ (vBounds.x + vBounds.z - vSize.x) / 2 , vBounds.w  }, WHITE, g_pWeaponFont, szDistance);
}


void ESP::DrawElements(){

    //Loop through all stored entities and draw the show data
    for (std::unordered_map<int, EntityData>::iterator it = g_mEntityData.begin(); it != g_mEntityData.end(); it++) {
        //SCREEN SIZE HERE - dont render boxes if too big
        if (it->second.bDeleted || it->second.vSize.z - it->second.vSize.x < 5 || it->second.vSize.w - it->second.vSize.y < 5 || it->second.vSize.z - it->second.vSize.x > 800 || it->second.vSize.w - it->second.vSize.y > 800) continue;

        
        g_pRender-> DrawRectangle   ({ it->second.vSize.x, it->second.vSize.y, it->second.vSize.z - it->second.vSize.x, it->second.vSize.w - it->second.vSize.y }, TRANSPARENT_GRAY);
                    DrawOutline     (it->second.vSize, it->second.cColor);
                    DrawDistance    (it->second.vSize, it->second.iDistance);

        if (it->second.bPlayer) {
            Config::VisualsConfig::PlayerViz* pInfo = it->second.bEnemy ? &Settings.Visuals.Enemy : &Settings.Visuals.Friendly;
            if (pInfo->DrawHealth)    DrawHealth      (it->second.iHealth, it->second.vSize);
            if (pInfo->DrawArmor)     DrawArmor       (it->second.iArmor, it->second.vSize);
            if (pInfo->DrawName)      DrawPlayerName  (it->second.vSize, it->second.szName);
            if (pInfo->DrawBones)     DrawBones       (it->second.vBones, WHITE);
        }
        else {
            if (Settings.Visuals.Weapons.Enabled)       DrawWeaponName  (it->second.vSize, it->second.szName);
        }
    }
    
}


void ESP::DrawHealth(int iHealth, Vector4D vBounds) {

    g_pRender->DrawRectangle({ vBounds.x - 6, vBounds.y - 1, 3, vBounds.w - vBounds.y }, BLACK);

    g_pRender->DrawRectangle({ vBounds.x - 5, vBounds.y - 1, 1, (vBounds.w - vBounds.y) * iHealth / 100.f }, GREEN, LerpColor(GREEN, RED, iHealth / 100.f), true);

}

void ESP::DrawArmor(int iArmor, Vector4D vBounds) {

    g_pRender->DrawRectangle({ vBounds.z + 3, vBounds.y - 1, 3, vBounds.w - vBounds.y }, BLACK);

    g_pRender->DrawRectangle({ vBounds.z + 4, vBounds.y - 1, 1, (vBounds.w - vBounds.y) * iArmor / 100.f }, LIGHTBLUE);

}

void ESP::DrawBones(std::vector<Vector4D> vBones, D3DCOLOR cColor){

    for (std::vector<Vector4D>::iterator it = vBones.begin(); it != vBones.end(); it++) {
        g_pRender->DrawLine({ it->x, it->y }, { it->z, it->w }, cColor);
    }
}

bool ESP::WorldToScreen(Vector2D& vScreen, Vector vPos) {
    return WorldToScreen(vScreen, vPos, *g_pViewMatrix);
}

//Convertes world coordinates to screen space using matrix multiplication
bool ESP::WorldToScreen(Vector2D& vScreen, Vector vPos, VMatrix vMatrix){

    int iScreenWidth, iScreenHeight;
    g_pEngineClient->GetScreenSize(iScreenWidth, iScreenHeight);

    Vector4D vClipCoords;
    //Matrix multiplication
    vClipCoords.x = vPos.x * vMatrix[0][0] + vPos.y * vMatrix[0][1] + vPos.z * vMatrix[0][2] + vMatrix[0][3];
    vClipCoords.y = vPos.x * vMatrix[1][0] + vPos.y * vMatrix[1][1] + vPos.z * vMatrix[1][2] + vMatrix[1][3];
    vClipCoords.w = vPos.x * vMatrix[3][0] + vPos.y * vMatrix[3][1] + vPos.z * vMatrix[3][2] + vMatrix[3][3];

    if (vClipCoords.w < 0.1f) { vScreen = { 99999,99999 };  return false; }

    //Dividing by vClipCoords.w = Normalized perspective
    Vector NDC;
    NDC.x = vClipCoords.x / vClipCoords.w;
    NDC.y = vClipCoords.y / vClipCoords.w;
    vScreen.x = (iScreenWidth / 2.f ) + (NDC.x * iScreenWidth / 2.f);
    vScreen.y = (iScreenHeight / 2.f) - (NDC.y * iScreenHeight / 2.f);

    return true;

}
