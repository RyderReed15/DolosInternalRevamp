#include "FontManager.h"



bool InitializeFonts(HMODULE hMod) {
	g_hAvenir			= AddFontToResources(hMod, FONT_AVENIR);
	g_hWeapons			= AddFontToResources(hMod, FONT_WEAPONS);
	g_hWeaponsOutline	= AddFontToResources(hMod, FONT_WEAPONS_OUTLINE);
	return g_hAvenir && g_hWeapons && g_hWeaponsOutline;
}

bool UninitializeFonts() {
	if (g_hAvenir) {
		if (!RemoveFontFromResources(g_hAvenir)) {
			return false;
		}
	}
	if (g_hWeapons) {
		if (!RemoveFontFromResources(g_hWeapons)) {
			return false;
		}
	}
	if (g_hWeaponsOutline) {
		if (!RemoveFontFromResources(g_hWeaponsOutline)) {
			return false;
		}
	}
	return true;
}

bool AddD3DFonts(IDirect3DDevice9* pDevice) {
	if (S_OK != D3DXCreateFont(pDevice, 18, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "AvenirLTPro-Black", &g_pAvenirFont)) {
		return false;
	}
	
	if (S_OK != D3DXCreateFont(pDevice, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "csgo_icons", &g_pWeaponFont)) {
		return false; 
	}

	if (S_OK != D3DXCreateFont(pDevice, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "csgo_icons_outline", &g_pWeaponOutlineFont)){
		return false;
	}

	return true;

}

bool RemoveD3DFonts() {
	
	if (g_pAvenirFont) {
		g_pAvenirFont->Release();
	}

	if (g_pWeaponFont) {
		g_pWeaponFont->Release();
	}

	if (g_pWeaponOutlineFont) {
		g_pWeaponOutlineFont->Release();
	}
	return true;
}

HANDLE AddFontToResources(HMODULE hMod, int iFontId) {
	HRSRC hResource = FindResource(hMod, MAKEINTRESOURCE(iFontId), RT_FONT);
	if (hResource) {
		HGLOBAL hMem = LoadResource(hMod, hResource);
		void* pData = LockResource(hMem);
		int iLength = SizeofResource(hMod, hResource);
		DWORD nFonts;
		return AddFontMemResourceEx(pData, iLength, NULL, &nFonts);
	}
	return NULL;

}
bool RemoveFontFromResources(HANDLE hFont) {
	if (hFont) {
		return RemoveFontMemResourceEx(hFont);
	}
	return false;

}