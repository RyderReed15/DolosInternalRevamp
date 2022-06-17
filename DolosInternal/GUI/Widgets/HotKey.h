#pragma once

#ifndef GUI_HOTKEY_H
#define GUI_HOTKEY_H

#include "IGUIElement.h"
#include "../../Utils/HotKeyManager.h"

#define DIFF_UPPER_LOWER 0x20


class GUIEventHanlder;

struct HotKeyStruct;

class HotKey : public IGUIElement {
public:


				HotKey		(const char* szName, int iHotKeyId, HotKeyStruct* pHotKeyInfo, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cContainerColor, IGUIElement* pParent = nullptr);
				~HotKey		(void);

	HRESULT		Draw		(ID3DXFont* pFont, Render* pRender);

	std::string CreateString(void);
	void		SetupHotKey	(void);

	std::string MakeKey		(char chKey);

	void		OnRelease	(GUIEventHandler* pEventHandler, POINT ptLocation);

	void		OnKey		(GUIEventHandler* pEventHandler, char chKey, long keyInfo);
private:
	const char*		m_szName;
	std::string		m_szDisplay;
	float			m_flContainerSize;
	D3DCOLOR		m_cContainerColor;
	int				m_iHotKeyId;
	HotKeyStruct*	m_pHotKeyInfo;


};


#endif // !GUI_HOTKEY_H
