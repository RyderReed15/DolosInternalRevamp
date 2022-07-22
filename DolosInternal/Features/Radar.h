#pragma once

#ifndef RADAR_H
#define RADAR_H

#include "Json--/JsonParser.h"
#include "../GUI/Render.h"
#include "../Utils/EntityData.h"
#include "../Settings.h"



#include "../SDK/Math/Vector.h"
#include "../SDK/Interfaces/IConVar.h"

#define RADAR_PATH    "\\csgo\\resource\\overviews\\"

namespace RadarESP {

    

    void                DrawRadar       (Render* pRender);
    void                LoadRadar       (Render* pRender, const char* szMapName);

    Vector2D            GetRadarCoords  (Vector2D vCircleCenter, Vector2D vCoordCenter, Vector vPos, float flYaw);

    IDirect3DTexture9*  MakeRadarTexture(Render* pRender, IDirect3DTexture9* pTexture);

}



#endif // !RADAR_H
