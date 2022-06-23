#pragma once

#ifndef GUI_COLOR_PICKER_H
#define GUI_COLOR_PICKER_H

#include "IGUIElement.h"
#include "Slider.h"

class Slider;
class HueBox;
class ColorBox;
class ColorButton;

class ColorPicker : public IGUIElement {
public:
	friend ColorBox;
	friend HueBox;
	friend ColorButton;

					ColorPicker		(const char* szName, D3DCOLOR* pColor, D3DXVECTOR4 vBounds, D3DCOLOR cBackground, IGUIElement* pParent = nullptr);
					~ColorPicker	(void);
	HRESULT			Draw			(ID3DXFont* pFont, Render* pRender);

	void			UpdateTarget	(ColorButton* pColorButton);
	ColorButton*	GetTarget		(void);
	void			UpdateColor		(D3DCOLOR cColor);
	void			WriteColor		(void);

	void			UpdateSliders	(void);

	void			OnClick			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void			OnDrag			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void			OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);
private:
	ColorButton*	m_pTarget;
	HueBox*			m_pHueBox;
	ColorBox*		m_pColorBox;
	D3DCOLOR*		m_pColor;
	Slider*			m_pRedSlider, * m_pGreenSlider, * m_pBlueSlider, * m_pAlphaSlider;
	Slider*			m_pActiveSlider;
	D3DCOLOR		m_cOrignalColor;
	D3DCOLOR		m_cBackground;
	float			m_flRed, m_flGreen, m_flBlue, m_flAlpha;
	const char*		m_szName;

};


class ColorBox : public IGUIElement {

	friend ColorPicker;
	friend HueBox;

			ColorBox		(D3DXVECTOR4 vBounds, IGUIElement* pParent);
	HRESULT Draw			(ID3DXFont* pFont, Render* pRender);

	void	UpdateColor		(void);
	void	UpdatePosition	(void);

	void	OnClick			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnDrag			(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);

	
	HueBox*			m_pHueBox;
	ColorPicker*	m_pColorPicker;
	Vector2D		m_vSelectionLocation;

};


class HueBox : public IGUIElement {

	friend ColorBox;
	friend ColorPicker;

			HueBox		(D3DXVECTOR4 vBounds, D3DCOLOR cHue, IGUIElement* pParent);
	HRESULT Draw		(ID3DXFont* pFont, Render* pRender);

	void UpdateHue		(void);
	void UpdatePosition	(void);

	void	OnClick		(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnDrag		(GUIEventHandler* pEventHandler, POINT ptLocation);
	void	OnRelease	(GUIEventHandler* pEventHandler, POINT ptLocation);
	
	ColorBox*		m_pColorBox;
	ColorPicker*	m_pColorPicker;
	D3DCOLOR		m_cHue;
	float			m_flSelection;
};

class ColorButton : public IGUIElement {
	
public:
				ColorButton		(const char* szName, D3DCOLOR* pColor, D3DXVECTOR4 vBounds, ColorPicker* pPicker, IGUIElement* pParent = nullptr);
			
	HRESULT		Draw			(ID3DXFont* pFont, Render* pRender);

	const char* GetName			(void);
	D3DCOLOR*	GetColorPtr		(void);

	void		OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);
private:
	D3DCOLOR*		m_pColor;
	ColorPicker*	m_pColorPicker;
	const char*		m_szName = nullptr;

	
};
#endif // !GUI_COLOR_PICKER_H
