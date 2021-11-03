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
class CBaseEntity;
class CCSWeaponInfo;
class IClientEntityList;
extern IClientEntityList* g_pClientEntityList;



class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
    virtual void            Release     (void) = 0;
    virtual void            GetMouth    (void) = 0;
    virtual Vector&         GetAbsOrigin(void) const = 0;//in broken place use GetOrigin Below
    virtual const Vector&   GetAbsAngles(void) const = 0;



    NETVAR  (int            , GetTeam           , "DT_BaseEntity", "m_iTeamNum");
    ONETVAR (matrix3x4_t    , GetCoordinateFrame, "DT_BaseEntity", "m_CollisionGroup", -48);
    PNETVAR (bool           , SpottedPointer    , "DT_BaseEntity", "m_bSpotted");
    NETVAR  (float          , GetSimulationTime , "DT_BaseEntity", "m_flSimulationTime");
    NETVAR  (Vector         , GetVecOrigin      , "DT_BaseEntity", "m_vecOrigin");

    PNETVAR (int            , GetFlagsPointer   , "DT_BasePlayer", "m_fFlags");
    NETVAR  (int            , GetViewModelHandle, "DT_BasePlayer", "m_hViewModel[0]");

    ONETVAR (matrix3x4_t*   , GetBoneMatrix     , "DT_BaseAnimating", "m_nForceBone", 28);

    NETVAR  (int            , GetTickBase       , "DT_LocalPlayerExclusive", "m_nTickBase");
    NETVAR  (Vector         , GetVecViewOffset  , "DT_LocalPlayerExclusive", "m_vecViewOffset[0]");

    NETVAR  (EHANDLE        , GetActiveWeapon   , "DT_BaseCombatCharacter" , "m_hActiveWeapon");

    ONETVAR (int            , GetGlowIndex      , "DT_CSPlayer"             , "m_flFlashDuration", 24);
    NETVAR  (int            , GetArmor          , "DT_CSPlayer"             , "m_ArmorValue");
    ONETVAR (int            , GetCrosshairId    , "DT_CSPlayer"             , "m_bHasDefuser", 92);
    NETVAR  (bool           , GetImmunity       , "DT_CSPlayer"             , "m_bGunGameImmunity");

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
    VFUNC(const Vector& , GetOrigin     , 10 , (), (this));
    VFUNC(int           , GetHealth     , 122, (), (this));
    VFUNC(bool          , IsAlive       , 156, (), (this));
    VFUNC(bool          , IsPlayer      , 158, (), (this));
    VFUNC(bool          , IsWeapon      , 166, (), (this));
    VFUNC(float         , GetInaccuracy , 483, (), (this));
    VFUNC(CCSWeaponInfo*, GetWeaponData , 461, (), (this));

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

class CBaseEntity : public IClientEntity
{
public:
    NETVAR(EHANDLE  , GetOwner  , "DT_BaseEntity", "m_hOwnerEntity");
    PNETVAR(int     , ModelIndex, "DT_BaseEntity", "m_nModelIndex");

};
#endif // !CLIENT_ENTITY_H