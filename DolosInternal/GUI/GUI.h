#pragma once

#ifndef GUI_H
#define GUI_H

#include <d3d9.h>
#include <d3dx9.h>
#include "../Utils/ConfigManager.h"
#include "GUIContainer.h"
#include "GUIEventHandler.h"
#include "Widgets/IGUIElement.h"
#include "Widgets/Panel.h"
#include "Widgets/Button.h"
#include "Widgets/Slider.h"
#include "Widgets/CheckBox.h"
#include "Widgets/DropDown.h"
#include "Widgets/ColorPicker.h"
#include "Widgets/TextBox.h"
#include "Widgets/HotKey.h"
#include "Widgets/Text.h"
#include "../SDK/SDK.h"

inline GUIContainer*    g_pGUIContainer;
inline Render*          g_pRender;
inline bool             g_bMenuOpen = true;

bool InitializeGUI      (HMODULE hMod);
bool UninitializeGUI    ();
void ShowMenu();


#endif // !GUI_H
