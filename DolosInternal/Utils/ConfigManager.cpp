#include "ConfigManager.h"

bool InitializeConfig() {

    return LoadConfig("F:\\Coding Projects\\VS\\DolosInternal\\DolosInternal\\Resources\\Config.json");

}

bool UninitializeConfig() {
    
    bool bReturn =  SaveConfig("F:\\Coding Projects\\VS\\DolosInternal\\DolosInternal\\Resources\\Save.json");
    UnloadSkins();
    if (Settings.Aimbot.Targets) delete Settings.Aimbot.Targets;
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

    Settings.Aimbot.Active      = pAimbot->GetBoolean       ("active");
    Settings.Aimbot.Silent      = pAimbot->GetBoolean       ("silent");
    Settings.Aimbot.TargetAll   = pAimbot->GetBoolean       ("target_all");
    Settings.Aimbot.Overaim     = pAimbot->GetBoolean       ("overaim");
    Settings.Aimbot.Curve       = pAimbot->GetBoolean       ("curve");

    Settings.Aimbot.RangeFactor     = pAimbot->GetNumber<float> ("range_dec_multiplier");
    Settings.Aimbot.AimTime         = pAimbot->GetNumber<float> ("time_to_aim");
    Settings.Aimbot.WaitTime        = pAimbot->GetNumber<float> ("wait_time");
    Settings.Aimbot.AntilockFactor  = pAimbot->GetNumber<float> ("antilock_factor");

    JsonArray* pTargets = pAimbot->GetJsonArray("targets");

    Settings.Aimbot.TargetCount = pTargets->GetSize();
    if (Settings.Aimbot.Targets) delete Settings.Aimbot.Targets;
    Settings.Aimbot.Targets = new AimTarget[Settings.Aimbot.TargetCount];

    for (int i = 0; i < Settings.Aimbot.TargetCount; i++) {
        Settings.Aimbot.Targets[i].FOV      = pTargets->GetJsonObject(i)->GetNumber<float>  ("fov");
        Settings.Aimbot.Targets[i].Bone     = pTargets->GetJsonObject(i)->GetNumber<int>    ("bone");
    }

    JsonObject* pPlayers = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("players");

    Settings.Visuals.Players.ShowTeam   = pPlayers->GetBoolean("show_team");
    Settings.Visuals.Players.ShowEnemy  = pPlayers->GetBoolean("show_enemy");
    Settings.Visuals.Players.DrawBones  = pPlayers->GetBoolean("draw_bones");
    Settings.Visuals.Players.DrawHealth = pPlayers->GetBoolean("draw_health");
    Settings.Visuals.Players.DrawArmor  = pPlayers->GetBoolean("draw_armor");
    Settings.Visuals.Players.DrawName   = pPlayers->GetBoolean("draw_name");

    Settings.Visuals.Players.EnemyColor = ParseColor(pPlayers->GetString("enemy_color"));
    Settings.Visuals.Players.TeamColor  = ParseColor(pPlayers->GetString("team_color"));

    JsonObject* pWeapons = g_pParsedConfig->GetJsonObject("visuals")->GetJsonObject("weapons");

    Settings.Visuals.Weapons.Enabled    = pWeapons->GetBoolean("enabled");
    Settings.Visuals.Weapons.DrawName   = pWeapons->GetBoolean("draw_name");

    JsonObject* pRecoil = g_pParsedConfig->GetJsonObject("recoil");

    Settings.Recoil.Enabled         = pRecoil->GetBoolean       ("enabled");
    Settings.Recoil.ControlFactor   = pRecoil->GetNumber<float> ("factor");

    JsonObject* pMisc = g_pParsedConfig->GetJsonObject("misc");

    Settings.Misc.Bhop = pMisc->GetBoolean("bhop");

    Settings.Visuals.Weapons.Color  = ParseColor(pWeapons->GetString("color"));

    JsonObject* pSkinChanger = g_pParsedConfig->GetJsonObject("skin_changer");

    Settings.SkinChanger.TrackKills = pSkinChanger->GetBoolean("track_kills");

    JsonArray* pSkins = pSkinChanger->GetJsonArray("skins");

    for (int i = 0; i < pSkins->GetSize(); i++) {
        SkinChanger::SkinStruct* pSkin = ParseSkin(pSkins->GetJsonObject(i));
        Settings.SkinChanger.Skins[pSkins->GetJsonObject(i)->GetNumber<int>("weapon_id")] = pSkin;
        if (pSkins->GetJsonObject(i)->GetNumber<int>("new_index")) Settings.SkinChanger.Skins[pSkins->GetJsonObject(i)->GetNumber<int>("new_index")] = pSkin;
    }
}

