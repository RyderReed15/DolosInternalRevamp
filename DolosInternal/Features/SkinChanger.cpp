#include "SkinChanger.h"

std::map<CBaseCombatWeapon*, int> g_mOwnedWeapons;

//Rework to set g_mOwnedWeapons in PreTick to prevent bugs and game errors
void SkinChanger::PreTick() {
	for (int i = 1; i < g_pClientEntityList->GetHighestEntityIndex(); i++) {
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_pClientEntityList->GetClientEntity(i);
		if (!pWeapon) {
			continue;
		}
		if (g_mOwnedWeapons[pWeapon]) {
			SkinStruct* pSkin = Settings.SkinChanger.Skins[g_mOwnedWeapons[pWeapon]];
			if (pSkin != nullptr) {

				if (g_mOwnedWeapons[pWeapon] == 507) {

					*pWeapon->ModelIndex() = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
				}
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

			if (g_mOwnedWeapons[pWeapon]) {
				pSkin = Settings.SkinChanger.Skins[g_mOwnedWeapons[pWeapon]];
			}
			else {

				int nItemIndex = pWeapon->GetWeaponId();
				pSkin = Settings.SkinChanger.Skins[nItemIndex];
			}

			if (pSkin != nullptr) {

				if (g_mOwnedWeapons[pWeapon] == 507 || pSkin->nItemDefIndex == 507) {

					*pWeapon->ModelIndex() = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
				}


				ChangeItemIndex(pWeapon, pSkin->nItemDefIndex);

				

			}
		}
	}
}
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
				if (g_mOwnedWeapons[pWeapon] == 507 || pSkin->nItemDefIndex == 507) {

					*pWeapon->ModelIndex() = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
				}
				
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
	}

	*pItemIdHigh = -1;

}

void SkinChanger::OverrideModel(CBaseCombatWeapon* pWeapon, int nItemDefinitionIndex){
	if (nItemDefinitionIndex != 507 || !g_pLocalPlayer) return;

	CBaseViewModel* pViewModel = (CBaseViewModel*)g_pClientEntityList->GetClientEntityFromHandle(g_pLocalPlayer->GetViewModelHandle());

	if (!pViewModel) return;

	CBaseAttributableItem* pViewModelWeapon = (CBaseAttributableItem*)g_pClientEntityList->GetClientEntityFromHandle(pViewModel->GetWeapon());
	if (pViewModelWeapon != pWeapon) return;


	//Get name from item def index;
	*pViewModel->ModelIndex() = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	

}
