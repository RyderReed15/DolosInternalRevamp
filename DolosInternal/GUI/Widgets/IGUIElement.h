#pragma once

#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#define FADE_LENGTH	3

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
protected:
	bool			m_bShouldDraw		= true;
	bool			m_bEnabled			= true;
	D3DXVECTOR4		m_vBounds;
	int				m_iChildCount		= 0;
	IGUIElement*	m_pFirstChild		= nullptr;
	IGUIElement*	m_pSibling			= nullptr;
	IGUIElement*	m_pParent			= nullptr;

	int				m_iAnimStartTick		= 0;
private:


};



#endif // !GUI_ELEMENT_H
