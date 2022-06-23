#include "PatternScanner.h"
#include <iostream>



char* FindPattern(void* pDLLBase, const char* szPattern, bool bRelative, unsigned int aOffsets[], unsigned int iOffsetsSize) {
	char szParsedPattern[64]{ '\0' };
	char szMask[64]{ '\0' };

	MEMORY_BASIC_INFORMATION32 memInfo = { 0 };

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)pDLLBase, &modInfo, sizeof(MODULEINFO));
	DWORD dwModuleSize = modInfo.SizeOfImage;

	if (ParsePattern(szPattern, szParsedPattern, szMask)) {

		unsigned int iPatternSize = 0;
		while (szMask[iPatternSize] != '\0') {
			iPatternSize++;
		}
		//Iterate through each region, checking to make sure its readeable and not protected

		std::cout << "Scanning Module: " << pDLLBase << std::endl;
		for (char* pRegionBase = (char*)pDLLBase; pRegionBase < (char*)pDLLBase + dwModuleSize; pRegionBase = (char*)(memInfo.RegionSize + memInfo.BaseAddress)) {

			if (!VirtualQuery(reinterpret_cast<LPCVOID>(pRegionBase), reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&memInfo), sizeof(MEMORY_BASIC_INFORMATION))) //Iterate memory by using VirtualQuery
				continue;

			if (memInfo.State != MEM_COMMIT || memInfo.Protect == PAGE_NOACCESS || memInfo.Protect & PAGE_GUARD) //Filter Regions
				continue;
			std::cout << " - Scanning Region: " << (void*)memInfo.BaseAddress << " | " << (void*)(memInfo.BaseAddress + memInfo.RegionSize) << std::endl;
			unsigned int iLength = memInfo.BaseAddress + memInfo.RegionSize - iPatternSize + 1;
			for (char* j = (char*)memInfo.BaseAddress; j < (char*)iLength; j++) {

				//Check each byte within the region for the start and then following bytes of the pattern

				bool bFound = true;
				for (unsigned int x = 0; x < iPatternSize; x++) {

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

bool ParsePattern(const char* szInPattern, char szOutPattern[64], char szOutMask[64]) {
	//Replace string with actual byte array

	unsigned int i = 0;
	unsigned int iSize = 0;
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

char* ResolveOffsets(char* pAddress, unsigned int iOffsets[], unsigned int iOffsetsSize) {
	char* pOut = pAddress;
	if (iOffsetsSize) {
		for (unsigned int i = 0; i < iOffsetsSize; i++) {
			pOut += iOffsets[i];
			pOut = *(char**)pOut;
		}
	}
	return pOut;
	
}


bool AddressToPattern(void* pAddress, char* pPattern, unsigned int iPatternSize) {
	unsigned int iPointerSize = sizeof(void*);
	if (iPatternSize < iPointerSize * 3) return false;
	memset(pPattern, 0, iPatternSize);
	bool bLoop = false;
	while (iPointerSize > 0) {
		iPointerSize --;

		uint8_t byte = ((uintptr_t)pAddress >> iPointerSize * 8);
		_ltoa_s(byte, pPattern + (iPointerSize * 3), 3, 16);

		if (bLoop) pPattern[iPointerSize * 3 + 2] = ' ';

		bLoop = true;
	}
	return true;
}


