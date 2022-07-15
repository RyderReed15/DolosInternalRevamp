#include "SDK.h"

bool InitializeSDK() {

    fnCreateInterface pClientFactory        = GetFactory("client.dll");
    fnCreateInterface pEngineFactory        = GetFactory("engine.dll");
    fnCreateInterface pShaderFactory        = GetFactory("shaderapidx9.dll");
    fnCreateInterface pDataCache            = GetFactory("datacache.dll");
    fnCreateInterface pMaterialFactory      = GetFactory("materialsystem.dll");
    fnCreateInterface pValveFactory         = GetFactory("vstdlib.dll");
    fnCreateInterface pLocalizeFactory      = GetFactory("localize.dll");
    fnCreateInterface pInputFactory         = GetFactory("inputsystem.dll");
    fnCreateInterface pVGUIFactory          = GetFactory("vguimatsurface.dll");


    g_pBaseClient               = (IBaseClientDLL*)     GetInterface(pClientFactory     , "VClient018");
    g_pClientEntityList         = (IClientEntityList*)  GetInterface(pClientFactory     , "VClientEntityList003");
    g_pCVar                     = (ICVar*)              GetInterface(pValveFactory      , "VEngineCvar007");
    g_pEngineTrace              = (IEngineTrace*)       GetInterface(pEngineFactory     , "EngineTraceClient004");
    g_pEventManager             = (IGameEventManager*)  GetInterface(pEngineFactory     , "GAMEEVENTSMANAGER002");
    g_pGameMovement             = (IGameMovement*)      GetInterface(pClientFactory     , "GameMovement001");
    g_pMaterialSystem           = (IMaterialSystem*)    GetInterface(pMaterialFactory   , "VMaterialSystem080");
    g_pMDLCache                 = (IMDLCache*)          GetInterface(pDataCache         , "MDLCache004");
    g_pPrediction               = (IPrediction*)        GetInterface(pClientFactory     , "VClientPrediction001");
    g_pEngineClient             = (IVEngineClient*)     GetInterface(pEngineFactory     , "VEngineClient014");
    g_pModelInfo                = (IVModelInfoClient*)  GetInterface(pEngineFactory     , "VModelInfoClient004");
    g_pModelRender              = (IVModelRender*)      GetInterface(pEngineFactory     , "VEngineModel016");
    g_pLocalize                 = (ILocalize*)          GetInterface(pLocalizeFactory   , "Localize_001");
    g_pInput                    = (IInputSystem*)       GetInterface(pInputFactory      , "InputSystemVersion001");
    g_pSurface                  = (ISurface*)           GetInterface(pVGUIFactory       , "VGUI_Surface031");

   
    
    void* pClientBase = GetModuleHandle("client.dll");
    void* pEngineBase = GetModuleHandle("engine.dll");
    void* pShaderBase = GetModuleHandle("shaderapidx9.dll");

    if (g_pBaseClient) {

        g_pClientMode       = **(IClientMode***)        ((*(uintptr_t**)g_pBaseClient)[10] + 5);
        g_pGlobalVars       = **(CGlobalVarsBase***)    ((*(uintptr_t**)g_pBaseClient)[11] + 10);
        g_pMoveHelper       = **(IMoveHelper***)        ((*(uintptr_t**)g_pGameMovement)[31] + 0x2df);
       
        g_pD3DDevice                        = **(IDirect3DDevice9***)   (FindPattern(pShaderBase, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
        //void * g_pGlowObjectManager         = *(void**)                 (FindPattern(pClientBase, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
        //void * g_pClientState               = **(void***)               (FindPattern(pEngineBase, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);

        g_pMouseEnable      = g_pCVar->FindVar("cl_mouseenable");

        return true;
    }
    return false;
    
}

fnCreateInterface GetFactory(const char* szDLLName)
{
    HMODULE hDLLBase = GetModuleHandle(szDLLName);
    if (hDLLBase) {
        return (fnCreateInterface)GetProcAddress(hDLLBase, "CreateInterface");
    }
    return (fnCreateInterface)nullptr;
}

void* GetInterface(fnCreateInterface pFactory, const char* szInterface) {

    int iReturnCode;
    return pFactory(szInterface, &iReturnCode);
}