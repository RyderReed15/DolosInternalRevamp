#include "Slider.h"

Slider::Slider(const char* szName, float* pValue, float flMaxValue, float flMinValue, D3DXVECTOR4 vBounds, float flBarSize, D3DCOLOR cBackground, D3DCOLOR cFillOne, D3DCOLOR cFillTwo, bool bExponential, IGUIElement* pParent) : IValueElement(vBounds, pParent) {
	m_flMaxValue	= flMaxValue;
	m_flMinValue	= flMinValue;
	m_flRange		= flMaxValue - flMinValue;
	m_pValue		= pValue;

	m_bExponential	= bExponential;

	m_flBarPercent	= max(min(powf(((*pValue) - flMinValue) / m_flRange, 1.f / (1 + m_bExponential * .5f)), 1.f), 0.f);
	m_flBarSize		= flBarSize;

	m_szName		= szName;

	m_cBackground	= cBackground;
	m_cFillOne		= cFillOne;
	m_cFillTwo		= cFillTwo ? cFillTwo : cFillOne;

	m_pTextbox = new TextBox("", m_szValue, 32, {0, 0, vBounds.z - flBarSize, vBounds.w}, vBounds.z - flBarSize, EMPTY, this);
	
	m_pTextbox->SetCallback(GUI_EVENT_TYPE::KEYDOWN, std::bind(&Slider::UpdateValueFromString, this));
	m_pTextbox->SetCallback(GUI_EVENT_TYPE::UNFOCUS, std::bind(&Slider::UpdateSlider, this));

}

Slider::~Slider(void) {
	if (m_pTextbox) delete m_pTextbox;

}


HRESULT Slider::Draw(ID3DXFont* pFont, Render* pRender) {
	
	float flBarSizeAdj = m_flBarPercent * (m_flBarSize - m_vBounds.w) + m_vBounds.w;
	float flBarStart = m_vBounds.x + m_vBounds.z - m_flBarSize;
	
	D3DXVECTOR2 vSize	= pRender->GetStringSize(pFont, m_szName);
	D3DXVECTOR2 vSize2	= pRender->GetStringSize(pFont, m_szValue);

	pRender->DrawString({ m_vBounds.x, m_vBounds.y - TEXT_FEATURE_OFFSET }					, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
	pRender->DrawString({ flBarStart - vSize2.x - 10, m_vBounds.y - TEXT_FEATURE_OFFSET }	, (m_bEnabled) ? WHITE : GRAY, pFont, m_szValue);

	pRender->DrawRoundedRectangle({ flBarStart, m_vBounds.y, m_flBarSize, m_vBounds.w }				, m_vBounds.w / 2, m_cBackground);
	return pRender->DrawRoundedRectangle({ flBarStart, m_vBounds.y, flBarSizeAdj, m_vBounds.w }		, m_vBounds.w / 2, m_cFillOne, LerpColor(m_cFillOne, m_cFillTwo, m_flBarPercent), false);

	

}

void Slider::UpdateSlider() {
	m_flBarPercent = max(min(powf(((*m_pValue) - m_flMinValue) / m_flRange, 1.f / (1 + m_bExponential * .5f)), 1.f), 0.f);

	sprintf_s(m_szValue, 32, "%.2f", *m_pValue);
}

void Slider::OnClick(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_pTextbox && ptLocation.x < m_vBounds.x + m_vBounds.z - m_flBarSize) {
		pEventHandler->SetFocus(m_pTextbox);
	}
	else {
		pEventHandler->SetFocus(this);
	}	
}
void Slider::OnDrag(GUIEventHandler*, POINT ptLocation) {
	
		
	m_flBarPercent = max(min((ptLocation.x - (m_vBounds.x + m_vBounds.z - m_flBarSize + m_vBounds.w)) / (m_flBarSize - m_vBounds.w), 1.f), 0.f);
		
	*m_pValue = m_flRange * powf(m_flBarPercent, 1 + m_bExponential * .5f) + m_flMinValue;

	sprintf_s(m_szValue, 32, "%.2f", *m_pValue);
	
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

void Slider::UpdateValueFromString() {
	*m_pValue = static_cast<float>(_atof_l(m_szValue, g_Locale));
}
