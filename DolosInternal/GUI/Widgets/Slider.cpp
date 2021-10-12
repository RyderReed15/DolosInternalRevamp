#include "Slider.h"

Slider::Slider(const char* szName, float* pValue, float flMaxValue, float flMinValue, D3DXVECTOR4 vBounds, float flBarSize, D3DCOLOR cBackground, D3DCOLOR cFillOne, D3DCOLOR cFillTwo, bool bExponential, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	m_flMaxValue	= flMaxValue;
	m_flMinValue	= flMinValue;
	m_pValue		= pValue;

	m_flBarPercent	= (*pValue) / (flMaxValue - flMinValue) * (flBarSize - vBounds.w) + vBounds.w;
	m_flBarSize		= flBarSize;
	m_bExponential	= bExponential;

	m_szName		= szName;

	m_bMoving		= false;

	m_cBackground	= cBackground;
	m_cFillOne		= cFillOne;
	m_cFillTwo		= cFillTwo ? cFillTwo : cFillOne;

}


HRESULT Slider::Draw(ID3DXFont* pFont, Render* pRender) {
	
	float flBarStart = m_vBounds.x + m_vBounds.z - m_flBarSize;
	char szValue[33]; _itoa_s((int)*m_pValue, szValue, 10);

	D3DXVECTOR2 vSize	= pRender->GetStringSize(pFont, m_szName);
	D3DXVECTOR2 vSize2	= pRender->GetStringSize(pFont, szValue);

	pRender->DrawString({ m_vBounds.x, m_vBounds.y - 2 }				, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
	pRender->DrawString({ flBarStart - vSize2.x - 10, m_vBounds.y - 2 }	, (m_bEnabled) ? WHITE : GRAY, pFont, szValue);

	pRender->DrawRoundedRectangle({ flBarStart, m_vBounds.y, m_flBarSize, m_vBounds.w }				, m_vBounds.w / 2, m_cBackground);
	return pRender->DrawRoundedRectangle({ flBarStart, m_vBounds.y, m_flBarPercent, m_vBounds.w }	, m_vBounds.w / 2, m_cFillOne, pRender->LerpColor(m_cFillOne, m_cFillTwo, m_flBarPercent / m_flBarSize), false);

	

}