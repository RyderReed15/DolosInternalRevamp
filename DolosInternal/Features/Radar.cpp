#include "Radar.h"



void RadarESP::DrawRadar(Render* pRender) {
    if (!g_pLocalPlayer || !Settings.Visuals.Overview.vTextures.size() || !Settings.Visuals.Overview.bEnabled || !g_pMouseEnable->GetInt()) return;

    Config::VisualsConfig::Radar* pRadar = &Settings.Visuals.Overview;
    EntityData::LocalPlayerData* pLocalPlayerData = EntityData::GetLocalPlayerData();

    pRender->Begin(BUFFER_TYPE::BUFFER_TEXTURE);

    Vector2D vCenter = { pRadar->vTopLeft.x + pRadar->flSize, pRadar->vTopLeft.y + pRadar->flSize };
    Vector2D vPlayerCoords = GetRadarCoords(vCenter, vCenter, pLocalPlayerData->vPosition, 0);

    for (size_t i = pRadar->vTextures.size() - 1; i >= 0; i--) {
        if (pLocalPlayerData->vPosition.z < pRadar->vMaxs[i]) {
            pRender->SetTexture(pRadar->vTextures[i]);
            break;
        }
        if (i == 0) return; // Stop error from being outside minimap range
    }

    float flYaw = pLocalPlayerData->vViewAngles.y - 90;
    float flZoom = .3333f / pRadar->flZoom; //  1 / 2  *.666f  / zoom | 1/3 is to correct for increase in size of texture for black borders

    Vector2D vOffset = vPlayerCoords * flZoom / pRadar->flSize;
    pRender->DrawTextureCircle({ vCenter.x, vCenter.y }, pRadar->flSize, 100, { vOffset.x - flZoom, vOffset.y - flZoom, vOffset.x + flZoom, vOffset.y + flZoom}, 1, flYaw);

    pRender->End(BUFFER_TYPE::BUFFER_TEXTURE);

    pRender->Begin(BUFFER_TYPE::BUFFER_TRI);
    pRender->SetTexture(nullptr);

    std::unordered_map<int, EntityData::PlayerData>* mPlayerData = EntityData::GetAllPlayerData();

    for (std::unordered_map<int, EntityData::PlayerData>::iterator it = mPlayerData->begin(); it != mPlayerData->end(); it++) {

        if (it->second.bAccessible) {
            Vector2D vCoords = GetRadarCoords(vCenter, vPlayerCoords, it->second.vPosition, flYaw);
            D3DCOLOR cColor = (it->second.bFriendly ? Settings.Visuals.Friendly.TeamColor : Settings.Visuals.Enemy.TeamColor);

            //Draw either up or down arrows - arbitrary height for now
            if (pLocalPlayerData->vPosition.z > it->second.vPosition.z + 100) { // Down
                pRender->DrawCircle({ vCoords.x, vCoords.y + 9 }, 4, 1, cColor, .25f, 225);
            }
            else if(pLocalPlayerData->vPosition.z < it->second.vPosition.z - 100){ // Up
                pRender->DrawCircle({ vCoords.x, vCoords.y - 9 }, 4, 1, cColor, .25f, 45);
            }

            pRender->DrawCircle({vCoords.x, vCoords.y}, 5, 10, cColor);
        }
    }
    pRender->DrawCircle({ vCenter.x, vCenter.y, }, 10, 2, LIGHTBLUE, .75f, 135); // Makes an arrow - magic numbers dont change
    pRender->End(BUFFER_TYPE::BUFFER_TRI);
}

