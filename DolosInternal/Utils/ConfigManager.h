#pragma once

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "JsonParser/JsonParser.h"
#include <d3d9.h>
#include <vector>
#include <string>

inline const char* version = "beta-1.0";



inline JsonObject*  g_pParsedConfig;   


struct Config {
    struct AimConfig {
        bool Active;
        bool Silent;
        float FOV;
    } Aimbot;
    struct VisualsConfig {
        struct PlayerViz {
            bool ShowTeam;
            bool ShowEnemy;
            bool DrawBones;
            bool DrawHealth;
            bool DrawArmor;
            D3DCOLOR TeamColor;
            D3DCOLOR EnemyColor;
        } Players;
        struct WeaponViz {
            bool Enabled;
            D3DCOLOR Color;
        } Weapons;
       
    } Visuals;
    struct MiscConfig {
        bool b;
    } Misc;
    struct SkinConfig {
        bool b;
    } Skins;
    struct TriggerConfig {
        bool b;
    } Trigger;
    struct RecoilConfig {
        bool b;
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

D3DCOLOR ParseColor(const std::string& szColor);


#endif // !CONFIG_MANAGER_H
