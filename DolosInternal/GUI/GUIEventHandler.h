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
    KEYDOWN,
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

    

    void            HandleClick         (POINT ptLocation);
    void            HandleDrag          (POINT ptLocation);
    void            HandleRelease       (POINT ptLocation);
    void            HandleHover         (POINT ptLocation);
    void            HandleType          (char chKey);
    void            HandleKeyDown       (char chKey, long keyInfo);

    bool            CreateGUIEvent      (GUI_EVENT_TYPE tEventType, std::function<void()> pFunc);
    void            ProccessEvents      (void);

    GUIContainer*   GetContainer        (void);

    void            SetFocus            (IGUIElement* pFocus);
    void            ReleaseFocus        (void);

    template <typename Callable, typename ...Args>
    std::function<void()> BuildFunction(Callable&& caFunction, Args&&... args);

private:

    std::queue<GUIEvent>    m_qEvents;
    GUIContainer*           m_pGUI;
    IGUIElement*            m_pFocus;
};

template <typename Callable, typename ...Args>
std::function<void()> GUIEventHandler::BuildFunction(Callable&& caFunction, Args&&... args) {

    return std::bind(std::forward<Callable>(caFunction), std::forward<Args>(args)...);
}

#endif // !GUI_EVENT_HANDLER_H
