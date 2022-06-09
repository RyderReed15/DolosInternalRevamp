#include "Aimbot.h"


void Aimbot::Tick(CUserCmd* pCmd) {

    if (g_pLocalPlayer && g_pLocalPlayer->IsAlive()) {

        Vector vAimPunch = g_pLocalPlayer->GetAimPunch();
        Vector vViewAngles = { pCmd->qViewAngles.pitch, pCmd->qViewAngles.yaw, 0 };

        if (Settings.Aimbot.Active) {
            
            Vector vPlayerPos = g_pLocalPlayer->GetEyePos();

            Vector vTarget = FindClosestTarget(vPlayerPos, vViewAngles, vAimPunch, pCmd->iTickCount);

            if (vTarget.IsValid()) {

                QAngle qNewAngles = GetNewAngles(vViewAngles, vTarget, pCmd->iTickCount);
                if (!Settings.Aimbot.Silent) g_pEngineClient->SetViewAngles(qNewAngles);
                pCmd->qViewAngles = qNewAngles;
                return;
            }
            
        }
        
        QAngle qNewAngles = RecoilControl::RecoilControl(vViewAngles, vAimPunch, false);
        if (!Settings.Aimbot.Silent) g_pEngineClient->SetViewAngles(qNewAngles);
        pCmd->qViewAngles = qNewAngles;
       
        
    }
}

Vector Aimbot::FindClosestTarget(Vector vPlayerPos, Vector vAngles, Vector vAimPunch, int iTickCount) {

    Vector vClosest; vClosest.Invalidate();
    float flClosest = INFINITY;
    IClientEntity* pNewTarget = nullptr;

    if (pTarget) {
        if (pTarget->SanityCheck() && pTarget->GetCollideable()){
            

            for (unsigned int j = 0; j < Settings.Aimbot.TargetCount; j++) {
                Vector vEnemyPos = pTarget->GetBonePos(Settings.Aimbot.Targets[j].Bone);
                bool bVisible = Trace(g_pEngineTrace, g_pLocalPlayer, pTarget, vPlayerPos, vEnemyPos);

                if (bVisible) {
                    float flDist = vPlayerPos.Distance(vEnemyPos);

                    Vector vRotateAngle = CalculateAngle(vPlayerPos, vEnemyPos);

                    vRotateAngle = RecoilControl::RecoilControl(vRotateAngle, vAimPunch, true);

                    float flRotateDistance = vRotateAngle.Lerp(vAngles, 1).Magnitude();

                    if (flRotateDistance <= FOVFormula(Settings.Aimbot.Targets[j].FOV, flDist)) {

                        float flWeight = WeightFormula(Settings.Aimbot.Targets[j].FOV, flDist, flRotateDistance);

                        if (flWeight < flClosest) {
                            flClosest = flWeight;
                            vClosest = vRotateAngle;
                        }
                    }
                }
            }
        }
    }
    if (vClosest.IsValid()) {
        return vClosest;
    }


    for (int i = 1; i < g_pEngineClient->GetMaxClients(); i++) {

        IClientEntity* pEntity = g_pClientEntityList->GetClientEntity(i);

        if (!pEntity) continue;

        if (pEntity->SanityCheck() && pEntity != g_pLocalPlayer) {
            if (Settings.Aimbot.TargetAll || pEntity->GetTeam() != g_pLocalPlayer->GetTeam()) {
                if (pEntity->GetCollideable()) {
                    
                    for (unsigned int j = 0; j < Settings.Aimbot.TargetCount; j++) {
                        Vector vEnemyPos = pEntity->GetBonePos(Settings.Aimbot.Targets[j].Bone);
                        bool bVisible = Trace(g_pEngineTrace, g_pLocalPlayer, pEntity, vPlayerPos, vEnemyPos);

                        if (bVisible) {
                            float flDist = vPlayerPos.Distance(vEnemyPos);

                            Vector vRotateAngle = CalculateAngle(vPlayerPos, vEnemyPos);

                            vRotateAngle = RecoilControl::RecoilControl(vRotateAngle, vAimPunch, true);

                            float flRotateDistance = vRotateAngle.Lerp(vAngles, 1).Magnitude();

                            if (flRotateDistance <= FOVFormula(Settings.Aimbot.Targets[j].FOV, flDist)) {

                                float flWeight = WeightFormula(Settings.Aimbot.Targets[j].FOV, flDist, flRotateDistance);

                                if (flWeight < flClosest) {
                                    flClosest = flWeight;
                                    vClosest = vRotateAngle;
                                    pNewTarget = pEntity;
                                }
                            }  
                        }
                    }
                }
            }
        }
    }
    if (pNewTarget != pTarget) {
        // New target found when there was a prevoius target, ignore case if already a wait time
        if (pTarget != nullptr && pNewTarget != nullptr && iStartTick <= iTickCount) iStartTick = iTickCount + static_cast<int>(.5f + Settings.Aimbot.WaitTime / g_pGlobalVars->interval_per_tick);
        else if (iStartTick <= iTickCount) iStartTick = iTickCount; // No wait time needed.
        pTarget = pNewTarget;
        vOveraim.Invalidate();

    }
    return vClosest;
}

