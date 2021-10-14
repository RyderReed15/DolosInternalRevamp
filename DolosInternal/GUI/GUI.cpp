#include "GUI.h"


Panel* pTest2;
Slider* pSlider;
CheckBox* pCheckBox;
DropDown* pDrop;
ColorPicker* pcPicker;
ColorButton* pButton, *pButton2;
TextBox* pTextBox;
bool bCheck = false;
float test;
int iDrop;
D3DCOLOR cColor = 0xffff0000;
D3DCOLOR cColor2 = 0xffff0000;
Panel pTest = Panel({ 100 ,100, 1000, 600 }, true, DARKGRAY);
char szName[64];
DropDownElement pElements[] = { {"None", -1}, {"Head", 2}, {"Neck", 3}, { "Chest", 4 }, { "Pelvis", 5 } };
bool InitializeGUI(HMODULE hMod) {
	
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	g_pGUIContainer->AddElement(&pTest);
	g_pGUIContainer->AddElement(pSlider = new Slider("Slider", &test, 110, 10, D3DXVECTOR4(200, 150, 200, 12), 100, BLACK, LIGHTBLUE, PINK));
	g_pGUIContainer->AddElement(pCheckBox = new CheckBox("CheckBox", &bCheck, { 200,200,200,16 }, OFFWHITE, LIGHTBLUE));
	g_pGUIContainer->AddElement(pDrop = new DropDown("CheckBox", 0, &iDrop, pElements, 5, { 200,300,200,20 }, 100, LIGHTGRAY));
	g_pGUIContainer->AddElement(pcPicker = new ColorPicker("Color", { 400, 400, 280, 330 }, DARKGRAY, &cColor));
	g_pGUIContainer->AddElement(pButton = new ColorButton("Color", &cColor, { 200,400,200,20 }, pcPicker));
	g_pGUIContainer->AddElement(pButton2 = new ColorButton("Color 2", &cColor2, { 200,450,200,20 }, pcPicker));
	g_pGUIContainer->AddElement(pTextBox = new TextBox("Name", szName, 64, { 200,500,500,20 }, 400, LIGHTGRAY));
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
	}if (pcPicker) {
		delete pcPicker;
	}
	if (pButton) {
		delete pButton;
	}if (pButton2) {
		delete pButton2;
	}if (pTextBox) {
		delete pTextBox;
	}
	
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}