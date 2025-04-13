#include <cwchar>

#include "systemclass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	WCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	wprintf(L"��ǰ����Ŀ¼��%s\n", buffer);

	// Create the system object.
	System = new SystemClass;

	// Initialize and run the system object.
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}