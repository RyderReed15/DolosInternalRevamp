#include "DropDown.h"
#include <iostream>

DropDown::DropDown(const char* szName, int* pValue, std::map<int, std::string>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent) : IValueElement(vBounds, pParent) {

	
	m_cColor				= cColor;
	m_szName				= szName;
	m_pValue				= pValue;
	m_iHover				= *pValue;
	m_iNumDisplayElements	= iNumDisplayElements;



	D3DXVECTOR4 vContainerBounds(m_vBounds.z - flContainerSize, -TEXT_FEATURE_OFFSET, flContainerSize, m_vBounds.w);
	m_pContainer	= new DropDownContainer(pElements, pElements->size(), vContainerBounds, cColor, this);
	
}

DropDown::~DropDown() {
	if (m_pContainer) {
		delete m_pContainer;
	}
}


void DropDown::SetValue(int iValue) {
	if (m_pValue) {
		*m_pValue = iValue;
	}
}


HRESULT DropDown::Draw(ID3DXFont* pFont, Render* pRender) {
	
	D3DXVECTOR2 vSize = pRender->GetStringSize(pFont, m_szName);
	pRender->DrawString({ m_vBounds.x, m_vBounds.y }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);

	m_pContainer->SetEnabled(m_bEnabled);

	return m_pContainer->Draw(pFont, pRender);

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

void DropDown::SetMapPointer(std::map<int, std::string>* pMap){
	m_pContainer->SetMapPointer(pMap);
}

std::map<int, std::string>* DropDown::GetMapPointer(void){
	return m_pContainer->GetMapPointer();
}

DropDownContainer* DropDown::GetContainer(void){
	return m_pContainer;
}

void DropDown::SetHover(int iHover) {
	m_iHover = iHover;
}

DropDownContainer::DropDownContainer(std::map<int, std::string>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	
	m_iTopIndex				= 0;
	m_iNumDisplayElements	= iNumDisplayElements;
	m_cColor				= cColor;
	m_pElements				= pElements;
	m_bOpen					= false;

}

HRESULT DropDownContainer::Draw(ID3DXFont* pFont, Render* pRender) {


	int pValue = *static_cast<int*>(((DropDown*)m_pParent)->GetValuePointer());
	int pHover = ((DropDown*)m_pParent)->GetHover();

	pRender->DrawSprite({ 0,0,32,32 }, { m_vBounds.x + m_vBounds.z - m_vBounds.w, m_vBounds.y }, WHITE, m_vBounds.w / 32.f, GetAnimLerp(FADE_LENGTH) * PI * m_bOpen);

	pRender->DrawRoundedRectangle(m_vBounds, ROUND_CORNER_SIZE, m_cColor);

	if (m_pElements->count(pValue)) pRender->DrawString({ m_vBounds.x + 5, m_vBounds.y + TEXT_FEATURE_OFFSET }, WHITE, pFont, m_pElements->at(pValue).c_str());
	else pRender->DrawString({ m_vBounds.x + 5, m_vBounds.y + TEXT_FEATURE_OFFSET }, WHITE, pFont, "ERROR");

	if (m_bOpen) {
		// Draw current selection
		pRender->DrawRoundedRectangle({ m_vBounds.x, m_vBounds.y + m_vBounds.w + TEXT_FEATURE_OFFSET, m_vBounds.z, m_vBounds.w * (m_pElements->size() * BOX_TEXT_RATIO + .25f) }, ROUND_CORNER_SIZE, LerpAlpha(m_cColor, GetAnimLerp(FADE_LENGTH)));
		
		int i = 0;
		for (std::map<int, std::string>::iterator it = m_pElements->begin(); it != m_pElements->end(); it++) {
			// Draw drop down menu - Increments with every item
			pRender->DrawString({ m_vBounds.x + 15, m_vBounds.w * .25f + m_vBounds.y + (i + 1) * m_vBounds.w * BOX_TEXT_RATIO - 3, }, (pHover == it->first) ? WHITE : GRAY, pFont, it->second.c_str());
			i++;
		}
	}

	return S_OK;

}

void DropDownContainer::SetOpen(bool bOpen) {
	m_bOpen = bOpen;
	SetAnimStartTick();
}

bool DropDownContainer::GetOpen() {
	return m_bOpen;
}


void DropDownContainer::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	
	//only ever called when bOpen is true because this element isn't indexed
	int i = 0;
	for (std::map<int, std::string>::iterator it = m_pElements->begin(); it != m_pElements->end(); it++) {
		//Check if the mouse is over this element
		D3DXVECTOR4 vBounds = { m_vBounds.x, m_vBounds.w * .25f + m_vBounds.y + (i + 1) * m_vBounds.w * BOX_TEXT_RATIO - 3, m_vBounds.z, m_vBounds.w * BOX_TEXT_RATIO };
		if (ptLocation.x >= vBounds.x && ptLocation.x <= vBounds.x + vBounds.z && ptLocation.y >= vBounds.y && ptLocation.y <= vBounds.y + vBounds.w) {

			((DropDown*)m_pParent)->SetValue(it->first); // Update value if it is
			break;
		}
		i++;
		
	}
	
	//for (unsigned int i = m_iTopIndex; i < m_iTopIndex + m_iNumDisplayElements && i < m_iNumElements; i++) {
	
	pEventHandler->ReleaseFocus();
	m_bOpen = false;
}
void DropDownContainer::OnHover(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_bOpen) {
		int i = 0;
		for (std::map<int, std::string>::iterator it = m_pElements->begin(); it != m_pElements->end(); it++) {

			//Check if the mouse is over this element
			D3DXVECTOR4 vBounds = { m_vBounds.x, m_vBounds.w * .25f + m_vBounds.y + (i + 1) * m_vBounds.w * BOX_TEXT_RATIO - 3, m_vBounds.z, m_vBounds.w * BOX_TEXT_RATIO };
			if (ptLocation.x >= vBounds.x && ptLocation.x <= vBounds.x + vBounds.z && ptLocation.y >= vBounds.y && ptLocation.y <= vBounds.y + vBounds.w) {

				((DropDown*)m_pParent)->SetHover(it->first); // Set hover value for use in draw
			}
			i++;
		}
	}
}

void DropDownContainer::SetMapPointer(std::map<int, std::string>* pMap) {
	m_pElements = pMap;
}

std::map<int, std::string>* DropDownContainer::GetMapPointer(void) {
	return m_pElements;
}

