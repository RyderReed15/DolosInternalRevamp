#include "GUIEventHandler.h"

GUIEventHandler::GUIEventHandler(GUIContainer* pGUI) {
    m_pGUI = pGUI;
    m_pFocus = nullptr;
}
GUIEventHandler::~GUIEventHandler() {
    while (m_qEvents.size()) { m_qEvents.pop(); };
}

void GUIEventHandler::HandleMouseInput(GUI_EVENT_TYPE tType, POINT ptLocation) {
    switch (tType) {
    case GUI_EVENT_TYPE::CLICK:
        CreateGUIEvent(tType, BuildFunction(&GUIEventHandler::HandleClick   , this, ptLocation));
        break;
    case GUI_EVENT_TYPE::DRAG:
        CreateGUIEvent(tType, BuildFunction(&GUIEventHandler::HandleDrag    , this, ptLocation));
        break;
    case GUI_EVENT_TYPE::RELEASE:
        CreateGUIEvent(tType, BuildFunction(&GUIEventHandler::HandleRelease , this, ptLocation));
        break;
    case GUI_EVENT_TYPE::HOVER:
        CreateGUIEvent(tType, BuildFunction(&GUIEventHandler::HandleHover   , this, ptLocation));
        break;

    }
}
void GUIEventHandler::HandleClick(POINT ptLocation) {

    if (m_pFocus) {
        m_pFocus->OnClick(this, ptLocation);
    }
    else {
        IGUIElement* pWidget = m_pGUI->GetWidgetAt(ptLocation);
        if (pWidget && pWidget->GetEnabled()) pWidget->OnClick(this, ptLocation);
    }

    
}
void GUIEventHandler::HandleDrag(POINT ptLocation) {
    
    if (m_pFocus) m_pFocus->OnDrag(this, ptLocation);
}
void GUIEventHandler::HandleRelease(POINT ptLocation) {
    if (m_pFocus) {
        m_pFocus->OnRelease(this, ptLocation);
    }else{
        IGUIElement* pWidget = m_pGUI->GetWidgetAt(ptLocation);
        if (pWidget && pWidget->GetEnabled()) pWidget->OnRelease(this, ptLocation);
    }
    
}
void GUIEventHandler::HandleHover(POINT ptLocation) {
    IGUIElement* pWidget = m_pGUI->GetWidgetAt(ptLocation);
    if (pWidget && pWidget->GetEnabled()) pWidget->OnHover(this, ptLocation);
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

GUIContainer* GUIEventHandler::GetContainer(void)
{
    return m_pGUI;
}
void GUIEventHandler::SetFocus(IGUIElement* pFocus) {
    m_pFocus = pFocus;
}

void GUIEventHandler::ReleaseFocus(void)
{
    m_pFocus = nullptr;
}



