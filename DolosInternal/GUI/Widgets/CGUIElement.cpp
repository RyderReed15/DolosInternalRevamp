#include "IGUIElement.h"


IGUIElement::IGUIElement(D3DXVECTOR4 vBounds, IGUIElement* pParent) {
	m_vBounds = vBounds;

	if (pParent) {
		pParent->AddChild(this);
		D3DXVECTOR2 vCoords = pParent->GetCoords();
		m_vBounds.x += vCoords.x;
		m_vBounds.y += vCoords.y;
	}
}

bool IGUIElement::OnClick(D3DXVECTOR2 vLocation) {
	if (CheckBounds(vLocation)) {
		return true;
	}
	return false;
}
bool IGUIElement::OnDrag(D3DXVECTOR2 vLocation) {
	if (CheckBounds(vLocation)) {
		return true;
	}
	return false;
}
bool IGUIElement::OnRelease(D3DXVECTOR2 vLocation) {
	if (CheckBounds(vLocation)) {
		return true;
	}
	return false;
}
bool IGUIElement::OnHover(D3DXVECTOR2 vLocation) {
	if (CheckBounds(vLocation)) {
		return true;
	}
	return false;
}

void IGUIElement::SetAnimStartTick(void) {
	m_iAnimStartTick = GetTickCount();
}
float IGUIElement::GetAnimLerp(float flAnimLength) {
	if (m_iAnimStartTick == 0) {
		return 1;
	}
	float flAnimLerp = ((GetTickCount() - m_iAnimStartTick) / 1000.f) / flAnimLength;
	if (flAnimLength > 1) {
		m_iAnimStartTick = 0;
		return 1;
	}
	return flAnimLerp;
}

void IGUIElement::SetDrawState(bool bDrawState) {

	m_bShouldDraw = bDrawState;
}
bool IGUIElement::GetDrawState(void) {
	return m_bShouldDraw;
}
void IGUIElement::SetEnabled(bool bEnabled) {
	m_bEnabled = bEnabled;
}
bool IGUIElement::GetEnabled(void) {
	return m_bEnabled;
}

D3DXVECTOR2 IGUIElement::GetCoords(void) {
	return { m_vBounds.x, m_vBounds.y };
}
bool IGUIElement::CheckBounds(D3DXVECTOR2 vLocation) {
	return (vLocation.x >= m_vBounds.x && vLocation.x <= m_vBounds.x + m_vBounds.z && vLocation.y >= m_vBounds.y && vLocation.y <= m_vBounds.y + m_vBounds.w);
}
void IGUIElement::MoveDelta(D3DXVECTOR2 vLocation) {
	m_vBounds.x += vLocation.x;
	m_vBounds.y += vLocation.y;
	IGUIElement* pCurr = m_pFirstChild;
	while (pCurr) {
		pCurr->MoveDelta(vLocation);
		pCurr = pCurr->GetSibling();
	}
}

void IGUIElement::AddChild(IGUIElement* pElement) {
	if (m_pFirstChild) {
		IGUIElement* pCurr = m_pFirstChild;
		while (pCurr->GetSibling()) {
			pCurr = pCurr->GetSibling();
		}
		pCurr->SetSibling(pElement);
	}
	else {
		m_pFirstChild = pElement;
	}
	m_iChildCount++;
	pElement->SetParent(this);
}
IGUIElement* IGUIElement::GetFirstChild(void) {
	return m_pFirstChild;
}
void IGUIElement::SetSibling(IGUIElement* pElement) {
	m_pSibling = pElement;
}
IGUIElement* IGUIElement::GetSibling(void) {
	return m_pSibling;
}
void IGUIElement::SetParent(IGUIElement* pElement) {
	m_pParent = pElement;
}
IGUIElement* IGUIElement::GetParent(void) {
	return m_pParent;
}