#include "Initializer.h"


bool InitializeCheat(HMODULE hMod) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	if (!InitializeSDK()) {
		return false;
	}
	std::cout << "SDK Initialized" << std::endl;
	
	//fclose(f);
	//FreeConsole();
	//FreeLibraryAndExitThread(hMod, 0);
	if (!InitializeNetvars(nullptr)) {
		return false;
	}
	std::cout << "Netvars Initialized" << std::endl;
	if (!InitializeHooks()) {
		UninitializeHooks();
		return false;
	}

	std::cout << "Hooks Initialized" << std::endl;
	if (!InitializeFonts(hMod)) {

		UninitializeHooks();
		UninitializeFonts();
		return false;
	}
	std::cout << "Fonts Initialized" << std::endl;
	if (!InitializeGUI(hMod)) {

		UninitializeHooks();
		UninitializeFonts();
		return false;
	}
	std::cout << "GUI Initialized" << std::endl;
	for (int i = 0; i < 10; i++) {
		Sleep(1000);
	}
	fclose(f);
	FreeConsole();
	UninitializeCheat(hMod);
	return true;
}

bool UninitializeCheat(HMODULE hMod) {
	if (!UninitializeHooks()) {
		FreeLibraryAndExitThread(hMod, 0);
		return false;
	}

	if (!UninitializeFonts()) {
		FreeLibraryAndExitThread(hMod, 0);
		return false;
	}

	if (!UninitializeGUI()) {
		FreeLibraryAndExitThread(hMod, 0);
		return false;
	}

	FreeLibraryAndExitThread(hMod, 0);
	return true;
}