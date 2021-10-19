#pragma once

#ifndef INITIALIZER_H
#define INITIALIZER_H
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <Windows.h>
#include "SDK/SDK.h"
#include "Utils/NetvarManager.h"
#include "Hooks.h"
#include "GUI/FontManager.h"
#include "GUI/GUI.h"
#include "Utils/ConfigManager.h"


bool UninitializeCheat  (void);
bool StartCheat         (HMODULE hMod);
bool InitializeCheat    (HMODULE hMod);
bool CloseCheat         (HMODULE hMod);

LRESULT hkInitWndProc   (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif