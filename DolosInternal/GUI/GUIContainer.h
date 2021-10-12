#pragma once

#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#define MAX_GUI_ELEMENTS 512

#include "Widgets/IGUIElement.h"
#include "GUIEventHandler.h"
#include <vector>

class GUIEventHandler;
class IGUIElement;

class GUIContainer {
public:
                        GUIContainer        (POINT pScreenSize);
                        ~GUIContainer       (void);

    void                AddElement          (IGUIElement* pElement);
    void                RemoveElement       (int iElement);

    void                ResizeScreen        (POINT pScreenSize);
    void                DeleteMap           (void);
    void                InitializeMap       (void);
    void                GenerateMap         (void);
        
    GUIEventHandler*    GetEventHandler     (void);

    IGUIElement*        GetWidgetAt         (POINT pLocation);
    IGUIElement*        GetWidgetById       (int iElement);

    void                DrawElements        (Render* pRender, ID3DXFont* pFont);
private:
    GUIEventHandler*            m_pEventHandler;
    POINT                       m_pScreenSize;
    short**                     m_aMap;
    std::vector<IGUIElement*>   m_vElements;

};
#endif // !GUI_CONTAINER_H