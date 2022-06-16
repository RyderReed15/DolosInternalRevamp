#include "Triggerbot.h"

void Triggerbot::Tick(CUserCmd* pCmd){
    if (!Settings.Trigger.Enabled) return;

    CGameTrace trace;
    Ray_t ray;
    CTraceFilter tracefilter;
    tracefilter.pSkip = g_pLocalPlayer;
    Vector vViewAngles = { pCmd->qViewAngles.pitch, pCmd->qViewAngles.yaw, 0 };
    ray.Init(g_pLocalPlayer->GetEyePos(), vViewAngles.Forward(Settings.Trigger.Range) + g_pLocalPlayer->GetEyePos());

    g_pEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &tracefilter, &trace);


    if (trace.hit_entity) {
        if (trace.hit_entity->SanityCheck() && (Settings.Aimbot.TargetAll || trace.hit_entity->GetTeam() != g_pLocalPlayer->GetTeam())) {
            CBaseCombatWeapon* pWeapon = g_pLocalPlayer->GetWeapon();
            if (pWeapon) {

                if (pWeapon->m_flNextPrimaryAttack() > g_pGlobalVars->curtime || (pWeapon->m_iClip1() <= 0 && pWeapon->GetClientClass()->m_ClassID != CKNIFE)) {

                    if (pWeapon->GetWeaponId() == WeaponIndices::WEAPON_REVOLVER) {
                        pCmd->iButtons &= ~IN_ATTACK2;
                    }else{
                        pCmd->iButtons &= ~IN_ATTACK;
                    }
                }
                else {
                    if (pWeapon->GetWeaponId() == WeaponIndices::WEAPON_REVOLVER) {

                        pCmd->iButtons |= IN_ATTACK2;
                    }
                    else {
                        pCmd->iButtons |= IN_ATTACK;
                    }
                }
            }
        }
    } 
}
