#pragma once

#ifndef GUI_PANEL_H
#define GUI_PANEL_H

#include "IGUIElement.h"
#include "../GUIEventHandler.h"


class GUIEventHandler;

class Panel : public IGUIElement {
public:
            Panel       (D3DXVECTOR4 vBounds, bool bMoveable, D3DCOLOR cColor, D3DCOLOR cColor2 = EMPTY, IGUIElement * pParent = nullptr);

    HRESULT Draw        (ID3DXFont* pFont, Render* pRender);

    void    OnClick     (GUIEventHandler* pEventHandler, POINT ptLocation);
    void    OnDrag      (GUIEventHandler* pEventHandler, POINT ptLocation);
    void    OnRelease   (GUIEventHandler* pEventHandler, POINT ptLocation);
private:
    D3DCOLOR    m_cColor1, m_cColor2;
    POINT       m_ptMoveOrigin;
    bool        m_bMoving;
    bool        m_bCanMove;
};



#endif // !GUI_PANEL_H
