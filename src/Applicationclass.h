#pragma once

#include <windows.h>
#include "D3dclass.h"
#include "Cameraclass.h"
#include "Modelclass.h"
#include "Textureshaderclass.h"
#include "SkyboxShader.h"


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

	CameraClass* GetCamera();
 
private:
	bool Render(float);
	//ID3D11Texture2D* CreateCubeMapTexture();
	bool LoadDDS(ID3D11Device*  , const wchar_t*);


private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	Modelclass* m_Model_sun;
	Modelclass* m_Model_earth;
	Modelclass* m_Model_moon;
	Modelclass* m_Model_ship;
	Modelclass* m_Model_SkyBox;


	//ColorShaderClass* m_ColorShader;
	Textureshaderclass* m_TextureShader;
	SkyboxShader* m_SkyBoxShader;
	ID3D11ShaderResourceView* mCubeMapSRV;
	GeometryData skyboxData;

	//LightShaderClass* m_LightShader;
	//LightClass* m_Light;
	float deltaTime;
	LARGE_INTEGER t0, t1, freq;
};

