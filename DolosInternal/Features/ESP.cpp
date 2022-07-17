#include "ESP.h"

//Render data on a render thread
void ESP::Tick() {
    
    DrawElements();
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



void ESP::DrawName(Vector4D vBounds, const char* szName){
    if (szName) {
        D3DXVECTOR2 vSize = g_pRender->GetStringSize(g_pWeaponFont, szName);
        g_pRender->DrawString({ (vBounds.x + vBounds.z - vSize.x) / 2 , vBounds.y - vSize.y }, WHITE, g_pWeaponFont, szName);
    }
}

void ESP::DrawName(Vector4D vBounds, const wchar_t* wszName){
    if (wszName) {
        D3DXVECTOR2 vSize = g_pRender->GetStringSize(g_pWeaponFont, wszName);
        g_pRender->DrawString({ (vBounds.x + vBounds.z - vSize.x) / 2 , vBounds.y - vSize.y }, WHITE, g_pWeaponFont, wszName);
    } 
}

void ESP::DrawDistance(Vector4D vBounds, int iDistance){
   

    char szDistance[33];  _itoa_s(iDistance, szDistance, 10);
    D3DXVECTOR2 vSize = g_pRender->GetStringSize(g_pWeaponFont, szDistance);
    g_pRender->DrawString({ (vBounds.x + vBounds.z - vSize.x) / 2 , vBounds.w  }, WHITE, g_pWeaponFont, szDistance);
}


void ESP::DrawElements(){
    if (g_pLocalPlayer == 0) return;

    g_pRender->Begin();

    std::unordered_map<int, EntityData::PlayerData>* mPlayerData = EntityData::GetAllPlayerData();
    std::unordered_map<int, EntityData::WeaponData>* mWeaponData = EntityData::GetAllWeaponData();

    //Loop through all stored entities and draw the data
    for (std::unordered_map<int, EntityData::PlayerData>::iterator it = mPlayerData->begin(); it != mPlayerData->end(); it++) {
        //SCREEN SIZE HERE - dont render boxes if too big
        if (!it->second.bAccessible || it->second.vBounds.z - it->second.vBounds.x < 5 || it->second.vBounds.w - it->second.vBounds.y < 5 || it->second.vBounds.z - it->second.vBounds.x > 800 || it->second.vBounds.w - it->second.vBounds.y > 800) continue;

        Config::VisualsConfig::PlayerViz* pInfo = it->second.bFriendly ? &Settings.Visuals.Friendly : &Settings.Visuals.Enemy;
        
        g_pRender-> DrawRectangle   ({ it->second.vBounds.x, it->second.vBounds.y, it->second.vBounds.z - it->second.vBounds.x, it->second.vBounds.w - it->second.vBounds.y }, TRANSPARENT_GRAY);
                    DrawOutline     (it->second.vBounds, pInfo->TeamColor);
                    DrawDistance    (it->second.vBounds, it->second.iDistance);

        if (pInfo->DrawHealth)    DrawHealth      (it->second.iHealth, it->second.vBounds);
        if (pInfo->DrawArmor)     DrawArmor       (it->second.iArmor, it->second.vBounds);
        if (pInfo->DrawName)      DrawName        (it->second.vBounds, it->second.szName);
        if (pInfo->DrawBones)     DrawBones       (it->second.vBones, WHITE);
    }
    for (std::unordered_map<int, EntityData::WeaponData>::iterator it = mWeaponData->begin(); it != mWeaponData->end(); it++) {
        //SCREEN SIZE HERE - dont render boxes if too big
        if (!it->second.bAccessible || it->second.eOwner != -1 || it->second.vBounds.z - it->second.vBounds.x < 5 || it->second.vBounds.w - it->second.vBounds.y < 5 || it->second.vBounds.z - it->second.vBounds.x > 800 || it->second.vBounds.w - it->second.vBounds.y > 800) continue;


        g_pRender-> DrawRectangle   ({ it->second.vBounds.x, it->second.vBounds.y, it->second.vBounds.z - it->second.vBounds.x, it->second.vBounds.w - it->second.vBounds.y }, TRANSPARENT_GRAY);
                    DrawOutline     (it->second.vBounds, Settings.Visuals.Weapons.Color);
                    DrawDistance    (it->second.vBounds, it->second.iDistance);

        if (Settings.Visuals.Weapons.DrawName)  DrawName(it->second.vBounds, it->second.wszName);

    }


    g_pRender->End();
    
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


