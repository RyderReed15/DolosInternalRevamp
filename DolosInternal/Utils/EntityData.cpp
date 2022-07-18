#include "EntityData.h"

std::unordered_map<int, EntityData::WeaponData> mWeaponData;
std::unordered_map<int, EntityData::PlayerData> mPlayerData;
EntityData::LocalPlayerData dLocalPlayerData;

//Use a game thread to get and store data for all displayable entities
void EntityData::UpdateEntityData() {
    if (g_pLocalPlayer == 0) return;
    for (int i = 0; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {

        if (mWeaponData.count(i)) { if (!mWeaponData[i].bAccessible) mWeaponData.erase(i); else mWeaponData[i].bAccessible = false; }
        if (mPlayerData.count(i)) { if (!mPlayerData[i].bAccessible) mPlayerData.erase(i); else mPlayerData[i].bAccessible = false; }
        dLocalPlayerData.bAccessible = false;

        CBaseEntity* pEntity = g_pClientEntityList->GetClientEntity(i);

        if (!pEntity || !(pEntity->IsWeapon() || pEntity->IsPlayer())) continue;

        if (pEntity->IsWeapon()) {
            UpdateWeaponData(pEntity);
        }
        else if (pEntity->SanityCheck() && pEntity != g_pLocalPlayer) {
            UpdatePlayerData(pEntity);
        }
    }
    UpdateLocalPlayerData();
}

// Finds bounding boxes, names, and more info
void EntityData::UpdateWeaponData(CBaseEntity* pWeapon) {
    if (!pWeapon) return;

    WeaponData* pWeaponData = &mWeaponData[pWeapon->Index()];

    Vector vMax, vMin;

    //Find bounding boxes for weapons and model dimensions for players
    ICollideable* pCollide = pWeapon->GetCollideable();
    if (!pCollide) return;

    pWeaponData->vObbsMax = pCollide->OBBMaxs();
    pWeaponData->vObbsMin = pCollide->OBBMins();
    vMax = pWeaponData->vObbsMax;
    vMin = pWeaponData->vObbsMin;
   

    //Outerbounds of the hitboxes
    pWeaponData->mCoordinateFrame = pWeapon->GetCoordinateFrame();
    
    
    pWeaponData->iDistance  = static_cast<int>((pWeapon->GetOrigin() - g_pLocalPlayer->GetVecOrigin()).Magnitude() * INCH_TO_METER);
    pWeaponData->vBounds    = GetBoundingBox(vMin, vMax, pWeaponData->mCoordinateFrame);

    pWeaponData->eOwner     = pWeapon->GetOwner();

    if (strcmp(pWeaponData->szUnLocalizedName, pWeapon->GetWeaponData()->szHudName) != 0) {
        wcscpy_s(pWeaponData->wszName, 128, g_pLocalize->LocalizeStringSafeW(pWeapon->GetWeaponData()->szHudName));
        strcpy_s(pWeaponData->szUnLocalizedName, 128, pWeapon->GetWeaponData()->szHudName);
    }

    pWeaponData->bAccessible = true;

}


// Finds bounding boxes, names, health, armor, location and more info 
void EntityData::UpdatePlayerData(CBaseEntity* pPlayer) {
    if (!pPlayer) return;

    PlayerData* pPlayerData = &mPlayerData[pPlayer->Index()];

    Vector vMax, vMin;

    //Find bounding boxes for weapons and model dimensions for players
    model_t* pModel = pPlayer->GetModel();

    if (!pModel) return;

    studiohdr_t* pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);

    if (!pStudioHdr) return;

    vMax = pStudioHdr->hull_max;
    vMin = pStudioHdr->hull_min;
    pPlayerData->vObbsMax = vMax;
    pPlayerData->vObbsMin = vMin;
    

    //Outerbounds of the hitboxes
    pPlayerData->mCoordinateFrame = pPlayer->GetCoordinateFrame();
    

    pPlayerData->vBounds    = GetBoundingBox(vMin, vMax, pPlayerData->mCoordinateFrame);
    pPlayerData->vPosition  = pPlayer->GetOrigin();
    pPlayerData->iDistance  = static_cast<int>((pPlayer->GetOrigin() - g_pLocalPlayer->GetOrigin()).Magnitude() * INCH_TO_METER);


    pPlayerData->bFriendly  = pPlayer->GetTeam() == g_pLocalPlayer->GetTeam();
    pPlayerData->iTeam      = pPlayer->GetTeam();
    pPlayerData->iHealth    = pPlayer->GetHealth();
    pPlayerData->iArmor     = pPlayer->GetArmor();

    player_info_t playerInfo; 

    if (g_pEngineClient->GetPlayerInfo(pPlayer->Index(), &playerInfo) && strcmp(pPlayerData->szName, playerInfo.szName) != 0) {
        strcpy_s(pPlayerData->szName, 128, playerInfo.szName);
    }
    
    UpdatePlayerBones(pPlayer);

    pPlayerData->bAccessible = true;

}

