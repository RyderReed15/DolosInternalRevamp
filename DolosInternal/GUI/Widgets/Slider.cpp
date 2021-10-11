#include "Slider.h"

Slider::Slider(const char* szName, float* pValue, float flMaxValue, float flMinValue, D3DXVECTOR4 vBounds, int iBarSize, D3DCOLOR cBackground, D3DCOLOR cFillOne, D3DCOLOR cFillTwo = EMPTY, bool bExponential = false, IGUIElement* pParent = nullptr) : IGUIElement(vBounds, pParent) {
	m_flMaxValue	= flMaxValue;
	m_flMinValue	= flMinValue;
	m_pValue		= pValue;

	m_iBarPercent	= (*pValue) / (flMaxValue - flMinValue) * (iBarSize - vBounds.w) + vBounds.w;
	m_iBarSize		= iBarSize;
	m_bExponential	= bExponential;

	m_szName		= szName;

	m_bMoving		= false;

	m_cBackground	= cBackground;
	m_cFillOne		= cFillOne;
	m_cFillTwo		= cFillTwo ? cFillTwo : cFillOne;

}


HRESULT Slider::Draw(ID3DXFont* pFont, Render* pRender) {
	if (m_bShouldDraw) {
		float flBarStart = m_vBounds.x + m_vBounds.z - m_iBarSize;
		char szValue[33]; _itoa_s((int)*m_pValue, szValue, 10);
		D3DXVECTOR2 vSize = pRender->GetStringSize(pFont, m_szName);
		D3DXVECTOR2 vSize2 = pRender->GetStringSize(pFont, szValue);
		pRender->DrawString({ m_vBounds.x, m_vBounds.y - 2 }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
		pRender->DrawString({ flBarStart - vSize2.x - 10, m_vBounds.y - 2 }, (m_bEnabled) ? WHITE : GRAY, pFont, szValue);
		pRender->DrawRoundedRectangle({ flBarStart, m_vBounds.y, (float)m_iBarSize, m_vBounds.w }, m_vBounds.w / 2, m_cBackground);
		return pRender->DrawRoundedRectangle({ flBarStart, m_vBounds.y, (float)m_iBarPercent, m_vBounds.w }, m_vBounds.w / 2, m_cFillOne, pRender->LerpColor(m_cFillOne, m_cFillTwo, (float)m_iBarPercent / m_iBarSize), false);

	}
	return S_OK;

}