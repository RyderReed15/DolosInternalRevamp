#pragma once

#ifndef GAME_EVENT_LISTENER_H
#define GAME_EVENT_LISTENER_H
#include "../../Utils/Macros.h"
#include "../Math/UtlVector.h"
class KeyValues;

class IGameEvent {
public:
    virtual ~IGameEvent();
    virtual char* GetName();
    VFUNC(int, GetInt, 6, (const char* szKeyName, int iDefaultValue = 0), (this, szKeyName, iDefaultValue));
    VFUNC(float, GetFloat, 8, (const char* szKeyName, float flDefaultValue = 0), (this, szKeyName, flDefaultValue));
    VFUNC(const char*, GetString, 9, (const char* szKeyName, const char* szDefaultValue = ""), (this, szKeyName, szDefaultValue));
    VFUNC(int, SetInt, 13, (const char* szKeyName, int iValue), (this, szKeyName, iValue));
    VFUNC(float, SetFloat, 16, (const char* szKeyName, float flValue), (this, szKeyName, flValue));

};
class IGameEventDescriptor {
public:
    int	iEventId;
    int iElementIndex;
    KeyValues* pKeys;
    UtlVector<void*> vListeners;
};

class IGameEventListener {
public:
    virtual ~IGameEventListener() { delete this; };
    virtual void FireGameEvent(IGameEvent* pEvent) = 0;
    virtual int GetEventDebugId() { return 42; };
};

class IGameEventManager {
public:
    virtual             ~IGameEventManager() = 0;
    virtual int         LoadEventsFromFile(const char* filename) = 0;
    virtual void        Reset() = 0;
    virtual bool        AddListener(IGameEventListener* listener, const char* name, bool bServerSide) = 0;
    virtual bool        FindListener(IGameEventListener* listener, const char* name) = 0;
    virtual int         RemoveListener(IGameEventListener* listener) = 0;
    virtual IGameEvent* CreateEvent(const char* name, bool bForce, unsigned int dwUnknown) = 0;
    virtual bool        FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
    virtual bool        FireEventClientSide(IGameEvent* event) = 0;
    virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
    virtual void        FreeEvent(IGameEvent* event) = 0;
    virtual bool        SerializeEvent(IGameEvent* event, void* buf) = 0;
    virtual IGameEvent* UnserializeEvent(void* buf) = 0;
};
#endif // !GAME_EVENT_LISTENER_H
