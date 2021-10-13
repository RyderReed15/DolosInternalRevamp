#include "GUI.h"


Panel* pTest2;
Slider* pSlider;
CheckBox* pCheckBox;
DropDown* pDrop;
bool bCheck = false;
float test;
int iDrop;
Panel pTest = Panel({ 100 ,100, 1000, 600 }, true, DARKGRAY);
DropDownElement pElements[] = { {"None", -1}, {"Head", 2}, {"Neck", 3}, { "Chest", 4 }, { "Pelvis", 5 } };
bool InitializeGUI(HMODULE hMod) {
	
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	g_pGUIContainer->AddElement(&pTest);
	g_pGUIContainer->AddElement(pSlider = new Slider("Slider", &test, 110, 10, D3DXVECTOR4(200, 150, 200, 12), 100, BLACK, LIGHTBLUE, PINK));
	g_pGUIContainer->AddElement(pCheckBox = new CheckBox("CheckBox", &bCheck, { 200,200,200,16 }, OFFWHITE, LIGHTBLUE));
	g_pGUIContainer->AddElement(pDrop = new DropDown("CheckBox", 0, &iDrop, pElements, 5, { 200,300,200,20 }, 100, LIGHTGRAY));
	g_pGUIContainer->GenerateMap();
	return true;
}

bool UninitializeGUI() {
	if (pSlider) {
		delete pSlider;
	}
	if (pCheckBox) {
		delete pCheckBox;
	}if (pDrop) {
		delete pDrop;
	}
	
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}