#pragma once

#ifndef GUI_TEXT_BOX_H
#define GUI_TEXT_BOX_H

#include "IGUIElement.h"

class GUIEventHanlder;

class TextBox : public IGUIElement{
public:


	TextBox(const char* szName, char* aText, short iMaxCharacters, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cContainerColor, IGUIElement * pParent = nullptr);
		
	HRESULT Draw(ID3DXFont* pFont, Render* pRender);


	void	OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation);

	void	OnType(GUIEventHandler* pEventHandler, char chKey);
private:
	const char* m_szName;
	char* m_aText;
	short m_iTextSize;
	short m_iMaxSize;
	short m_iStartChar;
	short m_iCurrentChar;
	float m_flContainerSize;
	D3DCOLOR m_cContainerColor;


};

#endif // !GUI_TEXT_BOX_H
