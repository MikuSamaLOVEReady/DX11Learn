#include "systemclass.h"

SystemClass::SystemClass():m_Input(nullptr),m_Application(nullptr)
{
}

SystemClass::SystemClass(const SystemClass&other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	// window init
	int screenWidth, screenHeight;
	bool result;

	screenHeight = 0;
	screenWidth = 0;
	InitializeWindows(screenWidth, screenHeight);

	m_Input = new InputClass();
	m_Input->Initialize();

	m_Application = new ApplicationClass();
	result = m_Application->Initialize(screenWidth,screenHeight,m_hwnd);
	if (!result) return false;
	else return true;
}

void SystemClass::Shutdown()
{
	if(m_Application)
	{
		m_Application->ShutDown();
		delete m_Application;
		m_Application = nullptr;
	}

	if(m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));
	done = false;
	while (!done)
	{
		if (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
			{
				m_Input->KeyDown(static_cast<unsigned int>(wparam));
				return 0;
			}
		case WM_KEYUP:
			{
				m_Input->KeyUp(static_cast<unsigned int>(wparam));
				return 0;
			}
		default:
			{
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			}
	}
}

bool SystemClass::Frame()
{
	bool result;
	if (m_Input->IsKeyDown(VK_SPACE))
	{
		return false;
	}

	result = m_Application->Frame();
	if (!result)
	{
		return false;
	}
	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;		/// device Mode 
	int posX, posY;

	ApplicationHandle = this;
	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"Rulomi DX11";


	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;		/// Long Pointer to Function¡£ Register Callback
	wc.cbClsExtra = 0;				/// Count of Bytes Class Extrat
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
	screenWidth = GetSystemMetrics(SM_CXSCREEN);		///full Screen Width
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(screenWidth);	// pels presnet pixels
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(screenHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		posX = posY = 0;	/// position of window top left
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, nullptr);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	ShowCursor(false);
	return;
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
	return;
}

LRESULT WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

