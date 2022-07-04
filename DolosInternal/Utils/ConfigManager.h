#pragma once

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "Json--/JsonParser.h"
#include "../Features/SkinChanger.h"
#include <d3d9.h>
#include <vector>
#include <string>
#include <map>

inline const char* version = "beta-1.1";



inline JsonObject*  g_pParsedConfig;   

struct AimTarget {
    float FOV;
    int Bone;
    char Name[128];
    bool Enabled;
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
        float OveraimFactor;
        std::vector<AimTarget> Targets;
    } Aimbot;
    struct VisualsConfig {
        struct PlayerViz {
            bool ShowTeam;
            bool DrawBones;
            bool DrawHealth;
            bool DrawArmor;
            bool DrawName;
            D3DCOLOR TeamColor;
        } Friendly, Enemy;
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
        std::vector<SkinChanger::SkinInfo> Skins;
    } SkinChanger;
    struct TriggerConfig {
        bool    Enabled;
        float   Range;
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

D3DCOLOR                    ParseColor  (const std::string& szColor);
std::string                 WriteColor  (const D3DCOLOR cColor);
SkinChanger::SkinInfo       ParseSkin   (JsonObject* pSkinObject);
JsonObject*                 WriteSkin   (const SkinChanger::SkinInfo* pSkin);


#endif // !CONFIG_MANAGER_H
