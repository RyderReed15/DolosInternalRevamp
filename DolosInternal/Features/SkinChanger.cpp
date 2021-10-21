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


			if (pWeapon->GetWeaponId() == WEAPON_AK47) {
				char name[] = "Impress";
				OverrideSkin(pWeapon, { 675, .6, 100, 1, name });
			}
			if (pWeapon->GetWeaponId() == WEAPON_M4A4) {
				char name[] = "Rawr XD";
				OverrideSkin(pWeapon, { 309, .9, 100, 1, name });
			}
			if (pWeapon->GetWeaponId() == WEAPON_AWP) {
				char name[] = "Hyper Based";
				OverrideSkin(pWeapon, { 475, .5, 500, 1, name });
			}

		}

	}
}

void SkinChanger::OverrideSkin(CBaseCombatWeapon* pWeapon, SkinStruct skinInfo) {
	int* pItemIdHigh = pWeapon->ItemIDHigh();
	if (!pItemIdHigh) {
		return;
	}
	*pItemIdHigh = -1;
	

	*pWeapon->FallbackPaintKit()	= skinInfo.iPaintKit;
	*pWeapon->FallbackWear()		= skinInfo.flWear;
	*pWeapon->FallbackStatTrak()	= skinInfo.iStatTrak;
	*pWeapon->FallbackSeed()		= skinInfo.iSeed;
	*pWeapon->EntityQuality() = 3;
	
	if (skinInfo.szCustomName) {
		sprintf_s(pWeapon->CustomName(), 32, "%s", skinInfo.szCustomName);
	}
	if (skinInfo.iStatTrak) {
		player_info_s playerInfo;
		g_pEngineClient->GetPlayerInfo(g_pEngineClient->GetLocalPlayer(), &playerInfo);
		*(pWeapon->AccountID()) = playerInfo.xuid_low;
	}
	
}