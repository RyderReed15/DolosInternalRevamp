#include "HookManager.h"


VMTManager::VMTManager() {
	m_iSize = 0;
	m_pOldTable = nullptr;
	m_pTable = nullptr;
}

VMTManager::VMTManager(void*** pVMT) {
	m_iSize = GetTableSize(*pVMT);
	m_pOldTable = new void* [m_iSize];


	DWORD old;
	VirtualProtect(pVMT, m_iSize * 4, PAGE_EXECUTE_READWRITE, &old);

	memcpy(m_pOldTable, *pVMT, m_iSize * 4);
	m_pTable = *pVMT;

	VirtualProtect(pVMT, m_iSize * 4, old, &old);

}

VMTManager::~VMTManager() {
	for (int i = 0; i < m_iSize; i++) {
		if (!FreeFunction(i)) {
			std::cout << "Failed to unhook function " << i << std::endl;
		}
	}
	delete[] m_pOldTable;
}


void* VMTManager::HookFunction(int iFuncIndex, void* pHookAddress)
{

	DWORD old;
	VirtualProtect(m_pTable + iFuncIndex, 4, PAGE_EXECUTE_READWRITE, &old);
	m_pTable[iFuncIndex] = pHookAddress;
	if (m_pTable[iFuncIndex] != pHookAddress) {
		return 0;
	}

	VirtualProtect(m_pTable + iFuncIndex, 4, old, &old);
	return m_pOldTable[iFuncIndex];
}

bool VMTManager::FreeFunction(int iFuncIndex) {
	DWORD old;
	VirtualProtect(m_pTable + iFuncIndex, 4, PAGE_EXECUTE_READWRITE, &old);
	m_pTable[iFuncIndex] = m_pOldTable[iFuncIndex];
	bool out = m_pTable[iFuncIndex] == m_pOldTable[iFuncIndex];
	VirtualProtect(m_pTable + iFuncIndex, 4, old, &old);
	return out;
}

void* VMTManager::GetOriginalFunction(int iFuncIndex) {
	return m_pOldTable[iFuncIndex];
}

bool VMTManager::IsPopulated() {
	return m_iSize;
}

int VMTManager::GetTableSize(void** pVMT) {
	int iSize = 0;
	for (; pVMT[iSize]; iSize++) {

	}
	return iSize;
}