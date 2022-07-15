#pragma once

#include "../../Utils/Macros.h"

class ISurface {
public:
    VFUNC(bool, IsCursorUnlocked, 58, (), (this));
    VFUNC(void, UnlockCursor, 66, (), (this));
    VFUNC(void, LockCursor, 67, (), (this));
};