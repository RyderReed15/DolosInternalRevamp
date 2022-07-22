#include "GUI.h"


std::map<int, std::string> mAimOptions = { { -1 , "None"}, { BONES::HEAD, "Head" }, { BONES::NECK, "Neck" }, { BONES::CHEST, "Chest" }, { BONES::PELVIS, "Pelvis" }, { BONES::SPINE1, "Spine" } };
D3DCOLOR cDummy;

HotKeyStruct OpenMenu = { ShowMenu, VK_DELETE, false, false, false, false };
Panel* pMain = nullptr;
Panel* pAimbot, *pESP, *pChams, *pSkinChanger, *pSettings = nullptr;
CheckBox* pAimCheck , *pFriendlyESP, *pEnemyESP = nullptr;
ColorPicker* pColorPicker = nullptr;
GUIList<AimTarget>* pTargetsList = nullptr;
GUIList<SkinChanger::SkinInfo>* pSkinsList = nullptr;

AimTarget* pDefaultTarget = nullptr;
SkinChanger::SkinInfo* pDefaultSkin = nullptr;

ElementEditor<AimTarget>* pAimEditor = nullptr;
ElementEditor<SkinChanger::SkinInfo>* pSkinEditor = nullptr;






bool InitializeGUI(HMODULE hMod) {

	g_pInput->EnableInput(false);

	g_Locale = _get_current_locale();
	
	g_pRender = new Render(g_pD3DDevice, hMod);
	g_pGUIContainer = new GUIContainer({ 1920,1080 });
	g_pGUIContainer->AddElement(pMain = new Panel({ 100,100,250,PANEL_HEIGHT }, true, DARKGRAY));


	g_pGUIContainer->AddElement(pColorPicker = new ColorPicker("Color Picker", &cDummy, { 1000, 0, COLUMN_WIDTH, 350 }, LIGHTGRAY, pMain));

	g_pGUIContainer->AddElement(pAimbot = new Panel({ 250, 0, PANEL_WIDTH, PANEL_HEIGHT }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("AIMBOT", std::bind(ShowPanel, pAimbot), { 0, 0, 250, 75 }, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(pAimCheck = new CheckBox("Aimbot", &Settings.Aimbot.Active, { 50, 50, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pAimbot));
	g_pGUIContainer->AddElement(new CheckBox(" - Silent"	, &Settings.Aimbot.Silent	, { 0, ROW_HEIGHT	 , COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pAimCheck));
	g_pGUIContainer->AddElement(new CheckBox(" - Overaim"	, &Settings.Aimbot.Overaim	, { 0, ROW_HEIGHT * 2, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pAimCheck));
	g_pGUIContainer->AddElement(new CheckBox(" - Target All", &Settings.Aimbot.TargetAll, { 0, ROW_HEIGHT * 3, COLUMN_WIDTH, ROW_FEATURE_HEIGHT }, OFFWHITE, LIGHTBLUE, pAimCheck));

	pAimEditor = CreateAimEditor();

	g_pGUIContainer->AddElement(pTargetsList = new GUIList<AimTarget> ("Targets", ROW_HEIGHT, &Settings.Aimbot.Targets, pAimEditor, { 400, 50, COLUMN_WIDTH, 400 }, LIGHTGRAY, pAimbot));
	
	pDefaultTarget = new AimTarget({ 0, -1, "Target", false });

	pTargetsList->CreateDefault(pDefaultTarget->Name, pDefaultTarget);

	for (size_t i = 0; i < Settings.Aimbot.Targets.size(); i++) {
		
		ListElement<AimTarget>* pElem = pTargetsList->AddElement(Settings.Aimbot.Targets[i].Name, &Settings.Aimbot.Targets[i], pAimEditor);
		
	}


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

	pSkinEditor = CreateSkinEditor();

	

	pDefaultSkin = new SkinChanger::SkinInfo({ 1, 0, 0, 0, -1, 0, 0, ""});

	pSkinsList->CreateDefault(pDefaultSkin->szCustomName, pDefaultSkin);

	for (size_t i = 0; i < Settings.SkinChanger.Skins.size(); i++) {

		ListElement<SkinChanger::SkinInfo>* pElem = pSkinsList->AddElement(Settings.SkinChanger.Skins[i].szCustomName, &Settings.SkinChanger.Skins[i], pSkinEditor);

	}

	g_pGUIContainer->AddElement(pSettings = new Panel({ 250, 0, PANEL_WIDTH, PANEL_HEIGHT }, false, DARKGRAY, DARKGRAY, pMain));
	g_pGUIContainer->AddElement(new Button("SETTINGS", std::bind(ShowPanel, pSettings), { 0, 300, 250, 75 }, DARKGRAY, pMain));

	g_pGUIContainer->AddElement(new HotKey("Open/Close GUI", 0x1001, &OpenMenu, { 50, 50, COLUMN_WIDTH, ROW_HEIGHT }, 150, LIGHTGRAY, pSettings));
	//g_pGUIContainer->AddElement(new TextBox("Testing", test, 64, { 10,50, 300, 20 }, 200, LIGHTGRAY, pMain));

	ShowPanel(pAimbot);
	return true;
}

bool UninitializeGUI() {

	delete pDefaultTarget;
	delete pDefaultSkin;
	delete g_pRender;
	delete g_pGUIContainer;
	return true;
}

ElementEditor<AimTarget>* CreateAimEditor() {

	AimTarget* pDummy = new AimTarget({ 0, 0 });
	Panel* pEditorPanel;

	pEditorPanel = new Panel({ 1000, 0, COLUMN_WIDTH, 150 }, false, DARKGRAY, DARKGRAY, pMain); // dont set parent here, it will be set in pEditor

	TextBox*	pName		= new TextBox	("Name: "	, pDummy->Name, 128, { 20, 20, 260, 20 }, 150, LIGHTGRAY, pEditorPanel);
	DropDown*	pTarget		= new DropDown	("Target: "	, &pDummy->Bone, &mAimOptions, 5, { 20, 60, 260, 20 }, 150, LIGHTGRAY, pEditorPanel);
	Slider*		pFOV		= new Slider	("FOV: "	, &pDummy->FOV, 120, 0, { 20, 80, 260, 12 }, 150, BLACK, LIGHTBLUE, PINK, true, pEditorPanel);
	CheckBox*	pCheckBox	= new CheckBox	("Enabled: ", &pDummy->Enabled, { 20, 100, 260, 12 }, OFFWHITE, LIGHTBLUE, pEditorPanel);

	ElementEditor<AimTarget>* pEditor = new ElementEditor<AimTarget>(pDummy, pEditorPanel);

	g_pGUIContainer->AddElement(pEditor);

	pEditor->SetDrawState(false);

	return pEditor;
}

ElementEditor<SkinChanger::SkinInfo>* CreateSkinEditor() {

	SkinChanger::SkinInfo* pDummy = new SkinChanger::SkinInfo();
	Panel* pEditorPanel;

	pEditorPanel = new Panel({ 600, 0, 350, PANEL_HEIGHT }, false, DARKGRAY, DARKGRAY, pMain); // dont set parent here, it will be set in pEditor


	Slider*		pWear			= new Slider	("Float: "	, &pDummy->flWear, 1, 0, { 50, 100, 300, 12 }, 200, BLACK, LIGHTBLUE, PINK, false, pEditorPanel);
	DropDown*	pSkinPicker		= new DropDown	(L"Skin: "	, &pDummy->iPaintKit, &(SkinChanger::g_mWeapSkins[0]), 5, { 50, 75, 300, 20 }, 200, LIGHTGRAY, pEditorPanel);
	DropDown*	pWeaponPicker	= new DropDown	(L"Weapon: ", &pDummy->nItemId, &SkinChanger::g_mWeapNames, 5, { 50, 50, 300, 20 }, 200, LIGHTGRAY, pEditorPanel);

	ElementEditor<SkinChanger::SkinInfo>* pEditor = new ElementEditor<SkinChanger::SkinInfo>(pDummy, pEditorPanel);

	pWeaponPicker->GetContainer()->SetCallback(GUI_EVENT_TYPE::RELEASE, std::bind(SetSkinList, pWeaponPicker, pSkinPicker));

	pEditor->SetCallback(GUI_EVENT_TYPE::RELEASE, std::bind(SetSkinList, pWeaponPicker, pSkinPicker));

	g_pGUIContainer->AddElement(pEditor);

	g_pGUIContainer->AddElement(pSkinsList = new GUIList<SkinChanger::SkinInfo>("Skins", ROW_HEIGHT, &Settings.SkinChanger.Skins, pEditor, { 50, 50, COLUMN_WIDTH, 400 }, LIGHTGRAY, pSkinChanger));

	pSkinsList->SetCallback(GUI_EVENT_TYPE::RELEASE, std::bind(SetSkinList, pWeaponPicker, pSkinPicker));

	pEditor->SetDrawState(false);

	return pEditor;
}

void ShowMenu() {
	g_bMenuOpen = !g_bMenuOpen;
	g_pInput->EnableInput(!g_bMenuOpen);
	
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
	ChangeChildren(pAimbot, false);
	ChangeChildren(pESP, false);
	ChangeChildren(pChams, false);
	ChangeChildren(pSkinChanger, false);
	ChangeChildren(pSettings, false);

	pAimEditor->SetDrawState(false);
	pSkinEditor->SetDrawState(false);

	ChangeChildren(pPanel, true);

	g_pGUIContainer->GenerateMap();
}

void SetSkinList(DropDown* pWeaponPicker, DropDown* pSkinPicker) {
	pSkinPicker->SetMapPointerW(&(SkinChanger::g_mWeapSkins[*(int*)pWeaponPicker->GetValuePointer()]));
}