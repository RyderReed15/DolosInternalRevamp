#include "Slider.h"

Slider::Slider(const char* szName, float* pValue, float flMaxValue, float flMinValue, D3DXVECTOR4 vBounds, float flBarSize, D3DCOLOR cBackground, D3DCOLOR cFillOne, D3DCOLOR cFillTwo, bool bExponential, IGUIElement* pParent) : IValueElement(vBounds, pParent) {
	m_flMaxValue	= flMaxValue;
	m_flMinValue	= flMinValue;
	m_flRange		= flMaxValue - flMinValue;
	m_pValue		= pValue;

	m_bExponential	= bExponential;

	m_flBarPercent	= max(min(powf(((*pValue) - flMinValue) / m_flRange, 1 + bExponential * .5f), 1.f), 0.f);
	m_flBarSize		= flBarSize;

	m_szName		= szName;

	m_cBackground	= cBackground;
	m_cFillOne		= cFillOne;
	m_cFillTwo		= cFillTwo ? cFillTwo : cFillOne;

}


HRESULT Slider::Draw(ID3DXFont* pFont, Render* pRender) {
	
	float flBarSizeAdj = m_flBarPercent * (m_flBarSize - m_vBounds.w) + m_vBounds.w;
	float flBarStart = m_vBounds.x + m_vBounds.z - m_flBarSize;
	char szValue[33]; _itoa_s((int)*m_pValue, szValue, 10);

	D3DXVECTOR2 vSize	= pRender->GetStringSize(pFont, m_szName);
	D3DXVECTOR2 vSize2	= pRender->GetStringSize(pFont, szValue);

	pRender->DrawString({ m_vBounds.x, m_vBounds.y - TEXT_FEATURE_OFFSET }					, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
	pRender->DrawString({ flBarStart - vSize2.x - 10, m_vBounds.y - TEXT_FEATURE_OFFSET }	, (m_bEnabled) ? WHITE : GRAY, pFont, szValue);

	pRender->DrawRoundedRectangle({ flBarStart, m_vBounds.y, m_flBarSize, m_vBounds.w }				, m_vBounds.w / 2, m_cBackground);
	return pRender->DrawRoundedRectangle({ flBarStart, m_vBounds.y, flBarSizeAdj, m_vBounds.w }		, m_vBounds.w / 2, m_cFillOne, LerpColor(m_cFillOne, m_cFillTwo, m_flBarPercent), false);

	

}

void Slider::UpdateSlider() {
	m_flBarPercent = max(min(powf(((*m_pValue) - m_flMinValue) / m_flRange, 1 + m_bExponential * .5f), 1.f), 0.f);
}

void Slider::OnClick(GUIEventHandler* pEventHandler, POINT) {
	
	pEventHandler->SetFocus(this);
	
	
}
void Slider::OnDrag(GUIEventHandler*, POINT ptLocation) {
	
		
	m_flBarPercent = max(min((ptLocation.x - (m_vBounds.x + m_vBounds.z - m_flBarSize + m_vBounds.w)) / (m_flBarSize - m_vBounds.w), 1.f), 0.f);
		
	*m_pValue = m_flRange * powf(m_flBarPercent, 1 + m_bExponential * .5f) + m_flMinValue;
		
	
}
void Slider::OnRelease(GUIEventHandler* pEventHandler, POINT) {
	pEventHandler->ReleaseFocus();
	
}

void Slider::SetValuePointer(void* pValue) {
	m_pValue = static_cast<float*>(pValue);
	UpdateSlider();
}

void* Slider::GetValuePointer() {
	return m_pValue;
}
