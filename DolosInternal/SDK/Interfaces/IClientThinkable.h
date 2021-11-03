#pragma once

#ifndef CLIENT_THINKABLE_H
#define CLIENT_THINKABLE_H

#include "IClientUnknown.h"


class IClientThinkable
{
public:
    virtual IClientUnknown* GetIClientUnknown(void) = 0;
    virtual void            ClientThink(void) = 0;
    virtual void*           GetThinkHandle(void) = 0;
    virtual void            SetThinkHandle(void* hThink) = 0;
    virtual void            Release(void) = 0;

};

#endif
