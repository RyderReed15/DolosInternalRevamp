#include "Panel.h"

Panel::Panel(D3DXVECTOR4 vBounds, bool bMoveable, D3DCOLOR cColor, D3DCOLOR cColorTwo, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	
	m_ptMoveOrigin	= { 0, 0 };
	m_bMoving		= false;
	m_bMoveable		= bMoveable;
	m_cColorOne		= cColor;
	m_cColorTwo		= cColorTwo ? cColorTwo : cColor;

}

HRESULT Panel::Draw(ID3DXFont* pFont, Render* pRender) {
	
	return pRender->DrawRectangle(m_vBounds, LerpAlpha(m_cColorOne, GetAnimLerp(FADE_LENGTH)), LerpAlpha(m_cColorTwo, GetAnimLerp(FADE_LENGTH)), true);;

}

void Panel::OnClick(GUIEventHandler* pEventHandler, POINT ptLocation) {
	
	if (m_bMoveable) {
		pEventHandler->SetFocus(this); 
		m_ptMoveOrigin = ptLocation;
		m_bMoving = m_bMoveable;
	}
		
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
		pEventHandler->ReleaseFocus();
		pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::RELEASE, pEventHandler->BuildFunction(&GUIContainer::GenerateMap, pEventHandler->GetContainer()));
	}
	m_bMoving = false;
}

