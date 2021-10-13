#pragma once

#ifndef GUI_DROP_DOWN_H
#define GUI_DROP_DOWN_H

#include "IGUIElement.h"

class DropDownElement;
class DropDownContainer;

class DropDown : public IGUIElement {
public:


			DropDown	(const char* szName, int iDefaultIndex, int* pValue, DropDownElement* pElemementArray, int iNumElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent = nullptr);
			~DropDown	(void);
	HRESULT Draw		(ID3DXFont* pFont, Render* pRender);

	void	SetIndex	(int iIndex);
	void	SetValue	(int iValue);


	void	OnClick		(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnRelease	(GUIEventHandler* pEventHandler, POINT ptLocation);
private:
	D3DCOLOR			m_cColor;
	int					m_iIndex;
	int*				m_pValue;
	const char*			m_szName;
	int					m_iArraySize;
	DropDownContainer*	m_pContainer;


};

class DropDownContainer : public IGUIElement {
public:
			DropDownContainer	(DropDownElement* m_pElementArray, int iNumElements, D3DXVECTOR4 vBounds, int iNumDisplayElements, int iStartIndex, D3DCOLOR cColor, IGUIElement* pParent);
	HRESULT Draw				(ID3DXFont* pFont, Render* pRender);

	void	SetOpen				(bool bOpen);
	bool	GetOpen				(void);


	void	OnRelease			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnHover				(GUIEventHandler* pEventHandler, POINT ptLocation);
private:

	DropDownElement*	m_pElementArray;
	int					m_iNumElements;
	int					m_iNumDisplayElements;
	int					m_iTopIndex, m_iIndex;
	D3DCOLOR			m_cColor;
	bool				m_bOpen;
};

class DropDownElement : public IGUIElement {
public:
				DropDownElement	(const char* szName, int iValue);
	HRESULT		Draw			(ID3DXFont* pFont, Render* pRender);

	void		SetBounds		(D3DXVECTOR4 vBounds);

	int			GetValue		(void);
	const char* GetName			(void);

	void		OnHover			(GUIEventHandler* pEventHandler, POINT ptLocation);

private:
	const char* m_szName;
	int			m_iValue;

};
#endif // !GUI_DROP_DOWN_H
