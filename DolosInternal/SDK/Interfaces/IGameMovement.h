#pragma once

#ifndef GAME_MOVEMENT_H
#define GAME_MOVEMENT_H
#include "../../Utils/Macros.h"

class CMoveData;
class IClientEntity;

class IGameMovement {
public:
	VFUNC(void, ProcessMovement				, 1, (IClientEntity* pLocalPlayer, CMoveData* pMoveData), (this, pLocalPlayer, pMoveData));
	VFUNC(void, StartTrackPredictionErrors	, 3, (IClientEntity* pLocalPlayer)						, (this, pLocalPlayer));
	VFUNC(void, FinishTrackPredictionErrors	, 4, (IClientEntity* pLocalPlayer)						, (this, pLocalPlayer));
};



#endif // !GAME_MOVEMENT_H
