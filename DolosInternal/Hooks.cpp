#include "Hooks.h"

std::vector<DetourManager*> g_vDetours;

VMTManager* vD3D;
VMTManager* vClient;
VMTManager* vModelRender;
VMTManager* vClientBase;
VMTManager* vSurface;


bool InitializeHooks() {


	//Verify hook below inspired by https://www.unknowncheats.me/forum/3360552-post12.html

	const char* aDLLNames[] = { "client.dll", "engine.dll", "studiorender.dll", "materialsystem.dll" };
	for (const char* szDLL : aDLLNames) {

		void* pPattern  = FindPattern(GetModuleHandle(szDLL), "55 8B EC 56 8B F1 33 C0 57 8B 7D 08");
		g_vDetours.push_back(new DetourManager(pPattern, hkVerifyReturn));
	}
	
	vClient					= new VMTManager((void***)g_pClientMode);
	vClientBase				= new VMTManager((void***)g_pBaseClient);
	vD3D					= new VMTManager((void***)g_pD3DDevice);
	vModelRender			= new VMTManager((void***)g_pModelRender);
	vSurface				= new VMTManager((void***)g_pSurface);
	

	oCreateMove				= (fnCreateMove)			vClient->HookFunction		(CREATE_MOVE_INDEX			, hkCreateMove);

	oFrameStageNotify		= (fnFrameStageNotify)		vClientBase->HookFunction	(FRAME_STAGE_INDEX			, hkFrameStageNotify);

	oDrawModelExecute		= (fnDrawModelExecute)		vModelRender->HookFunction	(DRAW_MODEL_EXECUTE_INDEX	, hkDrawModelExecute);

	oLockCursor				= (fnLockCursor)			vSurface->HookFunction		(LOCK_CURSOR_INDEX			, hkLockCursor);

	oBeginScene				= (fnBeginScene)			vD3D->HookFunction			(BEGIN_SCENE_INDEX			, hkBeginScene);
	oDrawIndexedPrimitive	= (fnDrawIndexedPrimitive)	vD3D->HookFunction			(DRAW_INDEXED_PRIM_INDEX	, hkDrawIndexedPrimitive);
	oPresent				= (fnPresent)				vD3D->HookFunction			(PRESENT_INDEX				, hkPresent);
	oEndScene				= (fnEndScene)				vD3D->HookFunction			(END_SCENE_INDEX			, hkEndscene);
	oReset					= (fnReset)					vD3D->HookFunction			(RESET_INDEX				, hkReset);

	g_hValveWnd				= FindWindow("Valve001", NULL);

	return true;
}

bool UninitializeHooks() {
	for (size_t i = 0; i < g_vDetours.size(); i++) {
		delete g_vDetours[i];
	}

	vClient->FreeFunction		(CREATE_MOVE_INDEX);

	vClientBase->FreeFunction	(FRAME_STAGE_INDEX);
	
	vModelRender->FreeFunction	(DRAW_MODEL_EXECUTE_INDEX);

	vSurface->FreeFunction		(LOCK_CURSOR_INDEX);

	vD3D->FreeFunction			(BEGIN_SCENE_INDEX);
	vD3D->FreeFunction			(DRAW_INDEXED_PRIM_INDEX);
	vD3D->FreeFunction			(PRESENT_INDEX);
	vD3D->FreeFunction			(END_SCENE_INDEX);
	vD3D->FreeFunction			(RESET_INDEX);

	delete vClient;
	delete vClientBase;
	delete vD3D;
	delete vModelRender;
	delete vSurface;
	SetWindowLongPtrW(g_hValveWnd, GWLP_WNDPROC, oWndProc);
	return true;
}



LRESULT hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//Hook WndProc to get keyboard and mouse inputs which are then passes to the gui event handler

	if (uMsg == WM_HOTKEY) {
		CallHotKey(wParam);
	}
	if (g_bMenuOpen && g_pGUIContainer) {
		POINTS ptLoc = *(POINTS*)&lParam;
		POINT ptLocation = { ptLoc.x, ptLoc.y };

		GUIEventHandler* pEventHandler = g_pGUIContainer->GetEventHandler();
		switch (uMsg) {
		case WM_CHAR:
		case WM_SYSCHAR:
			pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::KEYDOWN, pEventHandler->BuildFunction(&GUIEventHandler::HandleType, pEventHandler, wParam));
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::KEYDOWN, pEventHandler->BuildFunction(&GUIEventHandler::HandleKeyDown, pEventHandler, wParam, lParam));
			break;
		case WM_LBUTTONDOWN:
			pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::CLICK, pEventHandler->BuildFunction(&GUIEventHandler::HandleClick, pEventHandler, ptLocation));
			break;

		case WM_LBUTTONUP:
			pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::RELEASE, pEventHandler->BuildFunction(&GUIEventHandler::HandleRelease, pEventHandler, ptLocation));
			break;
		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON) {
				pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::DRAG, pEventHandler->BuildFunction(&GUIEventHandler::HandleDrag, pEventHandler, ptLocation));
			}
			else {
				pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::HOVER, pEventHandler->BuildFunction(&GUIEventHandler::HandleHover, pEventHandler, ptLocation));
			}
			break;
		case WM_MOUSEWHEEL:
			pEventHandler->CreateGUIEvent(GUI_EVENT_TYPE::SCROLL, pEventHandler->BuildFunction(&GUIEventHandler::HandleScroll, pEventHandler, ptLocation, GET_WHEEL_DELTA_WPARAM(wParam)));
			break;
		}
	}
	
	return CallWindowProc((WNDPROC)oWndProc, hWnd, uMsg, wParam, lParam);
}


