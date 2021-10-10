#pragma once

#ifndef HANDLE_ENTITY_H
#define HANDLE_ENTITY_H

typedef unsigned long EHANDLE;

class CBaseHandle;
class IHandleEntity
{
public:
    virtual ~IHandleEntity() {}
    virtual void SetRefEHandle(const CBaseHandle& handle) = 0;
    virtual const EHANDLE& GetRefEHandle() const = 0;
};



#endif // !HANDLE_ENTITY_H
