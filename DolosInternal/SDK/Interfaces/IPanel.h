#pragma once

#ifndef PANEL_H
#define PANEL_H

#include "../../Utils/Macros.h"
class IPanel {
public:
	VFUNC(void, SetKeyboardInputState, 31, (unsigned int iPanel, bool bState), (this, iPanel, bState));
	VFUNC(void, SetMouseInputState, 32, (unsigned int iPanel, bool bState), (this, iPanel, bState));
};


#endif // !PANEL_H
