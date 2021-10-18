#include "ESP.h"


void ESP::Tick() {
    g_pRender->Begin();
    if (g_pLocalPlayer == 0) g_pLocalPlayer = g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
    for (int i = 0; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {

        IClientEntity* pEntity = g_pClientEntityList->GetClientEntity(i);
        if (!pEntity || !(pEntity->IsWeapon() || pEntity->IsPlayer())) continue;
        if (pEntity->IsWeapon()) {
            if (Settings.Visuals.Weapons.Enabled) {
                if (((CBaseCombatWeapon*)pEntity)->GetOwner() == -1) {
                    if (pEntity->GetClientClass()->m_pNetworkName[1] != 'B') {
                        DrawBoundingBox(pEntity, Settings.Visuals.Weapons.Color);
                    }
                }
            }
            
        }
        else if (pEntity->SanityCheck() && pEntity != g_pLocalPlayer) {
            bool bSameTeam = pEntity->GetTeam() == g_pLocalPlayer->GetTeam();

            if (!bSameTeam && Settings.Visuals.Players.ShowEnemy) {

                DrawBoundingBox(pEntity, Settings.Visuals.Players.EnemyColor);
                if (Settings.Visuals.Players.DrawBones)     DrawBones(pEntity);
            }
            else if (bSameTeam && Settings.Visuals.Players.ShowTeam) {

                DrawBoundingBox(pEntity, Settings.Visuals.Players.TeamColor);
                if (Settings.Visuals.Players.DrawBones)     DrawBones(pEntity);
            }
        }
    }
    g_pRender->End();
}

void ESP::DrawBoundingBox(IClientEntity* pEntity, D3DCOLOR cColor) {
    Vector vMax, vMin;
    if (!pEntity) return;

    if (pEntity->GetClientClass()->m_pNetworkName[1] == 'B') {
        return;
    }

    if (pEntity->IsWeapon()) {
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
    WorldToScreen(&vScreen, vTransformed[0]);
    D3DXVECTOR4 vSize = { vScreen.x, vScreen.y, vScreen.x, vScreen.y };
    for (int i = 1; i < 8; i++) {
       
        if (WorldToScreen(&vScreen, vTransformed[i])) {
            if (vSize.x > vScreen.x) vSize.x = vScreen.x;
            if (vSize.y > vScreen.y) vSize.y = vScreen.y;
            if (vSize.z < vScreen.x) vSize.z = vScreen.x;
            if (vSize.w < vScreen.y) vSize.w = vScreen.y;
        }
        

    }
    //SCREEN SIZE HERE
    if (vSize.z - vSize.x < 5 || vSize.w - vSize.y < 5 || vSize.z - vSize.x > 800 || vSize.w - vSize.y > 800) return;

    g_pRender->DrawRectangle({ vSize.x, vSize.y, vSize.z - vSize.x, vSize.w - vSize.y }, cColor);
    if (pEntity->IsPlayer()) {
        if (Settings.Visuals.Players.DrawHealth)    DrawHealth  (pEntity->GetHealth()   , (Vector4D)vSize);
        if (Settings.Visuals.Players.DrawArmor)     DrawArmor   (pEntity->GetArmor()    , (Vector4D)vSize);
    }
    

    
}

void ESP::DrawBones(IClientEntity* pEntity) {
    if (!pEntity || !pEntity->GetModel()) return;

    studiohdr_t* pStudioHdr = g_pModelInfo->GetStudiomodel(pEntity->GetModel());

    if (!pStudioHdr) return;

    Vector vChest   = pEntity->GetBonePos(BONES::CHEST);
    Vector vNeck    = pEntity->GetBonePos(BONES::NECK);


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
                if (vChild.Distance(vAttach) < 5) continue;
                if (vParent.Distance(vAttach) < 5) vParent = vAttach;
            }
            
            Vector2D vScreenChild, vScreenParent;
            WorldToScreen(&vScreenChild, vChild);
            WorldToScreen(&vScreenParent, vParent);
            g_pRender->DrawLine(vScreenChild, vScreenParent, WHITE);
            
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

bool ESP::WorldToScreen(Vector2D* vScreen, Vector vPos) {
    return WorldToScreen(vScreen, vPos, *g_pViewMatrix);
}

bool ESP::WorldToScreen(Vector2D* vScreen, Vector vPos, VMatrix vMatrix){

    int iScreenWidth, iScreenHeight;
    g_pEngineClient->GetScreenSize(iScreenWidth, iScreenHeight);

    Vector4D vClipCoords;
    vClipCoords.x = vPos.x * vMatrix[0][0] + vPos.y * vMatrix[0][1] + vPos.z * vMatrix[0][2] + vMatrix[0][3];
    vClipCoords.y = vPos.x * vMatrix[1][0] + vPos.y * vMatrix[1][1] + vPos.z * vMatrix[1][2] + vMatrix[1][3];
    vClipCoords.w = vPos.x * vMatrix[3][0] + vPos.y * vMatrix[3][1] + vPos.z * vMatrix[3][2] + vMatrix[3][3];

    if (vClipCoords.w < 0.1f) { *vScreen = { 99999,99999 };  return false; }

    //perspective division, dividing by clip.W = Normalized Device Coordinates
    Vector NDC;
    NDC.x = vClipCoords.x / vClipCoords.w;
    NDC.y = vClipCoords.y / vClipCoords.w;
    vScreen->x = (iScreenWidth / 2.f ) + (NDC.x * iScreenWidth / 2.f);
    vScreen->y = (iScreenHeight / 2.f) - (NDC.y * iScreenHeight / 2.f);

    return true;

}
