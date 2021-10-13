#include "GUI.h"


Panel* pTest2;
Panel pTest = Panel({ 100 ,100, 1000, 600 }, true, WHITE);
bool InitializeGUI(HMODULE hMod) {
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	g_pGUIContainer->AddElement(&pTest);
	g_pGUIContainer->AddElement(pTest2 = new Panel(D3DXVECTOR4(500, 1000, 40, 100), true, LIGHTBLUE));
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