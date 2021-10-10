#pragma once

#ifndef CLIENT_NETWORKABLE_H
#define CLIENT_NETWORKABLE_H

class IClientClass;
class IClientUnknown;

class IClientNetworkable
{
public:
    virtual IClientUnknown* GetIClientUnknown(void) = 0;
    virtual void Release(void) = 0;
    virtual IClientClass* GetClientClass(void) = 0;
    virtual void NotifyShouldTransmit(void) = 0;
    virtual void OnPreDataChanged(void) = 0;
    virtual void OnDataChanged(void) = 0;
    virtual void PreDataUpdate(void) = 0;
    virtual void PostDataUpdate(void) = 0;
    virtual void OnDataUnchangedInPVS(void) = 0;
    virtual bool IsDormant(void) = 0;
    virtual int Index(void) const = 0;
    virtual void ReceiveMessage(int classID) = 0;
    virtual void* GetDataTableBasePtr(void) = 0;
    virtual void SetDestroyedOnRecreateEntities(void) = 0;
};

#endif