void UpdateValues() {
    JsonObject* pAimbot = g_pParsedConfig->GetJsonObject("aimbot");

    pAimbot->SetBoolean ("active"       , Settings.Aimbot.Active);
    pAimbot->SetBoolean ("silent"       , Settings.Aimbot.Silent);
    pAimbot->SetBoolean ("target_all"   , Settings.Aimbot.TargetAll);
    pAimbot->SetBoolean ("overaim"      , Settings.Aimbot.Overaim);
    pAimbot->SetBoolean ("curve"        , Settings.Aimbot.Curve);

    pAimbot->SetNumber("range_dec_multiplier"   , Settings.Aimbot.RangeFactor);
    pAimbot->SetNumber("time_to_aim"            , Settings.Aimbot.AimTime);
    pAimbot->SetNumber("wait_time"              , Settings.Aimbot.WaitTime);
    pAimbot->SetNumber("antilock_factor"        , Settings.Aimbot.AntilockFactor);

    JsonArray* pTargets = pAimbot->GetJsonArray("targets");

    while (pTargets->GetSize()) {
        pTargets->RemoveValue(0);
    }

    for (int i = 0; i < Settings.Aimbot.TargetCount; i++) {
        JsonObject* pTarget = new JsonObject();
        pTarget->AddNumber("fov", Settings.Aimbot.Targets[i].FOV);
        pTarget->AddNumber("bone", Settings.Aimbot.Targets[i].Bone);
        pTargets->AddJsonObject(pTarget);
    }

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

    JsonObject* pRecoil = g_pParsedConfig->GetJsonObject("recoil");

    pRecoil->SetBoolean("enabled"   , Settings.Recoil.Enabled);
    pRecoil->SetNumber("factor"     , Settings.Recoil.ControlFactor);

    JsonObject* pMisc = g_pParsedConfig->GetJsonObject("misc");

    pMisc->SetBoolean("bhop", Settings.Misc.Bhop);

    JsonObject* pSkinChanger = g_pParsedConfig->GetJsonObject("skins");

    pSkinChanger->SetBoolean("track_kills", Settings.SkinChanger.TrackKills);

    JsonArray* pSkins = pSkinChanger->GetJsonArray("skin_changer");

    while (pSkins->GetSize())
    {
        pSkins->RemoveValue(0);
    }

    std::map<int, SkinChanger::SkinStruct*>::iterator it = Settings.SkinChanger.Skins.begin();
    while (it != Settings.SkinChanger.Skins.end()) {
        JsonObject* pSkin = WriteSkin(it->second);
        if (pSkin->GetNumber<int>("new_index")) {
            Settings.SkinChanger.Skins.erase(pSkin->GetNumber<int>("new_index"));
        }
        pSkin->AddNumber("weapon_id", it->first);
        pSkins->AddJsonObject(pSkin);
        it++;
    }


}

void UnloadSkins() {
    std::map<int, SkinChanger::SkinStruct*>::iterator it = Settings.SkinChanger.Skins.begin();
    while (it != Settings.SkinChanger.Skins.end()) {
        delete it->second;
        it++;
    }
}

D3DCOLOR ParseColor(const std::string& szColor) {
    return strtoul(szColor.c_str(), 0, 16);
}


SkinChanger::SkinStruct* ParseSkin(JsonObject* pSkinObject) {
    if (!pSkinObject) return nullptr;
    SkinChanger::SkinStruct* pSkinInfo = new SkinChanger::SkinStruct;

    pSkinInfo->nItemDefIndex        = pSkinObject->GetNumber<int>   ("new_index");
    pSkinInfo->iPaintKit            = pSkinObject->GetNumber<int>   ("skin_id");
    pSkinInfo->flWear               = pSkinObject->GetNumber<float> ("wear");
    pSkinInfo->iStatTrak            = pSkinObject->GetNumber<int>   ("stattrak_kills");
    pSkinInfo->iSeed                = pSkinObject->GetNumber<int>   ("seed");
    pSkinInfo->iQuality             = pSkinObject->GetNumber<int>   ("quality");

    sprintf_s(pSkinInfo->szCustomName, 32, "%s", pSkinObject->GetString("custom_name").c_str());
    return pSkinInfo;
}



JsonObject* WriteSkin(SkinChanger::SkinStruct* pSkin) {
    if (!pSkin) return nullptr;

    JsonObject* pObject = new JsonObject();

    pObject->AddNumber("new_index"      , pSkin->nItemDefIndex);
    pObject->AddNumber("skin_id"        , pSkin->iPaintKit);
    pObject->AddNumber("wear"           , pSkin->flWear);
    pObject->AddNumber("stattrak_kills" , pSkin->iStatTrak);
    pObject->AddNumber("seed"           , pSkin->iSeed);
    pObject->AddNumber("quality"        , pSkin->iQuality);

    pObject->AddString("custom_name"    , pSkin->szCustomName);
    return pObject;
}


