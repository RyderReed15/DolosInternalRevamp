#pragma once
#ifndef PATTERNSCANNER_H
#define PATTERNSCANNER_H

#include <Windows.h>
#include <iostream>
#include <Psapi.h>

char*   ResolveOffsets  (char* pAddress, unsigned int iOffsets[], unsigned int iOffsetsSize);
bool    ParsePattern    (const char* szInPattern, char szOutPattern[64], char szOutMask[64]);
char*   FindPattern     (void* pDLLBase, const char* szPattern, bool bRelative = false, unsigned int aOffsets[] = { 0 }, unsigned int iOffsetsSize = 0);
bool    AddressToPattern(void* pAddress, char* pPattern, unsigned int iPatternSize);




#endif // !PATTERNSCANNER_H
