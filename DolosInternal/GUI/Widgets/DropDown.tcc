#include "DropDown.h"
template <typename T>
DropDownContainer<T>::DropDownContainer(std::map<int, std::basic_string<T>>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent) : IDropDownContainer(iNumDisplayElements, vBounds, cColor, pParent) {

	m_iTopIndex = 0;
	m_iNumDisplayElements = iNumDisplayElements;
	m_cColor = cColor;
	m_pElements = pElements;
	m_bOpen = false;

}

template <typename T>
HRESULT DropDownContainer<T>::Draw(ID3DXFont* pFont, Render* pRender) {


	int pValue = *static_cast<int*>(((DropDown*)m_pParent)->GetValuePointer());
	int pHover = ((DropDown*)m_pParent)->GetHover();

	pRender->DrawSprite({ 0,0,32,32 }, { m_vBounds.x + m_vBounds.z - m_vBounds.w, m_vBounds.y }, WHITE, m_vBounds.w / 32.f, GetAnimLerp(FADE_LENGTH) * PI * m_bOpen);

	pRender->DrawRoundedRectangle(m_vBounds, ROUND_CORNER_SIZE, m_cColor);

	if (m_pElements->count(pValue)) pRender->DrawString({ m_vBounds.x + 5, m_vBounds.y + TEXT_FEATURE_OFFSET }, WHITE, pFont, m_pElements->at(pValue).c_str());
	else pRender->DrawString({ m_vBounds.x + 5, m_vBounds.y + TEXT_FEATURE_OFFSET }, WHITE, pFont, "ERROR");

	if (m_bOpen) {
		// Draw current selection
		pRender->DrawRoundedRectangle({ m_vBounds.x, m_vBounds.y + m_vBounds.w + TEXT_FEATURE_OFFSET, m_vBounds.z, m_vBounds.w * (m_iNumDisplayElements * BOX_TEXT_RATIO + .25f) }, ROUND_CORNER_SIZE, LerpAlpha(m_cColor, GetAnimLerp(FADE_LENGTH)));

		typename std::map<int, std::basic_string<T>>::iterator it = m_pElements->begin();
		std::advance(it, m_iTopIndex);
		for (unsigned int i = 0; i < m_iNumDisplayElements && it != m_pElements->end(); it++, i++) {
			// Draw drop down menu - Increments with every item
			pRender->DrawString({ m_vBounds.x + 15, m_vBounds.w * .25f + m_vBounds.y + (i + 1) * m_vBounds.w * BOX_TEXT_RATIO - 3, }, (pHover == it->first) ? WHITE : GRAY, pFont, it->second.c_str());
		}
	}

	return S_OK;

}

template <typename T>
void DropDownContainer<T>::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	//The element slecected can be done with math instead of loops


	//only ever called when bOpen is true because this element isn't indexed
	typename std::map<int, std::basic_string<T>>::iterator it = m_pElements->begin();
	std::advance(it, m_iTopIndex);
	for (unsigned int i = 0; i < m_iNumDisplayElements && it != m_pElements->end(); it++, i++) {
		//Check if the mouse is over this element
		D3DXVECTOR4 vBounds = { m_vBounds.x, m_vBounds.w * .25f + m_vBounds.y + (i + 1) * m_vBounds.w * BOX_TEXT_RATIO - 3, m_vBounds.z, m_vBounds.w * BOX_TEXT_RATIO };
		if (ptLocation.x >= vBounds.x && ptLocation.x <= vBounds.x + vBounds.z && ptLocation.y >= vBounds.y && ptLocation.y <= vBounds.y + vBounds.w) {

			((DropDown*)m_pParent)->SetValue(it->first); // Update value if it is
			break;
		}

	}

	//for (unsigned int i = m_iTopIndex; i < m_iTopIndex + m_iNumDisplayElements && i < m_iNumElements; i++) {

	pEventHandler->ReleaseFocus();
	m_bDrawOnTop = false;
	m_bOpen = false;
}
template <typename T>
void DropDownContainer<T>::OnHover(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_bOpen) {
		typename std::map<int, std::basic_string<T>>::iterator it = m_pElements->begin();
		std::advance(it, m_iTopIndex);
		for (unsigned int i = 0; i < m_iNumDisplayElements && it != m_pElements->end(); it++, i++) {

			//Check if the mouse is over this element
			D3DXVECTOR4 vBounds = { m_vBounds.x, m_vBounds.w * .25f + m_vBounds.y + (i + 1) * m_vBounds.w * BOX_TEXT_RATIO - 3, m_vBounds.z, m_vBounds.w * BOX_TEXT_RATIO };
			if (ptLocation.x >= vBounds.x && ptLocation.x <= vBounds.x + vBounds.z && ptLocation.y >= vBounds.y && ptLocation.y <= vBounds.y + vBounds.w) {

				((DropDown*)m_pParent)->SetHover(it->first); // Set hover value for use in draw
			}
		}
	}
}

template <typename T>
void DropDownContainer<T>::OnScroll(GUIEventHandler* pEventHandler, POINT ptLocation, short zDelta) {
	if (m_bOpen) {
		if (m_pElements->size() < m_iNumDisplayElements) {
			m_iTopIndex = 0;
			return;
		}
		m_iTopIndex -= zDelta / 120;
		if (m_iTopIndex > m_pElements->size() + 10) m_iTopIndex = 0;
		else m_iTopIndex = min(m_pElements->size() - m_iNumDisplayElements, m_iTopIndex);

	}
}
template <typename T>
void DropDownContainer<T>::SetMapPointer(std::map<int, std::basic_string<T>>* pMap) {
	m_pElements = pMap;
	m_iTopIndex = 0;
}

template <typename T>
std::map<int, std::basic_string<T>>* DropDownContainer<T>::GetMapPointer(void) {
	return m_pElements;
}