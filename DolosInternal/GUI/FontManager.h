#pragma once

#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <Windows.h>
#include "../resource.h"
#include <d3d9.h>
#include <d3dx9.h>

inline HANDLE		g_hAvenir;
inline HANDLE		g_hWeapons;
inline HANDLE		g_hWeaponsOutline;

inline ID3DXFont*	g_pAvenirFont;
inline ID3DXFont*	g_pWeaponFont;
inline ID3DXFont*	g_pWeaponOutlineFont;

bool	InitializeFonts			(HMODULE hMod);
bool	UninitializeFonts		(void);
bool	AddD3DFonts				(IDirect3DDevice9* pDevice);
bool	RemoveD3DFonts			(void);
HANDLE	AddFontToResources		(HMODULE hMod, int iFontId);
bool	RemoveFontFromResources	(HANDLE hFont);



#endif // !FONT_MANAGER_H
