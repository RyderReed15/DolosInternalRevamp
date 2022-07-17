#pragma once

#ifndef ENTITY_DATA_H
#define ENTITY_DATA_H

#include "../SDK/SDK.h"

#define INCH_TO_METER       0.0254f

enum BONES {
    GROUND = 1,
    HEIGHT,
    PELVIS,
    SPINE1,
    SPINE2,
    CHEST,
    NECK,
    HEAD,
    NECK2
};

namespace EntityData {

    struct BaseData {

        bool    bAccessible; // Data may be incorrect or unreadable if false | possibly use mutex lock instead however may cause stuttering or frame drops
        int     iDistance;
        Vector  vObbsMin, vObbsMax;
        matrix3x4_t mCoordinateFrame;
    };

    struct WeaponData : BaseData {
        EHANDLE     eOwner;
        Vector4D    vBounds;
        wchar_t     wszName[128];
        char        szUnLocalizedName[128];
    };

    struct PlayerData : BaseData {
        Vector  vPosition;

        bool    bFriendly;

        int     iHealth;
        int     iArmor;
        int     iTeam;

        char    szName[128];

        Vector4D                vBounds;
        std::vector<Vector4D>   vBones;
    };

    struct LocalPlayerData {
        Vector  vPosition;

        bool    bAccessible; // Not needed as no data like pointers or vectors that could go bad by end and local player should never be deleted

        int     iHealth;
        int     iArmor;
        int     iTeam;

        EHANDLE eHandle;

        Vector vAimPunch;
        Vector vViewAngles;

    };

    

    void                UpdateEntityData        (void);
    void                UpdateWeaponData        (CBaseEntity* pWeapon);
    void                UpdatePlayerData        (CBaseEntity* pPlayer);
    void                UpdateLocalPlayerData   (void);
    void                UpdatePlayerBones       (IClientEntity* pPlayer);

    Vector4D            GetBoundingBox          (Vector vMin, Vector vMax, matrix3x4_t mCoordinateFrame);

    bool                WorldToScreen           (Vector2D& vScreen, Vector vPos);
    bool                WorldToScreen           (Vector2D& vScreen, Vector vPos, VMatrix vMatrix);

    LocalPlayerData*    GetLocalPlayerData      (void);
    PlayerData*         GetPlayerData           (const int nIndex);
    PlayerData*         GetPlayerData           (CBaseEntity* pPlayer);
    std::unordered_map<int, PlayerData>* GetAllPlayerData(void);

    WeaponData*         GetWeaponData           (const int nIndex);
    WeaponData*         GetWeaponData           (CBaseEntity* pWeapon);
    std::unordered_map<int, WeaponData>* GetAllWeaponData(void);


}

#endif // !ENTITY_DATA_H
