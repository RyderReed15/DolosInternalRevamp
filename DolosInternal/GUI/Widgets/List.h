#pragma once

#ifndef GUI_LIST_H
#define GUI_LIST_H

#include "IGUIElement.h"
#include "IValueElement.h"

template <class T> class ListElement;
template <class T> class ElementEditor;

template <class T>
class GUIList : public IGUIElement {
public:

					//holds list elements
					GUIList			(const char* szName, float flElementSize, std::vector<T>* pArray, ElementEditor<T>* pEditor, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent = nullptr);
					~GUIList		(void);

	HRESULT			Draw			(ID3DXFont* pFont, Render* pRender);

	ListElement<T>* AddElement		(const char* szName, T* pStruct, ElementEditor<T>* pEditor);
	ListElement<T>*	AddElement		(const char* szName, ElementEditor<T>* pEditor);
	void			RemoveElement	(unsigned int iIndex);
	void			RemoveElement	(const char* szName);
	void			RemoveElement	(ListElement<T>* pElement);


	void			CreateDefault	(const char* szName, T* pStruct);

	void			OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);
	void			OnScroll		(GUIEventHandler* pEventHandler, POINT ptLocation, short zDelta);
private:
	D3DCOLOR					m_cColor;
	const char*					m_szName;
	std::vector<ListElement<T>*>m_vElements;
	float						m_flElementSize;
	float						m_flTotalSize;
	std::vector<T>*				m_pArray;
	ElementEditor<T>*			m_pEditor;
	ListElement<T>*				m_pDefault;
	unsigned int				m_iTopIndex;
	unsigned int				m_iNumDisplayElements;


};

template <class T>
class ListElement : public IGUIElement {
public:
			//Holds a structure for drawing on list and holding a pointer to the values
	friend	GUIList<T>;

				ListElement	(const char* szName, T* pStruct, ElementEditor<T>* pEditor, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent = nullptr);

	HRESULT		Draw		(ID3DXFont* pFont, Render* pRender);

	void		OnRelease	(GUIEventHandler* pEventHandler, POINT ptLocation);


	const char* GetName		(void);

	void		UpdateStruct(T* pStruct);
	T*			GetStruct	(void);
private:
	D3DCOLOR			m_cColor;
	const char*			m_szName;
	ElementEditor<T>*	m_pEditor;
	T*					m_pStruct;
	uintptr_t			m_iNameOffset;



};
template <class T>
class ElementEditor : public IGUIElement {
public:
			//Contains a structure to house a set of GUI elements for editing the list values
			ElementEditor	(T* pStruct, IGUIElement* pEditor);
			~ElementEditor	(void);

	HRESULT Draw			(ID3DXFont* pFont, Render* pRender);

	void	OnClick			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnDrag			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnHover			(GUIEventHandler* pEventHandler, POINT ptLocation);

	void	UpdateTarget	(T* pStruct);
	T*		GetTarget		(void);
private:
	
	std::unordered_map<unsigned int, IValueElement*>	m_mElements;
	std::vector<IGUIElement*>							m_vElements;
	T*													m_pStruct;
};










