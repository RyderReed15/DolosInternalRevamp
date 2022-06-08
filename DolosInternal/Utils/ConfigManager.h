#pragma once

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "Json--/JsonParser.h"
#include "../Features/SkinChanger.h"
#include <d3d9.h>
#include <vector>
#include <string>
#include <map>

inline const char* version = "beta-1.0";



inline JsonObject*  g_pParsedConfig;   

struct AimTarget {
    float FOV;
    int Bone;
};


struct Config {
    struct AimConfig {
        bool Active;
        bool Silent;
        bool TargetAll;
        bool Overaim;
        bool Curve;
        float RangeFactor;
        float AimTime;
        float WaitTime;
        float AntilockFactor;
        AimTarget* Targets;
        unsigned int TargetCount;
    } Aimbot;
    struct VisualsConfig {
        struct PlayerViz {
            bool ShowTeam;
            bool ShowEnemy;
            bool DrawBones;
            bool DrawHealth;
            bool DrawArmor;
            bool DrawName;
            D3DCOLOR TeamColor;
            D3DCOLOR EnemyColor;
        } Players;
        struct WeaponViz {
            bool Enabled;
            bool DrawName;
            D3DCOLOR Color;
        } Weapons;
       
    } Visuals;
    struct MiscConfig {
        bool Bhop;
    } Misc;
    struct SkinConfig {
        bool TrackKills;
        std::map<int, SkinChanger::SkinStruct*> Skins;
    } SkinChanger;
    struct TriggerConfig {
        bool b;
    } Trigger;
    struct RecoilConfig {
        bool Enabled;
        float ControlFactor;
    } Recoil;
    struct AntiAimConfig {
        bool b;
    } AntiAim;
};

inline Config       Settings;

bool InitializeConfig   (void);
bool UninitializeConfig (void);
bool LoadConfig         (const char* szPath);
bool SaveConfig         (const char* szPath);
void StoreValues        (void);
void UpdateValues       (void);
void UnloadSkins        (void);

D3DCOLOR                    ParseColor  (const std::string& szColor);
SkinChanger::SkinStruct*    ParseSkin   (JsonObject* pSkinObject);
JsonObject*                 WriteSkin   (SkinChanger::SkinStruct* pSkin);


#endif // !CONFIG_MANAGER_H
