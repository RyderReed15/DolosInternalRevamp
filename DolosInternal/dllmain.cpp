// dllmain.cpp : Defines the entry point for the DLL application.
#include "Initializer.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID 
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)StartCheat, hModule, 0, nullptr)) {
            return CloseHandle(hThread);
        }
        
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        UninitializeCheat();
        break;
    }
    return TRUE;
}

