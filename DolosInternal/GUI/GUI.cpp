#include "GUI.h"





DropDownElement pElements[] = { {"None", -1}, {"Head", 2}, {"Neck", 3}, { "Chest", 4 }, { "Pelvis", 5 } };
HotKeyStruct OpenMenu = { ShowMenu, VK_DELETE, false, false, false, false };

void ShowMenu() {
	g_bMenuOpen = !g_bMenuOpen;
}




bool InitializeGUI(HMODULE hMod) {
	
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	
	g_pGUIContainer->GenerateMap();
	return true;
}

bool UninitializeGUI() {
	
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}