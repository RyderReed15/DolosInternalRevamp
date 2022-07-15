#pragma once

#ifndef RADAR_H
#define RADAR_H

#include "Json--/JsonParser.h"
#include "../SDK/SDK.h"
#include "../GUI/GUI.h"
#include "../Utils/EntityData.h"
#include "../GUI/Render.h"
#include "../Settings.h"

#define RADAR_PATH    "\\csgo\\resource\\"

namespace RadarESP {

    

    void                DrawRadar       (Render* pRender);
    void                LoadRadar       (Render* pRender, const char* szMapName);

    Vector2D            GetRadarCoords  (Vector2D vCircleCenter, Vector2D vCoordCenter, Vector vPos, float flYaw);

    IDirect3DTexture9*  MakeRadarTexture(Render* pRender, IDirect3DTexture9* pTexture);

}



#endif // !RADAR_H
