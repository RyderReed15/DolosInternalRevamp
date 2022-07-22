#include "Aimbot.h"

Aimbot::ClosestTarget   vClosestTarget;
Vector                  vLastAngles;
int                     iTargetIndex = -1;
int                     iStartTick = 0;
Vector                  vOveraim;

bool Aimbot::Tick(CUserCmd* pCmd) {

    if (g_pLocalPlayer && g_pLocalPlayer->IsAlive()) {

        EntityData::LocalPlayerData* pLocalPlayerData = EntityData::GetLocalPlayerData();

        Vector vAimPunch = pLocalPlayerData->vAimPunch;
        Vector vViewAngles = pLocalPlayerData->vViewAngles;

        if (!vLastAngles.IsValid()) vLastAngles = vViewAngles;

        if (Settings.Aimbot.Active) {
            
            Vector vPlayerPos = g_pLocalPlayer->GetEyePos();

            Vector vTarget = FindClosestTarget(vPlayerPos, vViewAngles, vAimPunch, pCmd->iTickCount);

            if (vTarget.IsValid()) {

                if (!Settings.Aimbot.Silent) {
                    QAngle qNewAngles = GetNewAngles(vViewAngles, vTarget, pCmd->iTickCount);

                    RecoilControl::UpdateOldPunch(vAimPunch);
                    g_pEngineClient->SetViewAngles(qNewAngles);

                    pCmd->qViewAngles = qNewAngles;
                }
                else {
                    QAngle qNewAngles = GetNewAngles(vLastAngles, vTarget, pCmd->iTickCount);

                    QAngle qRecoil = RecoilControl::RecoilControl(vViewAngles, vAimPunch, false);
                    g_pEngineClient->SetViewAngles(qRecoil); //Using engine set angles moves the players screen as well, pCmd only moves server view

                    pCmd->qViewAngles = qNewAngles;
                    vLastAngles = { qNewAngles.pitch, qNewAngles.yaw, 0 };
                    
                }
               
                return !Settings.Aimbot.Silent;
            }
        }
        if (Settings.Aimbot.Silent) {
            vLastAngles = GetNewAngles(vLastAngles, vViewAngles, pCmd->iTickCount);
            QAngle qNewAngles = RecoilControl::RecoilControl(vLastAngles, vAimPunch, false);
            pCmd->qViewAngles = qNewAngles;
            vLastAngles = { qNewAngles.pitch, qNewAngles.yaw, 0 };
            return false;
        }
        else {
            QAngle qNewAngles = RecoilControl::RecoilControl(vViewAngles, vAimPunch, false);
            pCmd->qViewAngles = qNewAngles;
            return true;
        } 
        
    }
    return true;
}
/* Finds most valid target near the players crosshair applying a weight to distance both in space and quaternion
* @return Angles to point at best target */
Vector Aimbot::FindClosestTarget(Vector vPlayerPos, Vector vViewAngles, Vector vAimPunch, int iTickCount) {

    Vector vClosest; vClosest.Invalidate();
    float flClosest = INFINITY;
    int iNewTargetIndex = -1;
    vClosestTarget.flFOV = 0;

    std::unordered_map<int, EntityData::PlayerData>* pPlayerData = EntityData::GetAllPlayerData();

    if (pPlayerData->count(iTargetIndex) && pPlayerData->at(iTargetIndex).bAccessible) {

        IClientEntity* pEntity = g_pClientEntityList->GetClientEntity(iTargetIndex);
        if (pEntity) {
            for (size_t j = 0; j < Settings.Aimbot.Targets.size(); j++) {
                if (!Settings.Aimbot.Targets[j].Enabled) continue;

                Vector vEnemyPos = pEntity->GetBonePos(Settings.Aimbot.Targets[j].Bone);

                if (Trace(g_pEngineTrace, g_pLocalPlayer, pEntity, vPlayerPos, vEnemyPos)) {
                    float flDist = vPlayerPos.Distance(vEnemyPos);

                    Vector vRotateAngle = CalculateAngle(vPlayerPos, vEnemyPos);

                    vRotateAngle = RecoilControl::RecoilControl(vRotateAngle, vAimPunch, true);

                    float flRotateDistance = vRotateAngle.AngularDistance(vViewAngles);

                    if (flRotateDistance < flClosest) {
                        EntityData::WorldToScreen(vClosestTarget.vTarget, vEnemyPos);
                        vClosestTarget.flFOV = FOVFormula(Settings.Aimbot.Targets[j].FOV, flDist);

                        flClosest = flRotateDistance;

                        if (flRotateDistance <= FOVFormula(Settings.Aimbot.Targets[j].FOV, flDist)) {
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

    EntityData::LocalPlayerData* pLocalPlayerData = EntityData::GetLocalPlayerData();
    

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

                    if (flRotateDistance < flClosest) {
                        EntityData::WorldToScreen(vClosestTarget.vTarget, vEnemyPos);
                        vClosestTarget.flFOV = FOVFormula(Settings.Aimbot.Targets[j].FOV, flDist);
                        
                        flClosest = flRotateDistance;

                        if (flRotateDistance <= FOVFormula(Settings.Aimbot.Targets[j].FOV, flDist)) {
                            vClosest = vRotateAngle;
                            iNewTargetIndex = it->first;
                        }
                    }
                }
            } 
        }
    }
    if (iNewTargetIndex != iTargetIndex) {
        // New target found when there was a prevoius target, ignore case if already a wait time
        if (iNewTargetIndex != -1 && iTargetIndex != -1 && iStartTick <= iTickCount) iStartTick = iTickCount + static_cast<int>(.5f + Settings.Aimbot.WaitTime / g_pGlobalVars->interval_per_tick);
        else if (iStartTick <= iTickCount) iStartTick = iTickCount; // No wait time needed.
        iTargetIndex = iNewTargetIndex;
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

void Aimbot::DrawFOV() {


    if (vClosestTarget.flFOV <= 0) return;

    //ScreenFOV = tanf(half fov) * screenx / screeny / (4 / 3)
    //radius = tanf(closest) / screenFov * screenX / 2
    //Simplifies to tanf(closest) * screenY * .6666f for 90 degrees - default csgo FOV
    //https://www.unknowncheats.me/forum/counterstrike-global-offensive/129068-draw-aimbot-fov.html
    float flScreenFOVX = 1 / g_vScreenSize.y / .6666f;

    g_pRender->Begin(BUFFER_TYPE::BUFFER_LINE);
    g_pRender->DrawCircleOutline({ vClosestTarget.vTarget.x, vClosestTarget.vTarget.y }, tanf(vClosestTarget.flFOV * DEG_TO_RAD) / flScreenFOVX , 50, RED);
    g_pRender->End(BUFFER_TYPE::BUFFER_LINE);
}

//Decrease FOV with range
float Aimbot::FOVFormula(float flFOV, float flDistance){
    return flFOV / (flDistance / DISTANCE_REDUCTION_FACTOR);
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
    return vAngles.ToAngles();
}