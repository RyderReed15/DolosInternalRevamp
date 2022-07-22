#pragma once

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H


#include "../Utils/Hash.h"
#include "SkinChanger.h"
#include "Radar.h"

#include "../SDK/SDK.h"

namespace Events {

    void HandleEvent(IGameEvent* pEvent);

}



#endif // !EVENT_LISTENER_H