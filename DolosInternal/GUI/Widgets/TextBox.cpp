#include "TextBox.h"

TextBox::TextBox(const char* szName, char* aText, unsigned short iMaxCharacters, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cContainerColor, IGUIElement* pParent) :IValueElement(vBounds, pParent) {
    m_szName = szName;
    m_aText = aText;
    m_flContainerSize = flContainerSize;
    m_cContainerColor = cContainerColor;
    m_iMaxSize = iMaxCharacters;
    m_iCurrentChar = 0;
    m_iTextSize = 0;
    m_iStartChar = 0;
    if (aText != nullptr) {
        
        while (m_aText[m_iTextSize] != '\0') {
            m_iTextSize++;
        }
    }

}
    

HRESULT TextBox::Draw(ID3DXFont* pFont, Render* pRender) {
    float flLength = pRender->GetStringSize(pFont, m_aText + m_iStartChar).x;
    while (flLength > m_flContainerSize - 10) {
        m_iStartChar++;
        flLength = pRender->GetStringSize(pFont, m_aText + m_iStartChar).x;
        
    }
    pRender->DrawString({ m_vBounds.x, m_vBounds.y }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
    pRender->DrawRoundedRectangle({ m_vBounds.x + m_vBounds.z - m_flContainerSize, m_vBounds.y, m_flContainerSize, m_vBounds.w }, ROUND_CORNER_SIZE, BLACK);
    pRender->DrawRoundedRectangle({ m_vBounds.x + m_vBounds.z - m_flContainerSize + 1, m_vBounds.y + 1, m_flContainerSize - 2, m_vBounds.w - 2 }, ROUND_CORNER_SIZE, m_cContainerColor);
    return pRender->DrawString({ m_vBounds.x + m_vBounds.z - m_flContainerSize + 5, m_vBounds.y + 1 }, (m_bEnabled) ? WHITE : GRAY, pFont, m_aText + m_iStartChar);
}

void TextBox::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
    pEventHandler->ReleaseFocus();
    if (CheckBounds(ptLocation)) {
        pEventHandler->SetFocus(this);
        UpdateValues();
    }
    
}

void TextBox::OnType(GUIEventHandler* pEventHandler, char chKey) {

    switch (chKey) {
    case VK_BACK:
        if (m_iTextSize > 0) {
            m_aText[m_iTextSize-1] = '\0';
            m_iTextSize--;
            if (m_iStartChar > 0) {
                m_iStartChar--;
            }
        }
        break;
    case VK_RETURN:
        pEventHandler->ReleaseFocus();
        break;
    default:
        if (m_iTextSize + 1 < m_iMaxSize) {
            m_aText[m_iTextSize] = chKey;
            m_iTextSize++;
            m_aText[m_iTextSize] = '\0';
        }
       
        break;
    }
}

void TextBox::SetValuePointer(void* pValue) {
    m_aText = static_cast<char*>(pValue);
    UpdateValues();
}

void* TextBox::GetValuePointer() {
    return m_aText;
}

void TextBox::UpdateValues(void) {
    m_iStartChar = 0;
    m_iCurrentChar = 0;
    m_iTextSize = 0;
    while (m_aText[m_iTextSize] != '\0') {
        m_iTextSize++;
    }
    
}
