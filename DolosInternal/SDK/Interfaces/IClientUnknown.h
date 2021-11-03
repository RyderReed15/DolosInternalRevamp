#pragma once

#ifndef CLIENT_UNKNOWN_H
#define CLIENT_UNKNOWN_H

#include "IHandleEntity.h"

class IClientNetworkable;
class IClientEntity;
class IClientThinkable;
class ICollideable;
class IClientRenderable;
class CBaseEntity;

class IClientUnknown : public IHandleEntity
{
public:
    virtual ICollideable*       GetCollideable(void) = 0;
    virtual IClientNetworkable* GetClientNetworkable(void) = 0;
    virtual IClientRenderable*  GetClientRenderable(void) = 0;
    virtual IClientEntity*      GetIClientEntity(void) = 0;
    virtual CBaseEntity*        GetBaseEntity(void) = 0;
    virtual IClientThinkable*   GetClientThinkable(void) = 0;
};

#endif