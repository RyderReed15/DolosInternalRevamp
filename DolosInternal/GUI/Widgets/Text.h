#pragma once

#ifndef GUI_TEXT_H
#define GUI_TEXT_H

#include "IGUIElement.h"


class GUIText : public IGUIElement {

			GUIText	(const char* szText, D3DXVECTOR4 vBounds, D3DCOLOR cColor, IGUIElement* pParent = nullptr);

	HRESULT Draw	(ID3DXFont* pFont, Render* pRender);

private:
	D3DCOLOR	m_cColor;
	const char* m_szText;

};


#endif // !GUI_TEXT_H