template <class T>
GUIList<T>::GUIList(const char* szName, float flElementSize, std::vector<T>* pArray, ElementEditor<T>* pEditor, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {

	m_cColor		= cColor;
	m_szName		= szName;
	m_flElementSize = flElementSize;
	m_flTotalSize	= flElementSize + 5;
	m_pArray		= pArray;
	m_pEditor		= pEditor;
	m_pDefault		= nullptr;
	m_vBounds.w		= m_flElementSize;
	m_iTopIndex		= 0;
	m_iNumDisplayElements = static_cast<int>((vBounds.w - m_flElementSize) / m_flTotalSize);

}
template <class T>
GUIList<T>::~GUIList() {

	if (m_pDefault) delete m_pDefault;
	m_pDefault = nullptr;

}

template <class T>
HRESULT GUIList<T>::Draw(ID3DXFont* pFont, Render* pRender) {

	pRender->DrawString({ m_vBounds.x, m_vBounds.y - TEXT_FEATURE_OFFSET }, WHITE, pFont, m_szName);
	pRender->DrawSprite({ 288,0,32,32 }, { m_vBounds.x + m_vBounds.z - 20, m_vBounds.y - TEXT_FEATURE_OFFSET }, WHITE, .66f);
	//pRender->DrawRectangle(m_vBounds, LerpAlpha(m_cColor, GetAnimLerp(FADE_LENGTH)), LerpAlpha(m_cColor, GetAnimLerp(FADE_LENGTH)), true);

	for (size_t i = m_iTopIndex; (i - m_iTopIndex) < m_iNumDisplayElements && i < m_vElements.size(); i++) {
		m_vElements[i]->Draw(pFont, pRender);
	}

	return S_OK;
}
template <class T>
ListElement<T>* GUIList<T>::AddElement(const char* szName, T* pStruct, ElementEditor<T>* pEditor) {
	ListElement<T>* pElem = new ListElement<T>(szName, pStruct, pEditor, { 10, m_vElements.size() * m_flTotalSize + m_flElementSize, m_vBounds.z - 20, m_flElementSize }, m_cColor, this);

	m_vElements.push_back(pElem);
	
	for (size_t i = 0; i < m_vElements.size(); i++) {
		//Pointers need to be update everytime and element is added because the position in the vector could change
		m_vElements[i]->UpdateStruct(&(m_pArray->at(i)));
	}

	m_vBounds.w = min(m_iNumDisplayElements, m_vElements.size()) * m_flTotalSize + m_flElementSize;
	return pElem;
}

template <class T>
ListElement<T>* GUIList<T>::AddElement(const char* szName, ElementEditor<T>* pEditor) {
	if (m_pDefault && !szName) {

		T* pStruct = m_pDefault->GetStruct();
		m_pArray->push_back(T(*pStruct));
		if ((uintptr_t)m_pDefault->GetName() - (uintptr_t)pStruct < sizeof(T)) {

			szName = m_pDefault->GetName() - (char*)pStruct + (char*)&(m_pArray->at(m_pArray->size() - 1));
		}
		else {
			szName = "Default";
		}
	}
	else {
		m_pArray->push_back(T());
	}
	


	return AddElement(szName, &(m_pArray->at(m_pArray->size() - 1)), pEditor);
	
}
template <class T>
void GUIList<T>::RemoveElement(unsigned int iIndex) {
	RemoveChild(m_vElements[iIndex]);
	delete m_vElements[iIndex];
	m_pArray->erase(m_pArray->begin() + iIndex);
	m_vElements.erase(m_vElements.begin() + iIndex);
	if (m_iTopIndex != 0) m_iTopIndex--;
	for (size_t i = 0; i < m_vElements.size(); i++) {
		//Pointers need to be update everytime and element is added because the position in the vector could change
		//Bounds need to be updated to fill gap
		m_vElements[i]->m_vBounds.y = m_vBounds.y + (i - m_iTopIndex) * m_flTotalSize + m_flElementSize;
		m_vElements[i]->UpdateStruct(&(m_pArray->at(i)));
	}

}
template <class T>
void GUIList<T>::RemoveElement(const char* szName) {
	for (size_t i = 0; i < m_vElements.size(); i++) {
		if (!strcmp(m_vElements[i]->m_szName, szName)) {
			RemoveElement(i);
			break;
		}
	}

}
template <class T>
void GUIList<T>::RemoveElement(ListElement<T>* pElement) {
	for (size_t i = 0; i < m_vElements.size(); i++) {
		if (m_vElements[i] == pElement) {
			RemoveElement(i);
			break;
		}
	}
}
template <class T>
void GUIList<T>::CreateDefault(const char* szName, T* pStruct) {
	if (m_pDefault) delete m_pDefault;
	ListElement<AimTarget>* pDefault = new ListElement<AimTarget>(szName, pStruct, nullptr, {}, 0);
	m_pDefault = pDefault;
}

template <class T>
void GUIList<T>::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (ptLocation.x > m_vBounds.x + m_vBounds.z - 20 && ptLocation.y < m_vBounds.y + 20) {
		AddElement(nullptr, m_pEditor);
		m_pEditor->SetDrawState(false);
		pEventHandler->GetContainer()->GenerateMap();

	}
	else if (ptLocation.y - m_vBounds.y < m_vElements.size() * m_flTotalSize + m_flElementSize - 5 && !m_vElements.empty()) {
		m_vElements[m_iTopIndex + static_cast<int>((ptLocation.y - m_vBounds.y - (m_flTotalSize + 5)) / m_flTotalSize + .5f)]->OnRelease(pEventHandler, ptLocation);
		//Select element based on the position fo the mouse and math using the size of each element
	}
	else if (m_pParent) {
		m_pParent->OnRelease(pEventHandler, { 0, 0 });
	}

}

