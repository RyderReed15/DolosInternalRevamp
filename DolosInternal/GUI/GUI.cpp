#include "GUI.h"





DropDownElement pElements[] = { {"None", -1}, {"Head", BONES::HEAD}, {"Neck", BONES::NECK}, { "Chest", BONES::CHEST }, { "Pelvis", BONES::PELVIS } };
D3DCOLOR cDummy;

HotKeyStruct OpenMenu = { ShowMenu, VK_DELETE, false, false, false, false };
Panel* pMain = nullptr;
Panel* pAimbot, *pESP, *pChams, *pSkinChanger, *pSettings = nullptr;
CheckBox* pAimCheck , *pFriendlyESP, *pEnemyESP = nullptr;
ColorPicker* pColorPicker = nullptr;
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
	g_pGUIContainer->AddElement(pMain = new Panel({ 100,100,1000,PANEL_HEIGHT }, true, DARKGRAY));


	g_pGUIContainer->AddElement(pColorPicker = new ColorPicker("Color Picker", &cDummy, { 1000, 0, COLUMN_WIDTH, 350 }, LIGHTGRAY, pMain));

	g_pGUIContainer->AddElement(pAimbot = new Panel({ 250, 0, PANEL_WIDTH, PANEL_HEIGHT }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("AIMBOT", std::bind(ShowPanel, pAimbot), { 0, 0, 250, 75 }, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(pAimCheck = new CheckBox("Aimbot", &Settings.Aimbot.Active, { 50, 50, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pAimbot));
	g_pGUIContainer->AddElement(new CheckBox(" - Silent"	, &Settings.Aimbot.Silent	, { 0, ROW_HEIGHT	 , COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pAimCheck));
	g_pGUIContainer->AddElement(new CheckBox(" - Overaim"	, &Settings.Aimbot.Overaim	, { 0, ROW_HEIGHT * 2, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pAimCheck));
	g_pGUIContainer->AddElement(new CheckBox(" - Target All", &Settings.Aimbot.TargetAll, { 0, ROW_HEIGHT * 3, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pAimCheck));

	g_pGUIContainer->AddElement(new GUIText ("Targets", { 400, 0, COLUMN_WIDTH, 0 }, WHITE, pAimCheck));


	g_pGUIContainer->AddElement(pESP = new Panel({ 250, 0, PANEL_WIDTH, PANEL_HEIGHT }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("ESP", std::bind(ShowPanel, pESP), { 0, 75, 250, 75 }, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(pFriendlyESP = new CheckBox("Show Team", &Settings.Visuals.Friendly.ShowTeam, { 50, 50, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Bones"	, &Settings.Visuals.Friendly.DrawBones	, { 0, ROW_HEIGHT	 , COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pFriendlyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Health"	, &Settings.Visuals.Friendly.DrawHealth	, { 0, ROW_HEIGHT * 2, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pFriendlyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Armor"	, &Settings.Visuals.Friendly.DrawArmor	, { 0, ROW_HEIGHT * 3, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pFriendlyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Name"		, &Settings.Visuals.Friendly.DrawName	, { 0, ROW_HEIGHT * 4, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pFriendlyESP));
	g_pGUIContainer->AddElement(new ColorButton(" - Team Color"	, &Settings.Visuals.Friendly.TeamColor	, { 0, ROW_HEIGHT * 5, COLUMN_WIDTH, ROW_HEIGHT }, pColorPicker, pFriendlyESP));

	g_pGUIContainer->AddElement(pEnemyESP = new CheckBox("Show Enemies", &Settings.Visuals.Enemy.ShowTeam, { 400, 50, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Bones"	, &Settings.Visuals.Enemy.DrawBones	, { 0, ROW_HEIGHT	 , COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pEnemyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Health"	, &Settings.Visuals.Enemy.DrawHealth, { 0, ROW_HEIGHT * 2, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pEnemyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Armor"	, &Settings.Visuals.Enemy.DrawArmor	, { 0, ROW_HEIGHT * 3, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pEnemyESP));
	g_pGUIContainer->AddElement(new CheckBox(" - Show Name"		, &Settings.Visuals.Enemy.DrawName	, { 0, ROW_HEIGHT * 4, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pEnemyESP));
	g_pGUIContainer->AddElement(new ColorButton(" - Team Color"	, &Settings.Visuals.Enemy.TeamColor	, { 0, ROW_HEIGHT * 5, COLUMN_WIDTH, ROW_HEIGHT }, pColorPicker, pEnemyESP));

	g_pGUIContainer->AddElement(pChams = new Panel({ 250, 0, PANEL_WIDTH, PANEL_HEIGHT }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("CHAMS", std::bind(ShowPanel, pChams), { 0, 150, 250, 75 }, DARKGRAY, pMain));

	g_pGUIContainer->AddElement(pSkinChanger = new Panel({ 250, 0, PANEL_WIDTH, PANEL_HEIGHT }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("SKIN CHANGER", std::bind(ShowPanel, pSkinChanger), { 0, 225, 250, 75 }, DARKGRAY, pMain));

	g_pGUIContainer->AddElement(pSettings = new Panel({ 250, 0, PANEL_WIDTH, PANEL_HEIGHT }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("SETTINGS", std::bind(ShowPanel, pSettings), { 0, 300, 250, 75 }, DARKGRAY, pMain));

	g_pGUIContainer->AddElement(new HotKey("Open/Close GUI", 0x1001, &OpenMenu, { 50, 50, COLUMN_WIDTH, ROW_HEIGHT }, 150, LIGHTGRAY, pSettings));
	//g_pGUIContainer->AddElement(new TextBox("Testing", test, 64, { 10,50, 300, 20 }, 200, LIGHTGRAY, pMain));

	ShowPanel(pAimbot);
	return true;
}

bool UninitializeGUI() {
	
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}