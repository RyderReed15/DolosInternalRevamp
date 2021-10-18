#pragma once

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "JsonParser/JsonParser.h"
#include <vector>
#include <string>

inline const char* version = "beta-1.0";

struct Config;

inline JsonObject* g_pParsedConfig;   
inline Config*     g_pSettings;

struct Config {
    struct AimConfig {
        bool Active;
        bool Silent;
        float FOV;
    } Aimbot;
    struct VisualsConfig {
        bool b;
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



bool InitializeConfig   (void);
bool UninitializeConfig (void);
bool LoadConfig         (const char* szPath);
bool SaveConfig         (const char* szPath);
void StoreValues        (void);
void UpdateValues       (void);


#endif // !CONFIG_MANAGER_H
