#include "ColorPicker.h"

ColorPicker::ColorPicker(const char* szName, D3DXVECTOR4 vBounds, D3DCOLOR cBackground, D3DCOLOR* pColor, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	m_pColor = pColor;
	m_cBackground = cBackground;
	m_cOrignalColor = *m_pColor;
	m_szName = szName;

	m_pActiveSlider = nullptr;
	m_pTarget = nullptr;
	
	UpdateColor(*pColor);
	m_pColorBox = new ColorBox({ 10, 30, 200, 200 }, this);
	m_pHueBox = new HueBox({ 220, 30 ,20, 200 }, *pColor, this);
	m_pRedSlider = new Slider("Red: ", &m_flRed, 255, 0, { 10, 240, 260, 8 }, 150, BLACK, WHITE, RED, false, this);
	m_pGreenSlider = new Slider("Green: ", &m_flGreen, 255, 0, { 10, 260, 260, 8 }, 150, BLACK, WHITE, GREEN, false, this);
	m_pBlueSlider = new Slider("Blue: ", &m_flBlue, 255, 0, { 10, 280, 260, 8 }, 150, BLACK, WHITE, BLUE, false, this);
	m_pAlphaSlider = new Slider("Alpha: ", &m_flAlpha, 255, 0, { 10, 300, 260, 8 }, 150, BLACK, WHITE, BLACK, false, this);
	m_pColorBox->m_pHueBox = m_pHueBox;
	m_pHueBox->m_pColorBox = m_pColorBox;
	m_pColorBox->UpdatePosition();
}

ColorPicker::~ColorPicker() {
	if (m_pColorBox) {
		delete m_pColorBox;
	}
	if (m_pHueBox) {
		delete m_pHueBox;
	}
	if (m_pRedSlider) {
		delete m_pRedSlider;
	}
	if (m_pGreenSlider) {
		delete m_pGreenSlider;
	}
	if (m_pBlueSlider) {
		delete m_pBlueSlider;
	}
	if (m_pAlphaSlider) {
		delete m_pAlphaSlider;
	}
}

HRESULT ColorPicker::Draw(ID3DXFont* pFont, Render* pRender) {
	pRender->DrawString({ m_vBounds.x + 10, m_vBounds.y + 10 }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName); // Name
	//pRender->DrawSprite({ 320,0,32,32 }, { m_vBounds.x + m_vBounds.z - 20, m_vBounds.y }, WHITE, .66f);


	pRender->DrawRoundedRectangle(m_vBounds, 3, m_cBackground); // Background

	pRender->DrawRectangle({ m_pHueBox->m_vBounds.x + 30, m_vBounds.y + 30, 20, 200 }, *m_pColor); //Current Color

	m_pColorBox->Draw(pFont, pRender);
	m_pHueBox->Draw(pFont, pRender);
	m_pRedSlider->Draw(pFont, pRender);
	m_pGreenSlider->Draw(pFont, pRender);
	m_pBlueSlider->Draw(pFont, pRender);
	m_pAlphaSlider->Draw(pFont, pRender);
	return S_OK;

}

void ColorPicker::UpdateTarget(ColorButton* pColorButton) {
	//Updates color picker to match selected option
	m_pTarget = pColorButton;
	m_szName = pColorButton->GetName();
	m_pColor = pColorButton->GetColorPtr();
	UpdateColor(*m_pColor);
	m_pHueBox->UpdatePosition();
	m_pHueBox->UpdateHue();
	m_pColorBox->UpdatePosition();
	UpdateSliders();
}

ColorButton* ColorPicker::GetTarget() {
	return m_pTarget;
}

void ColorPicker::WriteColor() {
	*m_pColor = D3DCOLOR_ARGB((int)m_flAlpha, (int)m_flRed, (int)m_flGreen, (int)m_flBlue);
}

