#include "ConfigManager.h"

bool InitializeConfig() {

    return LoadConfig("F:\\Coding Projects\\VS\\DolosInternal\\DolosInternal\\Resources\\Save.json");

}

bool UninitializeConfig() {
    
    bool bReturn = SaveConfig("F:\\Coding Projects\\VS\\DolosInternal\\DolosInternal\\Resources\\Save.json");
    delete g_pParsedConfig;
    return bReturn;
    
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
    catch (char*) {
        //Malformed Config
    }
    return g_pParsedConfig;
}

bool SaveConfig(const char* szPath) {
    if (g_pParsedConfig) {
        try {
            UpdateValues();
        }
        catch (char*) {
            //Unable to write values
        }
        
        return WriteJsonFile(szPath, g_pParsedConfig);
    }
    return false;
}

void StoreValues() {
    JsonObject* pAimbot = g_pParsedConfig->GetJsonObject("aimbot");

    Settings.Aimbot.Active      = pAimbot->GetBoolean       ("enabled");
    Settings.Aimbot.Silent      = pAimbot->GetBoolean       ("silent");
    Settings.Aimbot.TargetAll   = pAimbot->GetBoolean       ("target_all");
    Settings.Aimbot.Overaim     = pAimbot->GetBoolean       ("overaim");
    Settings.Aimbot.Curve       = pAimbot->GetBoolean       ("curve");

    Settings.Aimbot.RangeFactor     = pAimbot->GetNumber<float> ("range_dec_multiplier");
    Settings.Aimbot.AimTime         = pAimbot->GetNumber<float> ("time_to_aim");
    Settings.Aimbot.WaitTime        = pAimbot->GetNumber<float> ("wait_time");
    Settings.Aimbot.AntilockFactor  = pAimbot->GetNumber<float> ("antilock_factor");
    Settings.Aimbot.OveraimFactor   = pAimbot->GetNumber<float> ("overaim_factor");

    JsonArray* pTargets = pAimbot->GetJsonArray("targets");

    for (size_t i = 0; i < pTargets->GetSize(); i++) {
        Settings.Aimbot.Targets.push_back({
            pTargets->GetJsonObject(i)->GetNumber<float>("fov"),
            pTargets->GetJsonObject(i)->GetNumber<int>("bone"),
            "",
            pTargets->GetJsonObject(i)->GetBoolean("enabled")

            });
        strcpy_s(Settings.Aimbot.Targets[i].Name, 128, pTargets->GetJsonObject(i)->GetString("name").c_str());
    }

    JsonObject* pFriendly = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("friendly");

    Settings.Visuals.Friendly.ShowTeam   = pFriendly->GetBoolean("show_team");
    Settings.Visuals.Friendly.DrawBones  = pFriendly->GetBoolean("draw_bones");
    Settings.Visuals.Friendly.DrawHealth = pFriendly->GetBoolean("draw_health");
    Settings.Visuals.Friendly.DrawArmor  = pFriendly->GetBoolean("draw_armor");
    Settings.Visuals.Friendly.DrawName   = pFriendly->GetBoolean("draw_name");

    Settings.Visuals.Friendly.TeamColor = ParseColor(pFriendly->GetString("team_color"));

    JsonObject* pEnemy = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("enemy");

    Settings.Visuals.Enemy.ShowTeam     = pEnemy->GetBoolean("show_team");
    Settings.Visuals.Enemy.DrawBones    = pEnemy->GetBoolean("draw_bones");
    Settings.Visuals.Enemy.DrawHealth   = pEnemy->GetBoolean("draw_health");
    Settings.Visuals.Enemy.DrawArmor    = pEnemy->GetBoolean("draw_armor");
    Settings.Visuals.Enemy.DrawName     = pEnemy->GetBoolean("draw_name");

    Settings.Visuals.Enemy.TeamColor    = ParseColor(pEnemy->GetString("team_color"));

    JsonObject* pWeapons = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("weapons");

    Settings.Visuals.Weapons.Enabled    = pWeapons->GetBoolean("enabled");
    Settings.Visuals.Weapons.DrawName   = pWeapons->GetBoolean("draw_name");

    JsonObject* pRecoil = g_pParsedConfig->GetJsonObject("recoil");

    Settings.Recoil.Enabled         = pRecoil->GetBoolean       ("enabled");
    Settings.Recoil.ControlFactor   = pRecoil->GetNumber<float> ("factor");

    JsonObject* pMisc = g_pParsedConfig->GetJsonObject("misc");

    Settings.Misc.Bhop = pMisc->GetBoolean("bhop");

    JsonObject* pTrigger = g_pParsedConfig->GetJsonObject("trigger");

    Settings.Trigger.Enabled    = pTrigger->GetBoolean      ("enabled");
    Settings.Trigger.Range      = pTrigger->GetNumber<float>("range");

    Settings.Visuals.Weapons.Color  = ParseColor(pWeapons->GetString("color"));

    JsonObject* pSkinChanger = g_pParsedConfig->GetJsonObject("skin_changer");

    Settings.SkinChanger.TrackKills = pSkinChanger->GetBoolean("track_kills");

    JsonArray* pSkins = pSkinChanger->GetJsonArray("skins");

    for (size_t i = 0; i < pSkins->GetSize(); i++) {
        SkinChanger::SkinInfo pSkin = ParseSkin(pSkins->GetJsonObject(i));
        Settings.SkinChanger.Skins.push_back(pSkin);
    }
}

