#include "Aimbot.h"


bool Aimbot::Tick(CUserCmd* pCmd) {

    if (g_pLocalPlayer && g_pLocalPlayer->IsAlive()) {

        EntityData::LocalPlayerData* pLocalPlayerData = EntityData::GetLocalPlayerData();

        Vector vAimPunch = pLocalPlayerData->vAimPunch;
        Vector vViewAngles = pLocalPlayerData->vViewAngles;

        if (Settings.Aimbot.Active) {
            
            Vector vPlayerPos = g_pLocalPlayer->GetEyePos();

            Vector vTarget = FindClosestTarget(vPlayerPos, vViewAngles, vAimPunch, pCmd->iTickCount);

            if (vTarget.IsValid()) {

                QAngle qNewAngles = GetNewAngles(vViewAngles, vTarget, pCmd->iTickCount);

                if (!Settings.Aimbot.Silent) {
                    RecoilControl::UpdateOldPunch(vAimPunch);
                    g_pEngineClient->SetViewAngles(qNewAngles);
                }
                else {
                    QAngle qRecoil = RecoilControl::RecoilControl(vViewAngles, vAimPunch, false);
                    g_pEngineClient->SetViewAngles(qRecoil); //Using engine set angles moves the players screen as well, pCmd only moves server view
                }
                pCmd->qViewAngles = qNewAngles;
                return !Settings.Aimbot.Silent;
            }
        }
        QAngle qNewAngles = RecoilControl::RecoilControl(vViewAngles, vAimPunch, false);
        pCmd->qViewAngles = qNewAngles;    
        
    }
    return true;
}
/* Finds most valid target near the players crosshair applying a weight to distance both in space and quaternion
* @return Angles to point at best target */
Vector Aimbot::FindClosestTarget(Vector vPlayerPos, Vector vViewAngles, Vector vAimPunch, int iTickCount) {

    Vector vClosest; vClosest.Invalidate();
    float flClosest = INFINITY;
    IClientEntity* pNewTarget = nullptr;

    if (pTarget && pTarget->SanityCheck() && pTarget->GetCollideable()) {
  
        for (size_t j = 0; j < Settings.Aimbot.Targets.size(); j++) {
            if (!Settings.Aimbot.Targets[j].Enabled) continue;

            Vector vEnemyPos = pTarget->GetBonePos(Settings.Aimbot.Targets[j].Bone);

            if (Trace(g_pEngineTrace, g_pLocalPlayer, pTarget, vPlayerPos, vEnemyPos)) {
                float flDist = vPlayerPos.Distance(vEnemyPos);

                Vector vRotateAngle = CalculateAngle(vPlayerPos, vEnemyPos);

                vRotateAngle = RecoilControl::RecoilControl(vRotateAngle, vAimPunch, true);

                float flRotateDistance = vRotateAngle.AngularDistance(vViewAngles);

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
    if (vClosest.IsValid()) {
        return vClosest;
    }

    EntityData::LocalPlayerData* pLocalPlayerData = EntityData::GetLocalPlayerData();
    std::unordered_map<int, EntityData::PlayerData>* pPlayerData = EntityData::GetAllPlayerData();

    //Loop through all possible targets and find best suited one if any
    for (std::unordered_map<int, EntityData::PlayerData>::iterator it = pPlayerData->begin(); it != pPlayerData->end(); it++) {
        if (!it->second.bAccessible) continue;

        IClientEntity* pEntity = g_pClientEntityList->GetClientEntity(it->first);

        if (pEntity && Settings.Aimbot.TargetAll || !it->second.bFriendly) {
                    
            for (size_t j = 0; j < Settings.Aimbot.Targets.size(); j++) {
                if (!Settings.Aimbot.Targets[j].Enabled) continue;

                Vector vEnemyPos = pEntity->GetBonePos(Settings.Aimbot.Targets[j].Bone);

                if (Trace(g_pEngineTrace, g_pLocalPlayer, pEntity, vPlayerPos, vEnemyPos)) {
                    float flDist = vPlayerPos.Distance(vEnemyPos);

                    Vector vRotateAngle = CalculateAngle(vPlayerPos, vEnemyPos); // Get target angle

                    vRotateAngle = RecoilControl::RecoilControl(vRotateAngle, vAimPunch, true);

                    float flRotateDistance = vRotateAngle.AngularDistance(vViewAngles); //Degrees to rotate to target

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
    if (pNewTarget != pTarget) {
        // New target found when there was a prevoius target, ignore case if already a wait time
        if (pTarget != nullptr && pNewTarget != nullptr && iStartTick <= iTickCount) iStartTick = iTickCount + static_cast<int>(.5f + Settings.Aimbot.WaitTime / g_pGlobalVars->interval_per_tick);
        else if (iStartTick <= iTickCount) iStartTick = iTickCount; // No wait time needed.
        pTarget = pNewTarget;
        vOveraim.Invalidate();

    }
    return vClosest;
}
// Calculates position to miss the target at
Vector Aimbot::CalculateOveraim(Vector vViewAngles, Vector vTargetAngles, int iTick){
    if (iTick == iStartTick && !vOveraim.IsValid()) {
        float flDistance = vTargetAngles.AngularDistance(vViewAngles) / (OVERAIM_REDUCTION_FACTOR * Settings.Aimbot.OveraimFactor);

        float flAngle = static_cast<float>(rand() % 360); //Find random direction

        vOveraim = { sinf(flAngle) * flDistance, cosf(flAngle) * flDistance, 0 }; //Travel that direction by overaim amount
    }
    else if (!vOveraim.IsValid()) {
        return vViewAngles;
    }
    return vViewAngles + vOveraim;
}

// Lerps between two sets of angles with various modifications
Vector Aimbot::GetNewAngles(Vector vViewAngles, Vector vTargetAngles, int iTick){

    if (Settings.Aimbot.Overaim) vViewAngles = CalculateOveraim(vViewAngles, vTargetAngles, iTick); 

    vViewAngles = vViewAngles.ToAngles();

    //Clamp flPercent - account for silent aim and antilocking
    float flPercent = (g_pGlobalVars->interval_per_tick * (iTick - iStartTick)) / Settings.Aimbot.AimTime;

    if (iTick - iStartTick < 2 || Settings.Aimbot.Silent) flPercent = flPercent >= 1 ? 1 : flPercent;
    else flPercent = flPercent > 1 ? Settings.Aimbot.AntilockFactor : flPercent;  

    if (flPercent < 0) return vViewAngles;

    if (vViewAngles.AngularDistance(vTargetAngles) < 1) vOveraim.Invalidate();

    Vector vDelta = vViewAngles.Lerp(vTargetAngles, flPercent);

    vDelta.x *= flPercent;
    vDelta.y *= flPercent;
    if (flPercent < .95f) {
        //Randomize and curve lerp to prevent an unnatural line to target
        vDelta.x *= (1 + AIM_CURVE_AMOUNT * Settings.Aimbot.Curve);
        vDelta.x *= 1 + .01f * (rand() / float(RAND_MAX) - .5f);
        vDelta.y *= 1 + .01f * (rand() / float(RAND_MAX) - .5f);
    }
    
    if (vDelta.Magnitude() > MAX_DEG_PER_TICK) {
        //Prevent Untrusted ban from rotating too fast
        vDelta = vDelta.Normalize() * MAX_DEG_PER_TICK;
    }
    return (vViewAngles - vDelta).ToAngles();
    
}

//Weight distance to player and angular distance
float Aimbot::WeightFormula(float flFOV, float flDistance, float flRotateDistance){
    return flRotateDistance + (flFOV / (Settings.Aimbot.RangeFactor * DISTANCE_WEIGHT / flDistance));
}

//Decrease FOV with range
float Aimbot::FOVFormula(float flFOV, float flDistance){
    return flFOV / (Settings.Aimbot.RangeFactor * flDistance / DISTANCE_REDUCTION_FACTOR);
}

// Calculates the angle required to aim at the destination from the start
Vector Aimbot::CalculateAngle(Vector vStart, Vector vDest) {
    Vector vAngles;

    Vector vDelta = (vStart - vDest);
    //form a right tringle between the two points and find the angle
    vAngles.x = atanf(vDelta.z / std::hypot(vDelta.x, vDelta.y)) * RAD_TO_DEG;
    vAngles.y = atanf(vDelta.y / vDelta.x) * RAD_TO_DEG;
    vAngles.z = 0.0f;

    if (vDelta.x >= 0.0f)
        vAngles.y += 180.0f;
    return vAngles;
}