#pragma once

#ifndef HOOK_MANAGER_H
#define HOOK_MANAGER_H

#include <Windows.h>
#include <winnt.h>
#include <memoryapi.h>
#include <iostream>

class VMTManager {
public:
            VMTManager              (void);
            VMTManager              (void*** pVMT);
            ~VMTManager             (void);
    void*   HookFunction            (unsigned int iFuncIndex, void* pHookAddress);
    bool    FreeFunction            (unsigned int iFuncIndex);
    void*   GetOriginalFunction     (unsigned int iFuncIndex);
    
private:
    bool            IsPopulated             (void);
    unsigned int    GetTableSize            (void** pVMT);

    void**          m_pTable;
    void**          m_pOldTable;
    unsigned int    m_iSize;
};



#endif // !HOOK_MANAGER_H
