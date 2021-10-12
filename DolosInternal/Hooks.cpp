#include "Hooks.h"


bool InitializeHooks() {

	
	g_vClient			= new VMTManager((void***)g_pClientMode);
	g_vClientBase		= new VMTManager((void***)g_pBaseClient);
	g_vD3D				= new VMTManager((void***)g_pD3DDevice);
	g_vModelRender		= new VMTManager((void***)g_pModelRender);
	

	oCreateMove				= (fnCreateMove)			g_vClient->HookFunction			(CREATE_MOVE_INDEX			, hkCreateMove);

	oFrameStageNotify		= (fnFrameStageNotify)		g_vClientBase->HookFunction		(FRAME_STAGE_INDEX			, hkFrameStageNotify);

	oDrawModelExecute		= (fnDrawModelExecute)		g_vModelRender->HookFunction	(DRAW_MODEL_EXECUTE_INDEX	, hkDrawModelExecute);

	oBeginScene				= (fnBeginScene)			g_vD3D->HookFunction			(BEGIN_SCENE_INDEX			, hkBeginScene);
	oDrawIndexedPrimitive	= (fnDrawIndexedPrimitive)	g_vD3D->HookFunction			(DRAW_INDEXED_PRIM_INDEX	, hkDrawIndexedPrimitive);
	oPresent				= (fnPresent)				g_vD3D->HookFunction			(PRESENT_INDEX				, hkPresent);
	oEndScene				= (fnEndScene)				g_vD3D->HookFunction			(END_SCENE_INDEX			, hkEndscene);
	oReset					= (fnReset)					g_vD3D->HookFunction			(RESET_INDEX				, hkReset);

	oWndProc = SetWindowLongPtrW(FindWindowW(L"Valve001", NULL), GWLP_WNDPROC, LONG_PTR(&hkWndProc));
	

	return true;
}

bool UninitializeHooks() {
	g_vClient->FreeFunction			(CREATE_MOVE_INDEX);

	g_vClientBase->FreeFunction		(FRAME_STAGE_INDEX);
	
	g_vModelRender->FreeFunction	(DRAW_MODEL_EXECUTE_INDEX);

	g_vD3D->FreeFunction			(BEGIN_SCENE_INDEX);
	g_vD3D->FreeFunction			(DRAW_INDEXED_PRIM_INDEX);
	g_vD3D->FreeFunction			(PRESENT_INDEX);
	g_vD3D->FreeFunction			(END_SCENE_INDEX);
	g_vD3D->FreeFunction			(RESET_INDEX);

	delete g_vClient;
	delete g_vClientBase;
	delete g_vD3D;
	delete g_vModelRender;
	SetWindowLongPtrW(FindWindowW(L"Valve001", NULL), GWLP_WNDPROC, oWndProc);
	return true;
}

LRESULT hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	POINTS ptLoc = *(POINTS*)&lParam;
	short iKeyCode = wParam;
	switch (uMsg) {
	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:

		g_pGUIContainer->GetEventHandler()->HandleMouseInput(GUI_EVENT_TYPE::CLICK, { ptLoc.x, ptLoc.y });

		break;
	
	case WM_LBUTTONUP:

		g_pGUIContainer->GetEventHandler()->HandleMouseInput(GUI_EVENT_TYPE::RELEASE, { ptLoc.x, ptLoc.y });

		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {

			g_pGUIContainer->GetEventHandler()->HandleMouseInput(GUI_EVENT_TYPE::DRAG, { ptLoc.x, ptLoc.y });
		}
		//Hover maybe?
		break;

	}

	
	return CallWindowProc((WNDPROC)oWndProc, hWnd, uMsg, wParam, lParam);
}


void __fastcall hkFrameStageNotify(void* _this, void* edx, ClientFrameStage_t stage) {


}

void __fastcall hkDrawModelExecute(void* _this, void* edx, void* pCtx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld) {


}

bool __fastcall hkCreateMove(void* _this, void* edx, float flInputSampleTime, CUserCmd* pCmd) {

	return oCreateMove(_this, edx, flInputSampleTime, pCmd);
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

	HRESULT hReturn = oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return hReturn;
}

HRESULT APIENTRY hkEndscene(IDirect3DDevice9* pDevice) {

	HRESULT hReturn = oEndScene(pDevice);
	if (g_pGUIContainer && g_pRender) {
		g_pGUIContainer->GetEventHandler()->ProccessEvents();
		g_pGUIContainer->DrawElements(g_pRender, g_pAvenirFont);
		FixD3D((void***)pDevice);
	}
	
	return hReturn;
}

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {

	HRESULT hReturn = oReset(pDevice, pPresentationParameters);
	return hReturn;
}

void FixD3D(void*** pDevice) {
	(*pDevice)[BEGIN_SCENE_INDEX]			= hkBeginScene;
	(*pDevice)[END_SCENE_INDEX]				= hkEndscene;
	(*pDevice)[RESET_INDEX]					= hkReset;
	(*pDevice)[PRESENT_INDEX]				= hkPresent;
	(*pDevice)[DRAW_INDEXED_PRIM_INDEX]		= hkDrawIndexedPrimitive;
}

