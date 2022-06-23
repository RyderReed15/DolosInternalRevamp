#include "Text.h"

GUIText::GUIText(const char* szText, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
    m_szText = szText;
    m_cColor = cColor;
}

HRESULT GUIText::Draw(ID3DXFont* pFont, Render* pRender) {
    return pRender->DrawString({ m_vBounds.x, m_vBounds.y - TEXT_FEATURE_OFFSET }, m_cColor, pFont, m_szText);
}