void ColorPicker::UpdateColor(D3DCOLOR cColor) {
	*m_pColor = cColor;
	m_flAlpha = (float)(byte)(cColor >> 24);
	m_flRed = (float)(byte)(cColor >> 16);
	m_flGreen = (float)(byte)(cColor >> 8);
	m_flBlue = (float)(byte)(cColor);
}

void ColorPicker::UpdateSliders(void) {
	m_pRedSlider->UpdateSlider();
	m_pBlueSlider->UpdateSlider();
	m_pGreenSlider->UpdateSlider();
	m_pAlphaSlider->UpdateSlider();
}

void ColorPicker::OnClick(GUIEventHandler* pEventHandler, POINT ptLocation) {
	if (m_pColorBox->CheckBounds(ptLocation)) {
		m_pColorBox->OnClick(pEventHandler, ptLocation);

	}else if (m_pHueBox->CheckBounds(ptLocation)) {
		m_pHueBox->OnClick(pEventHandler, ptLocation);
	}
	else if (m_pRedSlider->CheckBounds(ptLocation)) {
		pEventHandler->SetFocus(this);
		m_pActiveSlider = m_pRedSlider;
	}
	else if (m_pGreenSlider->CheckBounds(ptLocation)) {
		pEventHandler->SetFocus(this);
		m_pActiveSlider = m_pGreenSlider;
	}
	else if (m_pBlueSlider->CheckBounds(ptLocation)) {
		pEventHandler->SetFocus(this);
		m_pActiveSlider = m_pBlueSlider;
	}
	else if (m_pAlphaSlider->CheckBounds(ptLocation)) {
		pEventHandler->SetFocus(this);
		m_pActiveSlider = m_pAlphaSlider;
	}
	
}
void ColorPicker::OnDrag(GUIEventHandler* pEventHandler, POINT ptLocation) {
	m_pActiveSlider->OnDrag(pEventHandler, ptLocation);
	WriteColor();
	m_pHueBox->UpdatePosition();
	m_pHueBox->UpdateHue();
	m_pColorBox->UpdatePosition();
}
void ColorPicker::OnRelease(GUIEventHandler* pEventHandler, POINT) {
	if (m_pActiveSlider) {
		pEventHandler->ReleaseFocus();
		return;
	}

}


ColorBox::ColorBox(D3DXVECTOR4 vBounds, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	m_pColorPicker = (ColorPicker*)pParent;
	m_vSelectionLocation = { 0,0 };
	m_pHueBox = m_pColorPicker->m_pHueBox;
}

void ColorBox::UpdateColor() {

	m_pColorPicker->UpdateColor(LerpColor(LerpColor(WHITE, m_pHueBox->m_cHue, (m_vSelectionLocation.x) / m_vBounds.z), BLACK, (m_vSelectionLocation.y) / m_vBounds.w));

}

void ColorBox::UpdatePosition() {
	// Calculates current postion of color on the box given the hex
	if ((byte)(m_pHueBox->m_cHue >> 16) == 255) {

		float flPercent = m_pColorPicker->m_flRed / 255.f;
		m_vSelectionLocation.y = (1 - flPercent) * m_vBounds.w;

		if ((byte)(m_pHueBox->m_cHue >> 8) == 0) {
			flPercent = 1 - m_pColorPicker->m_flGreen / 255.f / (flPercent);
			m_vSelectionLocation.x = flPercent * m_vBounds.z;
		}
		else {
			flPercent = 1 - m_pColorPicker->m_flBlue / 255.f / (flPercent);
			m_vSelectionLocation.x = flPercent * m_vBounds.z;
		}
	}
	else if ((byte)(m_pHueBox->m_cHue >> 8) == 255) {

		float flPercent = m_pColorPicker->m_flGreen / 255.f;
		m_vSelectionLocation.y = (1 - flPercent) * m_vBounds.w;

		if ((byte)(m_pHueBox->m_cHue >> 16) == 0) {
			flPercent = 1 - m_pColorPicker->m_flRed / 255.f / (flPercent);
			m_vSelectionLocation.x = flPercent * m_vBounds.z;
		}
		else {
			flPercent = 1 - m_pColorPicker->m_flBlue / 255.f / (flPercent);
			m_vSelectionLocation.x = flPercent * m_vBounds.z;
		}
	}
	else if ((byte)(m_pHueBox->m_cHue) == 255) {

		float flPercent = m_pColorPicker->m_flBlue / 255.f;
		m_vSelectionLocation.y = (1 - flPercent) * m_vBounds.w;

		if ((byte)(m_pHueBox->m_cHue >> 16) == 0) {
			flPercent = 1 - m_pColorPicker->m_flRed / 255.f / (flPercent);
			m_vSelectionLocation.x = flPercent * m_vBounds.z;
		}
		else {
			flPercent = 1 - m_pColorPicker->m_flGreen / 255.f / (flPercent);
			m_vSelectionLocation.x = flPercent * m_vBounds.z;
		}
	}
}

