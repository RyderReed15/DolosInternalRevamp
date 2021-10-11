#include "Button.h"

Button::Button(const char* szText, void* pFunc, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {

	m_cColor = cColor;
	m_szText = szText;
	m_pClickFunc = (fnVoid)pFunc;
}

HRESULT Button::Draw(ID3DXFont* pFont, Render* pRender) {
	if (m_bShouldDraw) {
		
		D3DXVECTOR2 vSize = pRender->GetStringSize(pFont, m_szText);		
		HRESULT result = pRender->DrawRectangle({ m_vBounds.x, m_vBounds.y, m_vBounds.z, m_vBounds.w}, pRender->LerpAlpha(m_cColor, GetAnimLerp(FADE_LENGTH)));
		pRender->DrawString({ m_vBounds.x + (m_vBounds.z - vSize.x) / 2, m_vBounds.y + (m_vBounds.w - vSize.y) / 2 }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szText);
		return result;
	}
	return S_OK;

}

bool Button::OnRelease(POINT pLocation) {
	if (CheckBounds(pLocation) && m_bShouldDraw && m_bEnabled) {
		//Change to be handled by GUI Event Handler
		
		return true;
	}
	return false;
}