Vector2D RadarESP::GetRadarCoords(Vector2D vCircleCenter, Vector2D vCoordCenter, Vector vPos, float flYaw) {
    float flCos = cosf(flYaw * DEG_TO_RAD);
    float flSin = sinf(flYaw * DEG_TO_RAD);

    Config::VisualsConfig::Radar* pRadar = &Settings.Visuals.Overview;

    Vector2D vAdjCoords = { vPos.x - pRadar->vWorldCoords.x, pRadar->vWorldCoords.y - vPos.y}; // Adjust for radar offset

    Vector2D vRadarCoords = vAdjCoords / pRadar->flScale * pRadar->flZoom * pRadar->flSize * 2; // Scale to size of radar

    Vector2D vPolarCoords = vRadarCoords - vCoordCenter; // Convert to coordinates centered around the chosen center

    Vector2D vRotatedCoords = {vPolarCoords.x * flCos - vPolarCoords.y * flSin, vPolarCoords.x * flSin + vPolarCoords.y * flCos}; // Rotate around coordinate center point

    if (vRotatedCoords.Length() > pRadar->flSize && vCircleCenter != vCoordCenter) {
        vRotatedCoords = vRotatedCoords.Normalized() * pRadar->flSize; // Max out position at radar edge
    }

    return vRotatedCoords + vCircleCenter;

}
void RadarESP::LoadRadar(Render* pRender, const char* szMapName) {
    char name[MAX_PATH];

    GetModuleFileName(NULL, name, MAX_PATH);

    //Locate game files containing model and skin info
    std::string szRadarPath = name;
    szRadarPath = szRadarPath.substr(0, szRadarPath.find_last_of('\\')) + RADAR_PATH + szMapName;
    
    JsonObject* pOverview = ParseJsonFile((szRadarPath + ".txt").c_str());
    if (pOverview) {
       
        Settings.Visuals.Overview.vWorldCoords  = { strtof(pOverview->GetString("pos_x").c_str(), 0),  strtof(pOverview->GetString("pos_y").c_str(), 0) };
        Settings.Visuals.Overview.flScale       = ( strtof(pOverview->GetString("scale").c_str(), 0) + .1f) * 1000.f;

        for (size_t i = 0; i < Settings.Visuals.Overview.vTextures.size(); i++) {
            Settings.Visuals.Overview.vTextures[i]->Release();
        }
        Settings.Visuals.Overview.vTextures.clear();
        Settings.Visuals.Overview.vMaxs.clear();

        JsonObject* pLevels = pOverview->GetJsonObject("verticalsections");
        if (pLevels) {
            for (size_t i = 0; i < pLevels->m_vValues.size(); i++) {

                if (pLevels->m_vValues[i].m_tType != VALUE_TYPE::OBJECT) continue;
                if (pLevels->m_vValues[i].m_pObject->m_szName == "default") {

                    Settings.Visuals.Overview.vTextures.push_back(MakeRadarTexture(pRender, pRender->LoadTexture((szRadarPath + "_radar.dds").c_str())));
                }
                else {
                    std::string szRadarName = szRadarPath + "_" + pLevels->m_vValues[i].m_pObject->m_szName + "_radar.dds";
              
                    Settings.Visuals.Overview.vTextures.push_back(MakeRadarTexture(pRender, pRender->LoadTexture(szRadarName.c_str())));
                }
                Settings.Visuals.Overview.vMaxs.push_back(strtof(pLevels->m_vValues[i].m_pObject->GetString("AltitudeMax").c_str(), 0));
            }
        }else{
            Settings.Visuals.Overview.vTextures.push_back(MakeRadarTexture(pRender, pRender->LoadTexture((szRadarPath + "_radar.dds").c_str())));
            Settings.Visuals.Overview.vMaxs.push_back(999999);
        }
    }
    delete pOverview;
}

//Builds a texture with the original texture at 0, 0, 2/3, 2/3 leaving black space around the edge to prevent tiling within the radar
IDirect3DTexture9* RadarESP::MakeRadarTexture(Render* pRender, IDirect3DTexture9* pTexture) {
    if (!pTexture) return nullptr;
    IDirect3DSurface9* pRenderSurface, * pTextureSurface;
    IDirect3DTexture9* pRenderTexture;

    D3DSURFACE_DESC texDesc;
    pTexture->GetLevelDesc(0, &texDesc);

    RECT rDest = { 0, 0, static_cast<long>(texDesc.Width), static_cast<long>(texDesc.Height) };

    pRender->GetDevice()->CreateTexture(static_cast<unsigned int>(texDesc.Width * 1.5), static_cast<unsigned int>(texDesc.Height * 1.5), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRenderTexture, NULL);

    pTexture->GetSurfaceLevel(0, &pTextureSurface);
    pRenderTexture->GetSurfaceLevel(0, &pRenderSurface);

    pRender->GetDevice()->ColorFill(pRenderSurface, NULL, BLACK);
    pRender->GetDevice()->StretchRect(pTextureSurface, NULL, pRenderSurface, &rDest, D3DTEXF_NONE);

    pTexture->Release();

    return pRenderTexture;
}