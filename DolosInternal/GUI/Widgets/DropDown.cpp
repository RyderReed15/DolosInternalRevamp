#include "DropDown.h"
#include <iostream>

DropDown::DropDown(const char* szName, int iDefaultIndex, int* pValue, DropDownElement* pElemementArray, int iNumElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {

	
	m_cColor		= cColor;
	m_iIndex		= iDefaultIndex;
	m_szName		= szName;
	m_iArraySize	= iNumElements;
	m_pValue		= pValue;



	D3DXVECTOR4 vContainerBounds(m_vBounds.z - flContainerSize, -2, flContainerSize, m_vBounds.w);
	m_pContainer	= new DropDownContainer(pElemementArray, iNumElements, vContainerBounds, iNumElements, iDefaultIndex, cColor, this);
	
}

DropDown::~DropDown() {
	if (m_pContainer) {
		delete m_pContainer;
	}
}

void DropDown::SetIndex(int iIndex) {
	m_iIndex = iIndex;
}
void DropDown::SetValue(int iValue) {
	if (m_pValue) {
		*m_pValue = iValue;
	}

}

HRESULT DropDown::Draw(ID3DXFont* pFont, Render* pRender) {
	
	D3DXVECTOR2 vSize = pRender->GetStringSize(pFont, m_szName);
	pRender->DrawString({ m_vBounds.x, m_vBounds.y }, (m_bEnabled && m_iIndex) ? WHITE : GRAY, pFont, m_szName);

	m_pContainer->SetEnabled(m_bEnabled);

	return m_pContainer->Draw(pFont, pRender);

}

void DropDown::OnClick(GUIEventHandler* pEventHandler, POINT ptLocation) {
	
}
void DropDown::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_pContainer->CheckBounds(ptLocation)) {
		m_pContainer->SetOpen(true);
		//Set focus to container so that it can be clicked even without being indexed
		pEventHandler->SetFocus(m_pContainer);
	}
	
}

DropDownContainer::DropDownContainer(DropDownElement* pElementArray, int iNumElements, D3DXVECTOR4 vBounds, int iNumDisplayElements, int iStartIndex, D3DCOLOR cColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	
	m_iTopIndex				= iStartIndex;
	m_iIndex				= iStartIndex;
	m_iNumDisplayElements	= iNumDisplayElements;
	m_cColor				= cColor;
	m_pElementArray			= pElementArray;
	m_iNumElements			= iNumElements;
	m_bOpen					= false;

}

HRESULT DropDownContainer::Draw(ID3DXFont* pFont, Render* pRender) {
	

	pRender->DrawSprite({ 0,0,32,32 }, { m_vBounds.x + m_vBounds.z - m_vBounds.w, m_vBounds.y }, WHITE, m_vBounds.w / 32.f, GetAnimLerp(FADE_LENGTH) * PI * m_bOpen);

	pRender->DrawRoundedRectangle(m_vBounds, 2, m_cColor);

	pRender->DrawString({ m_vBounds.x + 5, m_vBounds.y + 2 }, (m_iIndex) ? WHITE : GRAY, pFont, m_pElementArray[m_iIndex].GetName());

	if (m_bOpen) {
		pRender->DrawRoundedRectangle({ m_vBounds.x, m_vBounds.y + m_vBounds.w + 2, m_vBounds.z, m_vBounds.w * (m_iNumDisplayElements * 1.3f + .25f) }, 2, pRender->LerpAlpha(m_cColor, GetAnimLerp(FADE_LENGTH)));
			
		for (int i = m_iTopIndex; i < m_iTopIndex + m_iNumDisplayElements && i < m_iNumElements; i++) {

			m_pElementArray[i].SetBounds({ m_vBounds.x, m_vBounds.w * .25f + m_vBounds.y + (i + 1) * m_vBounds.w * 1.3f - 3, m_vBounds.z, m_vBounds.w * 1.3f });
			m_pElementArray[i].Draw(pFont, pRender);
		}
	}

	return S_OK;

}

void DropDownContainer::SetOpen(bool bOpen) {
	m_bOpen = bOpen;
	SetAnimStartTick();
	std::cout << m_iAnimStartTick << std::endl;
}

bool DropDownContainer::GetOpen() {
	return m_bOpen;
}


void DropDownContainer::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	
	//only ever called when bOpen is true because this element isn't indexed
	for (int i = m_iTopIndex; i < m_iTopIndex + m_iNumDisplayElements && i < m_iNumElements; i++) {
		if (m_pElementArray[i].CheckBounds(ptLocation)) {

			m_iIndex = i;

			((DropDown*)m_pParent)->SetIndex(i);
			((DropDown*)m_pParent)->SetValue(m_pElementArray[i].GetValue());

			break;
		}
	}
	
	pEventHandler->ReleaseFocus();
	m_bOpen = false;
}
void DropDownContainer::OnHover(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_bOpen) {

		for (int i = m_iTopIndex; i < m_iTopIndex + m_iNumDisplayElements && i < m_iNumElements; i++) {

			if (m_pElementArray[i].CheckBounds(ptLocation)) {
				m_pElementArray[i].OnHover(pEventHandler, ptLocation);

			}else{
				m_pElementArray[i].SetEnabled(false);
			}
		}
	}
}

DropDownElement::DropDownElement(const char* szName, int iValue) : IGUIElement({ 0,0,0,0 }, nullptr) {
	m_szName	= szName;
	m_iValue	= iValue;
	m_bEnabled	= false;
}

HRESULT DropDownElement::Draw(ID3DXFont* pFont, Render* pRender) {
	return pRender->DrawString({ m_vBounds.x + 15, m_vBounds.y }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
}

void DropDownElement::SetBounds(D3DXVECTOR4 vBounds) {
	m_vBounds.x = vBounds.x;
	m_vBounds.y = vBounds.y;
	m_vBounds.z = vBounds.z;
	m_vBounds.w = vBounds.w;
}

int DropDownElement::GetValue() {
	return m_iValue;
}

const char* DropDownElement::GetName() {
	return m_szName;
}

void DropDownElement::OnHover(GUIEventHandler* pEventHandler, POINT ptLocation) {
	m_bEnabled = true;
}