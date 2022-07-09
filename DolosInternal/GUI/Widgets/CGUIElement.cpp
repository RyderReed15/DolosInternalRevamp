#include "IGUIElement.h"


IGUIElement::IGUIElement(D3DXVECTOR4 vBounds, IGUIElement* pParent) {

	m_bShouldDraw		= true;
	m_bDrawOnTop		= false;
	m_bEnabled			= true;
	m_iChildCount		= 0;
	m_pFirstChild		= nullptr;
	m_pSibling			= nullptr;
	m_pParent			= nullptr;
	m_iAnimStartTick	= 0;

	m_vBounds			= vBounds;

	if (pParent) {
		pParent->AddChild(this);
		D3DXVECTOR2 vCoords = pParent->GetCoords();
		m_vBounds.x += vCoords.x;
		m_vBounds.y += vCoords.y;
	}
}

IGUIElement::~IGUIElement(void)
{
	return;
}

void IGUIElement::OnClick(GUIEventHandler* pEventHandler, POINT ptLocation) {
	
	return;
}
void IGUIElement::OnDrag(GUIEventHandler* pEventHandler, POINT ptLocation) {
	return;
}
void IGUIElement::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	return;
}
void IGUIElement::OnHover(GUIEventHandler* pEventHandler, POINT ptLocation) {
	return;
}
void IGUIElement::OnScroll(GUIEventHandler* pEventHandler, POINT ptLocation, short zDelta) {
	return;
}
void IGUIElement::OnType(GUIEventHandler* pEventHandler, char chKey) {
	return;
}
void IGUIElement::OnKey(GUIEventHandler* pEventHandler, char chKey, long keyInfo) {
	return;
}

void IGUIElement::SetCallback(GUI_EVENT_TYPE tEvent, std::function<void()> pCallback){
	switch (tEvent)
	{
	case GUI_EVENT_TYPE::CLICK:
		m_pClickCallback = pCallback;
		break;
	case GUI_EVENT_TYPE::DRAG:
		m_pDragCallback = pCallback;
		break;
	case GUI_EVENT_TYPE::RELEASE:
		m_pReleaseCallback = pCallback;
		break;
	case GUI_EVENT_TYPE::HOVER:
		m_pHoverCallback = pCallback;
		break;
	case GUI_EVENT_TYPE::KEYDOWN:
		m_pTypeCallback = pCallback;
		break;
	case GUI_EVENT_TYPE::SCROLL:
		m_pScrollCallback = pCallback;
		break;
	case GUI_EVENT_TYPE::FOCUS:
		m_pFocusCallback = pCallback;
		break;
	case GUI_EVENT_TYPE::UNFOCUS:
		m_pUnfocusCallback = pCallback;
		break;
	case GUI_EVENT_TYPE::BUTTON:
	default:
		break;
	}
}

std::function<void()> IGUIElement::GetCallback(GUI_EVENT_TYPE tEvent){
	switch (tEvent)
	{
	case GUI_EVENT_TYPE::CLICK:		return m_pClickCallback;
	case GUI_EVENT_TYPE::DRAG:		return m_pDragCallback;
	case GUI_EVENT_TYPE::RELEASE:	return m_pReleaseCallback;
	case GUI_EVENT_TYPE::HOVER:		return m_pHoverCallback;
	case GUI_EVENT_TYPE::KEYDOWN:	return m_pTypeCallback;
	case GUI_EVENT_TYPE::SCROLL:	return m_pScrollCallback;
	case GUI_EVENT_TYPE::FOCUS:		return m_pFocusCallback;
	case GUI_EVENT_TYPE::UNFOCUS:	return m_pUnfocusCallback;
	case GUI_EVENT_TYPE::BUTTON:
	default:
		return nullptr;
	}

}

void IGUIElement::RunCallback(GUI_EVENT_TYPE tEvent){

	switch (tEvent)
	{
	case GUI_EVENT_TYPE::CLICK:		{if (m_pClickCallback)	m_pClickCallback(); break; }
	case GUI_EVENT_TYPE::DRAG:		{if (m_pDragCallback)	m_pDragCallback(); break; }
	case GUI_EVENT_TYPE::RELEASE:	{if (m_pReleaseCallback)m_pReleaseCallback(); break; }
	case GUI_EVENT_TYPE::HOVER:		{if (m_pHoverCallback)	m_pHoverCallback(); break; }
	case GUI_EVENT_TYPE::KEYDOWN:	{if (m_pTypeCallback)	m_pTypeCallback(); break; }
	case GUI_EVENT_TYPE::SCROLL:	{if (m_pScrollCallback)	m_pScrollCallback(); break; }
	case GUI_EVENT_TYPE::FOCUS:		{if (m_pFocusCallback)	m_pFocusCallback(); break; }
	case GUI_EVENT_TYPE::UNFOCUS:	{if (m_pUnfocusCallback)m_pUnfocusCallback(); break; }
	case GUI_EVENT_TYPE::BUTTON:
	default:
		break;
	}
}

void IGUIElement::SetAnimStartTick(void) {
	m_iAnimStartTick = GetTickCount();
}
float IGUIElement::GetAnimLerp(float flAnimLength) {
	if (m_iAnimStartTick == 0) {
		return 1;
	}
	float flAnimLerp = ((GetTickCount() - m_iAnimStartTick) / 1000.f) / flAnimLength;
	if (flAnimLerp > 1) {
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

void IGUIElement::SetDrawOnTop(bool bDrawState) {
	m_bDrawOnTop = bDrawState;
}
bool IGUIElement::GetDrawOnTop(void) {
	return m_bDrawOnTop;
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

D3DXVECTOR4 IGUIElement::GetBounds(void) {
	return m_vBounds;
}
bool IGUIElement::CheckBounds(POINT ptLocation) {
	return (ptLocation.x >= m_vBounds.x && ptLocation.x <= m_vBounds.x + m_vBounds.z && ptLocation.y >= m_vBounds.y && ptLocation.y <= m_vBounds.y + m_vBounds.w);
}
void IGUIElement::MoveDelta(POINT ptLocation) {
	m_vBounds.x += ptLocation.x;
	m_vBounds.y += ptLocation.y;
	IGUIElement* pCurr = m_pFirstChild;
	while (pCurr) {
		pCurr->MoveDelta(ptLocation);
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

void IGUIElement::RemoveChild(IGUIElement* pElement) {
	if (m_pFirstChild) {
		IGUIElement* pCurr = m_pFirstChild;
		IGUIElement* pPrev = nullptr;
		while (pCurr->GetSibling() && pCurr != pElement) {
			pPrev = pCurr;
			pCurr = pCurr->GetSibling();
		}
		if (pCurr == pElement) {
			if (pPrev) pPrev->SetSibling(pCurr->GetSibling());
			else if (pCurr->GetSibling()) m_pFirstChild = pCurr->GetSibling();
			else m_pFirstChild = nullptr;
			
		}
		else return;

		m_iChildCount--;
	}
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

IGUIElement* IGUIElement::GetRoot(void) {
	IGUIElement* pCurr = this;
	while (pCurr->GetParent()) {
		pCurr = pCurr->GetParent();
	}
	return pCurr;
}