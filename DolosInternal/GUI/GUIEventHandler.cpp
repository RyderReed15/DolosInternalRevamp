#include "GUIEventHandler.h"

GUIEventHandler::GUIEventHandler(GUIContainer* pGUI) {
    m_pGUI = pGUI;
    m_pFocus = nullptr;
}
GUIEventHandler::~GUIEventHandler() {
    while (m_qEvents.size()) { m_qEvents.pop(); };
}

void GUIEventHandler::HandleType(char chKey) {
    if (m_pFocus) {
        IGUIElement* pOldFocus = m_pFocus;
        m_pFocus->OnType(this, chKey);
        pOldFocus->RunCallback(GUI_EVENT_TYPE::KEYDOWN);
    }
}
void GUIEventHandler::HandleKeyDown(char chKey, long keyInfo) {
   
    if (m_pFocus) {
        IGUIElement* pOldFocus = m_pFocus;
        m_pFocus->OnKey(this, chKey, keyInfo);
        pOldFocus->RunCallback(GUI_EVENT_TYPE::KEYDOWN);
    }
}
void GUIEventHandler::HandleClick(POINT ptLocation) {

    if (m_pFocus) {

        IGUIElement* pOldFocus = m_pFocus;
        m_pFocus->OnClick(this, ptLocation);
        pOldFocus->RunCallback(GUI_EVENT_TYPE::CLICK);
    }
    else {
        IGUIElement* pWidget = m_pGUI->GetWidgetAt(ptLocation);
        if (pWidget && pWidget->GetEnabled()) {
            pWidget->OnClick(this, ptLocation);
            pWidget->RunCallback(GUI_EVENT_TYPE::CLICK);
        }
    }

    
}
void GUIEventHandler::HandleDrag(POINT ptLocation) {
    
    if (m_pFocus) {
        IGUIElement* pOldFocus = m_pFocus;
        m_pFocus->OnDrag(this, ptLocation);
        pOldFocus->RunCallback(GUI_EVENT_TYPE::DRAG);
    }
}
void GUIEventHandler::HandleRelease(POINT ptLocation) {
    if (m_pFocus) {
        IGUIElement* pOldFocus = m_pFocus;
        m_pFocus->OnRelease(this, ptLocation);
        pOldFocus->RunCallback(GUI_EVENT_TYPE::RELEASE);

    }else{
        IGUIElement* pWidget = m_pGUI->GetWidgetAt(ptLocation);
        if (pWidget && pWidget->GetEnabled()) {
            pWidget->OnRelease(this, ptLocation);
            pWidget->RunCallback(GUI_EVENT_TYPE::RELEASE);
        }
    }
    
}
void GUIEventHandler::HandleHover(POINT ptLocation) {
    if (m_pFocus) {
        IGUIElement* pOldFocus = m_pFocus;
        m_pFocus->OnHover(this, ptLocation);
        pOldFocus->RunCallback(GUI_EVENT_TYPE::HOVER);
    }
    else {
        IGUIElement* pWidget = m_pGUI->GetWidgetAt(ptLocation);
        if (pWidget && pWidget->GetEnabled()) {
            pWidget->OnHover(this, ptLocation);
            pWidget->RunCallback(GUI_EVENT_TYPE::HOVER);
        }
    }
    
}
void GUIEventHandler::HandleScroll(POINT ptLocation, short zDelta){
    if (m_pFocus) {
        IGUIElement* pOldFocus = m_pFocus;
        m_pFocus->OnScroll(this, ptLocation, zDelta);
        pOldFocus->RunCallback(GUI_EVENT_TYPE::SCROLL);
    }
    else {
        IGUIElement* pWidget = m_pGUI->GetWidgetAt(ptLocation);
        if (pWidget && pWidget->GetEnabled()) {
            pWidget->OnScroll(this, ptLocation, zDelta);
            pWidget->RunCallback(GUI_EVENT_TYPE::SCROLL);
        }
    }

}
//Pushes an event to the queue
bool GUIEventHandler::CreateGUIEvent(GUI_EVENT_TYPE tEventType, std::function<void()> pFunc){
    if (pFunc) {
        GUIEvent eEvent = { tEventType, pFunc };
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


GUIContainer* GUIEventHandler::GetContainer(void){
    return m_pGUI;
}
void GUIEventHandler::SetFocus(IGUIElement* pFocus) {
    m_pFocus = pFocus;
    if(m_pFocus) m_pFocus->RunCallback(GUI_EVENT_TYPE::FOCUS);
}

IGUIElement* GUIEventHandler::GetFocus(void){
    return m_pFocus;
}

void GUIEventHandler::ReleaseFocus(void){
    if(m_pFocus) m_pFocus->RunCallback(GUI_EVENT_TYPE::UNFOCUS);
    m_pFocus = nullptr;
}