template <class T>
void GUIList<T>::OnScroll(GUIEventHandler* pEventHandler, POINT ptLocation, short zDelta) {
	if (m_pArray->size() < m_iNumDisplayElements) {
		m_iTopIndex = 0;
		return;
	}

	m_iTopIndex -= zDelta / 120;
	if (m_iTopIndex > m_pArray->size() + 10) m_iTopIndex = 0;
	else m_iTopIndex = min(m_pArray->size() - m_iNumDisplayElements, m_iTopIndex);

	for (size_t i = m_iTopIndex; (i - m_iTopIndex) < m_iNumDisplayElements && i < m_vElements.size(); i++) {
		m_vElements[i]->m_vBounds.y = m_vBounds.y + (i - m_iTopIndex) * m_flTotalSize + m_flElementSize;
	}

	
}










template <class T>
ListElement<T>::ListElement(const char* szName, T* pStruct, ElementEditor<T>* pEditor, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {

	
	m_szName		= szName;
	m_cColor		= cColor;
	m_pEditor		= pEditor;
	m_pStruct		= pStruct;


	if ((uintptr_t)szName - (uintptr_t)pStruct < sizeof(T)) {

		m_iNameOffset = (uintptr_t)szName - (uintptr_t)pStruct;
	}else{
		m_iNameOffset = SIZE_MAX;
	}
}

template <class T>
HRESULT ListElement<T>::Draw(ID3DXFont* pFont, Render* pRender) {

	pRender->DrawString({ m_vBounds.x + 5, m_vBounds.y }, WHITE, pFont, m_szName);
	pRender->DrawSprite({ 320,0,32,32 }, { m_vBounds.x + m_vBounds.z - 20, m_vBounds.y }, WHITE, .66f);
	pRender->DrawSprite({ 448,352,32,32 }, { m_vBounds.x + m_vBounds.z - 40, m_vBounds.y + TEXT_FEATURE_OFFSET }, WHITE, .50f);
	pRender->DrawRoundedRectangle(m_vBounds, 5, LerpAlpha(m_cColor, GetAnimLerp(FADE_LENGTH)), LerpAlpha(m_cColor, GetAnimLerp(FADE_LENGTH)), true);

	return S_OK;
}

template <class T>
void ListElement<T>::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (ptLocation.x > m_vBounds.x + m_vBounds.z - 20) {
		//remove this element
		((GUIList<T>*)m_pParent)->RemoveElement(this);
		m_pEditor->SetDrawState(false);
		pEventHandler->GetContainer()->GenerateMap();
	}
	else if (ptLocation.x > m_vBounds.x + m_vBounds.z - 40) {
		//Update target to this one and toggle draw state
		if (m_pEditor) {
			if (m_pEditor->GetTarget() == m_pStruct) {
				m_pEditor->SetDrawState(!m_pEditor->GetDrawState());
			}
			else {
				m_pEditor->SetDrawState(true);
			}

			m_pEditor->UpdateTarget(m_pStruct);
			pEventHandler->GetContainer()->GenerateMap();
		}

	}

}

template <class T>
const char* ListElement<T>::GetName() {
	return m_szName;
}

template <class T>
void ListElement<T>::UpdateStruct(T* pStruct) {
	m_pStruct = pStruct;
	if (m_iNameOffset != SIZE_MAX) {
		m_szName = (char*)pStruct + m_iNameOffset;
	}
}

template <class T>
T* ListElement<T>::GetStruct() {
	return m_pStruct;
}










template <class T>
ElementEditor<T>::ElementEditor(T* pStruct, IGUIElement* pEditor) : IGUIElement(pEditor->GetBounds(), nullptr) {

	//Loop through all children and store elements with editable values in a seperate array
	IGUIElement* pCurr = pEditor;
	while (pCurr) {
		IValueElement* pInstance = dynamic_cast<IValueElement*>(pCurr); //will return nullptr if element is not an instance of IValueElement
		if (pInstance) {
			unsigned int iOffset = reinterpret_cast<unsigned int>(pInstance->GetValuePointer()) - reinterpret_cast<unsigned int>(pStruct);
			m_mElements[iOffset] = pInstance;
		}
		else {
			m_vElements.push_back(pCurr);
		}
		if(pCurr == pEditor) pCurr = pCurr->GetFirstChild();
		else pCurr = pCurr->GetSibling();
		
		
	}
	delete pStruct;
	m_pStruct = nullptr;
	m_bShouldDraw = false;
	pEditor->GetParent()->AddChild(this); 
	// Parent shoudl be set when creating the top level element passed to pEditor
	// Assigning the parent through IGuiElement causes an offset between display and interaction
}