void UpdateValues() {
    JsonObject* pAimbot = g_pParsedConfig->GetJsonObject("aimbot");

    pAimbot->SetBoolean ("enabled"      , Settings.Aimbot.Active);
    pAimbot->SetBoolean ("silent"       , Settings.Aimbot.Silent);
    pAimbot->SetBoolean ("target_all"   , Settings.Aimbot.TargetAll);
    pAimbot->SetBoolean ("overaim"      , Settings.Aimbot.Overaim);
    pAimbot->SetBoolean ("curve"        , Settings.Aimbot.Curve);

    pAimbot->SetNumber("range_dec_multiplier"   , Settings.Aimbot.RangeFactor);
    pAimbot->SetNumber("time_to_aim"            , Settings.Aimbot.AimTime);
    pAimbot->SetNumber("wait_time"              , Settings.Aimbot.WaitTime);
    pAimbot->SetNumber("antilock_factor"        , Settings.Aimbot.AntilockFactor);
    pAimbot->SetNumber("overaim_factor"         , Settings.Aimbot.OveraimFactor);

    JsonArray* pTargets = pAimbot->GetJsonArray("targets");

    while (pTargets->GetSize()) {
        pTargets->RemoveValue(0);
    }

    for (size_t i = 0; i < Settings.Aimbot.Targets.size(); i++) {
        JsonObject* pTarget = new JsonObject();
        pTarget->AddNumber("fov", Settings.Aimbot.Targets[i].FOV);
        pTarget->AddNumber("bone", Settings.Aimbot.Targets[i].Bone);
        pTarget->AddString("name", Settings.Aimbot.Targets[i].Name);
        pTarget->AddBoolean("enabled", Settings.Aimbot.Targets[i].Enabled);
        pTargets->AddJsonObject(pTarget);
    }

    JsonObject* pFriendly = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("friendly");

    pFriendly->SetBoolean("show_team"     , Settings.Visuals.Friendly.ShowTeam);
    pFriendly->SetBoolean("draw_bones"    , Settings.Visuals.Friendly.DrawBones);
    pFriendly->SetBoolean("draw_health"   , Settings.Visuals.Friendly.DrawHealth);
    pFriendly->SetBoolean("draw_armor"    , Settings.Visuals.Friendly.DrawArmor);

    pFriendly->SetString("team_color"     , WriteColor(Settings.Visuals.Friendly.TeamColor));

    JsonObject* pEnemy = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("enemy");

    pEnemy->SetBoolean("show_team"  , Settings.Visuals.Enemy.ShowTeam);
    pEnemy->SetBoolean("draw_bones" , Settings.Visuals.Enemy.DrawBones);
    pEnemy->SetBoolean("draw_health", Settings.Visuals.Enemy.DrawHealth);
    pEnemy->SetBoolean("draw_armor" , Settings.Visuals.Enemy.DrawArmor);

    pEnemy->SetString("team_color"  , WriteColor(Settings.Visuals.Enemy.TeamColor));

    JsonObject* pWeapons = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("weapons");

    pWeapons->SetBoolean("enabled"   , Settings.Visuals.Weapons.Enabled);

    pWeapons->SetString("color"      , std::to_string(Settings.Visuals.Weapons.Color));

    JsonObject* pRecoil = g_pParsedConfig->GetJsonObject("recoil");

    pRecoil->SetBoolean("enabled"   , Settings.Recoil.Enabled);
    pRecoil->SetNumber("factor"     , Settings.Recoil.ControlFactor);

    JsonObject* pMisc = g_pParsedConfig->GetJsonObject("misc");

    pMisc->SetBoolean("bhop", Settings.Misc.Bhop);

    JsonObject* pTrigger = g_pParsedConfig->GetJsonObject("trigger");

    pTrigger->SetBoolean("enabled", Settings.Trigger.Enabled);
    pTrigger->SetNumber ("range", Settings.Trigger.Range);

    JsonObject* pSkinChanger = g_pParsedConfig->GetJsonObject("skin_changer");

    pSkinChanger->SetBoolean("track_kills", Settings.SkinChanger.TrackKills);

    JsonArray* pSkins = pSkinChanger->GetJsonArray("skins");

    while (pSkins->GetSize())
    {
        pSkins->RemoveValue(0);
    }

    
    for (std::vector<SkinChanger::SkinInfo>::iterator it = Settings.SkinChanger.Skins.begin(); it != Settings.SkinChanger.Skins.end(); it++) {
        JsonObject* pSkin = WriteSkin(&*it);
        if (!pSkin) continue;
        pSkins->AddJsonObject(pSkin);
    }


}


