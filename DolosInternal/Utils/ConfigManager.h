#pragma once

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "Json--/JsonParser.h"
#include "../Settings.h"

inline const char* version = "beta-1.1";

inline JsonObject*  g_pParsedConfig;   



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
