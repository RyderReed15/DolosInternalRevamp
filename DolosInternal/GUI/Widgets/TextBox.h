#pragma once

#ifndef GUI_TEXT_BOX_H
#define GUI_TEXT_BOX_H

#include "IGUIElement.h"
#include "IValueElement.h"

class GUIEventHanlder;

class TextBox : public IValueElement {
public:


			TextBox			(const char* szName, char* aText, unsigned short iMaxCharacters, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cContainerColor, IGUIElement * pParent = nullptr);
		
	HRESULT Draw			(ID3DXFont* pFont, Render* pRender);


	void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);

	void	OnType			(GUIEventHandler* pEventHandler, char chKey);

	void	SetValuePointer	(void* pValue);
	void*	GetValuePointer	(void);
private:

	void	UpdateValues	(void);

	const char*			m_szName;
	char*				m_aText;
	unsigned short		m_iTextSize;
	unsigned short		m_iMaxSize;
	unsigned short		m_iStartChar;
	unsigned short		m_iCurrentChar;
	float				m_flContainerSize;
	D3DCOLOR			m_cContainerColor;


};

#endif // !GUI_TEXT_BOX_H
