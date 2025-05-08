#pragma once

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include "Inputclass.h"
#include "Applicationclass.h"


class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	//static SystemClass* GetSys();
	void OnMouseMove(WPARAM btnState, int x, int y);
	//LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

protected:
	//static SystemClass* mSys;

private:
	LPCWSTR m_applicationName;		/// const wchar_t* XXX  
	HINSTANCE m_hinstance;			/// 
	HWND m_hwnd;

	POINT mLastMousePos;
	InputClass* m_Input;
	ApplicationClass* m_Application;

};

///CALLBACK
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = nullptr;