#pragma once

#ifndef GUI_SLIDER_H
#define GUI_SLIDER_H

#include "IGUIElement.h"
#include "IValueElement.h"
#include "TextBox.h"
#include "../GUIEventHandler.h"
#include "../GUI.h"


class GUIEventHandler;

class Slider : public IValueElement {
public:
			Slider			(const char* szName, float* pValue, float flMaxValue, float flMinValue, D3DXVECTOR4 vBounds, float flBarSize, D3DCOLOR cBackground, D3DCOLOR cFillOne, D3DCOLOR cFillTwo = EMPTY, bool bExponential = false, IGUIElement* pParent = nullptr);
			~Slider			(void);

	HRESULT Draw			(ID3DXFont* pFont, Render* pRender);

	void	OnClick			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnDrag			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	UpdateSlider	(void);

	void	SetValuePointer	(void* pValue);
	void*	GetValuePointer	(void);
private:

	void	UpdateValueFromString(void);

	D3DCOLOR		m_cBackground, m_cFillOne, m_cFillTwo;
	float			m_flBarPercent, m_flBarSize;
	float			m_flMaxValue, m_flMinValue, m_flRange;
	const char*		m_szName;
	float*			m_pValue;
	bool			m_bExponential;
	char			m_szValue[32];

	TextBox*		m_pTextbox;
};

#endif // !GUI_SLIDER_H
