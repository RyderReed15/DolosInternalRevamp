#include "Panel.h"

Panel::Panel(D3DXVECTOR4 vBounds, bool bMoveable, D3DCOLOR cColor, D3DCOLOR cColor2, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	
	m_pMoveOrigin	= { 0, 0 };
	m_bMoving		= false;
	m_bCanMove		= bMoveable;
	m_cColor1		= cColor;
	m_cColor2		= cColor2 ? cColor2 : cColor;

}

HRESULT Panel::Draw(ID3DXFont* pFont, Render* pRender) {
	if (m_bShouldDraw) {

	
		HRESULT result = pRender->DrawRectangle(m_vBounds, pRender->LerpAlpha(m_cColor1, GetAnimLerp(FADE_LENGTH)), pRender->LerpAlpha(m_cColor2, GetAnimLerp(FADE_LENGTH)), true);
		
	}
	return S_OK;

}

bool Panel::OnClick(POINT pLocation) {
	if (CheckBounds(pLocation) && m_bShouldDraw) {
		
		
		m_pMoveOrigin = pLocation;
		m_bMoving = m_bCanMove;
		
		return true;
	}

	return false;
}

bool Panel::OnDrag(POINT pLocation) {
	if (m_bShouldDraw && m_bMoving) {
			
		POINT pMoveDelta = { pLocation.x - m_pMoveOrigin.x, pLocation.y - m_pMoveOrigin.y };
		m_pMoveOrigin = pLocation;

		MoveDelta(pMoveDelta);
		return true;
		
	}

	return false;

}

bool Panel::OnRelease(POINT pLocation) {
	m_bMoving = false;

	if (CheckBounds(pLocation)) {
		true;
	}
	return false;
}

