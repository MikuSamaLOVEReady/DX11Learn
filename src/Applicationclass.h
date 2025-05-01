#pragma once

#include <windows.h>

#include "D3dclass.h"
#include "Cameraclass.h"
#include "Modelclass.h"
//#include "Colorshaderclass.h"
#include "Textureshaderclass.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;


class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int ,HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	Modelclass* m_Model;

	//ColorShaderClass* m_ColorShader;
	Textureshaderclass* m_TextureShader;

	//LightShaderClass* m_LightShader;
	//LightClass* m_Light;
	float deltaTime;
	LARGE_INTEGER t0, t1, freq;
};