D3DCOLOR ParseColor(const std::string& szColor) {
    return strtoul(szColor.c_str(), 0, 16);
}

std::string WriteColor(const D3DCOLOR cColor)
{
    char szBuffer[16];
    _ltoa_s(cColor, szBuffer, 16, 16);
    return std::string(szBuffer);
}


SkinChanger::SkinInfo ParseSkin(JsonObject* pSkinObject) {
    if (!pSkinObject) return {};
    SkinChanger::SkinInfo pSkinInfo = SkinChanger::SkinInfo();

    pSkinInfo.nItemId               = pSkinObject->GetNumber<int>   ("weapon_id");
    pSkinInfo.nItemDefIndex         = pSkinObject->GetNumber<int>   ("new_index");
    pSkinInfo.iPaintKit             = pSkinObject->GetNumber<int>   ("skin_id");
    pSkinInfo.flWear                = pSkinObject->GetNumber<float> ("wear");
    pSkinInfo.iStatTrak             = pSkinObject->GetNumber<int>   ("stattrak_kills");
    pSkinInfo.iSeed                 = pSkinObject->GetNumber<int>   ("seed");
    pSkinInfo.iQuality              = pSkinObject->GetNumber<int>   ("quality");

    strcpy_s(pSkinInfo.szCustomName, 32, pSkinObject->GetString("custom_name").c_str());
    return pSkinInfo;
}



JsonObject* WriteSkin(const SkinChanger::SkinInfo* pSkin) {
    if (!pSkin) return nullptr;

    JsonObject* pObject = new JsonObject();

    pObject->AddNumber("weapon_id"      , pSkin->nItemId);
    pObject->AddNumber("new_index"      , pSkin->nItemDefIndex);
    pObject->AddNumber("skin_id"        , pSkin->iPaintKit);
    pObject->AddNumber("wear"           , pSkin->flWear);
    pObject->AddNumber("stattrak_kills" , pSkin->iStatTrak);
    pObject->AddNumber("seed"           , pSkin->iSeed);
    pObject->AddNumber("quality"        , pSkin->iQuality);

    pObject->AddString("custom_name"    , pSkin->szCustomName);
    return pObject;
}


