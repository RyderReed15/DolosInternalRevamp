#include "EnginePrediction.h"

float flOldTime, flOldFrameTime;

//Predicts player locations to the location at the end of the tick
int EnginePrediction::Begin(IClientEntity* pLocalPlayer, CUserCmd* pCmd) {

	if (!pLocalPlayer) {
		return 0;
	}

	flOldTime = g_pGlobalVars->curtime;
	flOldFrameTime = g_pGlobalVars->frametime;

	int iFlags = pLocalPlayer->GetFlags();

	// Set time to current tick not previous
	g_pGlobalVars->curtime = pLocalPlayer->GetTickBase() * g_pGlobalVars->interval_per_tick; 
	g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick;

	g_pGameMovement->StartTrackPredictionErrors(pLocalPlayer);
	CMoveData moveData = { 0 };

	g_pMoveHelper->SetHost(pLocalPlayer);
	g_pPrediction->SetupMove(pLocalPlayer, pCmd, g_pMoveHelper, &moveData);

	g_pGameMovement->ProcessMovement(pLocalPlayer, &moveData);

	g_pPrediction->FinishMove(pLocalPlayer, pCmd, &moveData);

	return iFlags;




}
//Resets player locations and time to original
void EnginePrediction::End(IClientEntity* pLocalPlayer) {

	g_pGameMovement->FinishTrackPredictionErrors(pLocalPlayer);

	g_pMoveHelper->SetHost(nullptr);

	g_pGlobalVars->curtime = flOldTime;
	g_pGlobalVars->frametime = flOldFrameTime;
}