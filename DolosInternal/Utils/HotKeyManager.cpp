#include "HotKeyManager.h"

bool MakeHotKey(int iHotKeyId, HotKeyStruct* pHotKeyInfo) {
    if (!g_hValveWnd) g_hValveWnd = FindWindow("Valve001", NULL);
    mKeys[iHotKeyId] = pHotKeyInfo;
    unsigned int bModifiers = MOD_NOREPEAT | (pHotKeyInfo->Ctrl * MOD_CONTROL) | (pHotKeyInfo->Alt * MOD_ALT) | (pHotKeyInfo->Shift * MOD_SHIFT) | (pHotKeyInfo->Win * MOD_WIN);
    return RegisterHotKey(g_hValveWnd, iHotKeyId, bModifiers, pHotKeyInfo->Key);
}
bool DestroyHotKey(int iHotKey) {
    mKeys.erase(iHotKey);
    return UnregisterHotKey(g_hValveWnd, iHotKey);
    
}
void CallHotKey(int iHotKeyId) {
    if (mKeys[iHotKeyId]->Key) {
        if (g_hValveWnd == GetFocus()) {
            ((fnVoid)(mKeys[iHotKeyId]->Function))();
        }else{
            //Pass hotkey through to another program with the focus
            PostMessage(GetForegroundWindow(), WM_KEYDOWN, mKeys[iHotKeyId]->Key, 0);            
        } 
    }
}