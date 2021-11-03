#include "PatternScanner.h"
#include <iostream>



char* FindPattern(void* pDLLBase, const char* szPattern, bool bRelative, int aOffsets[], int iOffsetsSize) {
	char szParsedPattern[64]{ '\0' };
	char szMask[64]{ '\0' };

	MEMORY_BASIC_INFORMATION32 memInfo = { 0 };

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)pDLLBase, &modInfo, sizeof(MODULEINFO));
	int iModuleSize = modInfo.SizeOfImage;

	if (ParsePattern(szPattern, szParsedPattern, szMask)) {

		int iPatternSize = 0;
		while (szMask[iPatternSize] != '\0') {
			iPatternSize++;
		}
		std::cout << "Scanning Module: " << pDLLBase << std::endl;
		for (char* pRegionBase = (char*)pDLLBase; pRegionBase < (char*)pDLLBase + iModuleSize; pRegionBase = (char*)(memInfo.RegionSize + memInfo.BaseAddress)) {

			if (!VirtualQuery(reinterpret_cast<LPCVOID>(pRegionBase), reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&memInfo), sizeof(MEMORY_BASIC_INFORMATION))) //Iterate memory by using VirtualQuery
				continue;

			if (memInfo.State != MEM_COMMIT || memInfo.Protect == PAGE_NOACCESS || memInfo.Protect & PAGE_GUARD) //Filter Regions
				continue;
			std::cout << " - Scanning Region: " << (void*)memInfo.BaseAddress << " | " << (void*)(memInfo.BaseAddress + memInfo.RegionSize) << std::endl;
			int iLength = memInfo.BaseAddress + memInfo.RegionSize - iPatternSize + 1;
			for (char* j = (char*)memInfo.BaseAddress; j < (char*)iLength; j++) {

				bool bFound = true;
				for (int x = 0; x < iPatternSize; x++) {

					if (szMask[x] != '?' && szParsedPattern[x] != j[x]) {
						bFound = false;
						break;
					}
				}
				if (bFound) {
					return (char*)ResolveOffsets(j, aOffsets, iOffsetsSize) - (int)pDLLBase * bRelative;
				}
			}
		}
	}
	return 0;
}

bool ParsePattern(const char* szInPattern,  char szOutPattern[64], char szOutMask[64]) {
	int i = 0;
	int iSize = 0;
	while (szInPattern[i + 1] != '\0') {
		if (szInPattern[i] == '?') {
			szOutPattern[iSize] = '\xFF';
			szOutMask[iSize] = '?';
			iSize++;

		}
		else if (szInPattern[i] != ' ' && szInPattern[i + 1] != '?' && szInPattern[i + 1] != ' '){
			char szByte[2] = { szInPattern[i], szInPattern[i + 1] };
			szOutPattern[iSize] = (char)strtol(szByte, 0, 16);
			szOutMask[iSize] = 'X';
			iSize++;
		}
		i++;
	}
	return iSize;
}

char* ResolveOffsets(char* pAddress, int iOffsets[], int iOffsetsSize) {
	char* pOut = pAddress;
	if (iOffsetsSize) {
		for (int i = 0; i < iOffsetsSize; i++) {
			pOut += iOffsets[i];
			pOut = *(char**)pOut;
		}
	}
	return pOut;
	
}


