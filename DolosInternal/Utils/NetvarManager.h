#pragma once

#ifndef NETVARMANAGER_H
#define NETVARMANAGER_H



#include <unordered_map>
#include <fstream>
#include <iomanip>
#include "Hash.h"
#include "../SDK/Interfaces/IClientClass.h"


bool    InitializeNetvars   (IClientClass* pClientClass);
void    GetNetvarPointer    (RecvTable * pRecvTable, unsigned int pOffset = 0);
char*   GetNetvar           (const char* szTableName, const char* szVarName);
void    DumpNetvars         (IClientClass* pClientClass, const char* szPath);
void    DumpPointer         (std::ostream& fOut, std::unordered_map<unsigned int, std::unordered_map<unsigned int, char*>>* pNetvars, RecvTable* pRecvTable, unsigned int pOffset = 0);

#endif // !NETVARMANAGER_H