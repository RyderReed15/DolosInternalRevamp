#pragma once

#ifndef GUI_HOTKEY_H
#define GUI_HOTKEY_H

#include "IGUIElement.h"
#include "../../Utils/HotKeyManager.h"

class GUIEventHanlder;


class HotKey : public IGUIElement {
public:


			HotKey(const char* szName, void* fnHotKey, int iHotKeyId, char chKey, int bModifiers, D3DXVECTOR4 vBounds, float flContainerSize, D3DCOLOR cContainerColor, IGUIElement* pParent = nullptr);

	HRESULT Draw(ID3DXFont* pFont, Render* pRender);

	std::string ConvertToString(char chKey, int bModifiers);
	void		SetupHotKey(void);

	void	OnRelease(GUIEventHandler* pEventHandler, POINT ptLocation);

	void	OnKey(GUIEventHandler* pEventHandler, char chKey, long keyInfo);
private:
	const char* m_szName;
	std::string m_szDisplay;
	float		m_flContainerSize;
	D3DCOLOR	m_cContainerColor;
	int			m_iHotKeyId;
	int			m_bModifiers;
	void*		m_fnHotKey;
	char		m_chKey;


};


#endif // !GUI_HOTKEY_H
