#include "Initializer.h"


bool InitializeCheat(HMODULE hMod) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	if (!InitializeSDK()) {
		return false;
	}
	std::cout << "SDK Initialized" << std::endl;
	
	if (!InitializeNetvars(nullptr)) {
		return false;
	}
	std::cout << "Netvars Initialized" << std::endl;
	if (!InitializeConfig()) {
		UninitializeGUI();
		UninitializeFonts();
		UninitializeHooks();
		return false;
	}
	std::cout << "Config Initialized" << std::endl;
	
	if (!InitializeFonts(hMod)) {

		UninitializeFonts();
		return false;
	}
	std::cout << "Fonts Initialized" << std::endl;
	if (!InitializeGUI(hMod)) {

		UninitializeGUI();
		UninitializeFonts();
		return false;
	}
	std::cout << "GUI Initialized" << std::endl;
	if (!InitializeHooks()) {

		UninitializeGUI();
		UninitializeFonts();
		UninitializeHooks();
		return false;
	}

	std::cout << "Hooks Initialized" << std::endl;

	
	for (int i = 0; i < 20; i++) {
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
	if (!UninitializeConfig()) {
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