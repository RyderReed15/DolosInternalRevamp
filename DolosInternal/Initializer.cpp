#include "Initializer.h"


bool InitializeCheat(HMODULE hMod) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	if (!InitializeSDK()) {
		return false;
	}
	for (int i = 0; i < 30; i++) {
		Sleep(1000);
	}
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hMod, 0);
	if (!InitializeNetvars(nullptr)) {
		return false;
	}

	if (!InitializeHooks()) {
		UninitializeHooks();
		return false;
	}

	if (!InitializeFonts(hMod)) {

		UninitializeHooks();
		UninitializeFonts();
		return false;
	}

	if (!InitializeGUI()) {

		UninitializeHooks();
		UninitializeFonts();
		return false;
	}
	
	
	return true;
}