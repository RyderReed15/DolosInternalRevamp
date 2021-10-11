#pragma once

#ifndef GUI_PANEL_H
#define GUI_PANEL_H

#include "IGUIElement.h"

class Panel : public IGUIElement {
public:
            Panel       (D3DXVECTOR4 vBounds, bool bMoveable, D3DCOLOR cColor, D3DCOLOR cColor2 = EMPTY, IGUIElement * pParent = nullptr);

    HRESULT Draw        (ID3DXFont* pFont, Render* pRender);

    bool    OnClick     (POINT pLocation);
    bool    OnDrag      (POINT pLocation);
    bool    OnRelease   (POINT pLocation);
private:
    D3DCOLOR    m_cColor1, m_cColor2;
    POINT       m_pMoveOrigin;
    bool        m_bMoving;
    bool        m_bCanMove;
};



#endif // !GUI_PANEL_H
