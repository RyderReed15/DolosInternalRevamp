#pragma once

#ifndef NETVARMANAGER_H
#define NETVARMANAGER_H




#include <unordered_map>
#include "../SDK/Interfaces/IBaseClientDLL.h"

class IClientClass;
class RecvTable;


bool    InitializeNetvars   (IClientClass* pClientClass);
void    GetNetvarPointer    (RecvTable * pRecvTable);
char*   GetNetvar           (const char* szTableName, const char* szVarName);

#endif // !NETVARMANAGER_H