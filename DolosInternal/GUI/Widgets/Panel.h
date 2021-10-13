#pragma once

#ifndef GUI_PANEL_H
#define GUI_PANEL_H

#include "IGUIElement.h"
#include "../GUIEventHandler.h"


class GUIEventHandler;

class Panel : public IGUIElement {
public:
            Panel       (D3DXVECTOR4 vBounds, bool bMoveable, D3DCOLOR cColor, D3DCOLOR cColorTwo = EMPTY, IGUIElement * pParent = nullptr);

    HRESULT Draw        (ID3DXFont* pFont, Render* pRender);

    void    OnClick     (GUIEventHandler* pEventHandler, POINT ptLocation);
    void    OnDrag      (GUIEventHandler* pEventHandler, POINT ptLocation);
    void    OnRelease   (GUIEventHandler* pEventHandler, POINT ptLocation);
private:
    D3DCOLOR    m_cColorOne, m_cColorTwo;
    POINT       m_ptMoveOrigin;
    bool        m_bMoving, m_bMoveable;
};



#endif // !GUI_PANEL_H
