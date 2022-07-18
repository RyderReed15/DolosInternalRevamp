#include "EventListener.h"
// Link to events list and desc https://wiki.alliedmods.net/Counter-Strike:_Global_Offensive_Events

void Events::HandleEvent(IGameEvent* pEvent) {

    switch (const_hash(pEvent->GetName())) {
    case const_hash("player_death") :
        SkinChanger::UpdateStatTrak(pEvent);
        SkinChanger::OverrideKillIcon(pEvent);
        break;
    }
    //game_newmap, player_spawn, player_hurt, player_connect_full, player_death, 

}


