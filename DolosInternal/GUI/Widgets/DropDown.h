#pragma once

#ifndef GUI_DROP_DOWN_H
#define GUI_DROP_DOWN_H

#include "IGUIElement.h"
#include "IValueElement.h"
#include <map>

#define BOX_TEXT_RATIO 1.3f


template <typename T> class DropDownContainer;
class IDropDownContainer;

class DropDown : public IValueElement {
public:
			DropDown		(const char* szName, int* pValue, std::map<int, std::string>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent = nullptr);
			DropDown		(const wchar_t* szName, int* pValue, std::map<int, std::wstring>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cColor, IGUIElement* pParent = nullptr);
			~DropDown		(void);
				
	HRESULT Draw			(ID3DXFont* pFont, Render* pRender);

	void	SetValue		(int iValue);

	void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);

	void	SetValuePointer	(void* pValue);
	void*	GetValuePointer	(void);

	void	SetHover		(int iHover);
	int		GetHover		(void);

	void						SetMapPointerA(std::map<int, std::string>* pMap);
	void						SetMapPointerW(std::map<int, std::wstring>* pMap);
	std::map<int, std::string>* GetMapPointerA(void);
	std::map<int, std::wstring>*GetMapPointerW(void);

	IDropDownContainer* GetContainer(void);

protected:
	D3DCOLOR			m_cColor;
	int*				m_pValue;
	int					m_iHover;
	IDropDownContainer* m_pContainer;
	unsigned int		m_iNumDisplayElements;
	bool				m_bWide;
	union {
		const char*		m_szName;
		const wchar_t*	m_wszName;
	};
};


class IDropDownContainer : public IGUIElement {
public:

			IDropDownContainer	(unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent);

	void	SetOpen				(bool bOpen);
	bool	GetOpen				(void);

protected:

	unsigned int	m_iNumDisplayElements;
	unsigned int	m_iTopIndex;
	D3DCOLOR		m_cColor;
	bool			m_bOpen;
};

template <typename T>
class DropDownContainer : public IDropDownContainer {
public:
			DropDownContainer	(std::map<int, std::basic_string<T>>* pElements, unsigned int iNumDisplayElements, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent);
	HRESULT Draw				(ID3DXFont* pFont, Render* pRender);

	void	OnRelease			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnHover				(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnScroll			(GUIEventHandler* pEventHandler, POINT ptLocation, short zDelta);

	void									SetMapPointer(std::map<int, std::basic_string<T>>* pMap);
	std::map<int, std::basic_string<T>>*	GetMapPointer(void);
private:

	std::map<int, std::basic_string<T>>* m_pElements;
};

#include "DropDown.tcc"

#endif // !GUI_DROP_DOWN_H
