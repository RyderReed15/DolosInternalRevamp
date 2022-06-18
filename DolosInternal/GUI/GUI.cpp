#include "GUI.h"





DropDownElement pElements[] = { {"None", -1}, {"Head", 2}, {"Neck", 3}, { "Chest", 4 }, { "Pelvis", 5 } };
HotKeyStruct OpenMenu = { ShowMenu, VK_DELETE, false, false, false, false };
Panel* pMain = nullptr;
HotKey* pOpenHotkey = nullptr;
char test[64] = "";
void ShowMenu() {
	g_bMenuOpen = !g_bMenuOpen;
}




bool InitializeGUI(HMODULE hMod) {
	
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	g_pGUIContainer->AddElement(pMain = new Panel({ 100,100,1000,700 }, false, DARKGRAY));
	g_pGUIContainer->AddElement(pOpenHotkey = new HotKey("Open/Close GUI", 0x1001, &OpenMenu, { 10,10, 300, 20 }, 200, LIGHTGRAY, pMain));
	g_pGUIContainer->AddElement(new TextBox("Testing", test, 64, { 10,50, 300, 20 }, 200, LIGHTGRAY, pMain));
	//g_pGUIContainer->GetEventHandler()->CreateGUIEvent(GUI_EVENT_TYPE::KEYDOWN, g_pGUIContainer->GetEventHandler()->BuildFunction(&HotKey::SetupHotKey, pOpenHotkey));
	g_pGUIContainer->GenerateMap();
	return true;
}

bool UninitializeGUI() {
	
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}