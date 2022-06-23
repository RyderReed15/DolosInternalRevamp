#pragma once

#ifndef HOOK_MANAGER_H
#define HOOK_MANAGER_H

#include <Windows.h>
#include <winnt.h>
#include <memoryapi.h>
#include <iostream>

#ifdef _WIN32
#define JMP_SIZE                    5
#else
#define JMP_SIZE                    9
#endif 

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

class DetourManager {
public:
            DetourManager   (void* pDetourAddress, void* pHookAddress, unsigned int iOpSize = JMP_SIZE);
            ~DetourManager  (void);
    
    void*   GetOriginal     (void);

private:

    unsigned int    m_iOpSize = 5;
    void*           m_pOriginalFunc;
    unsigned char   m_aOriginalBytes[16];
    void*           m_pTrampoline;


};



#endif // !HOOK_MANAGER_H
