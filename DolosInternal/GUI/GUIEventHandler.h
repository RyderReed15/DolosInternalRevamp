#pragma once

#ifndef GUI_EVENT_HANDLER_H
#define GUI_EVENT_HANDLER_H

#include "Widgets/IGUIElement.h"
#include "GUIContainer.h"
#include <queue>
#include <functional>

class GUIContainer;
class IGUIElement;

enum class GUI_EVENT_TYPE {
    CLICK = 0,
    DRAG,
    RELEASE,
    HOVER,
    BUTTON
};

class GUIEvent {
public:
    GUIEvent(GUI_EVENT_TYPE tType, std::function<void()> pFunc) { m_tType = tType; m_pFunc = pFunc; };
    GUI_EVENT_TYPE m_tType; 
    std::function<void()> m_pFunc;
};

class GUIEventHandler {
public:
            GUIEventHandler     (GUIContainer* pGUI);
            ~GUIEventHandler    (void);

    void    HandleInput         (GUI_EVENT_TYPE tType, POINT pLocation);
    void    HandleClick         (POINT pLocation);
    void    HandleDrag          (POINT pLocation);
    void    HandleRelease       (POINT pLocation);
    void    HandleHover         (POINT pLocation);
    bool    CreateGUIEvent      (GUI_EVENT_TYPE tEventType, std::function<void()> pFunc);
    void    ProccessEvents      (void);

    template <typename callable, typename ...Args>
    std::function<void()> BuildFunction(callable&& caFunction, Args&&... args);

private:

    std::queue<GUIEvent>    m_qEvents;
    GUIContainer*           m_pGUI;
    IGUIElement*            m_pItemBeingDragged;
};

template <typename callable, typename ...Args>
std::function<void()> GUIEventHandler::BuildFunction(callable&& caFunction, Args&&... args) {

    return std::bind(std::forward<callable>(caFunction), std::forward<Args>(args)...);
}

#endif // !GUI_EVENT_HANDLER_H
