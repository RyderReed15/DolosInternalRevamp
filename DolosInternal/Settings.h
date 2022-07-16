#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H


#include "Features/SkinChanger.h"
#include <d3d9.h>
#include <vector>
#include <string>

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
        struct Radar {
            bool        bEnabled;
            Vector2D    vTopLeft;
            Vector2D    vWorldCoords;
            float       flSize;
            float       flZoom;
            float       flScale;
            std::vector<IDirect3DTexture9*> vTextures;
            std::vector<float>              vMaxs;
        } Overview;

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


#endif // !SETTINGS_H
