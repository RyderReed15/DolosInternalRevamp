#include "EventListener.h"
// Link to events list and desc https://wiki.alliedmods.net/Counter-Strike:_Global_Offensive_Events

void Events::HandleEvent(IGameEvent* pEvent) {

    switch (const_hash(pEvent->GetName())) {
    case const_hash("player_death") :
        SkinChanger::UpdateStatTrak(pEvent);
        SkinChanger::OverrideKillIcon(pEvent);
        break;
    case const_hash("game_newmap") :
        RadarESP::LoadRadar(g_pRender, g_pEngineClient->GetLevelNameShort());
        break;
    case const_hash("player_connect_full") :
        g_pLocalPlayer = g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
        EntityData::UpdateLocalPlayerDataOnce();
        break;
    case const_hash("cs_game_disconnected"): 
    case const_hash("achievement_info_loaded"): // Called when server map is changed but before game_newmap
        g_pLocalPlayer = nullptr;
        break;
    }
    //game_newmap, player_spawn, player_hurt, player_connect_full, player_death, cs_disconnected

}


