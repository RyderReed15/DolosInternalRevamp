#include "DropDown.h"

DropDown::DropDown(const char* szName, int* pValue, std::map<int, std::string>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent) : IValueElement(vBounds, pParent) {
	
	m_cColor				= cColor;
	m_pValue				= pValue;
	m_iHover				= *pValue;
	m_iNumDisplayElements	= iNumDisplayElements;
	m_szName				= szName;
	m_bWide					= false;

	D3DXVECTOR4 vContainerBounds(m_vBounds.z - flContainerSize, - TEXT_FEATURE_OFFSET, flContainerSize, m_vBounds.w);
	m_pContainer = new DropDownContainer<char>(pElements, iNumDisplayElements, vContainerBounds, cColor, this);

}


DropDown::DropDown(const wchar_t* szName, int* pValue, std::map<int, std::wstring>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent) : IValueElement(vBounds, pParent) {
	m_cColor				= cColor;
	m_pValue				= pValue;
	m_iHover				= *pValue;
	m_iNumDisplayElements	= iNumDisplayElements;
	m_wszName				= szName;
	m_bWide					= true;

	D3DXVECTOR4 vContainerBounds(m_vBounds.z - flContainerSize, - TEXT_FEATURE_OFFSET, flContainerSize, m_vBounds.w);
	m_pContainer = new DropDownContainer<wchar_t>(pElements, iNumDisplayElements, vContainerBounds, cColor, this);

}

DropDown::~DropDown() {
	if (m_pContainer) {
		delete m_pContainer;
	}
}

HRESULT DropDown::Draw(ID3DXFont* pFont, Render* pRender) {
	if (m_bWide) {
		D3DXVECTOR2 vSize = pRender->GetStringSize(pFont, m_wszName);
		pRender->DrawString({ m_vBounds.x, m_vBounds.y }, (m_bEnabled) ? WHITE : GRAY, pFont, m_wszName);
	}
	else {
		D3DXVECTOR2 vSize = pRender->GetStringSize(pFont, m_szName);
		pRender->DrawString({ m_vBounds.x, m_vBounds.y }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
	}

	m_pContainer->SetEnabled(m_bEnabled);

	return m_pContainer->Draw(pFont, pRender);

}

void DropDown::SetValue(int iValue) {
	if (m_pValue) {
		*m_pValue = iValue;
	}
}

void DropDown::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_pContainer->CheckBounds(ptLocation)) {
		m_pContainer->SetOpen(true);
		m_iHover = *m_pValue;
		//Set focus to container so that it can be clicked even without being indexed
		pEventHandler->SetFocus(m_pContainer);
	}

}

void DropDown::SetValuePointer(void* pValue) {
	m_pValue = static_cast<int*>(pValue);
}

void* DropDown::GetValuePointer() {
	return m_pValue;
}

int DropDown::GetHover() {
	return m_iHover;
}

void DropDown::SetHover(int iHover) {
	m_iHover = iHover;
}

void DropDown::SetMapPointerA(std::map<int, std::string>* pMap) {
	if (m_bWide) return;
	reinterpret_cast<DropDownContainer<char>*>(m_pContainer)->SetMapPointer(pMap);
}

std::map<int, std::string>* DropDown::GetMapPointerA(void) {
	if (m_bWide) return nullptr;
	return reinterpret_cast<DropDownContainer<char>*>(m_pContainer)->GetMapPointer();
}


void DropDown::SetMapPointerW(std::map<int, std::wstring>* pMap) {
	if (!m_bWide) return;
	reinterpret_cast<DropDownContainer<wchar_t>*>(m_pContainer)->SetMapPointer(pMap);
}

std::map<int, std::wstring>* DropDown::GetMapPointerW(void) {
	if (!m_bWide) return nullptr;
	return reinterpret_cast<DropDownContainer<wchar_t>*>(m_pContainer)->GetMapPointer();
}

IDropDownContainer::IDropDownContainer(unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {

	m_iTopIndex = 0;
	m_iNumDisplayElements = iNumDisplayElements;
	m_cColor = cColor;
	m_bOpen = false;

}

void IDropDownContainer::SetOpen(bool bOpen) {
	m_bOpen = bOpen;
	m_bDrawOnTop = bOpen;
	SetAnimStartTick();
}

bool IDropDownContainer::GetOpen() {
	return m_bOpen;
}

IDropDownContainer* DropDown::GetContainer(void) {
	return m_pContainer;
}