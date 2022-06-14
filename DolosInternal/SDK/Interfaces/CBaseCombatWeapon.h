#pragma once

#ifndef BASE_COMBAT_WEAPON_H
#define BASE_COMBAT_WEAPON_H

#include "../../Utils/Macros.h"

#include "IClientEntity.h"



class CBaseAttributableItem : public CBaseEntity {
public:
    PNETVAR(int, FallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
    PNETVAR(float, FallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");
    PNETVAR(int, FallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
    PNETVAR(int, FallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");

    PNETVAR(__int32, OriginalOwnerLow, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    PNETVAR(__int32, OriginalOwnerHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");


    int* ItemIDHigh() {
        return (int*)((int)this + (int)GetNetvar("DT_AttributeContainer", "m_Item") + (int)GetNetvar("DT_BaseAttributableItem", "m_AttributeManager") + GetNetvar("DT_ScriptCreatedItem", "m_iItemIDHigh"));

    }
	int16_t* ItemDefinitionIndex() {
        return (int16_t*)((int)this + (int)GetNetvar("DT_AttributeContainer", "m_Item") + (int)GetNetvar("DT_BaseAttributableItem", "m_AttributeManager") + GetNetvar("DT_ScriptCreatedItem", "m_iItemDefinitionIndex"));

    }
    int* AccountID() {
        return (int*)((int)this + (int)GetNetvar("DT_AttributeContainer", "m_Item") + (int)GetNetvar("DT_BaseAttributableItem", "m_AttributeManager") + GetNetvar("DT_ScriptCreatedItem", "m_iAccountID"));

    }

    char* CustomName() {
        return (char*)((int)this + (int)GetNetvar("DT_AttributeContainer", "m_Item") + (int)GetNetvar("DT_BaseAttributableItem", "m_AttributeManager") + GetNetvar("DT_ScriptCreatedItem", "m_szCustomName"));
    }

    int* EntityQuality() {
        return (int*)((int)this + (int)GetNetvar("DT_AttributeContainer", "m_Item") + (int)GetNetvar("DT_BaseAttributableItem", "m_AttributeManager") + GetNetvar("DT_ScriptCreatedItem", "m_iEntityQuality"));
    }
    int GetWeaponId() {
        return *ItemDefinitionIndex();
    }

};

class CBaseCombatWeapon : public CBaseAttributableItem
{
public:
    NETVAR(float, m_flNextPrimaryAttack, "DT_LocalActiveWeaponData", "m_flNextPrimaryAttack");
    NETVAR(float, m_flNextSecondaryAttack, "DT_LocalActiveWeaponData", "m_flNextSecondaryAttack");
    NETVAR(int, m_iClip1, "DT_BaseCombatWeapon", "m_iClip1");
    NETVAR(int, m_iClip2, "DT_BaseCombatWeapon", "m_iClip2");
    NETVAR(float, m_flRecoilIndex, "DT_WeaponCSBase", "m_flRecoilIndex");
	NETVAR(int, m_iViewModelIndex, "DT_BaseCombatWeapon", "m_iViewModelIndex");
    NETVAR(int, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
    NETVAR(int, m_iWorldDroppedModelIndex, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
    NETVAR(bool, m_bPinPulled, "DT_BaseCSGrenade", "m_bPinPulled");
    NETVAR(float, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
    NETVAR(float, m_flPostponeFireReadyTime, "DT_BaseCombatWeapon", "m_flPostponeFireReadyTime");
    NETVAR(EHANDLE, m_hWeaponWorldModel, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");

};

class CCSWeaponInfo {
public:
    int8_t pad0[20];
    int32_t iMaxClip1;
    int8_t pad1[12];
    int32_t iMaxReservedAmmo;
    int8_t pad2[96];
    char* szHudName;
    char* szWeaponName;
    int8_t pad3[56];
    int32_t iWeaponType;
    int8_t pad4[4];
    int32_t iWeaponPrice;
    int32_t iKillAward;
    int8_t pad5[20];
    uint8_t bFullAuto;
    int8_t pad6[3];
    int32_t iDamage;
    float_t flArmorRatio;
    int32_t iBullets;
    float_t flPenetration;
    int8_t pad7[8];
    float_t flRange;
    float_t flRangeModifier;
    int8_t pad8[16];
    uint8_t bHasSilencer;
    int8_t pad9[15];
    float_t flSpread;
    float_t flSpreadAlt;
    int8_t pad10[76];
    int32_t iRecoilSeed;
    int8_t pad11[32];
};
enum WeaponIndices
{
    WEAPON_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_ZONE_REPULSOR = 20,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SHIELD = 37,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS = 69,
	WEAPON_BREACHCHARGE = 70,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE = 75,
	WEAPON_HAMMER = 76,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB = 81,
	WEAPON_DIVERSION = 82,
	WEAPON_FRAG_GRENADE = 83,
	WEAPON_SNOWBALL = 84,
	WEAPON_BUMPMINE = 85,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	STUDDED_BROKENFANG_GLOVES = 4725,
	STUDDED_BLOODHOUND_GLOVES = 5027,
	T_GLOVES = 5028,
	CT_GLOVES = 5029,
	SPORTY_GLOVES = 5030,
	SLICK_GLOVES = 5031,
	LEATHER_HANDWRAPS = 5032,
	MOTORCYCLE_GLOVES = 5033,
	SPECIALIST_GLOVES = 5034,
	STUDDED_HYDRA_GLOVES = 5035
};

#endif // !BASE_COMBAT_WEAPON_H
