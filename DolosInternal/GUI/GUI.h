#pragma once

#ifndef GUI_H
#define GUI_H

#include <d3d9.h>
#include <d3dx9.h>
#include "../Settings.h"
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
#include "Widgets/List.h"
#include "../SDK/Interfaces/IInputSystem.h"
#include "../Features/SkinChanger.h"

#define COLUMN_WIDTH        300
#define ROW_HEIGHT          20
#define ROW_FEATURE_HEIGHT  12
#define PANEL_WIDTH         750
#define PANEL_HEIGHT        600

inline GUIContainer*    g_pGUIContainer;
inline bool             g_bMenuOpen = true;

bool InitializeGUI      (HMODULE hMod);
bool UninitializeGUI    ();



ElementEditor<AimTarget>*               CreateAimEditor (void);
ElementEditor<SkinChanger::SkinInfo>*   CreateSkinEditor(void);

void                        ChangeChildren  (IGUIElement* pElement, bool bShow);
void                        ShowPanel       (Panel* pPanel);
void                        ShowMenu        (void);
void                        SetSkinList     (DropDown* pWeaponPicker, DropDown* pSkinPicker);


#endif // !GUI_H
