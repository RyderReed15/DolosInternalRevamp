#pragma once

#ifndef GAME_EVENT_LISTENER_H
#define GAME_EVENT_LISTENER_H
#include "../../Utils/Macros.h"
#include "../Math/UtlVector.h"
class KeyValues;

class IGameEvent {
public:
    virtual                 ~IGameEvent (void);
    virtual const char*     GetName     (void) const = 0;

    virtual bool            IsReliable  (void) const = 0;
    virtual bool            IsLocal     (void) const = 0;
    virtual bool            IsEmpty     (const char* szKeyName = nullptr) = 0;

    virtual bool            GetBool     (const char* szKeyName = nullptr, bool bDefaultValue = false) = 0;
    virtual int             GetInt      (const char* szKeyName = nullptr, int iDefaultValue = 0) = 0;
    virtual uint64_t        GetUint64   (const char* szKeyName = nullptr, unsigned long ulDefaultValue = 0) = 0;
    virtual float           GetFloat    (const char* szKeyName = nullptr, float flDefaultValue = 0.0f) = 0;
    virtual const char*     GetString   (const char* szKeyName = nullptr, const char* szDefaultValue = "") = 0;
    virtual const wchar_t*  GetWString  (const char* szKeyName, const wchar_t* wszDefaultValue = L"") = 0;

    virtual void            Unknown000  (void) = 0;

    virtual void            SetBool     (const char* szKeyName, bool bValue) = 0;
    virtual void            SetInt      (const char* szKeyName, int iValue) = 0;
    virtual void            SetUint64   (const char* szKeyName, unsigned long ulValue) = 0;
    virtual void            SetFloat    (const char* szKeyName, float flValue) = 0;
    virtual void            SetString   (const char* szKeyName, const char* szValue) = 0;
    virtual void            SetWString  (const char* szKeyName, const wchar_t* wszValue) = 0;

};
class IGameEventDescriptor {
public:
    int	                iEventId;
    int                 iElementIndex;
    KeyValues*          pKeys;
    UtlVector<void*>    vListeners;
};

class IGameEventListener {
public:
    virtual         ~IGameEventListener() { delete this; };
    virtual void    FireGameEvent(IGameEvent* pEvent) = 0;
    virtual int     GetEventDebugId() { return 42; };
};

class IGameEventManager {
public:
    virtual             ~IGameEventManager() = 0;
    virtual int         LoadEventsFromFile(const char* filename) = 0;
    virtual void        Reset() = 0;
    virtual bool        AddListener(IGameEventListener* listener, const char* name, bool bServerSide) = 0;
    virtual bool        FindListener(IGameEventListener* listener, const char* name) = 0;
    virtual int         RemoveListener(IGameEventListener* listener) = 0;
    virtual bool        AddListenerGlobal(IGameEventListener* listener, bool bServerSide) = 0;
    virtual IGameEvent* CreateEvent(const char* name, bool bForce, unsigned int dwUnknown) = 0;
    virtual bool        FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
    virtual bool        FireEventClientSide(IGameEvent* event) = 0;
    virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
    virtual void        FreeEvent(IGameEvent* event) = 0;
    virtual bool        SerializeEvent(IGameEvent* event, void* buf) = 0;
    virtual IGameEvent* UnserializeEvent(void* buf) = 0;
};
#endif // !GAME_EVENT_LISTENER_H
