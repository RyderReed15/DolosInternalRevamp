#pragma once

#ifndef HOTKEY_MANAGER_H
#define HOTKEY_MANAGER_H


#include "../Hooks.h"
#include <unordered_map>
#include <Windows.h>
#include "winuser.h"

typedef void(__stdcall* fnVoid)();

struct HotKeyStruct {
    void* Function;
    char Key;
    bool Ctrl;
    bool Alt;
    bool Shift;
    bool Win;
};

inline std::unordered_map<int, HotKeyStruct*> mKeys;

bool MakeHotKey(int iHotKeyId, HotKeyStruct* pHotKeyInfo);
bool DestroyHotKey(int iHotKey);
void CallHotKey(int iHotKeyId);


#endif // !HOTKEY_MANAGER_H