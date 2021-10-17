#pragma once

#ifndef HOTKEY_MANAGER_H
#define HOTKEY_MANAGER_H

#include "HookManager.h"
#include <unordered_map>
#include "winuser.h"

typedef void(__stdcall* fnVoid)();

inline std::unordered_map<int, void*> mKeys;

bool MakeHotKey(void* pFunc, int iHotKeyId, int bModifiers, char chKey);
bool DestroyHotKey(int iHotKey);
void CallHotKey(int iHotKeyId);
#endif // !HOTKEY_MANAGER_H
