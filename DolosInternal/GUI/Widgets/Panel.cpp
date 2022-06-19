#include "Panel.h"

Panel::Panel(D3DXVECTOR4 vBounds, bool bMoveable, D3DCOLOR cColor, D3DCOLOR cColorTwo, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	
	m_ptMoveOrigin	= { 0, 0 };
	m_bMoving		= false;
	m_bMoveable		= bMoveable;
	m_cColorOne		= cColor;
	m_cColorTwo		= cColorTwo ? cColorTwo : cColor;

}

HRESULT Panel::Draw(ID3DXFont*, Render* pRender) {
	
	return pRender->DrawRectangle(m_vBounds, LerpAlpha(m_cColorOne, GetAnimLerp(FADE_LENGTH)), LerpAlpha(m_cColorTwo, GetAnimLerp(FADE_LENGTH)), true);;

}

void Panel::OnClick(GUIEventHandler* pEventHandler, POINT ptLocation) {
	
	if (m_bMoveable) {
		pEventHandler->SetFocus(this); 
		m_ptMoveOrigin = ptLocation;
		m_bMoving = m_bMoveable;
	}
	else if (m_pParent) {
		m_pParent->OnClick(pEventHandler, ptLocation);
	}
		
}

void Panel::OnDrag(GUIEventHandler*, POINT ptLocation) {
	if (m_bMoving) {
			
		POINT ptMoveDelta = { ptLocation.x - m_ptMoveOrigin.x, ptLocation.y - m_ptMoveOrigin.y };
		m_ptMoveOrigin = ptLocation;

		MoveDelta(ptMoveDelta);
	}
	else if (m_pParent) {
		m_pParent->OnDrag(nullptr, ptLocation);
	}
}

void Panel::OnRelease(GUIEventHandler* pEventHandler, POINT) {
	
	if (m_bMoving) {
		pEventHandler->ReleaseFocus();
		pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::RELEASE, pEventHandler->BuildFunction(&GUIContainer::GenerateMap, pEventHandler->GetContainer()));
	}
	else if (m_pParent) {
		m_pParent->OnDrag(pEventHandler, { 0, 0 });
	}
	m_bMoving = false;
}

