#include "SkinChanger.h"

std::map<CBaseCombatWeapon*, int> g_mOwnedWeapons; // this will hoepfully go when implementing an event listener
std::map<int, int> g_mNewModels;
std::map<int, std::string> g_mModelNames;

//Rework to set g_mOwnedWeapons in PreTick to prevent bugs and game errors
//Reqork to use entire list of changed skins
//Changes models and item index before frame stage notify call
void SkinChanger::PreTick() {
	for (int i = 1; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pClientEntityList->GetClientEntity(i);
		if (!pWeapon) {
			continue;
		}
		//Set weapon model before skin or else skin wont properly apply

		if (g_mOwnedWeapons[pWeapon]) {
			if (Settings.SkinChanger.Skins[g_mOwnedWeapons[pWeapon]]) {

				if (g_mNewModels.count(g_mOwnedWeapons[pWeapon])) *pWeapon->ModelIndex() = g_mNewModels.at(g_mOwnedWeapons[pWeapon]);
				
			}
		}
		if (!g_pClientEntityList->GetClientEntityFromHandle(pWeapon->GetOwner())) {
			continue;
		}
		if (g_pClientEntityList->GetClientEntityFromHandle(pWeapon->GetOwner()) != g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer())) {

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

			if (pSkin != nullptr) {

				if (g_mNewModels.count(g_mOwnedWeapons[pWeapon])) *pWeapon->ModelIndex() = g_mNewModels.at(g_mOwnedWeapons[pWeapon]);
				else if (g_mNewModels.count(pSkin->nItemDefIndex)) *pWeapon->ModelIndex() = g_mNewModels.at(pSkin->nItemDefIndex);
			

				ChangeItemIndex(pWeapon, pSkin->nItemDefIndex);

			}
		}
	}
}
//Changes model and skin after frame stage notifyu
void SkinChanger::PostTick() {
	for (int i = 1; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pClientEntityList->GetClientEntity(i);
		if (!pWeapon) {
			continue;
		}
		
		if (!g_pClientEntityList->GetClientEntityFromHandle(pWeapon->GetOwner())) {
			continue;
		}
		if (g_pClientEntityList->GetClientEntityFromHandle(pWeapon->GetOwner()) != g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer())) {

			continue;
		}
		
		if (pWeapon->IsWeapon()) {
			

			int nItemIndex = pWeapon->GetWeaponId();
			g_mOwnedWeapons[pWeapon] = nItemIndex;
			
			SkinStruct* pSkin = Settings.SkinChanger.Skins[g_mOwnedWeapons[pWeapon]];
			
			if (pSkin != nullptr) {

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

	CBaseViewModel* pViewModel = (CBaseViewModel*)g_pClientEntityList->GetClientEntityFromHandle(g_pLocalPlayer->GetViewModelHandle());

	if (!pViewModel) return;

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


bool SkinChanger::InitializeSkinChanger() {
	char name[MAX_PATH];

	GetModuleFileName(NULL, name, MAX_PATH);

	std::string szName = name;
	szName = szName.substr(0, szName.find_last_of('\\'));
	szName += "\\csgo\\scripts\\items\\items_game.txt";

	JsonObject* pItems = ParseJsonFile(szName.c_str());

	if (pItems) {
		JsonObject* pPrefabs = pItems->GetJsonObject("prefabs");
		JsonObject* pWeapons = pItems->GetJsonObject("items");

		for (size_t i = 0; i < pWeapons->m_vValues.size(); i++) {
			if (pWeapons->m_vValues[i].m_tType == VALUE_TYPE::OBJECT) {
				JsonObject* pItem = pWeapons->m_vValues[i].m_pObject;
				if (pItem->GetString("name").find("weapon") != -1 || pItem->GetString("prefab").find("hands") != -1) {
					g_mModelNames[strtol(pItem->m_szName.c_str(), 0, 10)] = ParseModelName(pItem, pPrefabs);

				}
			}
		}

		delete pItems;
		return true;
	}
	return false;

	
}
