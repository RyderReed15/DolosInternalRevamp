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

bool InitializeCheat(HMODULE hMod);

#endif