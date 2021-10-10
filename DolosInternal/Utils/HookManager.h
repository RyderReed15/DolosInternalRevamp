#pragma once

#ifndef HOOK_MANAGER_H
#define HOOK_MANAGER_H

#include <Windows.h>
#include <winnt.h>
#include <memoryapi.h>
#include <iostream>

class VMTManager {
public:
			VMTManager				(void);
			VMTManager				(void*** pVMT);
			~VMTManager				(void);
	void*	HookFunction			(int iFuncIndex, void* pHookAddress);
	bool	FreeFunction			(int iFuncIndex);
	void*	GetOriginalFunction		(int iFuncIndex);
	
private:
	bool	IsPopulated				(void);
	int		GetTableSize			(void** pVMT);

	void**	m_pTable;
	void**	m_pOldTable;
	int		m_iSize;
};



#endif // !HOOK_MANAGER_H
