#include "GUI.h"





DropDownElement pElements[] = { {"None", -1}, {"Head", BONES::HEAD}, {"Neck", BONES::NECK}, { "Chest", BONES::CHEST }, { "Pelvis", BONES::PELVIS } };
HotKeyStruct OpenMenu = { ShowMenu, VK_DELETE, false, false, false, false };
Panel* pMain = nullptr;
Panel* pAimbot, *pESP, *pChams, *pSkinChanger, *pSettings = nullptr;
CheckBox* pAimCheck , *pFriendlyESP, *pEnemyESP = nullptr;
char test[64] = "";


void ShowMenu() {
	g_bMenuOpen = !g_bMenuOpen;
}

void ChangeChildren(IGUIElement* pElement, bool bShow) {
	if (!pElement) return;

	IGUIElement* pCurr = pElement->GetFirstChild();
	while (pCurr) {
		ChangeChildren(pCurr, bShow);
		pCurr = pCurr->GetSibling();
	}
	pElement->SetDrawState(bShow);

}

void ShowPanel(Panel* pPanel) {
	ChangeChildren(pAimbot		, false);
	ChangeChildren(pESP			, false);
	ChangeChildren(pChams		, false);
	ChangeChildren(pSkinChanger	, false);
	ChangeChildren(pSettings	, false);

	ChangeChildren(pPanel		, true);

	g_pGUIContainer->GenerateMap();
}


bool InitializeGUI(HMODULE hMod) {
	
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	g_pGUIContainer->AddElement(pMain = new Panel({ 100,100,1000,700 }, true, DARKGRAY));

	g_pGUIContainer->AddElement(pAimbot = new Panel({ 250, 0, 750, 700 }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("AIMBOT", std::bind(ShowPanel, pAimbot), { 0, 0, 250, 75 }, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(pAimCheck = new CheckBox("Aimbot", &Settings.Aimbot.Active, { 50, 50, 300, 12 }, OFFWHITE, LIGHTBLUE, pAimbot));
	g_pGUIContainer->AddElement(new CheckBox(" - Silent"	, &Settings.Aimbot.Silent	, { 0, 20, 300, 12 }, OFFWHITE, LIGHTBLUE, pAimCheck));
	g_pGUIContainer->AddElement(new CheckBox(" - Overaim"	, &Settings.Aimbot.Overaim	, { 0, 40, 300, 12 }, OFFWHITE, LIGHTBLUE, pAimCheck));
	g_pGUIContainer->AddElement(new CheckBox(" - Target All", &Settings.Aimbot.TargetAll, { 0, 60, 300, 12 }, OFFWHITE, LIGHTBLUE, pAimCheck));


	g_pGUIContainer->AddElement(pESP = new Panel({ 250, 0, 750, 700 }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("ESP", std::bind(ShowPanel, pESP), { 0, 75, 250, 75 }, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(pFriendlyESP = new CheckBox("Show Team", &Settings.Visuals.Players.ShowTeam, { 50, 50, 300, 12 }, OFFWHITE, LIGHTBLUE, pESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Bones"	, &Settings.Visuals.Players.DrawBones	, { 0, 20, 300, 12 }, OFFWHITE, LIGHTBLUE, pFriendlyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Health"	, &Settings.Visuals.Players.DrawHealth	, { 0, 40, 300, 12 }, OFFWHITE, LIGHTBLUE, pFriendlyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Armor"	, &Settings.Visuals.Players.DrawArmor	, { 0, 60, 300, 12 }, OFFWHITE, LIGHTBLUE, pFriendlyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Name"		, &Settings.Visuals.Players.DrawName	, { 0, 80, 300, 12 }, OFFWHITE, LIGHTBLUE, pFriendlyESP));

	g_pGUIContainer->AddElement(pEnemyESP = new CheckBox("Show Enemies", &Settings.Visuals.Players.ShowEnemy, { 400, 50, 300, 12 }, OFFWHITE, LIGHTBLUE, pESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Bones"	, &Settings.Visuals.Players.DrawBones	, { 0, 20, 300, 12 }, OFFWHITE, LIGHTBLUE, pEnemyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Health"	, &Settings.Visuals.Players.DrawHealth	, { 0, 40, 300, 12 }, OFFWHITE, LIGHTBLUE, pEnemyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Armor"	, &Settings.Visuals.Players.DrawArmor	, { 0, 60, 300, 12 }, OFFWHITE, LIGHTBLUE, pEnemyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Name"		, &Settings.Visuals.Players.DrawName	, { 0, 80, 300, 12 }, OFFWHITE, LIGHTBLUE, pEnemyESP));

	g_pGUIContainer->AddElement(pChams = new Panel({ 250, 0, 750, 700 }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("CHAMS", std::bind(ShowPanel, pChams), { 0, 150, 250, 75 }, DARKGRAY, pMain));

	g_pGUIContainer->AddElement(pSkinChanger = new Panel({ 250, 0, 750, 700 }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("SKIN CHANGER", std::bind(ShowPanel, pSkinChanger), { 0, 225, 250, 75 }, DARKGRAY, pMain));

	g_pGUIContainer->AddElement(pSettings = new Panel({ 250, 0, 750, 700 }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("SETTINGS", std::bind(ShowPanel, pSettings), { 0, 300, 250, 75 }, DARKGRAY, pMain));

	g_pGUIContainer->AddElement(new HotKey("Open/Close GUI", 0x1001, &OpenMenu, { 50, 50, 300, 20 }, 150, LIGHTGRAY, pSettings));
	//g_pGUIContainer->AddElement(new TextBox("Testing", test, 64, { 10,50, 300, 20 }, 200, LIGHTGRAY, pMain));

	ShowPanel(pAimbot);
	return true;
}

bool UninitializeGUI() {
	
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}