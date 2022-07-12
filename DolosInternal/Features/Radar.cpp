#include "Radar.h"



void RadarESP::DrawRadar(Render* pRender, RadarESP::Radar* pRadar) {
    if (!g_pLocalPlayer || !pRadar) return;
    pRender->Begin(BUFFER_TYPE::BUFFER_TEXTURE);
    Vector vPlayerPos = g_pLocalPlayer->GetAbsOrigin();
    QAngle qViewAngles; g_pEngineClient->GetViewAngles(qViewAngles);
    Vector vCenter = { pRadar->vTopLeft.x + pRadar->flSize, pRadar->vTopLeft.y + pRadar->flSize, 0 };
    Vector vPlayerCoords = GetRadarCoords(pRadar, vCenter, vCenter, vPlayerPos, 0);
    

    pRender->SetTexture(pRadar->pTexture);
    float flZoom = (1.f / pRadar->flZoom) / 2.f;
    Vector2D vOffset = { (vPlayerCoords.x / pRadar->flZoom - pRadar->flSize) / pRadar->flSize / 2, (vPlayerCoords.y / pRadar->flZoom - pRadar->flSize) / pRadar->flSize / 2 };
    pRender->DrawTextureCircle({ vCenter.x, vCenter.y }, pRadar->flSize, 100, { (vOffset.x + .5f - flZoom) * .6666f, (vOffset.y + .5f - flZoom) * .6666f, (vOffset.x + .5f + flZoom) * .6666f, (vOffset.y + .5f + flZoom) * .6666f }, 1, qViewAngles.yaw - 90);

    pRender->End(BUFFER_TYPE::BUFFER_TEXTURE);

    pRender->Begin(BUFFER_TYPE::BUFFER_TRI);
    pRender->SetTexture(nullptr);

    
    for (std::unordered_map<int, EntityData>::iterator it = ESP::g_mEntityData.begin(); it != ESP::g_mEntityData.end(); it++) {

        if (it->second.bPlayer && !it->second.bDeleted) {
            Vector vCoords = GetRadarCoords(pRadar, vCenter, vPlayerCoords, it->second.vPos, qViewAngles.yaw - 90);

            if (vPlayerPos.z > it->second.vPos.z + 100) {
                g_pRender->DrawCircle({ vCoords.x, vCoords.y + 9 }, 4, 1, (it->second.bEnemy ? Settings.Visuals.Enemy.TeamColor : Settings.Visuals.Friendly.TeamColor), .25f, 225);
                
            }
            else if(vPlayerPos.z < it->second.vPos.z - 100){
                g_pRender->DrawCircle({ vCoords.x, vCoords.y - 9 }, 4, 1, (it->second.bEnemy ? Settings.Visuals.Enemy.TeamColor : Settings.Visuals.Friendly.TeamColor), .25f, 45);
            }
            g_pRender->DrawCircle({vCoords.x, vCoords.y}, 5, 10, (it->second.bEnemy ? Settings.Visuals.Enemy.TeamColor : Settings.Visuals.Friendly.TeamColor));
           
        }
        
    }
    g_pRender->DrawCircle({ vCenter.x, vCenter.y, }, 10, 2, BLUE, .75f, 135); // Makes an arrow - magic numbers dont change
    pRender->End(BUFFER_TYPE::BUFFER_TRI);
    
}

Vector RadarESP::GetRadarCoords(RadarESP::Radar* pRadar, Vector vCircleCenter, Vector vCoordCenter, Vector vPos, float flYaw) {
    float flCos = cosf(flYaw * DEG_TO_RAD);
    float flSin = sinf(flYaw * DEG_TO_RAD);

    Vector vAdjCoords = { vPos.x - pRadar->vWorldCoords.x, pRadar->vWorldCoords.y - vPos.y, vPos.z }; // Adjust for radar offset

    Vector vRadarCoords = vAdjCoords / pRadar->flScale * pRadar->flZoom * pRadar->flSize * 2; // Scale to size of radar

    Vector vPolarCoords = { vRadarCoords.x - vCoordCenter.x, vRadarCoords.y - vCoordCenter.y, vRadarCoords.z }; // Convert to coordinates centered around the chosen center

    Vector vRotatedCoords = {vPolarCoords.x * flCos - vPolarCoords.y * flSin, vPolarCoords.x * flSin + vPolarCoords.y * flCos, vPolarCoords.z}; // Rotate around coordinate center point

    if (vRotatedCoords.Magnitude() > pRadar->flSize && vCircleCenter != vCoordCenter) {
        vRotatedCoords = vRotatedCoords.Normalize() * pRadar->flSize;
    }

    return vRotatedCoords + vCircleCenter;

}

RadarESP::Radar* RadarESP::LoadRadar(Render* pRender, const char* szMapName) {
    char name[MAX_PATH];

    GetModuleFileName(NULL, name, MAX_PATH);

    //Locate game files containing model and skin info
    std::string szRadarPath = name;
    std::string szOverviewPath;
    szRadarPath = szRadarPath.substr(0, szRadarPath.find_last_of('\\'));
    szRadarPath += RADAR_PATH;
    szOverviewPath = szRadarPath + "overviews\\" + szMapName;
    szOverviewPath += ".txt";
    

    JsonObject* pFile = ParseJsonFile(szOverviewPath.c_str());
    if (pFile) {
        JsonObject* pOverview = pFile->GetJsonObject(szMapName);
        if (pOverview) {
            Radar* pRadar = new Radar();
            pRadar->vWorldCoords = { strtof(pOverview->GetString("pos_x").c_str(), 0),  strtof(pOverview->GetString("pos_y").c_str(), 0) };
            pRadar->vTopLeft = { 10, 10 };            
            pRadar->flSize = 200;
            pRadar->flScale = (strtof(pOverview->GetString("scale").c_str(), 0) + .1f) * 1000.f;
            pRadar->flZoom = 1.5f;
            std::string s(szRadarPath + pOverview->GetString("material"));
            s += (s.find("radar") == -1 ? "_radar" : "");
            s += ".dds";
            pRadar->pTexture = MakeRadarTexture(pRender, pRender->LoadTexture(s.c_str()));
            
            return pRadar;
        }
    }
    return nullptr;
}


IDirect3DTexture9* RadarESP::MakeRadarTexture(Render* pRender, IDirect3DTexture9* pTexture) {
    IDirect3DSurface9* pRenderSurface, * pTextureSurface;
    IDirect3DTexture9* pRenderTexture;

    D3DSURFACE_DESC texDesc;
    pTexture->GetLevelDesc(0, &texDesc);

    RECT rDest = { 0, 0, texDesc.Width, texDesc.Height };

    pRender->GetDevice()->CreateTexture(texDesc.Width * 1.5, texDesc.Height * 1.5, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pRenderTexture, NULL);

    pTexture->GetSurfaceLevel(0, &pTextureSurface);
    pRenderTexture->GetSurfaceLevel(0, &pRenderSurface);

    pRender->GetDevice()->ColorFill(pRenderSurface, NULL, BLACK);
    pRender->GetDevice()->StretchRect(pTextureSurface, NULL, pRenderSurface, &rDest, D3DTEXF_NONE);

    pTexture->Release();

    return pRenderTexture;
}