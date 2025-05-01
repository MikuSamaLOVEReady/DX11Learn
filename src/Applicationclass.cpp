#include "Applicationclass.h"

ApplicationClass::ApplicationClass():m_Direct3D(nullptr),m_Camera(nullptr),m_Model(nullptr), m_TextureShader(nullptr)
{
}

ApplicationClass::ApplicationClass(const ApplicationClass&)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char modelFilename[128];
	char textureFilename[128];
	bool result;

	/// Graphics BackEnd
	m_Direct3D = new D3DClass();
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not init Dirct3D", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -145.0f);

	//strcpy_s(modelFilename, "./src/Resource/sphere.txt");
	strcpy_s(modelFilename, "./src/Resource/Sun.txt");
	strcpy_s(textureFilename, "./src/Resource/Sun_2k.tga");

	m_Model = new Modelclass;
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 
		modelFilename , textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	/*
	m_ColorShader = new ColorShaderClass;
	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}*/

	m_TextureShader = new Textureshaderclass();
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown()
{
	/*
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}*/

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

bool ApplicationClass::Frame()
{
	bool result;
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();				/// update camera data
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	
	m_Model->Render(m_Direct3D->GetDeviceContext());	/// set VBO & IBO for context

	/*
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(),			
		m_Model->GetIndexCount(),worldMatrix, viewMatrix, projectionMatrix);
	*/

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());

	if (!result)
	{
		return false;
	}

	m_Direct3D->EndScene();
	return true;
}


