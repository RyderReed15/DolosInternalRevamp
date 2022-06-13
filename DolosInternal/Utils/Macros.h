#pragma once

#ifndef MACROS_H
#define MACROS_H

#include "NetvarManager.h"


#define VFUNC( type, func, index, args, argscall) type func args noexcept \
{\
	return VFuncCall<type, index>argscall; \
}
template <typename T, unsigned int index, typename ...Args>
inline static T VFuncCall(void* _this, Args ... args) {
	typedef T(__thiscall* name)(void*, Args...);

	return (T)((*(name**)_this)[index](_this, args...));
}



#define NETVAR( type, func, table, var ) type& func() \
{\
	return *(type*)((unsigned int)(this) + GetNetvar(table, var));\
}




#define ONETVAR( type, func, table, var, offset ) type& func() \
{\
	return *(type*)((unsigned int)(this) + GetNetvar(table, var) + offset);\
}




#define PNETVAR( type, func, table, var ) type* func() \
{\
	return (type*)((unsigned int)(this) + GetNetvar(table, var));\
}




#define OPNETVAR( type, func, table, var, offset ) type* func() \
{\
	return (type*)((unsigned int)(this) + GetNetvar(table, var) + offset);\
}


#endif