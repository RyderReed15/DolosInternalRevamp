#pragma once

#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#define MAX_GUI_ELEMENTS 512

#include "GUIEventHandler.h"
#include "Widgets/IGUIElement.h"
#include <vector>
#include <d3d9.h>

class GUIEventHandler;
class IGUIElement;
class Render;

inline _locale_t        g_Locale;

class GUIContainer {
public:
                        GUIContainer        (POINT ptScreenSize);
                        ~GUIContainer       (void);

    void                AddElement          (IGUIElement* pElement);
    void                RemoveElement       (int iElement);

    void                ResizeScreen        (POINT ptScreenSize);
    void                DeleteMap           (void);
    void                InitializeMap       (void);
    void                GenerateMap         (void);
        
    GUIEventHandler*    GetEventHandler     (void);

    IGUIElement*        GetWidgetAt         (POINT ptLocation);
    IGUIElement*        GetWidgetById       (int iElement);

    void                DrawElements        (Render* pRender, ID3DXFont* pFont);
private:
    GUIEventHandler*            m_pEventHandler;
    POINT                       m_ptScreenSize;
    short**                     m_aMap;
    std::vector<IGUIElement*>   m_vElements;

    

};



#endif // !GUI_CONTAINER_H
