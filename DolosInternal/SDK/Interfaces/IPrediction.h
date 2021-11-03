#pragma once

#ifndef PREDICTION_H
#define PREDICTION_H

#include "IClientEntity.h"
#include "IMoveHelper.h"
#include "IBaseClientDLL.h"

#include "../../Utils/Macros.h"
class CMoveData
{
public:
    bool    m_bFirstRunOfFunctions : 1;
    bool    m_bGameCodeMovedPlayer : 1;
    int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
    int     m_nImpulseCommand;      // Impulse command issued.
    Vector  m_vecViewAngles;        // Command view angles (local space)
    Vector  m_vecAbsViewAngles;     // Command view angles (world space)
    int     m_nButtons;             // Attack buttons.
    int     m_nOldButtons;          // From host_client->oldbuttons;
    float   m_flForwardMove;
    float   m_flSideMove;
    float   m_flUpMove;
    float   m_flMaxSpeed;
    float   m_flClientMaxSpeed;
    Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
    Vector  m_vecAngles;            // edict::angles
    Vector  m_vecOldAngles;
    float   m_outStepHeight;        // how much you climbed this move
    Vector  m_outWishVel;           // This is where you tried 
    Vector  m_outJumpVel;           // This is your jump velocity
    Vector  m_vecConstraintCenter;
    float   m_flConstraintRadius;
    float   m_flConstraintWidth;
    float   m_flConstraintSpeedFactor;
    float   m_flUnknown[5];
    Vector  m_vecAbsOrigin;
};

class IPrediction {
public:
    VFUNC(bool, InPrediction, 14, (), (this));
    VFUNC(void, RunCommand  , 19, (IClientEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper)                        , (this, pPlayer, pCmd, pMoveHelper));
    VFUNC(void, SetupMove   , 20, (IClientEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper, CMoveData* pMoveData)  , (this, pPlayer, pCmd, pMoveHelper, pMoveData));
    VFUNC(void, FinishMove  , 21, (IClientEntity* pPlayer, CUserCmd* pCmd, CMoveData* pMoveData)                            , (this, pPlayer, pCmd, pMoveData));
};


#endif // !PREDICTION_H

