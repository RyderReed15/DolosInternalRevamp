#pragma once

#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#define FADE_LENGTH	3

#include "../Render.h"

class IGUIElement {
public:

					IGUIElement		(D3DXVECTOR4 vBounds, IGUIElement* pParent = nullptr);

	virtual HRESULT Draw			(ID3DXFont* pFont, Render* pRender) = 0;

	virtual bool	OnClick			(D3DXVECTOR2 vLocation);
	virtual bool	OnDrag			(D3DXVECTOR2 vLocation);
	virtual bool	OnRelease		(D3DXVECTOR2 vLocation);
	virtual bool	OnHover			(D3DXVECTOR2 vLocation);

	void			SetAnimStartTick(void);
	float			GetAnimLerp		(float flAnimLength);
	void			SetDrawState	(bool bDrawState);
	bool			GetDrawState	(void);
	void			SetEnabled		(bool bEnabled);
	bool			GetEnabled		(void);

	D3DXVECTOR2		GetCoords		(void);
	bool			CheckBounds		(D3DXVECTOR2 vLocation);
	void			MoveDelta		(D3DXVECTOR2 vLocation);

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
