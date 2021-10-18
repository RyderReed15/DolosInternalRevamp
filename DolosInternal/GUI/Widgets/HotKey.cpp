#include "HotKey.h"

char ToUpper(char chKey) {

    return chKey - 0x20 * (chKey >= 'a' && chKey <= 'z');

}

HotKey::HotKey(const char* szName, void* fnHotKey, int iHotKeyId, char chKey, int bModifiers, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cContainerColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent){
    m_szName = szName;
    m_flContainerSize = flContainerSize;
    m_cContainerColor = cContainerColor;
    m_bModifiers = MOD_NOREPEAT | MOD_SHIFT;
    m_szDisplay = ConvertToString(chKey, bModifiers);
    m_iHotKeyId = m_iHotKeyId;
    m_fnHotKey = fnHotKey;
    m_chKey = chKey;

}





HRESULT HotKey::Draw(ID3DXFont* pFont, Render* pRender) {
    float flLength = pRender->GetStringSize(pFont, m_szDisplay.c_str()).x;
    pRender->DrawString({ m_vBounds.x, m_vBounds.y }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
    pRender->DrawRoundedRectangle({ m_vBounds.x + m_vBounds.z - m_flContainerSize, m_vBounds.y, m_flContainerSize, m_vBounds.w }, 2, BLACK);
    pRender->DrawRoundedRectangle({ m_vBounds.x + m_vBounds.z - m_flContainerSize, m_vBounds.y, m_flContainerSize, m_vBounds.w }, 2, m_cContainerColor);
    return pRender->DrawString({ m_vBounds.x + m_vBounds.z - m_flContainerSize/2 - flLength/2, m_vBounds.y + 1 }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szDisplay.c_str());
}

void HotKey::SetupHotKey(void) {
    MakeHotKey(m_fnHotKey, m_iHotKeyId, m_bModifiers, m_chKey);
}
std::string HotKey::ConvertToString(char chKey, int bModifiers) {
    std::string szOut = "";
    if (bModifiers & MOD_CONTROL) {
        szOut += "CTRL + ";
    }
    if (bModifiers & MOD_SHIFT) {
        szOut += "SHIFT + ";
    }
    if (bModifiers & MOD_ALT) {
        szOut += "ALT + ";
    }
    if (bModifiers & MOD_WIN) {
        szOut += "WIN + ";
    }
    szOut += ToUpper(chKey);
    return szOut;
}

void HotKey::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
    pEventHandler->ReleaseFocus();
    m_szDisplay = "- Enter Key -";
    DestroyHotKey(m_iHotKeyId);
    if (CheckBounds(ptLocation)) {
        
       
        pEventHandler->SetFocus(this);
    }

}

void HotKey::OnKey(GUIEventHandler* pEventHandler, char chKey, long keyInfo) {
    
   
    if (!m_szDisplay.compare("- Enter Key -")) {
        m_szDisplay = "";
        m_bModifiers = MOD_NOREPEAT;
    }
    switch (chKey) {
    case VK_LWIN:
    case VK_RWIN:
        if (m_bModifiers & MOD_WIN) return;
        m_szDisplay += "WIN";
        m_bModifiers |= MOD_WIN;
        m_szDisplay += " + ";
        return;
    case VK_SHIFT:
        if (m_bModifiers & MOD_SHIFT) return;
        m_szDisplay += "SHIFT";
        m_bModifiers |= MOD_SHIFT;
        m_szDisplay += " + ";
        return;
    case VK_CONTROL:
        if (m_bModifiers & MOD_CONTROL) return;
        m_szDisplay += "CTRL";
        m_bModifiers |= MOD_CONTROL;
        m_szDisplay += " + ";
        return;
    case VK_MENU:
        if (m_bModifiers & MOD_ALT) return;
        m_szDisplay += "ALT";
        m_bModifiers |= MOD_ALT;
        m_szDisplay += " + ";
        return;
    }
    switch (chKey) {
    case VK_TAB:        m_szDisplay += "TAB"; break;
    case VK_CAPITAL:    m_szDisplay += "CAPS"; break;
    case VK_PAUSE:      m_szDisplay += "PAUSE"; break;
    case VK_ESCAPE:     m_szDisplay += "ESC"; break;
    case VK_SPACE:      m_szDisplay += "SPACE"; break;
    case VK_PRIOR:      m_szDisplay += "PAGEUP"; break;
    case VK_NEXT:       m_szDisplay += "PAGEDOWN"; break;
    case VK_HOME:       m_szDisplay += "HOME"; break;
    case VK_END:        m_szDisplay += "END"; break;
    case VK_LEFT:       m_szDisplay += "LEFT"; break;
    case VK_RIGHT:      m_szDisplay += "RIGHT"; break;
    case VK_UP:         m_szDisplay += "UP"; break;
    case VK_DOWN:       m_szDisplay += "DOWN"; break;
    case VK_SELECT:     m_szDisplay += "SELECT"; break;
    case VK_PRINT:      m_szDisplay += "PRINT"; break;
    case VK_SNAPSHOT:   m_szDisplay += "PRTSCR"; break;
    case VK_INSERT:     m_szDisplay += "INS"; break;
    case VK_DELETE:     m_szDisplay += "DELETE"; break;
    case VK_BACK:       m_szDisplay += "BACK"; break;
    case VK_RETURN:     m_szDisplay += "ENTER"; break;
    default:            m_szDisplay += chKey; break;

    }
    if (!MakeHotKey(m_fnHotKey, m_iHotKeyId, m_bModifiers, chKey)) {
        m_szDisplay = "";
    }
    pEventHandler->ReleaseFocus();

    
       
}
