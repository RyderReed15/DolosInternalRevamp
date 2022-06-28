#pragma once

#ifndef GUI_DROP_DOWN_H
#define GUI_DROP_DOWN_H

#include "IGUIElement.h"
#include "IValueElement.h"
#include <map>

#define BOX_TEXT_RATIO 1.3f

class DropDownElement;
class DropDownContainer;

class DropDown : public IValueElement {
public:


			DropDown		(const char* szName, int* pValue, std::map<int, const char*>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent = nullptr);
			~DropDown		(void);
	HRESULT Draw			(ID3DXFont* pFont, Render* pRender);

	void	SetValue		(int iValue);

	void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);

	void	SetValuePointer	(void* pValue);
	void*	GetValuePointer	(void);
	void	SetHover		(int iHover);
	int		GetHover		(void);

private:
	D3DCOLOR			m_cColor;
	int*				m_pValue;
	int					m_iHover;
	const char*			m_szName;
	DropDownContainer*	m_pContainer;
	unsigned int		m_iNumDisplayElements;


};

class DropDownContainer : public IGUIElement {
public:
			DropDownContainer	(std::map<int, const char*>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent);
	HRESULT Draw				(ID3DXFont* pFont, Render* pRender);

	void	SetOpen				(bool bOpen);
	bool	GetOpen				(void);


	void	OnRelease			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnHover				(GUIEventHandler* pEventHandler, POINT ptLocation);
private:

	std::map<int, const char*>*				m_pElements;
	unsigned int							m_iNumDisplayElements;
	unsigned int							m_iTopIndex;
	D3DCOLOR								m_cColor;
	bool									m_bOpen;
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
