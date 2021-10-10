#pragma once

#ifndef MOVE_HELPER_H
#define MOVE_HELPER_H

#include "IClientEntity.h"

class IMoveHelper
{
public:
    virtual	void _vpad() = 0;
    virtual void SetHost(IClientEntity* host) = 0;
};


#endif // !MOVE_HELPER_H

