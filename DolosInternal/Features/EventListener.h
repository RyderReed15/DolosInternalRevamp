#pragma once

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "../SDK/SDK.h"
#include "../Utils/Hash.h"
#include "SkinChanger.h"
#include "Radar.h"

namespace Events {

    void HandleEvent(IGameEvent* pEvent);

}



#endif // !EVENT_LISTENER_H