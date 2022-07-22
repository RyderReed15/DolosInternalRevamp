#include "HookManager.h"



VMTManager::VMTManager() {
	m_iSize = 0;
	m_pOldTable = nullptr;
	m_pTable = nullptr;
}

VMTManager::VMTManager(void*** pVMT) {
	//Replace function table with our own copy, storing the old version

	m_iSize = GetTableSize(*pVMT);
	m_pOldTable = new void* [m_iSize];
	DWORD old;
	VirtualProtect(pVMT, m_iSize * sizeof(void*), PAGE_EXECUTE_READWRITE, &old);

	memcpy(m_pOldTable, *pVMT, m_iSize * sizeof(void*));
	
	m_pTable = *pVMT;

	VirtualProtect(pVMT, m_iSize * sizeof(void*), old, &old);

}

VMTManager::~VMTManager() {
	for (unsigned int i = 0; i < m_iSize; i++) {
		if (!FreeFunction(i)) {
			std::cout << "Failed to unhook function " << i << std::endl;
		}
	}
	
	delete[] m_pOldTable;	
}

void* VMTManager::HookFunction(unsigned int iFuncIndex, void* pHookAddress){
	//Set protection on function pointer then replace and reset protection

	DWORD old;
	VirtualProtect(m_pTable + iFuncIndex, sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
	m_pTable[iFuncIndex] = pHookAddress;
	if (m_pTable[iFuncIndex] != pHookAddress) {
		return nullptr;
	}

	VirtualProtect(m_pTable + iFuncIndex, sizeof(void*), old, &old);
	return m_pOldTable[iFuncIndex];
}

bool VMTManager::FreeFunction(unsigned int iFuncIndex) {
	DWORD old;
	VirtualProtect(m_pTable + iFuncIndex, sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
	m_pTable[iFuncIndex] = m_pOldTable[iFuncIndex];
	bool out = m_pTable[iFuncIndex] == m_pOldTable[iFuncIndex];
	VirtualProtect(m_pTable + iFuncIndex, sizeof(void*), old, &old);
	return out;
}

void* VMTManager::GetOriginalFunction(unsigned int iFuncIndex) {
	return m_pOldTable[iFuncIndex];
}

bool VMTManager::IsPopulated() {
	return m_iSize;
}

unsigned int VMTManager::GetTableSize(void** pVMT) {
	//Traverse table until it ends (page protection will be different
	unsigned int iSize = 0;
	MEMORY_BASIC_INFORMATION memInfo = { 0 };
	while (VirtualQuery(pVMT[iSize], &memInfo, sizeof(memInfo)) && memInfo.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) iSize++;
		
	
	return iSize;
}

DetourManager::DetourManager(void* pDetourAddress, void* pHookAddress, unsigned int iOpSize){

	unsigned char aBytes[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	*(uintptr_t*)(aBytes + 1) = ((uintptr_t)pHookAddress - (uintptr_t)pDetourAddress) - JMP_SIZE; //  == JMP pHookAddress

	m_pTrampoline = VirtualAlloc(0, iOpSize + JMP_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (m_pTrampoline) {
		memcpy(m_pTrampoline, pDetourAddress, iOpSize);
		*(unsigned char*)((uintptr_t)m_pTrampoline + iOpSize) = 0xE9;
		*(uintptr_t*)((uintptr_t)m_pTrampoline + iOpSize + 1) = ((uintptr_t)pDetourAddress - (uintptr_t)m_pTrampoline) - iOpSize; // Quite possibly wrong but cant see why
	}
	else {
		std::cout << "Unable to trampoline - perform only detour" << std::endl;
		memcpy(m_aOriginalBytes, pDetourAddress, iOpSize);
		m_pTrampoline = m_aOriginalBytes;
	}

	DWORD dwOld;
	VirtualProtect(pDetourAddress, iOpSize, PAGE_EXECUTE_READWRITE, &dwOld);

	memcpy(pDetourAddress, aBytes, iOpSize);

	VirtualProtect(pDetourAddress, iOpSize, dwOld, &dwOld);

	m_pOriginalFunc = pDetourAddress;
	m_iOpSize = iOpSize;

	
	

}

DetourManager::~DetourManager(void){

	DWORD dwOld;
	VirtualProtect(m_pOriginalFunc, m_iOpSize, PAGE_EXECUTE_READWRITE, &dwOld);

	memcpy(m_pOriginalFunc, m_pTrampoline, m_iOpSize);

	VirtualProtect(m_pOriginalFunc, m_iOpSize, dwOld, &dwOld);

	VirtualFree(m_pTrampoline, 0, MEM_RELEASE);
}

void* DetourManager::GetOriginal(void)
{
	return m_pTrampoline;
}