void EntityData::UpdateLocalPlayerData() {

    dLocalPlayerData.vPosition      = g_pLocalPlayer->GetOrigin();

    dLocalPlayerData.iTeam          = g_pLocalPlayer->GetTeam();
    dLocalPlayerData.iHealth        = g_pLocalPlayer->GetHealth();
    dLocalPlayerData.iArmor         = g_pLocalPlayer->GetArmor();
   
    dLocalPlayerData.vAimPunch      = g_pLocalPlayer->GetAimPunch();

    QAngle qViewAngles; g_pEngineClient->GetViewAngles(qViewAngles);
    dLocalPlayerData.vViewAngles    = { qViewAngles.pitch, qViewAngles.yaw , 0 };    

    dLocalPlayerData.bAccessible    = true;
}

void EntityData::UpdateLocalPlayerDataOnce() {
    dLocalPlayerData.eHandle        = g_pLocalPlayer->GetRefEHandle();

    player_info_t playerInfo;
    if (g_pEngineClient->GetPlayerInfo(g_pLocalPlayer->Index(), &playerInfo)) {
        dLocalPlayerData.iUserID    = playerInfo.userId;
    }
}

//Gets location of important bones in a player model
void EntityData::UpdatePlayerBones(IClientEntity* pPlayer) {
    if (!pPlayer || !pPlayer->GetModel()) return;

    PlayerData* pPlayerData = &mPlayerData[pPlayer->Index()];

    pPlayerData->vBones.clear();

    studiohdr_t* pStudioHdr = g_pModelInfo->GetStudiomodel(pPlayer->GetModel());

    if (!pStudioHdr) return;

    matrix3x4_t* pBoneMatrix = pPlayer->GetBoneMatrix();

    Vector vChest   = pBoneMatrix[BONES::CHEST].GetOrigin();
    Vector vNeck    = pBoneMatrix[BONES::NECK].GetOrigin();

    //Loop through bones finding only those with a hitbox and eliminating uneeded ones.
    //This will form a skeleton using head, feet, knees, elbows, chest, and arms
    Vector vAttach = (vChest + vNeck) / 2;
    for (int i = 0; i < pStudioHdr->numbones; i++) {
        mstudiobone_t* pBone = pStudioHdr->GetBone(i);
        if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && pBone->parent != -1) {

            Vector vChild   = pBoneMatrix[i].GetOrigin();
            Vector vParent  = pBoneMatrix[pBone->parent].GetOrigin();

            if (i == BONES::NECK) {
                vChild = vAttach;
            }
            else if (i != BONES::CHEST) {
                if (vChild.Distance(vAttach) < 5) continue; // Eliminate close together bones - magic number
                if (vParent.Distance(vAttach) < 5) vParent = vAttach;
            }

            Vector2D vScreenChild, vScreenParent;
            WorldToScreen(vScreenChild, vChild);
            WorldToScreen(vScreenParent, vParent);
            pPlayerData->vBones.push_back({ vScreenChild.x, vScreenChild.y, vScreenParent.x, vScreenParent.y });

        }
    }
}

Vector4D EntityData::GetBoundingBox(Vector vMin, Vector vMax, matrix3x4_t mCoordinateFrame) {
    VMatrix mTranspose(mCoordinateFrame);
    Vector vBoxes[] = {
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
        vTransformed[i] = mTranspose.VMul4x3(vBoxes[i]);
    }

    Vector2D vScreen;
    WorldToScreen(vScreen, vTransformed[0]);
    Vector4D vBounds = { vScreen.x, vScreen.y, vScreen.x, vScreen.y };
    //Most extreme coordinates
    for (int i = 1; i < 8; i++) {

        if (WorldToScreen(vScreen, vTransformed[i])) {
            if (vBounds.x > vScreen.x) vBounds.x = vScreen.x;
            if (vBounds.y > vScreen.y) vBounds.y = vScreen.y;
            if (vBounds.z < vScreen.x) vBounds.z = vScreen.x;
            if (vBounds.w < vScreen.y) vBounds.w = vScreen.y;
        }
    }
    return vBounds;
}

bool EntityData::WorldToScreen(Vector2D& vScreen, Vector vPos) {
    return WorldToScreen(vScreen, vPos, g_pEngineClient->WorldToScreenMatrix());
}

//Convertes world coordinates to screen space using matrix multiplication
bool EntityData::WorldToScreen(Vector2D& vScreen, Vector vPos, VMatrix vMatrix) {

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
    vScreen.x = (iScreenWidth / 2.f) + (NDC.x * iScreenWidth / 2.f);
    vScreen.y = (iScreenHeight / 2.f) - (NDC.y * iScreenHeight / 2.f);

    return true;

}


EntityData::LocalPlayerData* EntityData::GetLocalPlayerData() {
    return &dLocalPlayerData;
}

EntityData::PlayerData* EntityData::GetPlayerData(const int nIndex) {
    return &mPlayerData[nIndex];
}

EntityData::PlayerData* EntityData::GetPlayerData(CBaseEntity* pPlayer) {
    return &mPlayerData[pPlayer->Index()];
}

std::unordered_map<int, EntityData::PlayerData>* EntityData::GetAllPlayerData() {
    return &mPlayerData;
}

EntityData::WeaponData* EntityData::GetWeaponData(const int nIndex) {
    return &mWeaponData[nIndex];
}

EntityData::WeaponData* EntityData::GetWeaponData(CBaseEntity* pPlayer) {
    return &mWeaponData[pPlayer->Index()];
}

std::unordered_map<int, EntityData::WeaponData>* EntityData::GetAllWeaponData() {
    return &mWeaponData;
}
