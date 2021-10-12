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
                        GUIContainer        (POINT ptScreenSize);
                        ~GUIContainer       (void);

    void                AddElement          (IGUIElement* pElement);
    template <typename T, typename ...Args>
    T*                  CreateElement       (Args&&... args);
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

template <typename T, typename ...Args>
T* GUIContainer::CreateElement(Args&&... args) {
    T* pElement = new T(args...);
    AddElement(pElement);
    return pElement;
}

#endif // !GUI_CONTAINER_H
