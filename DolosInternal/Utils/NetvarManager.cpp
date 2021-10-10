#include "NetvarManager.h"


std::unordered_map<int, std::unordered_map<int, char*>> mOffsets;
std::hash<std::string> hash;

bool InitializeNetvars(IClientClass* pClientClass) {
    for (IClientClass* pCurrNode = pClientClass; pCurrNode; pCurrNode = pCurrNode->m_pNext) {

        GetNetvarPointer(pCurrNode->m_pRecvTable);

    }
    return true;
}

void GetNetvarPointer(RecvTable* pRecvTable) {

    for (int i = 0; i < pRecvTable->m_nProps; i++) {
        RecvProp* pRecvProp = &pRecvTable->m_pProps[i];
        if (pRecvProp->m_pDataTable) {

            GetNetvarPointer(pRecvProp->m_pDataTable);
        }

        if (!mOffsets[hash(pRecvTable->m_pNetTableName)][hash(pRecvProp->m_pVarName)] && (char*)pRecvProp->m_Offset) {

            mOffsets[hash(pRecvTable->m_pNetTableName)][hash(pRecvProp->m_pVarName)] = (char*)pRecvProp->m_Offset;
        }

    }
}


char* GetNetvar(const char* TableName, const char* VarName) {
    return mOffsets[hash(TableName)][hash(VarName)];
}