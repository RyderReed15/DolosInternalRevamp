#include "SkinChanger.h"

void SkinChanger::Tick() {
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
			SkinStruct* pSkin = Settings.SkinChanger.Skins[pWeapon->GetWeaponId()];
			if (pSkin != nullptr) {
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
	*pItemIdHigh = -1;
	

	*pWeapon->FallbackPaintKit()	= pSkinInfo->iPaintKit;
	*pWeapon->FallbackWear()		= pSkinInfo->flWear;
	*pWeapon->FallbackStatTrak()	= pSkinInfo->iStatTrak;
	*pWeapon->FallbackSeed()		= pSkinInfo->iSeed;
	*pWeapon->EntityQuality()		= pSkinInfo->iQuality;
	
	if (pSkinInfo->szCustomName) {
		sprintf_s(pWeapon->CustomName(), 32, "%s", pSkinInfo->szCustomName);
	}
	if (pSkinInfo->iStatTrak) {
		player_info_s playerInfo;
		g_pEngineClient->GetPlayerInfo(g_pEngineClient->GetLocalPlayer(), &playerInfo);
		*(pWeapon->AccountID()) = playerInfo.xuid_low;
	}
	
}