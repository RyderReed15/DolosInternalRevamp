#pragma once
#ifndef PATTERNSCANNER_H
#define PATTERNSCANNER_H

#include <Windows.h>
#include <stdlib.h>
#include <winnt.h>
#include <Psapi.h>

char*	ResolveOffsets	(char* pAddress, int iOffsets[], int iOffsetsSize);
bool	ParsePattern	(const char* szInPattern, char szOutPattern[64], char szOutMask[64]);
char*	FindPattern		(void* pDLLBase, const char* szPattern, bool bRelative = false, int aOffsets[] = { 0 }, int iOffsetsSize = 0);





#endif // !PATTERNSCANNER_H
