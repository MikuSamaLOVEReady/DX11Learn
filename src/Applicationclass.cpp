#include "Applicationclass.h"

ApplicationClass::ApplicationClass()
{
}

ApplicationClass::ApplicationClass(const ApplicationClass&)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int, int, HWND hwnd)
{
	return true;
}

void ApplicationClass::ShutDown()
{
	return;
}

bool ApplicationClass::Frame()
{
	return true;
}

bool ApplicationClass::Render()
{
	return true;
}


