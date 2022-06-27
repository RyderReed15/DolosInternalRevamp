#include "CheckBox.h"

CheckBox::CheckBox(const char* szName, bool* pBool, D3DXVECTOR4 vBounds, D3DCOLOR cDisabled, D3DCOLOR cActivated, IGUIElement* pParent) : IValueElement(vBounds, pParent) {
    m_cActivated = cActivated;
    m_cDisabled = cDisabled;

    m_szName = szName;
    m_pBool = pBool;
    m_bClickable = true;
}

HRESULT CheckBox::Draw(ID3DXFont* pFont, Render* pRender) {
	
		pRender->DrawString({ m_vBounds.x, m_vBounds.y - TEXT_FEATURE_OFFSET }, (*m_pBool && m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
		return pRender->DrawRoundedRectangle({ m_vBounds.x + m_vBounds.z - m_vBounds.w, m_vBounds.y, m_vBounds.w, m_vBounds.w }, 5, *m_pBool ? m_cActivated : m_cDisabled);
		
}

void CheckBox::SetClickable(bool bClickable) {
    m_bClickable = bClickable;
}


void CheckBox::OnRelease(GUIEventHandler*, POINT) {
	
	*m_pBool = !*m_pBool;
	IGUIElement* pCurr = m_pFirstChild;
	while (pCurr) {
		pCurr->SetEnabled(*m_pBool);
		pCurr = pCurr->GetSibling();
	}
	
		
}

void CheckBox::SetValuePointer(void* pValue) {
	m_pBool = static_cast<bool*>(pValue);
}

void* CheckBox::GetValuePointer() {
	return m_pBool;
}