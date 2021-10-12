#pragma once

#ifndef GUI_SLIDER_H
#define GUI_SLIDER_H

#include "IGUIElement.h"

class Slider : public IGUIElement {
public:
			Slider(const char* szName, float* pValue, float flMaxValue, float flMinValue, D3DXVECTOR4 vBounds, float flBarSize, D3DCOLOR cBackground, D3DCOLOR cFillOne, D3DCOLOR cFillTwo = EMPTY, bool bExponential = false, IGUIElement* pParent = nullptr);

	HRESULT Draw(ID3DXFont* pFont, Render* pRender);

	//bool	OnClick(POINT pLocation);
	//bool	OnDrag(POINT pLocation);
	//bool	OnRelease(POINT pLocation);
	void	UpdateSlider();
private:
	D3DCOLOR		m_cBackground, m_cFillOne, m_cFillTwo;
	bool			m_bMoving;
	float			m_flBarPercent, m_flBarSize;
	float			m_flMaxValue, m_flMinValue;
	const char*		m_szName;
	float*			m_pValue;
	bool			m_bExponential;
};

#endif // !GUI_SLIDER_H
