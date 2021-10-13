#pragma once

#ifndef CLIENT_ENTITY_H
#define CLIENT_ENTITY_H


#include "../../Utils/Macros.h"

#include "../Math/Vector.h"
#include "../Math/VMatrix.h"

#include "IClientNetworkable.h"
#include "IClientRenderable.h"
#include "IClientUnknown.h"
#include "IClientThinkable.h"
#include "IClientEntityList.h"

class CBaseCombatWeapon;
class CCSWeaponInfo;
class IClientEntityList;
extern IClientEntityList* g_pClientEntityList;

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
    virtual void Release(void) = 0;
    virtual void GetMouth(void) = 0;
    virtual Vector& GetAbsOrigin(void) const = 0;//in broken place use GetOrigin Below
    virtual const Vector& GetAbsAngles(void) const = 0;



    NETVAR(int, GetTeam, "DT_BaseEntity", "m_iTeamNum");
    PNETVAR(int, GetFlagsPointer, "DT_BasePlayer", "m_fFlags");
    NETVAR(Vector, GetVecOrigin, "DT_BaseEntity", "m_vecOrigin");
    NETVAR(Vector, GetVecViewOffset, "DT_LocalPlayerExclusive", "m_vecViewOffset[0]");
    ONETVAR(int, GetGlowIndex, "DT_CSPlayer", "m_flFlashDuration", 24);
    ONETVAR(matrix3x4_t, GetCoordinateFrame, "DT_BaseEntity", "m_CollisionGroup", -48);
    NETVAR(EHANDLE, GetActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
    NETVAR(int, GetTickBase, "DT_LocalPlayerExclusive", "m_nTickBase");
    NETVAR(int, GetArmor, "DT_CSPlayer", "m_ArmorValue");
    PNETVAR(bool, SpottedPointer, "DT_BaseEntity", "m_bSpotted");
    ONETVAR(int, GetCrosshairId, "DT_CSPlayer", "m_bHasDefuser", 92);
    NETVAR(float, GetSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
    NETVAR(bool, GetImmunity, "DT_CSPlayer", "m_bGunGameImmunity")

    ONETVAR(matrix3x4_t*, GetBoneMatrix, "DT_BaseAnimating", "m_nForceBone", 28);
    int GetFlags() {
        return *GetFlagsPointer();
    }
    void SetFlags(int iFlags) {
        *GetFlagsPointer() = iFlags;
    }
    void SetSpotted(bool bSpotted) {
        *SpottedPointer() = bSpotted;
    }

    Vector GetEyePos() {
        return GetVecOrigin() + GetVecViewOffset();
    }
    Vector GetBonePos(int iBoneId) {
        if (iBoneId < 0) {
            return { 0,0,-999999 };
        }
        return (GetBoneMatrix())[iBoneId].GetOrigin();
    }
    VFUNC(const Vector&, GetOrigin, 10, (), (this));
    VFUNC(int, GetHealth, 122, (), (this));
    VFUNC(bool, IsAlive, 156, (), (this));
    VFUNC(bool, IsPlayer, 158, (), (this));
    VFUNC(bool, IsWeapon, 166, (), (this));
    VFUNC(float, GetInaccuracy, 483, (), (this));
    VFUNC(CCSWeaponInfo*, GetWeaponData, 461, (), (this));

    CBaseCombatWeapon* GetWeapon(void)
    {

        return (CBaseCombatWeapon*)g_pClientEntityList->GetClientEntityFromHandle(GetActiveWeapon());
    }

    Vector GetAimPunch() {
        return *(Vector*)((int)this + (int)GetNetvar("DT_LocalPlayerExclusive", "m_Local") + GetNetvar("DT_Local", "m_aimPunchAngle"));

    }
    bool SanityCheck() {
        return IsAlive() && !IsDormant() && !GetImmunity();
    }
};
#endif // !CLIENT_ENTITY_H