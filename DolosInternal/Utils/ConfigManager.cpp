#include "ConfigManager.h"

bool InitializeConfig() {

    return LoadConfig("F:\\Coding Projects\\VS\\DolosInternal\\DolosInternal\\Resources\\Config.json");

}

bool UninitializeConfig() {
    return SaveConfig("F:\\Coding Projects\\VS\\DolosInternal\\DolosInternal\\Resources\\Save.json");
    

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

    Settings.Aimbot.Active  = pAimbot->GetBoolean       ("active");
    Settings.Aimbot.Silent  = pAimbot->GetBoolean       ("silent");
    Settings.Aimbot.FOV     = pAimbot->GetNumber<float> ("fov");

    JsonObject* pPlayers = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("players");

    Settings.Visuals.Players.ShowTeam   = pPlayers->GetBoolean("show_team");
    Settings.Visuals.Players.ShowEnemy  = pPlayers->GetBoolean("show_enemy");
    Settings.Visuals.Players.DrawBones  = pPlayers->GetBoolean("draw_bones");
    Settings.Visuals.Players.DrawHealth = pPlayers->GetBoolean("draw_health");
    Settings.Visuals.Players.DrawArmor  = pPlayers->GetBoolean("draw_armor");

    Settings.Visuals.Players.EnemyColor = ParseColor(pPlayers->GetString("enemy_color"));
    Settings.Visuals.Players.TeamColor  = ParseColor(pPlayers->GetString("team_color"));

    JsonObject* pWeapons = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("weapons");

    Settings.Visuals.Weapons.Enabled    = pWeapons->GetBoolean("enabled");

    Settings.Visuals.Weapons.Color      = ParseColor(pWeapons->GetString("color"));
}

void UpdateValues() {
    JsonObject* pAimbot = g_pParsedConfig->GetJsonObject("aimbot");

    pAimbot->SetBoolean ("active"   , Settings.Aimbot.Active);
    pAimbot->SetBoolean ("silent"   , Settings.Aimbot.Silent);
    pAimbot->SetNumber  ("fov"      , Settings.Aimbot.FOV);

    JsonObject* pPlayers = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("players");

    pAimbot->SetBoolean("show_team"     , Settings.Visuals.Players.ShowTeam);
    pAimbot->SetBoolean("show_enemy"    , Settings.Visuals.Players.ShowEnemy);
    pAimbot->SetBoolean("draw_bones"    , Settings.Visuals.Players.DrawBones);
    pAimbot->SetBoolean("draw_health"   , Settings.Visuals.Players.DrawHealth);
    pAimbot->SetBoolean("draw_armor"    , Settings.Visuals.Players.DrawArmor);

    pAimbot->SetString("enemy_color"    , std::to_string(Settings.Visuals.Players.EnemyColor));
    pAimbot->SetString("team_color"     , std::to_string(Settings.Visuals.Players.TeamColor));

    JsonObject* pWeapons = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("weapons");

    pAimbot->SetBoolean("enabled"   , Settings.Visuals.Weapons.Enabled);

    pAimbot->SetString("color"      , std::to_string(Settings.Visuals.Weapons.Color));
}

D3DCOLOR ParseColor(const std::string& szColor) {
    return strtoul(szColor.c_str(), 0, 16);
}

