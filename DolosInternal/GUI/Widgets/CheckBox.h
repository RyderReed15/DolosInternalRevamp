#pragma once

#ifndef GUI_CHECK_BOX
#define GUI_CHECK_BOX

#include "IGUIElement.h"

class CheckBox : public IGUIElement {
public:
			CheckBox		(const char* szName, bool* pBool, D3DXVECTOR4 vBounds, D3DCOLOR cDisabled, D3DCOLOR cActivated, IGUIElement* pParent = nullptr);
	
	HRESULT Draw			(ID3DXFont* pFont, Render* pRender);

	void	SetClickable	(bool bClickable);

	void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);
private:
	D3DCOLOR m_cDisabled, m_cActivated;
	const char* m_szName = nullptr;
	bool* m_pBool;
	bool m_bClickable;

};

#endif // !GUI_CHECK_BOX