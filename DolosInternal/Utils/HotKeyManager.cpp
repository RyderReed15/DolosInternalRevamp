#include "HotKeyManager.h"

bool MakeHotKey(int iHotKeyId, HotKeyStruct* pHotKeyInfo) {
    if (!hValveWnd) hValveWnd = FindWindow("Valve001", NULL);
    mKeys[iHotKeyId] = pHotKeyInfo;
    int bModifiers = MOD_NOREPEAT | (pHotKeyInfo->Ctrl * MOD_CONTROL) | (pHotKeyInfo->Alt * MOD_ALT) | (pHotKeyInfo->Shift * MOD_SHIFT) | (pHotKeyInfo->Win * MOD_WIN);
    return RegisterHotKey(hValveWnd, iHotKeyId, bModifiers, pHotKeyInfo->Key);
}
bool DestroyHotKey(int iHotKey) {
    mKeys.erase(iHotKey);
    return UnregisterHotKey(hValveWnd, iHotKey);
    
}
void CallHotKey(int iHotKeyId) {
    if (mKeys[iHotKeyId]->Key) {
        if (hValveWnd == GetFocus()) {
            ((fnVoid)(mKeys[iHotKeyId]->Function))();
        }else{
            SendMessage(GetForegroundWindow(), WM_CHAR, mKeys[iHotKeyId]->Key, 0);
        } 
    }
}