#pragma once

#ifndef GUI_DROP_DOWN_H
#define GUI_DROP_DOWN_H

#include "IGUIElement.h"

#define BOX_TEXT_RATIO 1.3f

class DropDownElement;
class DropDownContainer;

class DropDown : public IGUIElement {
public:


			DropDown	(const char* szName, unsigned int iDefaultIndex, int* pValue, DropDownElement* pElemementArray, unsigned int iNumElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent = nullptr);
			~DropDown	(void);
	HRESULT Draw		(ID3DXFont* pFont, Render* pRender);

	void	SetIndex	(unsigned  int iIndex);
	void	SetValue	(int iValue);

	void	OnRelease	(GUIEventHandler* pEventHandler, POINT ptLocation);
private:
	D3DCOLOR			m_cColor;
	unsigned int		m_iIndex;
	int*				m_pValue;
	const char*			m_szName;
	unsigned int		m_iArraySize;
	DropDownContainer*	m_pContainer;


};

class DropDownContainer : public IGUIElement {
public:
			DropDownContainer	(DropDownElement* m_pElementArray, unsigned int iNumElements, D3DXVECTOR4 vBounds, unsigned int iNumDisplayElements, unsigned int iStartIndex, D3DCOLOR cColor, IGUIElement* pParent);
	HRESULT Draw				(ID3DXFont* pFont, Render* pRender);

	void	SetOpen				(bool bOpen);
	bool	GetOpen				(void);


	void	OnRelease			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnHover				(GUIEventHandler* pEventHandler, POINT ptLocation);
private:

	DropDownElement*	m_pElementArray;
	unsigned int		m_iNumElements;
	unsigned int		m_iNumDisplayElements;
	unsigned int		m_iTopIndex, m_iIndex;
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
