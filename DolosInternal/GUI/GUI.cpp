#include "GUI.h"


Panel pTest = Panel({ 100 ,100, 1000, 600 }, true, WHITE);
bool InitializeGUI(HMODULE hMod) {
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	g_pGUIContainer->AddElement(&pTest);
	g_pGUIContainer->GenerateMap();
	return true;
}

bool UninitializeGUI() {
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}