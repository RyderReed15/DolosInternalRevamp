#include "SkinChanger.h"

std::map<CBaseCombatWeapon*, int> g_mOwnedWeapons; // this will hoepfully go when implementing an event listener
std::map<int, int> g_mNewModels;

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
		g_mNewModels[nItemDefIndex] = GetModelIndexFromId(nItemDefIndex);
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


int SkinChanger::GetModelIndexFromId(int iWeaponId) {
	//HELP!!! This is auto generated but i can't consistently locate the file needed to dump

	switch ((WeaponIndices)iWeaponId)
	{
	case WeaponIndices::WEAPON_NONE:				return -1;
	case WeaponIndices::WEAPON_DEAGLE:				return g_pModelInfo->GetModelIndex("models/weapons/v_pist_deagle.mdl");
	case WeaponIndices::WEAPON_ELITE:				return g_pModelInfo->GetModelIndex("models/weapons/v_pist_elite.mdl");
	case WeaponIndices::WEAPON_FIVESEVEN:			return g_pModelInfo->GetModelIndex("models/weapons/v_pist_fiveseven.mdl");
	case WeaponIndices::WEAPON_GLOCK:				return g_pModelInfo->GetModelIndex("models/weapons/v_pist_glock18.mdl");
	case WeaponIndices::WEAPON_AK47:				return g_pModelInfo->GetModelIndex("models/weapons/v_rif_ak47.mdl");
	case WeaponIndices::WEAPON_AUG:					return g_pModelInfo->GetModelIndex("models/weapons/v_rif_aug.mdl");
	case WeaponIndices::WEAPON_AWP:					return g_pModelInfo->GetModelIndex("models/weapons/v_snip_awp.mdl");
	case WeaponIndices::WEAPON_FAMAS:				return g_pModelInfo->GetModelIndex("models/weapons/v_rif_famas.mdl");
	case WeaponIndices::WEAPON_G3SG1:				return g_pModelInfo->GetModelIndex("models/weapons/v_rif_g3sg1.mdl");
	case WeaponIndices::WEAPON_GALILAR:				return g_pModelInfo->GetModelIndex("models/weapons/v_rif_galilar.mdl");
	case WeaponIndices::WEAPON_M249:				return g_pModelInfo->GetModelIndex("models/weapons/v_mach_m249para.mdl");
	case WeaponIndices::WEAPON_M4A1:				return g_pModelInfo->GetModelIndex("models/weapons/v_rif_m4a1.mdl");
	case WeaponIndices::WEAPON_MAC10:				return g_pModelInfo->GetModelIndex("models/weapons/v_smg_mac10.mdl");
	case WeaponIndices::WEAPON_P90:					return g_pModelInfo->GetModelIndex("models/weapons/v_smg_p90.mdl");
	case WeaponIndices::WEAPON_ZONE_REPULSOR:		return g_pModelInfo->GetModelIndex("models/weapons/v_repulsor.mdl");
	case WeaponIndices::WEAPON_MP5SD:				return g_pModelInfo->GetModelIndex("models/weapons/v_smg_mp5sd.mdl");
	case WeaponIndices::WEAPON_UMP45:				return g_pModelInfo->GetModelIndex("models/weapons/v_smg_ump45.mdl");
	case WeaponIndices::WEAPON_XM1014:				return g_pModelInfo->GetModelIndex("models/weapons/v_shot_xm1014.mdl");
	case WeaponIndices::WEAPON_BIZON:				return g_pModelInfo->GetModelIndex("models/weapons/v_smg_bizon.mdl");
	case WeaponIndices::WEAPON_MAG7:				return g_pModelInfo->GetModelIndex("models/weapons/v_shot_mag7.mdl");
	case WeaponIndices::WEAPON_NEGEV:				return g_pModelInfo->GetModelIndex("models/weapons/v_mach_negev.mdl");
	case WeaponIndices::WEAPON_SAWEDOFF:			return g_pModelInfo->GetModelIndex("models/weapons/v_shot_sawedoff.mdl");
	case WeaponIndices::WEAPON_TEC9:				return g_pModelInfo->GetModelIndex("models/weapons/v_pist_tec9.mdl");
	case WeaponIndices::WEAPON_TASER:				return g_pModelInfo->GetModelIndex("models/weapons/v_eq_taser.mdl");
	case WeaponIndices::WEAPON_HKP2000:				return g_pModelInfo->GetModelIndex("models/weapons/v_pist_hkp2000.mdl");
	case WeaponIndices::WEAPON_MP7:					return g_pModelInfo->GetModelIndex("models/weapons/v_smg_mp7.mdl");
	case WeaponIndices::WEAPON_MP9:					return g_pModelInfo->GetModelIndex("models/weapons/v_smg_mp9.mdl");
	case WeaponIndices::WEAPON_NOVA:				return g_pModelInfo->GetModelIndex("models/weapons/v_shot_nova.mdl");
	case WeaponIndices::WEAPON_P250:				return g_pModelInfo->GetModelIndex("models/weapons/v_pist_p250.mdl");
	case WeaponIndices::WEAPON_SHIELD:				return g_pModelInfo->GetModelIndex("models/weapons/v_shield.mdl");
	case WeaponIndices::WEAPON_SCAR20:				return g_pModelInfo->GetModelIndex("models/weapons/v_snip_scar20.mdl");
	case WeaponIndices::WEAPON_SG556:				return g_pModelInfo->GetModelIndex("models/weapons/v_rif_sg556.mdl");
	case WeaponIndices::WEAPON_SSG08:				return g_pModelInfo->GetModelIndex("models/weapons/v_snip_ssg08.mdl");
	case WeaponIndices::WEAPON_KNIFEGG:				return g_pModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");
	case WeaponIndices::WEAPON_KNIFE:				return g_pModelInfo->GetModelIndex("models/weapons/v__knife_default_ct.mdl");
	case WeaponIndices::WEAPON_FLASHBANG:			return g_pModelInfo->GetModelIndex("models/weapons/v_eq_flashbang.mdl");
	case WeaponIndices::WEAPON_HEGRENADE:			return g_pModelInfo->GetModelIndex("models/weapons/v_eq_fraggrenade.mdl");
	case WeaponIndices::WEAPON_SMOKEGRENADE:		return g_pModelInfo->GetModelIndex("models/weapons/v_eq_smokegrenade.mdl");
	case WeaponIndices::WEAPON_MOLOTOV:				return g_pModelInfo->GetModelIndex("models/weapons/v_eq_molotov.mdl");
	case WeaponIndices::WEAPON_DECOY:				return g_pModelInfo->GetModelIndex("models/weapons/v_eq_decoy.mdl");
	case WeaponIndices::WEAPON_INCGRENADE:			return g_pModelInfo->GetModelIndex("models/weapons/v_eq_incendiarygrenade.mdl");
	case WeaponIndices::WEAPON_C4:					return g_pModelInfo->GetModelIndex("models/weapons/v_ied.mdl");
	case WeaponIndices::WEAPON_HEALTHSHOT:			return g_pModelInfo->GetModelIndex("models/weapons/v_healthshot.mdl");
	case WeaponIndices::WEAPON_KNIFE_T:				return g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	case WeaponIndices::WEAPON_M4A1_SILENCER:		return g_pModelInfo->GetModelIndex("models/weapons/v_rif_m4a1_s.mdl");
	case WeaponIndices::WEAPON_USP_SILENCER:		return g_pModelInfo->GetModelIndex("models/weapons/v_pist_223.mdl");
	case WeaponIndices::WEAPON_CZ75A:				return g_pModelInfo->GetModelIndex("models/weapons/v_pist_cz_75.mdl");
	case WeaponIndices::WEAPON_REVOLVER:			return g_pModelInfo->GetModelIndex("models/weapons/v_pist_revolver.mdl");
	case WeaponIndices::WEAPON_TAGRENADE:			return g_pModelInfo->GetModelIndex("models/weapons/v_sonar_bomb.mdl");
	case WeaponIndices::WEAPON_FISTS:				return g_pModelInfo->GetModelIndex("models/weapons/v_fists.mdl");
	case WeaponIndices::WEAPON_BREACHCHARGE:		return g_pModelInfo->GetModelIndex("models/weapons/v_breachcharge.mdl");
	case WeaponIndices::WEAPON_TABLET:				return g_pModelInfo->GetModelIndex("models/weapons/v_tablet.mdl");
	case WeaponIndices::WEAPON_MELEE:				return g_pModelInfo->GetModelIndex("models/weapons/v_fists.mdl");
	case WeaponIndices::WEAPON_AXE:					return g_pModelInfo->GetModelIndex("models/weapons/v_axe.mdl");
	case WeaponIndices::WEAPON_HAMMER:				return g_pModelInfo->GetModelIndex("models/weapons/v_hammer.mdl");
	case WeaponIndices::WEAPON_SPANNER:				return g_pModelInfo->GetModelIndex("models/weapons/v_spanner.mdl");
	case WeaponIndices::WEAPON_KNIFE_GHOST:			return g_pModelInfo->GetModelIndex("models/weapons/v_knife_ghost.mdl");
	case WeaponIndices::WEAPON_FIREBOMB:			return g_pModelInfo->GetModelIndex("models/weapons/v_eq_incendiarygrenade.mdl");
	case WeaponIndices::WEAPON_DIVERSION:			return g_pModelInfo->GetModelIndex("models/weapons/v_eq_decoy.mdl");
	case WeaponIndices::WEAPON_FRAG_GRENADE:		return g_pModelInfo->GetModelIndex("models/weapons/v_eq_fraggrenade.mdl");
	case WeaponIndices::WEAPON_SNOWBALL:			return g_pModelInfo->GetModelIndex("models/weapons/v_eq_snowball.mdl");
	case WeaponIndices::WEAPON_BUMPMINE:			return g_pModelInfo->GetModelIndex("models/weapons/v_bumpmine.mdl");
	case WeaponIndices::WEAPON_BAYONET:				return g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	case WeaponIndices::WEAPON_KNIFE_CSS:			return g_pModelInfo->GetModelIndex("models/weapons/v_knife_css.mdl");
	case WeaponIndices::WEAPON_KNIFE_FLIP:			return g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	case WeaponIndices::WEAPON_KNIFE_GUT:			return g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	case WeaponIndices::WEAPON_KNIFE_KARAMBIT:		return g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	case WeaponIndices::WEAPON_KNIFE_M9_BAYONET:	return g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	case WeaponIndices::WEAPON_KNIFE_TACTICAL:		return g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	case WeaponIndices::WEAPON_KNIFE_FALCHION:		return g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	case WeaponIndices::WEAPON_KNIFE_SURVIVAL_BOWIE:return g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	case WeaponIndices::WEAPON_KNIFE_BUTTERFLY:		return g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	case WeaponIndices::WEAPON_KNIFE_PUSH:			return g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	case WeaponIndices::WEAPON_KNIFE_CORD:			return g_pModelInfo->GetModelIndex("models/weapons/v_knife_cord.mdl");
	case WeaponIndices::WEAPON_KNIFE_CANIS:			return g_pModelInfo->GetModelIndex("models/weapons/v_knife_canis.mdl");
	case WeaponIndices::WEAPON_KNIFE_URSUS:			return g_pModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
	case WeaponIndices::WEAPON_KNIFE_GYPSY_JACKKNIFE:return g_pModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	case WeaponIndices::WEAPON_KNIFE_OUTDOOR:		return g_pModelInfo->GetModelIndex("models/weapons/v_knife_outdoor.mdl");
	case WeaponIndices::WEAPON_KNIFE_STILETTO:		return g_pModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	case WeaponIndices::WEAPON_KNIFE_WIDOWMAKER:	return g_pModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
	case WeaponIndices::WEAPON_KNIFE_SKELETON:		return g_pModelInfo->GetModelIndex("models/weapons/v_knife_skeleton.mdl");
	case WeaponIndices::STUDDED_BROKENFANG_GLOVES:	return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_brokenfang.mdl");
	case WeaponIndices::STUDDED_BLOODHOUND_GLOVES:	return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl");
	case WeaponIndices::T_GLOVES:					return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless_icon.mdl");
	case WeaponIndices::CT_GLOVES:					return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_hardknuckle/v_glove_ct_hardknuckle_icon.mdl");
	case WeaponIndices::SPORTY_GLOVES:				return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl");
	case WeaponIndices::SLICK_GLOVES:				return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl");
	case WeaponIndices::LEATHER_HANDWRAPS:			return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl");
	case WeaponIndices::MOTORCYCLE_GLOVES:			return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl");
	case WeaponIndices::SPECIALIST_GLOVES:			return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl");
	case WeaponIndices::STUDDED_HYDRA_GLOVES:		return g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl");
	default:										return -1;

	}


}
