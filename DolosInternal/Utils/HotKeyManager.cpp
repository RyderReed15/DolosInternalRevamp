#include "HotKeyManager.h"

bool MakeHotKey(void* pFunc, int iHotKeyId, int bModifiers, char chKey) {
    
    mKeys[iHotKeyId] = pFunc;
    return RegisterHotKey(FindWindow("Valve001", NULL), iHotKeyId, bModifiers, chKey);;
}
bool DestroyHotKey(int iHotKey) {
    mKeys.erase(iHotKey);
    return UnregisterHotKey(FindWindow("Valve001", NULL), iHotKey);
    
}
void CallHotKey(int iHotKeyId) {
    if (mKeys[iHotKeyId]) {
        //Send keys if not focused
        ((fnVoid)(mKeys[iHotKeyId]))();
    }
}