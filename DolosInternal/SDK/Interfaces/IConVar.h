#pragma once

#ifndef CONVAR_H
#define CONVAR_H
#include "../../Utils/Macros.h"
class IConVar {
public:

	VFUNC(float, GetFloat, 12, (), (this));
	VFUNC(int, GetInt, 13, (), (this));
	VFUNC(void, SetValue, 14, (const char* szValue), (this, szValue));
	VFUNC(void, SetValue, 15, (float flValue), (this, flValue));
	VFUNC(void, SetValue, 16, (int iValue), (this, iValue));
};

class ICVar {
public:
	VFUNC(IConVar*, FindVar, 15, (const char* szVarName), (this, szVarName));
};


#endif // !CONVAR_H
