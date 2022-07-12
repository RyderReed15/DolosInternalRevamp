#pragma once

#ifndef RADAR_H
#define RADAR_H

#include "../SDK/SDK.h"
#include "../Utils/ConfigManager.h" 
#include "../GUI/Render.h"

struct EntityData;

#define RADAR_PATH    "\\csgo\\resource\\"

namespace RadarESP {

    struct Radar {
        Vector2D vTopLeft;
        Vector2D vWorldCoords;
        float flSize;
        float flZoom;
        float flScale;
        IDirect3DTexture9* pTexture;
    };

    void DrawRadar(Render* pRender, Radar* pRadar);

    Vector GetRadarCoords(Radar* pRadar, Vector vCircleCenter, Vector vCoordCenter, Vector vPos, float flYaw);

    Radar* LoadRadar(Render* pRedconst, const char* szMapName);

    IDirect3DTexture9* MakeRadarTexture(Render* pRender, IDirect3DTexture9* pTexture);

}
#include "ESP.h"

#endif // !RADAR_H
