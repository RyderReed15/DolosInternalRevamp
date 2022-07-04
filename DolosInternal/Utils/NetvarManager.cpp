#include "NetvarManager.h"




bool InitializeNetvars(IClientClass* pClientClass) {
    //Loop through all classes getting the pointer to the table

    for (IClientClass* pCurrNode = pClientClass; pCurrNode; pCurrNode = pCurrNode->m_pNext) {

        GetNetvarPointer(pCurrNode->m_pRecvTable);

    }
    return true;
}

void GetNetvarPointer(RecvTable* pRecvTable, unsigned int pOffset) {
    //Iterate through table for more classes and netvars
    for (int i = 0; i < pRecvTable->m_nProps; i++) {
        RecvProp* pRecvProp = &pRecvTable->m_pProps[i];
        if (pRecvProp->m_pDataTable) {

            GetNetvarPointer(pRecvProp->m_pDataTable, pRecvProp->m_Offset + pOffset);
        }
        // m_flNextRespawnWave & m_TeamRespawnWaveTimes have duplicates with different offsets and therefore might be a problem with this setup if they are needed
        if (!mOffsets[hash(pRecvTable->m_pNetTableName)][hash(pRecvProp->m_pVarName)] && pRecvProp->m_Offset) {

            mOffsets[hash(pRecvTable->m_pNetTableName)][hash(pRecvProp->m_pVarName)] = reinterpret_cast<char*>(pRecvProp->m_Offset + pOffset);
        }
    }
}


char* GetNetvar(const char* TableName, const char* VarName) {
    return mOffsets[hash(TableName)][hash(VarName)];
}

void DumpNetvars(IClientClass* pClientClass, const char* szPath){
    std::ofstream fOut; fOut.open(szPath);

    std::unordered_map<unsigned int, std::unordered_map<unsigned int, char*>> mNetvars;

    if (fOut.is_open()) {
        for (IClientClass* pCurrNode = pClientClass; pCurrNode; pCurrNode = pCurrNode->m_pNext) {
            DumpPointer(fOut, &mNetvars, pCurrNode->m_pRecvTable);
        }
    }

    fOut.close();
}

void DumpPointer(std::ostream& fOut, std::unordered_map<unsigned int, std::unordered_map<unsigned int, char*>>* pNetvars, RecvTable* pRecvTable, unsigned int pOffset) {
    //Iterate through table for more classes and netvars
    for (int i = 0; i < pRecvTable->m_nProps; i++) {
        RecvProp* pRecvProp = &pRecvTable->m_pProps[i];
        if (pRecvProp->m_pDataTable) {

            DumpPointer(fOut, pNetvars, pRecvProp->m_pDataTable, pRecvProp->m_Offset + pOffset);
        }
        if (!(*pNetvars)[hash(pRecvTable->m_pNetTableName)][hash(pRecvProp->m_pVarName)] && pRecvProp->m_Offset) {

            (*pNetvars)[hash(pRecvTable->m_pNetTableName)][hash(pRecvProp->m_pVarName)] = reinterpret_cast<char*>(pRecvProp->m_Offset + pOffset);

            char value[8]; sprintf_s(value, 8, "%03i", strtol(pRecvProp->m_pVarName, 0, 10)); // Eliminates 001, 002, 003 etc
            if(strcmp(pRecvProp->m_pVarName, value)) 
                fOut << std::setw(40) << pRecvTable->m_pNetTableName << " | " << std::setw(40) << pRecvProp->m_pVarName << " | " << std::setw(8) << std::hex << pRecvProp->m_Offset + pOffset << std::endl;
            
        }
    }
}
