#include "GUI.h"

inline Panel* pTest2;
Panel pTest = Panel({ 100 ,100, 1000, 600 }, true, WHITE);
bool InitializeGUI(HMODULE hMod) {
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	g_pGUIContainer->AddElement(&pTest);
	pTest2 = g_pGUIContainer->CreateElement<Panel>(D3DXVECTOR4(500, 500, 40, 100), true, RED);
	g_pGUIContainer->GenerateMap();
	return true;
}

bool UninitializeGUI() {
	if (pTest2) {
		delete pTest2;
	}
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}