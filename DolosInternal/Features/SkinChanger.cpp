#include "SkinChanger.h"

std::map<CBaseCombatWeapon*, int> g_mOwnedWeapons; // this will hoepfully go when implementing an event listener
std::unordered_map<int, int> g_mNewModels;
std::unordered_map<int, std::string> g_mModelNames;
std::unordered_map<int, std::string> g_mWeapNames;
std::unordered_map<std::string, int> g_mWeapIds;
std::unordered_map<int, std::vector<SkinChanger::GameSkin>> g_mWeapSkins; // I very much dislike this but have no better ideas right now

//Counter-Strike Global Offensive\csgo\scripts\items\items_game_cdn.txt contains links to pictures for all skins for use in gui
// //Counter-Strike Global Offensive\csgo\scripts\items\items_game.txt contains all skin ids paired with their names under paint_kits 
//Rework to set g_mOwnedWeapons in PreTick to prevent bugs and game errors
//Reqork to use entire list of changed skins
//Changes models and item index before frame stage notify call
void SkinChanger::PreTick() {
	if (!g_pLocalPlayer) return;
	for (int i = 1; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pClientEntityList->GetClientEntity(i);
		if (!pWeapon) {
			continue;
		}
		//Set weapon model before skin or else skin wont properly apply

		if (g_mOwnedWeapons[pWeapon] && Settings.SkinChanger.Skins[g_mOwnedWeapons[pWeapon]]) {

			if (g_mNewModels.count(g_mOwnedWeapons[pWeapon])) *pWeapon->ModelIndex() = g_mNewModels.at(g_mOwnedWeapons[pWeapon]);

		}
		if (!g_pClientEntityList->GetClientEntityFromHandle(pWeapon->GetOwner()) || g_pClientEntityList->GetClientEntityFromHandle(pWeapon->GetOwner()) != g_pLocalPlayer) {
			continue;
		}

		if (pWeapon->IsWeapon()) {


			SkinStruct* pSkin;
			int nItemIndex = pWeapon->GetWeaponId();

			if (g_mOwnedWeapons[pWeapon] && !Settings.SkinChanger.Skins[nItemIndex]) {
				//Skin is for a changed model in this case

				pSkin = Settings.SkinChanger.Skins[g_mOwnedWeapons[pWeapon]];
			}
			else {

				pSkin = Settings.SkinChanger.Skins[nItemIndex];
			}

			if (pSkin) {

				if (g_mNewModels.count(g_mOwnedWeapons[pWeapon])) *pWeapon->ModelIndex() = g_mNewModels.at(g_mOwnedWeapons[pWeapon]);
				else if (g_mNewModels.count(pSkin->nItemDefIndex)) *pWeapon->ModelIndex() = g_mNewModels.at(pSkin->nItemDefIndex);
			

				ChangeItemIndex(pWeapon, pSkin->nItemDefIndex);

			}
		}
	}
}
//Changes model and skin after frame stage notifyu
void SkinChanger::PostTick() {

	if (!g_pLocalPlayer) return;
	for (int i = 1; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pClientEntityList->GetClientEntity(i);
		if (!pWeapon) {
			continue;
		}
		
		if (!g_pClientEntityList->GetClientEntityFromHandle(pWeapon->GetOwner()) || g_pClientEntityList->GetClientEntityFromHandle(pWeapon->GetOwner()) != g_pLocalPlayer) {
			continue;
		}
		
		if (pWeapon->IsWeapon()) {
			

			int nItemIndex = pWeapon->GetWeaponId();
			g_mOwnedWeapons[pWeapon] = nItemIndex;
			
			SkinStruct* pSkin = Settings.SkinChanger.Skins[g_mOwnedWeapons[pWeapon]];
			
			if (pSkin) {

				if (g_mNewModels.count(g_mOwnedWeapons[pWeapon])) *pWeapon->ModelIndex() = g_mNewModels.at(g_mOwnedWeapons[pWeapon]);
				else if (g_mNewModels.count(pSkin->nItemDefIndex)) *pWeapon->ModelIndex() = g_mNewModels.at(pSkin->nItemDefIndex);
				

				OverrideModel(pWeapon, g_mOwnedWeapons[pWeapon]);

				OverrideSkin(pWeapon, pSkin);

			}
		}
	}
}

