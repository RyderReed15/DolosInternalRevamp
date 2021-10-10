#include "Panel.h"

Panel::Panel(D3DXVECTOR4 vBounds, bool bMoveable, D3DCOLOR cColor, D3DCOLOR cColor2, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	
	m_bMoving	= false;
	m_bCanMove	= bMoveable;
	m_cColor1	= cColor;
	m_cColor2	= cColor2 ? cColor2 : cColor;

}

HRESULT Panel::Draw(ID3DXFont* pFont, Render* pRender) {
	if (m_bShouldDraw) {

		IGUIElement* pCurr = m_pFirstChild;
		HRESULT result = pRender->DrawRectangle(m_vBounds, pRender->LerpAlpha(m_cColor1, GetAnimLerp(FADE_LENGTH)), pRender->LerpAlpha(m_cColor2, GetAnimLerp(FADE_LENGTH)), true);
		while (pCurr && result == S_OK) {
			result = pCurr->Draw(pFont, pRender);
			pCurr = pCurr->GetSibling();
		}
		return result;
	}
	return S_OK;

}

bool Panel::OnClick(D3DXVECTOR2 vLocation) {
	if (CheckBounds(vLocation) && m_bShouldDraw) {
		
		
		m_vMoveOrigin = vLocation;
		m_bMoving = m_bCanMove;
		
		return true;
	}

	return false;
}

bool Panel::OnDrag(D3DXVECTOR2 vLocation) {
	if (m_bShouldDraw && m_bMoving) {
			
		D3DXVECTOR2 vMoveDelta = { vLocation.x - m_vMoveOrigin.x, vLocation.y - m_vMoveOrigin.y };
		m_vMoveOrigin = vLocation;

		MoveDelta(vMoveDelta);
		return true;
		
	}

	return false;

}

bool Panel::OnRelease(D3DXVECTOR2 vLocation) {
	m_bMoving = false;

	if (CheckBounds(vLocation)) {
		true;
	}
	return false;
}

