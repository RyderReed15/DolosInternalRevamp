#pragma once

#ifndef CLINET_CLASS_H
#define CLIENT_CLASS_H

class CreateInterfaceFn;
class IClientNetworkable;
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();
class RecvTable;
class RecvProp;

#define CKNIFE 107

class IClientClass
{
public:
    CreateClientClassFn         m_pCreateFn;
    CreateEventFn               m_pCreateEventFn;
    char* m_pNetworkName;
    RecvTable* m_pRecvTable;
    IClientClass* m_pNext;
    int                         m_ClassID;
};
class RecvProp {
public:
    char* m_pVarName;
    int         m_RecvType;
    int         m_Flags;
    int         m_StringBufferSize;
    int         m_bInsideArray;
    const int   m_pExtraData;
    RecvProp* m_pArrayProp;
    int         m_ArrayLengthProxy;
    int         m_ProxyFn;
    int         m_DataTableProxyFn;
    RecvTable* m_pDataTable;
    int         m_Offset;
    int         m_ElementStride;
    int         m_nElements;
    const int   m_pParentArrayPropName;
};

class RecvTable {
public:
    RecvProp* m_pProps;
    int         m_nProps;
    int         m_pDecoder;
    char* m_pNetTableName;
    bool        m_bInitialized;
    bool        m_bInMainList;
};
#endif // !CLINET_CLASS_H
