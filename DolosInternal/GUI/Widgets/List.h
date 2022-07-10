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


#include "List.tcc"


#endif