void __fastcall hkFrameStageNotify(void* _this, void* edx, ClientFrameStage_t stage) {
	

	if (stage == ClientFrameStage_t::FRAME_START) {
		g_pLocalPlayer = g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
		EntityData::UpdateEntityData();
	}
	else if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		SkinChanger::PreTick();
		oFrameStageNotify(_this, edx, stage);
		SkinChanger::PostTick();
		return;
	}
	
	oFrameStageNotify(_this, edx, stage);

}

void __fastcall hkDrawModelExecute(void* _this, void*, void* pCtx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld) {

	return oDrawModelExecute(_this, pCtx, state, pInfo, pCustomBoneToWorld);
}

bool __fastcall hkCreateMove(void* _this, void* edx, float flInputSampleTime, CUserCmd* pCmd) {
	
	bool bReturn = oCreateMove(_this, edx, flInputSampleTime, pCmd);
	bool bAimbot = true;
	if (g_pLocalPlayer && pCmd->iTickCount != 0 && g_pEngineClient->IsInGame()) {

		int iFlags = EnginePrediction::Begin(g_pLocalPlayer, pCmd);

		bAimbot = Aimbot::Tick(pCmd);

		Misc::Bhop(pCmd, iFlags);

		Triggerbot::Tick(pCmd);

		EnginePrediction::End(g_pLocalPlayer);

		g_pLocalPlayer->SetFlags(iFlags); //Return flags to before prediction to preserve state

	}
	return bReturn && bAimbot;
}

void __stdcall hkLockCursor() {
	if (g_bMenuOpen) return g_pSurface->UnlockCursor();
	else return oLockCursor();
}

char __fastcall hkVerifyReturn(void* _this, void* edx, const char* szModuleName) {

	//((fnVerifyReturn)g_vDetours[0]->GetOriginal())(_this, edx, szModuleName); //Error for now
	//hopefully can just reconstruct in the future or spoof return addresses
	return 1;
}


HRESULT APIENTRY hkBeginScene(IDirect3DDevice9* pDevice) {

	HRESULT hReturn = oBeginScene(pDevice);
	return hReturn;
}

HRESULT APIENTRY hkDrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE tPrimitiveType, int iBaseVertexIndex, int iMinVertexIndex, int iNumVertices, int iStartIndex, int iPrimCount) {

	HRESULT hReturn = oDrawIndexedPrimitive(pDevice, tPrimitiveType, iBaseVertexIndex, iMinVertexIndex, iNumVertices, iStartIndex, iPrimCount);
	return hReturn;
}


HRESULT APIENTRY hkPresent(IDirect3DDevice9* pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
	//Main render thread

	IDirect3DStateBlock9* pState;
	pDevice->CreateStateBlock(D3DSBT_PIXELSTATE, &pState);
	
	if (g_pGUIContainer && g_pRender) {

		if (g_pEngineClient->IsInGame()) {
			ESP::Tick();
			if(!Settings.Visuals.Overview.vTextures.size()) RadarESP::LoadRadar(g_pRender, g_pEngineClient->GetLevelNameShort());
			else RadarESP::DrawRadar(g_pRender);
		}

		if (g_bMenuOpen) {
			g_pGUIContainer->GetEventHandler()->ProccessEvents();
			g_pGUIContainer->DrawElements(g_pRender, g_pAvenirFont);
		}

		FixD3D((void***)pDevice);
	}
	pState->Apply();
	pState->Release();
	HRESULT hReturn = oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return hReturn;
}

HRESULT APIENTRY hkEndscene(IDirect3DDevice9* pDevice) {
	
	HRESULT hReturn = oEndScene(pDevice);
	
	return hReturn;
}

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {

	HRESULT hReturn = oReset(pDevice, pPresentationParameters);
	return hReturn;
}

void FixD3D(void*** pDevice) {
	//D3D Virtual funcs are reset after every call for some reason
	(*pDevice)[BEGIN_SCENE_INDEX]			= hkBeginScene;
	(*pDevice)[END_SCENE_INDEX]				= hkEndscene;
	(*pDevice)[RESET_INDEX]					= hkReset;
	(*pDevice)[PRESENT_INDEX]				= hkPresent;
	(*pDevice)[DRAW_INDEXED_PRIM_INDEX]		= hkDrawIndexedPrimitive;
}

