#pragma once

#ifndef STRING_MANAGER_H
#define STRING_MANAGER_H

#include <Windows.h>
#include <d3d9.h>
#include <unordered_map>
#include "Json--/JsonParserW.h"
#include "../Utils/Hash.h"

#define LANG_PATH    "\\csgo\\resource\\csgo_"



inline std::unordered_map<unsigned int, std::wstring> mStrings;

bool            InitializeLocalization  (const char* szLanguage = "english");
bool            UpdateLocalization      (const char* szLanguage = "english");
std::wstring    GetLocalizedStringW     (const wchar_t* wszName);
std::wstring    GetLocalizedStringW     (const char* szName);
std::string     GetLocalizedString      (const wchar_t* wszName);
std::string     GetLocalizedString      (const char* szName);
std::wstring    wstring_cast            (std::string szString);
std::string     string_cast             (std::wstring wszString);




#endif // !STRING_MANAGER_H

