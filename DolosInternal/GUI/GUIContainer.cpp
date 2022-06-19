#include "GUIContainer.h"
#include <iostream>

GUIContainer::GUIContainer(POINT ptScreenSize) {
   
    m_ptScreenSize      = ptScreenSize;
    m_pEventHandler     = new GUIEventHandler(this);
    InitializeMap();
}

GUIContainer::~GUIContainer() {
    DeleteMap();
    for (size_t i = 0; i < m_vElements.size(); i++) {

        delete m_vElements[i];
    }
    m_vElements.clear();
  
    m_ptScreenSize  = { 0, 0 };
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
    for (long i = 0; i < m_ptScreenSize.x; i++) {
        m_aMap[i] = new short[m_ptScreenSize.y]{ -1 };
    }
}
// Generates an map of the screen with the id of the element stored as a short
void GUIContainer::GenerateMap() {
    
    for (long x = 0; x < m_ptScreenSize.x; x++)
    {
        wmemset((wchar_t*)m_aMap[x], WCHAR_MAX, m_ptScreenSize.y);
    }
    for (size_t i = 0; i < m_vElements.size(); i++) {
        if (m_vElements[i]->GetDrawState()) {
            D3DXVECTOR4 vBounds = m_vElements[i]->GetBounds();
            RECT rBounds = { vBounds.x, vBounds.y, vBounds.z, vBounds.w };


            long iXSize = min(rBounds.left + rBounds.right, m_ptScreenSize.x);
            long iYSize = min(rBounds.top + rBounds.bottom, m_ptScreenSize.y);

            for (long x = rBounds.left >= 0 ? rBounds.left : 0; x < iXSize; x++)
            {
                wmemset((wchar_t*)(m_aMap[x] + (rBounds.top >= 0 ? rBounds.top : 0)), i, (iYSize - rBounds.top));
               
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
    for (size_t i = 0; i < m_vElements.size(); i++) {
        if (m_vElements[i]->GetDrawState()) {
            m_vElements[i]->Draw(pFont, pRender);
        }
    }
    pRender->End();
}