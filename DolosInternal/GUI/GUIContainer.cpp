#include "GUIContainer.h"

GUIContainer::GUIContainer(POINT pScreenSize) {
   
    m_pScreenSize       = pScreenSize;
    m_pEventHandler     = new GUIEventHandler(this);
    InitializeMap();
}

GUIContainer::~GUIContainer() {
    DeleteMap();
   
    m_vElements.clear();
  
    m_pScreenSize   = { 0, 0 };
    m_aMap          = nullptr;
    delete          m_pEventHandler;
}

void GUIContainer::AddElement(IGUIElement* pElement) {
    m_vElements.push_back(pElement);
}
void GUIContainer::RemoveElement(int iElement) {
    m_vElements.erase(m_vElements.begin() + iElement);
}

void GUIContainer::ResizeScreen(POINT pScreenSize) {

    DeleteMap();

    m_pScreenSize = pScreenSize;

    InitializeMap();
    GenerateMap();
}

void GUIContainer::DeleteMap() {
    if (m_aMap) {
        for (int i = 0; i < m_pScreenSize.x; i++) {
            delete[] m_aMap[i];
        }
        delete[] m_aMap;
    }
}
void GUIContainer::InitializeMap() {
    m_aMap = new short* [m_pScreenSize.x];
    for (int i = 0; i < m_pScreenSize.x; i++) {
        m_aMap[i] = new short[m_pScreenSize.y]{ -1 };
    }
}
void GUIContainer::GenerateMap() {
    
    for (int x = 0; x < m_pScreenSize.x; x++)
    {
        memset(m_aMap[x], -1, m_pScreenSize.y * sizeof(short));
    }
    for (int i = 0; i < m_vElements.size(); i++) {
        if (m_vElements[i]->GetEnabled() && m_vElements[i]->GetDrawState()) {
            D3DXVECTOR4 vBounds = m_vElements[i]->GetBounds();
            RECT rBounds = { vBounds.x, vBounds.y, vBounds.z, vBounds.w };
            for (int x = rBounds.left; x <= rBounds.right; x++)
            {
                memset((char*)m_aMap[x] + rBounds.top, i, (rBounds.bottom - rBounds.top) * sizeof(short));
                
            }
        }
    }
}

GUIEventHandler* GUIContainer::GetEventHandler() {
    return m_pEventHandler;
}

IGUIElement* GUIContainer::GetWidgetAt(POINT pLocation) {
    short iElement = m_aMap[pLocation.x][pLocation.y];
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