#pragma once

#ifndef STRING_MANAGER_H
#define STRING_MANAGER_H

#include <Windows.h>
#include <d3d9.h>
#include <unordered_map>
#include "Json--/JsonParserW.h"

#define LANG_PATH    "\\csgo\\resource\\csgo_"



inline std::unordered_map<unsigned int, std::wstring> mStrings;
inline std::hash<std::wstring> localeHash;

bool            InitializeLocalization  (const char* szLanguage = "english");
bool            UpdateLocalization      (const char* szLanguage = "english");
std::wstring    GetLocalizedString      (const wchar_t* szName);
std::string     GetLocalizedString      (const char* szName);
std::wstring    wstring_cast            (std::string szString);
std::string     string_cast             (std::wstring wszString);




#endif // !STRING_MANAGER_H

