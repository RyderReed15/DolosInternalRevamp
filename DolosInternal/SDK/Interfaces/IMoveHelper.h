#pragma once

#ifndef MOVE_HELPER_H
#define MOVE_HELPER_H

class IClientEntity;

class IMoveHelper
{
public:
    virtual	void Unknown000() = 0;
    virtual void SetHost(IClientEntity* host) = 0;
};


#endif // !MOVE_HELPER_H

