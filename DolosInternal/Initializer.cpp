#include "Initializer.h"


HotKeyStruct EndThread = { UninitializeCheat , VK_INSERT, false, false, false, false };
HMODULE hModule;
FILE* fConsole;

bool StartCheat(HMODULE hMod) {
	// Hook WndProc and send it a message letting it know its been hooked so that initialization can be done from a game thread
	oWndProc = SetWindowLongPtrW(FindWindow("Valve001", NULL), GWLP_WNDPROC, LONG_PTR(&hkInitWndProc));

	SendMessage(FindWindow("Valve001", NULL), 0x9999, 0, (LONG_PTR)hMod);

	hModule = hMod;

	return true;
}

bool InitializeCheat(HMODULE hMod) {
	AllocConsole();
	freopen_s(&fConsole, "CONOUT$", "w", stdout);

	if (!InitializeSDK()) {
		return false;
	}
	std::cout << "SDK Initialized" << std::endl;
	
	if (!InitializeNetvars(g_pBaseClient->GetAllClasses())) {
		return false;
	}
	std::cout << "Netvars Initialized" << std::endl;
	if (!InitializeConfig()) {
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

	if (!SkinChanger::InitializeSkinChanger()) {

		UninitializeGUI();
		UninitializeFonts();
		UninitializeHooks();
		return false;
	}

	std::cout << "SkinChanger Initialized" << std::endl;

	return true;
}

bool UninitializeCheat() {
	UninitializeHooks();

	Sleep(100);

	UninitializeFonts();
	UninitializeGUI();

	UninitializeConfig();
	fclose(fConsole);
	FreeConsole();
	return CloseHandle(CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(CloseCheat), hModule, 0, nullptr));
}

bool CloseCheat(HMODULE hMod) {
	Sleep(100);
	FreeLibraryAndExitThread(hMod, 0);
	return true;
}

LRESULT hkInitWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == 0x9999) {
		InitializeCheat((HMODULE)lParam);
		MakeHotKey(0x1000, &EndThread);
		// Pass hook along to main hook function

		SetWindowLongPtrW(FindWindow("Valve001", NULL), GWLP_WNDPROC, (LONG_PTR)hkWndProc);
	}
	return CallWindowProc((WNDPROC)oWndProc, hWnd, uMsg, wParam, lParam);
}