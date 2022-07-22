#pragma once

#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "IGUIElement.h"
#include <functional>

class GUIEventHandler;



class Button : public IGUIElement {
public:
			Button		(const char* szText, std::function<void()> pFunc, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent = nullptr);

	HRESULT Draw		(ID3DXFont* pFont, Render* pRender);

	void	OnRelease	(GUIEventHandler* pEventHandler, POINT ptLocation);
private:
	D3DCOLOR					m_cColor;
	const char*					m_szText		= nullptr;
	std::function<void()>		m_pClickFunc	= nullptr;

};



#endif // !GUI_BUTTON_H
