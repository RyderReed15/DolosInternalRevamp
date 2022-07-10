#pragma once

#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "../../Utils/Macros.h"
#include "IAppSystem.h"

class IInputSystem : public IAppSystem {
public:
    VFUNC(void, EnableInput, 11, (bool bEnable), (this, bEnable));

};

#endif // !INPUT_SYSTEM_H
