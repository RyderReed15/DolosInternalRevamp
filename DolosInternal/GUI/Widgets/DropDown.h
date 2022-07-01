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


			DropDown		(const char* szName, int* pValue, std::map<int, std::string>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent = nullptr);
			~DropDown		(void);
	HRESULT Draw			(ID3DXFont* pFont, Render* pRender);

	void	SetValue		(int iValue);

	void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);

	void	SetValuePointer	(void* pValue);
	void*	GetValuePointer	(void);
	void	SetHover		(int iHover);
	int		GetHover		(void);

	void						SetMapPointer(std::map<int, std::string>* pMap);
	std::map<int, std::string>* GetMapPointer(void);

	DropDownContainer*			GetContainer(void);

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
			DropDownContainer	(std::map<int, std::string>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent);
	HRESULT Draw				(ID3DXFont* pFont, Render* pRender);

	void	SetOpen				(bool bOpen);
	bool	GetOpen				(void);


	void	OnRelease			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnHover				(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnScroll			(GUIEventHandler* pEventHandler, POINT ptLocation, short zDelta);

	void						SetMapPointer(std::map<int, std::string>* pMap);
	std::map<int, std::string>* GetMapPointer(void);
private:

	std::map<int, std::string>*				m_pElements;
	unsigned int							m_iNumDisplayElements;
	unsigned int							m_iTopIndex;
	D3DCOLOR								m_cColor;
	bool									m_bOpen;
};


#endif // !GUI_DROP_DOWN_H
