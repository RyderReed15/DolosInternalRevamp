#include "GUIContainer.h"
#include <iostream>

GUIContainer::GUIContainer(POINT ptScreenSize) {
   
    m_ptScreenSize       = ptScreenSize;
    m_pEventHandler     = new GUIEventHandler(this);
    InitializeMap();
}

GUIContainer::~GUIContainer() {
    DeleteMap();
   
    m_vElements.clear();
  
    m_ptScreenSize   = { 0, 0 };
    m_aMap          = nullptr;
    delete          m_pEventHandler;
}

void GUIContainer::AddElement(IGUIElement* pElement) {

    m_vElements.push_back(pElement);
}
void GUIContainer::RemoveElement(int iElement) {
    m_vElements.erase(m_vElements.begin() + iElement);
}

void GUIContainer::ResizeScreen(POINT ptScreenSize) {

    DeleteMap();

    m_ptScreenSize = ptScreenSize;

    InitializeMap();
    GenerateMap();
}

void GUIContainer::DeleteMap() {
    if (m_aMap) {
        for (int i = 0; i < m_ptScreenSize.x; i++) {
            delete[] m_aMap[i];
        }
        delete[] m_aMap;
    }
}
void GUIContainer::InitializeMap() {
    m_aMap = new short* [m_ptScreenSize.x];
    for (int i = 0; i < m_ptScreenSize.x; i++) {
        m_aMap[i] = new short[m_ptScreenSize.y]{ -1 };
    }
}
void GUIContainer::GenerateMap() {
    
    for (short x = 0; x < m_ptScreenSize.x; x++)
    {
        wmemset((wchar_t*)m_aMap[x], -1, m_ptScreenSize.y);
    }
    for (short i = 0; i < m_vElements.size(); i++) {
        if (m_vElements[i]->GetEnabled() && m_vElements[i]->GetDrawState()) {
            D3DXVECTOR4 vBounds = m_vElements[i]->GetBounds();
            RECT rBounds = { vBounds.x, vBounds.y, vBounds.z, vBounds.w };

            short iXSize = min(rBounds.left + rBounds.right, m_ptScreenSize.x);
            short iYSize = min(rBounds.top + rBounds.bottom, m_ptScreenSize.y);

            for (short x = rBounds.left; x < iXSize; x++)
            {
                wmemset((wchar_t*)(m_aMap[x] + rBounds.top), i, (iYSize - rBounds.top));
               
            }
        }
    }
}

GUIEventHandler* GUIContainer::GetEventHandler() {
    return m_pEventHandler;
}

IGUIElement* GUIContainer::GetWidgetAt(POINT ptLocation) {
    short iElement = m_aMap[ptLocation.x][ptLocation.y];
    if (iElement == -1) {
        return nullptr;
    }
    return m_vElements[iElement];
}
IGUIElement* GUIContainer::GetWidgetById(int iElement) {
    return m_vElements[iElement];
}


void GUIContainer::DrawElements(Render* pRender, ID3DXFont* pFont) {
    pRender->Begin();
    for (int i = 0; i < m_vElements.size(); i++) {
        if (m_vElements[i]->GetDrawState()) {
            m_vElements[i]->Draw(pFont, pRender);
        }
    }
    pRender->End();
}