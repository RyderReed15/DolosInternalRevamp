#include "ConfigManager.h"

bool InitializeConfig() {
    g_pSettings = new Config();
    LoadConfig("F:\\Coding Projects\\VS\\DolosInternal\\DolosInternal\\Resources\\Config.json");
    return g_pSettings;
}

bool UninitializeConfig() {
    SaveConfig("F:\\Coding Projects\\VS\\DolosInternal\\DolosInternal\\Resources\\Save.json");
    if (g_pSettings) {
        delete g_pSettings;
        return true;
    }
    return false;
}

bool LoadConfig(const char* szPath) {
    if (g_pParsedConfig) {
        delete g_pParsedConfig;
    }
    g_pParsedConfig = ParseJsonFile(szPath);
    if (g_pParsedConfig->GetString("version").compare(version)) {
        //Versions don't match, give the user a warning
    }
    try {
        StoreValues();
    }
    catch (char* e) {
        //Malformed Config
    }
    return g_pParsedConfig;
}

bool SaveConfig(const char* szPath) {
    if (g_pParsedConfig) {
        try {
            UpdateValues();
        }
        catch (char* e) {
            //Unable to write values
        }
        
        return WriteJsonFile(szPath, g_pParsedConfig);
    }
    return false;
}

void StoreValues() {
    JsonObject* pAimbot = g_pParsedConfig->GetJsonObject("aimbot");

    g_pSettings->Aimbot.Active  = pAimbot->GetBoolean       ("active");
    g_pSettings->Aimbot.Silent  = pAimbot->GetBoolean       ("silent");
    g_pSettings->Aimbot.FOV     = pAimbot->GetNumber<float> ("fov");
}

void UpdateValues() {
    JsonObject* pAimbot = g_pParsedConfig->GetJsonObject("aimbot");

    pAimbot->SetBoolean ("active"   , g_pSettings->Aimbot.Active);
    pAimbot->SetBoolean ("silent"   , g_pSettings->Aimbot.Silent);
    pAimbot->SetNumber  ("fov"      , g_pSettings->Aimbot.FOV);
}

