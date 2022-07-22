#pragma once

#ifndef SDK_H
#define SDK_H

#include "../Utils/Macros.h"
#include "../Utils/PatternScanner.h"
#include <d3d9.h>

#include "Math/QAngle.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Math/Vector4D.h"
#include "Math/VMatrix.h"
#include "Math/UtlMap.h"
#include "Math/UtlMemory.h"
#include "Math/UtlString.h"
#include "Math/UtlVector.h"
#include "Studio.h"

#include "Interfaces/CBaseCombatWeapon.h"
#include "Interfaces/CBaseViewModel.h"
#include "Interfaces/CGlobalVars.h"
#include "Interfaces/IAppSystem.h"
#include "Interfaces/IBaseClientDLL.h"
#include "Interfaces/IClientClass.h"
#include "Interfaces/IClientEntity.h"
#include "Interfaces/IClientEntityList.h"
#include "Interfaces/IClientMode.h"
#include "Interfaces/IClientNetworkable.h"
#include "Interfaces/IClientRenderable.h"
#include "Interfaces/IClientThinkable.h"
#include "Interfaces/IClientUnknown.h"
#include "Interfaces/ICollideable.h"
#include "Interfaces/IConVar.h"
#include "Interfaces/IEngineTrace.h"
#include "Interfaces/IGameEventListener.h"
#include "Interfaces/IGameMovement.h"
#include "Interfaces/IHandleEntity.h"
#include "Interfaces/IInputSystem.h"
#include "Interfaces/ILocalize.h"
#include "Interfaces/IMaterialSystem.h"
#include "Interfaces/IMDLCache.h"
#include "Interfaces/IMoveHelper.h"
#include "Interfaces/INetChannelInfo.h"
#include "Interfaces/IPanel.h"
#include "Interfaces/IPrediction.h"
#include "Interfaces/ISurface.h"
#include "Interfaces/IVEngineClient.h"
#include "Interfaces/IVModelInfoClient.h"
#include "Interfaces/IVModelRender.h"


inline IClientEntity*               g_pLocalPlayer;

inline CGlobalVarsBase*             g_pGlobalVars;
inline IBaseClientDLL*              g_pBaseClient;
inline IClientEntityList*           g_pClientEntityList;
inline IClientMode*                 g_pClientMode;
inline ICVar*                       g_pCVar;
inline IEngineTrace*                g_pEngineTrace;
inline IGameEventManager*           g_pEventManager;
inline IGameMovement*               g_pGameMovement;
inline IMaterialSystem*             g_pMaterialSystem;
inline IMDLCache*                   g_pMDLCache;
inline IMoveHelper*                 g_pMoveHelper;
inline IPrediction*                 g_pPrediction;
inline IVEngineClient*              g_pEngineClient;
inline IVModelInfoClient*           g_pModelInfo;
inline IVModelRender*               g_pModelRender;
inline ILocalize*                   g_pLocalize;
inline IInputSystem*                g_pInput;
inline ISurface*                    g_pSurface;

inline IDirect3DDevice9*            g_pD3DDevice;

inline IConVar*                     g_pMouseEnable;

inline Vector2D                     g_vScreenSize;


typedef void* (__cdecl* fnCreateInterface)(const char* szName, int* pReturnCode);


fnCreateInterface    GetFactory         (const char* szDLLName);
void*                GetInterface       (fnCreateInterface pFactory, const char* szInterface);
bool                 InitializeSDK      (void);

#endif