void SkinChanger::OverrideSkin(CBaseCombatWeapon* pWeapon, SkinStruct* pSkinInfo) {
	int* pItemIdHigh = pWeapon->ItemIDHigh();
	if (!pItemIdHigh) {
		return;
	}
	if (!pSkinInfo->nItemDefIndex || pWeapon->GetWeaponId() == pSkinInfo->nItemDefIndex) {
		//Update skin info
		*pWeapon->EntityQuality() = pSkinInfo->iQuality;
		*pWeapon->FallbackPaintKit() = pSkinInfo->iPaintKit;
		*pWeapon->FallbackWear() = pSkinInfo->flWear;
		*pWeapon->FallbackStatTrak() = pSkinInfo->iStatTrak;
		*pWeapon->FallbackSeed() = pSkinInfo->iSeed;


		if (pSkinInfo->szCustomName) {
			sprintf_s(pWeapon->CustomName(), 32, "%s", pSkinInfo->szCustomName);
		}

		*pItemIdHigh = -1;

		if (pSkinInfo->iStatTrak) {
			player_info_s playerInfo;
			g_pEngineClient->GetPlayerInfo(g_pEngineClient->GetLocalPlayer(), &playerInfo);
			*(pWeapon->AccountID()) = playerInfo.xuid_low;
		}
	}
		
	
	
}

void SkinChanger::ChangeItemIndex(CBaseCombatWeapon* pWeapon, int nItemDefIndex) {
	int* pItemIdHigh = pWeapon->ItemIDHigh();
	if (!pItemIdHigh) {
		return;
	}

	if (nItemDefIndex && pWeapon->GetWeaponId() != nItemDefIndex) {
		*pWeapon->ItemDefinitionIndex() = nItemDefIndex;
		g_mNewModels[nItemDefIndex] = g_pModelInfo->GetModelIndex(g_mModelNames[nItemDefIndex].c_str());
	}

	*pItemIdHigh = -1;

}

void SkinChanger::OverrideModel(CBaseCombatWeapon* pWeapon, int nItemDefinitionIndex){
	if (!g_mNewModels.count(nItemDefinitionIndex) || !g_pLocalPlayer) return;

	//Get players view model 
	CBaseViewModel* pViewModel = (CBaseViewModel*)g_pClientEntityList->GetClientEntityFromHandle(g_pLocalPlayer->GetViewModelHandle());

	if (!pViewModel) return;

	//Make sure current weapon is the desired one
	CBaseAttributableItem* pViewModelWeapon = (CBaseAttributableItem*)g_pClientEntityList->GetClientEntityFromHandle(pViewModel->GetWeapon());
	if (pViewModelWeapon != pWeapon) return;


	//Get model from item def index;
	*pViewModel->ModelIndex() = g_mNewModels.at(nItemDefinitionIndex);
	

}



std::string ParseModelName(JsonObject* pItem, JsonObject* pPrefabs) {
	std::string szModel = pItem->GetString("model_player");
	if (szModel == "") {
		JsonObject * pPrefab = pPrefabs->GetJsonObject(pItem->GetString("prefab"));
		if (pPrefab) {
			szModel = pPrefab->GetString("model_player");
		}
	}

	return szModel.c_str();
}

