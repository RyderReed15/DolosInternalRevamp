#pragma once

#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#define FADE_LENGTH	.5

#include "../Render.h"
#include "../GUIEventHandler.h"
class GUIEventHandler;

class IGUIElement {
public:

					IGUIElement		(D3DXVECTOR4 vBounds, IGUIElement* pParent = nullptr);

	virtual HRESULT Draw			(ID3DXFont* pFont, Render* pRender) = 0;

	virtual void	OnClick			(GUIEventHandler* pEventHandler, POINT ptLocation);
	virtual void	OnDrag			(GUIEventHandler* pEventHandler, POINT ptLocation);
	virtual void	OnRelease		(GUIEventHandler* pEventHandler, POINT ptLocation);
	virtual void	OnHover			(GUIEventHandler* pEventHandler, POINT ptLocation);

	virtual void	OnType			(GUIEventHandler* pEventHandler, char chKey);
	virtual void	OnKey			(GUIEventHandler* pEventHandler, char chKey, long keyInfo);

	void			SetAnimStartTick(void);
	float			GetAnimLerp		(float flAnimLength);
	void			SetDrawState	(bool bDrawState);
	bool			GetDrawState	(void);
	void			SetEnabled		(bool bEnabled);
	bool			GetEnabled		(void);

	D3DXVECTOR2		GetCoords		(void);
	D3DXVECTOR4		GetBounds		(void);
	bool			CheckBounds		(POINT ptLocation);
	void			MoveDelta		(POINT ptLocation);

	void			AddChild		(IGUIElement* pElement);
	IGUIElement*	GetFirstChild	(void);
	void			SetSibling		(IGUIElement* pElement);
	IGUIElement*	GetSibling		(void);
	void			SetParent		(IGUIElement* pElement);
	IGUIElement*	GetParent		(void);
	IGUIElement*	GetRoot			(void);

protected:
	bool			m_bShouldDraw;
	bool			m_bEnabled;
	D3DXVECTOR4		m_vBounds;
	unsigned int				m_iChildCount;
	IGUIElement*	m_pFirstChild;
	IGUIElement*	m_pSibling;
	IGUIElement*	m_pParent;

	unsigned int				m_iAnimStartTick;
private:


};



#endif // !GUI_ELEMENT_H