HRESULT ColorBox::Draw(ID3DXFont*, Render* pRender) {

	/* Color Gradient */
	pRender->DrawRectangle(m_vBounds, WHITE, m_pHueBox->m_cHue, false);
	pRender->DrawRectangle(m_vBounds, EMPTY, BLACK, true);

	/* Selector */
	m_vSelectionLocation += {m_vBounds.x, m_vBounds.y};
	pRender->DrawCircleOutline(m_vSelectionLocation, 5, 10, WHITE);
	pRender->DrawCircleOutline(m_vSelectionLocation, 6, 10, BLACK);

	m_vSelectionLocation -= {m_vBounds.x, m_vBounds.y};
	return S_OK;

}

void ColorBox::OnClick(GUIEventHandler* pEventHandler, POINT) {

	pEventHandler->SetFocus(this);
}
void ColorBox::OnDrag(GUIEventHandler*, POINT ptLocation) {

	m_vSelectionLocation = { min(max(ptLocation.x, m_vBounds.x), m_vBounds.x + m_vBounds.z), min(max(ptLocation.y, m_vBounds.y), m_vBounds.y + m_vBounds.w) }; m_vSelectionLocation -= {m_vBounds.x, m_vBounds.y};
	UpdateColor();
	m_pColorPicker->UpdateSliders();

}
void ColorBox::OnRelease(GUIEventHandler* pEventHandler, POINT) {
	pEventHandler->ReleaseFocus();

}


HueBox::HueBox(D3DXVECTOR4 vBounds, D3DCOLOR cHue, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	m_cHue = cHue;
	m_pColorPicker = (ColorPicker*)pParent;

	UpdatePosition();
	UpdateHue();
	m_pColorBox = m_pColorPicker->m_pColorBox;
}

HRESULT HueBox::Draw(ID3DXFont*, Render* pRender) {


	/* Hue Slider*/
	D3DXVECTOR4 vHue = m_vBounds;


	vHue.w = m_vBounds.w / 6.f;
	pRender->DrawRectangle(vHue, RED, MAGENETA, true);

	vHue.y += vHue.w;
	pRender->DrawRectangle(vHue, MAGENETA, BLUE, true);

	vHue.y += vHue.w;
	pRender->DrawRectangle(vHue, BLUE, CYAN, true);

	vHue.y += vHue.w;
	pRender->DrawRectangle(vHue, CYAN, GREEN, true);

	vHue.y += vHue.w;
	pRender->DrawRectangle(vHue, GREEN, YELLOW, true);

	vHue.y += vHue.w;
	pRender->DrawRectangle(vHue, YELLOW, RED, true);


	/* Selection Indicator */
	pRender->DrawCircle({ m_vBounds.x, m_flSelection + m_vBounds.y }, 7, 1, WHITE, .25f, 135); // Oh yeah! Manipulating circle code instead of writing a DrawTriangle function
	pRender->DrawCircle({ m_vBounds.x + m_vBounds.z, m_flSelection + m_vBounds.y }, 7, 1, WHITE, .25f, -45);


	return S_OK;

}