//Parses game files to find weapon models backtracking through prefabs to find the root model - only does weapons and gloves
//Places them in a hashmap for easy access later
bool SkinChanger::InitializeModels(JsonObject* pItems) {
	JsonObject* pPrefabs = pItems->GetJsonObject("prefabs");
	JsonObject* pWeapons = pItems->GetJsonObject("items");

	for (size_t i = 0; i < pWeapons->m_vValues.size(); i++) {
		if (pWeapons->m_vValues[i].m_tType == VALUE_TYPE::OBJECT) {
			JsonObject* pItem = pWeapons->m_vValues[i].m_pObject;
			if (pItem->GetString("name").find("weapon") != -1 || pItem->GetString("prefab").find("hands") != -1) {

				g_mModelNames[strtol(pItem->m_szName.c_str(), 0, 10)] = ParseModelName(pItem, pPrefabs);
				g_mWeapNames[strtol(pItem->m_szName.c_str(), 0, 10)] = pItem->GetString("name");
				g_mWeapIds[pItem->GetString("name")] = strtol(pItem->m_szName.c_str(), 0, 10);

			}
		}
	}
	return true;
}
//Parses game files to find weapon and glove skins 
//Sorts them into an array based on their parent weapon
bool SkinChanger::InitializeSkins(JsonObject* pItems, std::string szPath) {

	std::unordered_map<std::string, int> mSkinIds;

	JsonObject* pPaintKits = pItems->GetJsonObject("paint_kits");

	for (size_t i = 0; i < pPaintKits->m_vValues.size(); i++) {
		if (pPaintKits->m_vValues[i].m_tType == VALUE_TYPE::OBJECT) {

			JsonObject* pKit = pPaintKits->m_vValues[i].m_pObject;

			if (pKit->GetString("name") != "") {
				std::string szKitName = pKit->GetString("name");
				for (size_t i = 0; i < szKitName.length(); i++) {
					szKitName[i] = std::tolower(szKitName[i]);
				}
				//Place all skins into a map
				mSkinIds[szKitName] = strtol(pKit->m_szName.c_str(), 0, 10);
			}
		}
	}


	std::ifstream fCDN; fCDN.open(szPath);

	if (fCDN.is_open()) {
		std::string skinName;
		std::string temp;
		std::getline(fCDN, temp);
		std::getline(fCDN, temp);
		std::getline(fCDN, temp);
		while (!fCDN.eof() && fCDN.good()) {

			std::getline(fCDN, skinName, '=');
			std::getline(fCDN, temp);
			for (size_t i = 0; i < skinName.length(); i++) {
				skinName[i] = std::tolower(skinName[i]);
			}
			int iStart = 0;
			//Each line contains skins name formatted as follows - weapon_name_skin_name - previously parsed are just skin_name
			while (skinName.find('_', iStart) != -1) {
				if (mSkinIds.count(skinName.substr(iStart, skinName.length() - iStart))) {
					//Suse weapon_name to get id and assign the skin to it
					g_mWeapSkins[g_mWeapIds[skinName.substr(0, iStart - 1)]].push_back({ skinName.substr(iStart, skinName.length() - iStart), mSkinIds[skinName.substr(iStart, skinName.length() - iStart)] });
					break;
				}
				iStart = skinName.find('_', iStart) + 1;
			}
		}
		fCDN.close();
		return true;
	}
	else {
		return false;
	}
}

bool SkinChanger::InitializeSkinChanger() {
	char name[MAX_PATH];

	GetModuleFileName(NULL, name, MAX_PATH);

	//Locate game files containing model and skin info
	std::string szItemsName = name;
	std::string szCDNName;
	szItemsName = szItemsName.substr(0, szItemsName.find_last_of('\\'));
	szCDNName = szItemsName;
	szItemsName += ITEMS_PATH;
	szCDNName += CDN_PATH;

	JsonObject* pItems = ParseJsonFile(szItemsName.c_str());

	if (pItems) {

		if (InitializeModels(pItems) && InitializeSkins(pItems, szCDNName)) {
			delete pItems;
			return true;
		}
		
		delete pItems;
		return false;
	}
	return false;

	
}
