#pragma once

#ifndef LOCALIZE_H
#define LOCALIZE_H

#include "../../Utils/Macros.h"
#include "IAppSystem.h"

class ILocalize : public IAppSystem {
public:
    VFUNC(bool          , AddLocalizationFile   , 9 , (const char* szFileName, const char* pPathID = NULL, bool bIncludeFallbackSearchPaths = false), (this, szFileName, pPathID, bIncludeFallbackSearchPaths));
    VFUNC(const wchar_t*, LocalizeStringW       , 11, (const char* szToken), (this, szToken));
    VFUNC(const wchar_t*, LocalizeStringSafeW   , 12, (const char* szToken), (this, szToken));
    VFUNC(int           , ConvertAnsiToUnicode  , 15, (const char* szAnsi, wchar_t* wszUnicode, int iBufferSizeBytes), (this, szAnsi, wszUnicode, iBufferSizeBytes));
    VFUNC(int           , ConvertUnicodeToAnsi  , 16, (const wchar_t* wszUnicode, char* szAnsi, int iBufferSizeBytes), (this, wszUnicode, szAnsi, iBufferSizeBytes));
    VFUNC(const char*   , LocalizeStringA       , 47, (const char* szToken), (this, szToken));
};

#endif // !LOCALIZE_H