void HueBox::UpdateHue() {
	float flRotation = (m_flSelection) / m_vBounds.w * 6;

	switch ((int)flRotation % 7) {
	case 0: m_cHue = D3DCOLOR_ARGB(255, 255, 0, (int)(255 * flRotation)); break;
	case 1: m_cHue = D3DCOLOR_ARGB(255, (int)(255 * (2 - flRotation)), 0, 255); break;
	case 2: m_cHue = D3DCOLOR_ARGB(255, 0, (int)(255 * (flRotation - 2)), 255); break;
	case 3: m_cHue = D3DCOLOR_ARGB(255, 0, 255, (int)(255 * (4 - flRotation))); break;
	case 4: m_cHue = D3DCOLOR_ARGB(255, (int)(255 * (flRotation - 4)), 255, 0); break;
	case 5: m_cHue = D3DCOLOR_ARGB(255, 255, (int)(255 * (6 - flRotation)), 0); break;
	case 6: m_cHue = RED; break;
	}

}

void HueBox::UpdatePosition() {
	//Calculates hue change based on color change
	float red = m_pColorPicker->m_flRed;
	float green = m_pColorPicker->m_flGreen;
	float blue = m_pColorPicker->m_flBlue;

	float min = min(min(red, green), blue);
	float max = max(max(red, green), blue);

	if (min == max) {
		m_flSelection = 0;
		return;
	}

	float hue = 0.f;
	if (max == red) {
		hue = (green - blue) / (max - min);

	}
	else if (max == green) {
		hue = 2.f + (blue - red) / (max - min);

	}
	else {
		hue = 4.f + (red - green) / (max - min);
	}

	hue = m_vBounds.w - (hue / 6 * m_vBounds.w);
	if (hue >= m_vBounds.w) hue -= m_vBounds.w;
	m_flSelection = hue;
}

void HueBox::OnClick(GUIEventHandler* pEventHandler, POINT) {
	
	pEventHandler->SetFocus(this);
}
void HueBox::OnDrag(GUIEventHandler*, POINT ptLocation) {

	m_flSelection = min(max(ptLocation.y, m_vBounds.y), m_vBounds.y + m_vBounds.w) - m_vBounds.y;
	UpdateHue();

	m_pColorBox->UpdateColor();
	m_pColorPicker->UpdateSliders();
	
}
void HueBox::OnRelease(GUIEventHandler* pEventHandler, POINT) {
	pEventHandler->ReleaseFocus();
	
}


ColorButton::ColorButton(const char* szName, D3DCOLOR* pColor, D3DXVECTOR4 vBounds, ColorPicker* pPicker, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
	m_szName = szName;
	m_pColor = pColor;
	m_pColorPicker = pPicker;
	m_pColorPicker->SetDrawState(false);
}
HRESULT ColorButton::Draw(ID3DXFont* pFont, Render* pRender) {
	pRender->DrawString({ m_vBounds.x, m_vBounds.y - 2 }, (m_bEnabled) ? WHITE : GRAY, pFont, m_szName);
	return pRender->DrawSprite({ 96, 672, 32, 32 }, { m_vBounds.x + m_vBounds.z - m_vBounds.w, m_vBounds.y }, *m_pColor, m_vBounds.w / 32.f);
}


const char* ColorButton::GetName(void) {
	return m_szName;
}
D3DCOLOR* ColorButton::GetColorPtr(void) {
	return m_pColor;
}

void ColorButton::OnRelease(GUIEventHandler* pEventHandler, POINT) {

	if (m_pColorPicker->GetTarget() == this) {
		m_pColorPicker->SetDrawState(!m_pColorPicker->GetDrawState());
	}
	else {
		m_pColorPicker->SetDrawState(true);
	}

	m_pColorPicker->UpdateTarget(this);
	pEventHandler->GetContainer()->GenerateMap();
}