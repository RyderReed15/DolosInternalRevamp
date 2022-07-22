#pragma once

#ifndef ESP_H
#define ESP_H
#include "../Settings.h"
#include "../GUI/FontManager.h"
#include "../Utils/EntityData.h"
#include "../GUI/Render.h"



#include "../SDK/Math/Vector4D.h"

#define NAME_PREFIX_LEN     1

#define INCH_TO_METER       0.0254f


namespace ESP {

    void Tick();

    void DrawOutline        (Vector4D vBounds, D3DCOLOR cColor);
    void DrawHealth         (int iHealth, Vector4D vBounds);
    void DrawArmor          (int iArmor, Vector4D vBounds);
    void DrawBones          (std::vector<Vector4D> vBones, D3DCOLOR cColor);
    void DrawName           (Vector4D vBounds, const char* szName);
    void DrawName           (Vector4D vBounds, const wchar_t* wszName);
    void DrawDistance       (Vector4D vBounds, int iDistance);

    void DrawElements       (void);
}


#endif // !ESP_H