Vector Aimbot::CalculateOveraim(Vector vViewAngles, Vector vDest, int iTick){
    if (iTick == iStartTick && !vOveraim.IsValid()) {
        float flDistance = vDest.Lerp(vViewAngles, 1).Magnitude() / 30;

        float flAngle = static_cast<float>(rand() % 360);

        vOveraim = { sinf(flAngle) * flDistance, cosf(flAngle) * flDistance, 0 };
    }
    else if (!vOveraim.IsValid()) {
        return vViewAngles;
    }
    return vViewAngles + vOveraim;
}

Vector Aimbot::GetNewAngles(Vector vViewAngles, Vector vDest, int iTick){
    
    vViewAngles = vViewAngles.ToAngles();

    if (Settings.Aimbot.Overaim) {
        vViewAngles = CalculateOveraim(vViewAngles, vDest, iTick);
    }

    float flPercent = (g_pGlobalVars->interval_per_tick * (iTick - iStartTick)) / Settings.Aimbot.AimTime;
    if (iTick - iStartTick < 2) flPercent = flPercent > 1 ? 1 : flPercent;
    else flPercent = flPercent > 1 ? Settings.Aimbot.AntilockFactor : flPercent;  

    //Clamp flPercent

    if (flPercent < 0) return vViewAngles;

    float flDistance = vViewAngles.Distance(vDest);

    if (flDistance < 1) vOveraim.Invalidate();

    Vector vDelta = vViewAngles.Lerp(vDest, flPercent);

    vDelta.x *= flPercent;
    vDelta.y *= flPercent;
    if (flPercent < .95f) {
        vDelta.x *= (1 + .15f * Settings.Aimbot.Curve);
        vDelta.x *= 1 + .01f * (rand() / float(RAND_MAX) - .5f);
        vDelta.y *= 1 + .01f * (rand() / float(RAND_MAX) - .5f);
    }
    
    if (vDelta.Magnitude() > 35) {
        //Prevent Untrusted ban from rotating too fast
        vDelta = vDelta.Normalize() * 35;
    }
    return (vViewAngles - vDelta).ToAngles();
    
}

float Aimbot::WeightFormula(float flFOV, float flDistance, float flRotateDistance){
    return flRotateDistance + (flFOV / (.1f + Settings.Aimbot.RangeFactor * 3500 / flDistance));
}

float Aimbot::FOVFormula(float flFOV, float flDistance){
    return flFOV / (1 + Settings.Aimbot.RangeFactor * flDistance / 500);
}

Vector Aimbot::CalculateAngle(Vector vStart, Vector vDest) {
    Vector vAngles;

    Vector vDelta = (vStart - vDest);

    vAngles.x = atanf(vDelta.z / std::hypot(vDelta.x, vDelta.y)) * 180.f / PI;
    vAngles.y = atanf(vDelta.y / vDelta.x) * 180.f / PI;
    vAngles.z = 0.0f;

    if (vDelta.x >= 0.0f)
        vAngles.y += 180.0f;
    return vAngles;
}