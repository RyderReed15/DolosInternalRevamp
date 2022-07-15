#pragma once
#ifndef HOOKS_H
#define HOOKS_H

#include "Utils/HookManager.h"
#include "SDK/SDK.h"
#include "GUI/GUI.h"
#include "GUI/FontManager.h"
#include "Utils/HotKeyManager.h"
#include "Utils/EntityData.h"
#include <d3d9.h>


#include "Features/ESP.h"
#include "Features/SkinChanger.h"
#include "Features/Aimbot.h"
#include "Features/Misc.h"
#include "Features/EnginePrediction.h"
#include "Features/Triggerbot.h"
#include "Features/Radar.h"

#define CREATE_MOVE_INDEX           24

#define FRAME_STAGE_INDEX           37

#define DRAW_MODEL_EXECUTE_INDEX    21

#define LOCK_CURSOR_INDEX           67

#define BEGIN_SCENE_INDEX           41
#define DRAW_INDEXED_PRIM_INDEX     82
#define PRESENT_INDEX               17
#define END_SCENE_INDEX             42
#define RESET_INDEX                 16


inline VMTManager* g_vD3D;
inline VMTManager* g_vClient;
inline VMTManager* g_vModelRender;
inline VMTManager* g_vClientBase;
inline VMTManager* g_vSurface;

inline HWND hValveWnd;


typedef bool(__fastcall* fnCreateMove)              (void* _this, void* edx, float flInputSampleTime, CUserCmd* pCmd);
typedef void(__thiscall* fnDrawModelExecute)        (void* _this, void* edx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld);
typedef void(__fastcall* fnFrameStageNotify)        (void* _this, void* edx, ClientFrameStage_t stage);
typedef bool(__fastcall* fnVerifyReturn)            (void* _this, void* edx, const char* szModuleName);
typedef void(__stdcall*  fnLockCursor)              (void);


typedef HRESULT(__stdcall* fnEndScene)              (IDirect3DDevice9* pDevice);
typedef HRESULT(__stdcall* fnReset)                 (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(__stdcall* fnBeginScene)            (IDirect3DDevice9* pDevice);
typedef HRESULT(__stdcall* fnPresent)               (IDirect3DDevice9* pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
typedef HRESULT(__stdcall* fnDrawIndexedPrimitive)  (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE tPrimitiveType, int iBaseVertexIndex, int iMinVertexIndex, int iNumVertices, int iStartIndex, int iPrimCount);

inline fnCreateMove             oCreateMove;
inline fnDrawModelExecute       oDrawModelExecute;
inline fnFrameStageNotify       oFrameStageNotify;
inline fnLockCursor             oLockCursor;

inline fnBeginScene             oBeginScene;
inline fnDrawIndexedPrimitive   oDrawIndexedPrimitive;
inline fnPresent                oPresent;
inline fnEndScene               oEndScene;
inline fnReset                  oReset;

inline LONG_PTR                 oWndProc;

bool InitializeHooks    (void);
bool UninitializeHooks  (void);
void FixD3D             (void*** pDevice);

LRESULT             hkWndProc                   (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void __fastcall     hkFrameStageNotify          (void* _this, void* edx, ClientFrameStage_t stage);
void __fastcall     hkDrawModelExecute          (void* _this, void* edx, void* pCtx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld);
bool __fastcall     hkCreateMove                (void* _this, void* edx, float flInputSampleTime, CUserCmd* pCmd);
void __stdcall      hkLockCursor                (void);
char __fastcall     hkVerifyReturn              (void* _this, void* edx, const char* szModuleName);

HRESULT APIENTRY    hkBeginScene                (IDirect3DDevice9* pDevice);
HRESULT APIENTRY    hkDrawIndexedPrimitive      (IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE tPrimitiveType, int iBaseVertexIndex, int iMinVertexIndex, int iNumVertices, int iStartIndex, int iPrimCount);
HRESULT APIENTRY    hkPresent                   (IDirect3DDevice9* pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
HRESULT APIENTRY    hkEndscene                  (IDirect3DDevice9* pDevice);
HRESULT APIENTRY    hkReset                     (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);



#endif // !HOOKS_H
