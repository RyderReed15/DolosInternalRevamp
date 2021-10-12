#include "Panel.h"

Panel::Panel(D3DXVECTOR4 vBounds, bool bMoveable, D3DCOLOR cColor, D3DCOLOR cColor2, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	
	m_ptMoveOrigin	= { 0, 0 };
	m_bMoving		= false;
	m_bCanMove		= bMoveable;
	m_cColor1		= cColor;
	m_cColor2		= cColor2 ? cColor2 : cColor;

}

HRESULT Panel::Draw(ID3DXFont* pFont, Render* pRender) {
	
	return pRender->DrawRectangle(m_vBounds, pRender->LerpAlpha(m_cColor1, GetAnimLerp(FADE_LENGTH)), pRender->LerpAlpha(m_cColor2, GetAnimLerp(FADE_LENGTH)), true);;

}

void Panel::OnClick(GUIEventHandler* pEventHandler, POINT ptLocation) {
	
	m_ptMoveOrigin = ptLocation;
	m_bMoving = m_bCanMove;
		
}

void Panel::OnDrag(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_bMoving) {
			
		POINT ptMoveDelta = { ptLocation.x - m_ptMoveOrigin.x, ptLocation.y - m_ptMoveOrigin.y };
		m_ptMoveOrigin = ptLocation;

		MoveDelta(ptMoveDelta);
	}
}

void Panel::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_bMoving) {
		pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::RELEASE, pEventHandler->BuildFunction(&GUIContainer::GenerateMap, pEventHandler->GetContainer()));
	}
	m_bMoving = false;
}

