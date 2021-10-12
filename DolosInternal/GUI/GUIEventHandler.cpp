#include "GUIEventHandler.h"

GUIEventHandler::GUIEventHandler(GUIContainer* pGUI) {
    m_pGUI = pGUI;
    m_pItemBeingDragged = nullptr;
}
GUIEventHandler::~GUIEventHandler() {
    while (m_qEvents.size()) { m_qEvents.pop(); };
}

void GUIEventHandler::HandleInput(GUI_EVENT_TYPE tType, POINT pLocation) {
    switch (tType) {
    case GUI_EVENT_TYPE::CLICK:
        
        CreateGUIEvent(tType, BuildFunction(&GUIEventHandler::HandleClick, this, pLocation));
        
        break;
    case GUI_EVENT_TYPE::DRAG:
        break;
    case GUI_EVENT_TYPE::RELEASE:
        break;
    case GUI_EVENT_TYPE::HOVER:
        break;

    }
}
void GUIEventHandler::HandleClick(POINT pLocation) {
    IGUIElement* pWidget = m_pGUI->GetWidgetAt(pLocation);
    if (pWidget && pWidget->OnClick(pLocation)) {
        m_pItemBeingDragged = pWidget;
    }
}
void GUIEventHandler::HandleDrag(POINT pLocation) {
    
    if (m_pItemBeingDragged) m_pItemBeingDragged->OnDrag(pLocation);
}
void GUIEventHandler::HandleRelease(POINT pLocation) {
    IGUIElement* pWidget = m_pGUI->GetWidgetAt(pLocation);
    if (pWidget) pWidget->OnRelease(pLocation);

}
void GUIEventHandler::HandleHover(POINT pLocation) {
    IGUIElement* pWidget = m_pGUI->GetWidgetAt(pLocation);
    if (pWidget) pWidget->OnHover(pLocation);
}

bool GUIEventHandler::CreateGUIEvent(GUI_EVENT_TYPE tEventType, std::function<void()> pFunc){
    if (pFunc) {
        GUIEvent eEvent = GUIEvent(tEventType, pFunc);
        m_qEvents.push(eEvent);
        return true;
    }
    return false;
   
}

void GUIEventHandler::ProccessEvents() {
   
    while (m_qEvents.size()) {
        m_qEvents.front().m_pFunc();
        m_qEvents.pop();
    }
}



