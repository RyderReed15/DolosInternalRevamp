#include "HotKey.h"


char ToUpper(char chKey);

HotKey::HotKey(const char* szName, int iHotKeyId, HotKeyStruct* pHotKeyInfo, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cContainerColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent){
    m_szName = szName;
    m_flContainerSize = flContainerSize;
    m_cContainerColor = cContainerColor;
    m_pHotKeyInfo = pHotKeyInfo;
    m_pHotKeyInfo->Key = ToUpper(m_pHotKeyInfo->Key);
    m_szDisplay = CreateString();
    m_iHotKeyId = iHotKeyId;
    MakeHotKey(m_iHotKeyId, m_pHotKeyInfo);
}


HotKey::~HotKey() {
    DestroyHotKey(m_iHotKeyId);
}


HRESULT HotKey::Draw(ID3DXFont* pFont, Render* pRender) {
    float flLength = pRender->GetStringSize(pFont, m_szDisplay.c_str()).x;
    pRender->DrawString({ m_vBounds.x, m_vBounds.y }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
    pRender->DrawRoundedRectangle({ m_vBounds.x + m_vBounds.z - m_flContainerSize, m_vBounds.y, m_flContainerSize, m_vBounds.w }, ROUND_CORNER_SIZE, BLACK);
    pRender->DrawRoundedRectangle({ m_vBounds.x + m_vBounds.z - m_flContainerSize, m_vBounds.y, m_flContainerSize, m_vBounds.w }, ROUND_CORNER_SIZE, m_cContainerColor);
    return pRender->DrawString({ m_vBounds.x + m_vBounds.z - m_flContainerSize/2 - flLength/2, m_vBounds.y + 1 }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szDisplay.c_str());
}

void HotKey::SetupHotKey(void) {
    MakeHotKey(m_iHotKeyId, m_pHotKeyInfo);
}
std::string HotKey::CreateString() {
    std::string szOut = "";
    if (m_pHotKeyInfo->Ctrl) {
        szOut += "CTRL + ";
    }
    if (m_pHotKeyInfo->Shift) {
        szOut += "SHIFT + ";
    }
    if (m_pHotKeyInfo->Alt) {
        szOut += "ALT + ";
    }
    if (m_pHotKeyInfo->Win) {
        szOut += "WIN + ";
    }
    szOut += MakeKey(m_pHotKeyInfo->Key);
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

void HotKey::OnKey(GUIEventHandler* pEventHandler, char chKey, long) {
    
   
    if (!m_szDisplay.compare("- Enter Key -")) {
        m_szDisplay = "";
        m_pHotKeyInfo->Win      = false;
        m_pHotKeyInfo->Shift    = false;
        m_pHotKeyInfo->Ctrl     = false;
        m_pHotKeyInfo->Alt      = false;
    }
    switch (chKey) {
    case VK_LWIN:
    case VK_RWIN:
        if (m_pHotKeyInfo->Win) return;
        m_szDisplay += "WIN";
        m_pHotKeyInfo->Win = true;
        m_szDisplay += " + ";
        return;
    case VK_SHIFT:
        if (m_pHotKeyInfo->Shift) return;
        m_szDisplay += "SHIFT";
        m_pHotKeyInfo->Shift = true;
        m_szDisplay += " + ";
        return;
    case VK_CONTROL:
        if (m_pHotKeyInfo->Ctrl) return;
        m_szDisplay += "CTRL";
        m_pHotKeyInfo->Ctrl = true;
        m_szDisplay += " + ";
        return;
    case VK_MENU:
        if (m_pHotKeyInfo->Alt) return;
        m_szDisplay += "ALT";
        m_pHotKeyInfo->Alt = true;
        m_szDisplay += " + ";
        return;
    }
    chKey = ToUpper(chKey);
    m_szDisplay += MakeKey(chKey);

    m_pHotKeyInfo->Key = chKey;
    if (!MakeHotKey(m_iHotKeyId, m_pHotKeyInfo)) {
        m_szDisplay = "";
    }
    pEventHandler->ReleaseFocus();

    
       
}

std::string HotKey::MakeKey(char chKey) {
    switch (chKey) {
    case VK_TAB:        return "TAB"; 
    case VK_CAPITAL:    return "CAPS"; 
    case VK_PAUSE:      return "PAUSE"; 
    case VK_ESCAPE:     return "ESC"; 
    case VK_SPACE:      return "SPACE"; 
    case VK_PRIOR:      return "PAGEUP"; 
    case VK_NEXT:       return "PAGEDOWN"; 
    case VK_HOME:       return "HOME"; 
    case VK_END:        return "END"; 
    case VK_LEFT:       return "LEFT"; 
    case VK_RIGHT:      return "RIGHT"; 
    case VK_UP:         return "UP"; 
    case VK_DOWN:       return "DOWN"; 
    case VK_SELECT:     return "SELECT"; 
    case VK_PRINT:      return "PRINT"; 
    case VK_SNAPSHOT:   return "PRTSCR"; 
    case VK_INSERT:     return "INS"; 
    case VK_DELETE:     return "DELETE";
    case VK_BACK:       return "BACK"; 
    case VK_RETURN:     return "ENTER"; 
    default:            return std::string(1, chKey);
    }
}

char ToUpper(char chKey) {

    return chKey - DIFF_UPPER_LOWER * (chKey >= 'a' && chKey <= 'z');

}