template <class T>
ElementEditor<T>::~ElementEditor() {

	for (std::unordered_map<unsigned int, IValueElement*>::iterator it = m_mElements.begin(); it != m_mElements.end(); it++) {
		delete it->second;
	}
	for (std::vector<IGUIElement*>::iterator it = m_vElements.begin(); it != m_vElements.end(); it++) {
		delete	*it;
	}

}

template <class T>
HRESULT ElementEditor<T>::Draw(ID3DXFont* pFont, Render* pRender) {
	if (!m_pStruct) return S_OK;
	for (std::vector<IGUIElement*>::iterator it = m_vElements.begin(); it != m_vElements.end(); it++) {
		(*it)->Draw(pFont, pRender);
	}
	for (std::unordered_map<unsigned int, IValueElement*>::iterator it = m_mElements.begin(); it != m_mElements.end(); it++) {
		it->second->Draw(pFont, pRender);
	}
	
	return S_OK;
}
template <class T>
void ElementEditor<T>::OnClick(GUIEventHandler* pEventHandler, POINT ptLocation) {
	for (std::unordered_map<unsigned int, IValueElement*>::iterator it = m_mElements.begin(); it != m_mElements.end(); it++) {
		if (it->second && it->second->CheckBounds(ptLocation)) {
			it->second->OnClick(pEventHandler, ptLocation);
			return;
		}
	}
	//First one is likely to be a panel so check it last
	for (std::vector<IGUIElement*>::iterator it = m_vElements.begin(); it != m_vElements.end(); it++) {
		if (*it && (*it)->CheckBounds(ptLocation)) {
			(*it)->OnClick(pEventHandler, ptLocation);
			return;
		}
	}

}
template <class T>
void ElementEditor<T>::OnDrag(GUIEventHandler* pEventHandler, POINT ptLocation) {
	for (std::unordered_map<unsigned int, IValueElement*>::iterator it = m_mElements.begin(); it != m_mElements.end(); it++) {
		if (it->second && it->second->CheckBounds(ptLocation)) {
			it->second->OnDrag(pEventHandler, ptLocation);
			return;
		}
	}
	//First one is likely to be a panel so check it last
	for (std::vector<IGUIElement*>::iterator it = m_vElements.begin(); it != m_vElements.end(); it++) {
		if (*it && (*it)->CheckBounds(ptLocation)) {
			(*it)->OnDrag(pEventHandler, ptLocation);
			return;
		}
	}

}
template <class T>
void ElementEditor<T>::OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation) {
	for (std::unordered_map<unsigned int, IValueElement*>::iterator it = m_mElements.begin(); it != m_mElements.end(); it++) {
		if (it->second && it->second->CheckBounds(ptLocation)) {
			it->second->OnRelease(pEventHandler, ptLocation);
			return;
		}
	}
	//First one is likely to be a panel so check it last
	for (std::vector<IGUIElement*>::iterator it = m_vElements.begin(); it != m_vElements.end(); it++) {
		if (*it && (*it)->CheckBounds(ptLocation)) {
			(*it)->OnRelease(pEventHandler, ptLocation);
			return;
		}
	}

}
template <class T>
void ElementEditor<T>::OnHover(GUIEventHandler* pEventHandler, POINT ptLocation) {
	for (std::unordered_map<unsigned int, IValueElement*>::iterator it = m_mElements.begin(); it != m_mElements.end(); it++) {
		if (it->second && it->second->CheckBounds(ptLocation)) {
			it->second->OnHover(pEventHandler, ptLocation);
			return;
		}
	}
	//First one is likely to be a panel so check it last
	for (std::vector<IGUIElement*>::iterator it = m_vElements.begin(); it != m_vElements.end(); it++) {
		if (*it && (*it)->CheckBounds(ptLocation)) {
			(*it)->OnHover(pEventHandler, ptLocation);
			return;
		}
	}
	
}

template <class T>
void ElementEditor<T>::UpdateTarget(T* pStruct) {
	m_pStruct = pStruct;
	for (std::unordered_map<unsigned int, IValueElement*>::iterator it = m_mElements.begin(); it != m_mElements.end(); it++) {
		it->second->SetValuePointer(reinterpret_cast<char*>(pStruct) + it->first);
	}

}

template <class T>
T* ElementEditor<T>::GetTarget() {
	return m_pStruct;

}

#endif