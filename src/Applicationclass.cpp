#include "Applicationclass.h"

#include <DirectXTex.h>
#include <vector>

ApplicationClass::ApplicationClass():m_Direct3D(nullptr), m_FirstPersonCam(nullptr),
                                     m_Model_sun(nullptr), m_Model_earth(nullptr), m_Model_ship(nullptr),
                                     m_TextureShader(nullptr), deltaTime(0.0f)
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

	char earthFilename[128];
	char earthTexFilename[128];

	char moonFilename[128];
	char moonTexFilename[128];

	char shipFilename[128];
	char shipTexFilename[128];

	bool result;

	/// Graphics BackEnd
	m_Direct3D = new D3DClass();
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not init Dirct3D", L"Error", MB_OK);
		return false;
	}

	//m_Camera = new CameraClass;
	//m_Camera->SetPosition(0.0f, 0.0f, -245.0f);

	m_FirstPersonCam = new FirstPersonCamera;
	m_ThirdPersonCam = new ThirdPersonCamera;

	m_FirstPersonCam->LookTo(XMFLOAT3(0.0f, 0.0f, -10.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_ThirdPersonCam->SetDistance(20.0f);
	m_ThirdPersonCam->SetDistanceMinMax(10.0f, 90.0f);
	m_ThirdPersonCam->SetRotationX(XM_PIDIV4);

	

	//strcpy_s(modelFilename, "./src/Resource/sphere.txt");
	strcpy_s(modelFilename, "./src/Resource/Sun.txt");
	strcpy_s(textureFilename, "./src/Resource/Sun_2k.tga");
	strcpy_s(earthFilename, "./src/Resource/Earth.txt");
	strcpy_s(earthTexFilename, "./src/Resource/Earth_2K.tga");
	strcpy_s(moonFilename, "./src/Resource/Moon.txt");
	strcpy_s(moonTexFilename, "./src/Resource/Moon_2K.tga");
	strcpy_s(shipFilename, "./src/Resource/fly.txt");
	strcpy_s(shipTexFilename, "./src/Resource/fly_2K.tga");

	m_Model_sun = new Modelclass();
	result = m_Model_sun->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		modelFilename , L"./src/Resource/Sun_2k.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Model_earth = new Modelclass();
	result = m_Model_earth->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		earthFilename, L"./src/Resource/Earth_2K.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Model_moon = new Modelclass();
	result = m_Model_moon->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),
		moonFilename, L"./src/Resource/Moon_2K.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Model_ship = new Modelclass();
	result = m_Model_ship->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 
		shipFilename, L"./src/Resource/fly2_2K.png");
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

	/// cube map
	m_Model_SkyBox = new Modelclass();
	skyboxData = Geometry::CreateBox();
	m_Model_SkyBox->CreateFromGeometry(m_Direct3D->GetDevice(), skyboxData);
	/// load cubemap
	LoadDDS(m_Direct3D->GetDevice(), L"./src/Resource/cubemap3.dds");
	m_SkyBoxShader = new SkyboxShader();
	result = m_SkyBoxShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the SKY shader object.", L"Error", MB_OK);
		return false;
	}


	/// Time
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t0);


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
		m_TextureShader = nullptr;
	}

	if (m_Model_sun)
	{
		m_Model_sun->Shutdown();
		delete m_Model_sun;
		m_Model_sun = nullptr;
	}

 

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}

	return;
}

void ApplicationClass::ProcessKeyboardInput()
{

	if (GetAsyncKeyState('E') & 0x8000)		/// 切换地球轨道
		orbitID = 1;
	if (GetAsyncKeyState('M') & 0x8000)     /// 切换月球轨道
		orbitID = 2;
	if (GetAsyncKeyState('T') & 0x8000)     /// 切换至地月转移轨道
		orbitID = 0;
	if (GetAsyncKeyState('F') & 0x8000)
		SetFPViewMod();
	if (GetAsyncKeyState('G') & 0x8000)
		SetTPViewMod();
}

bool ApplicationClass::Frame()
{
	static float rotation = 0.0f;
	bool result;

	rotation -= 0.0174532925f * 0.25f * 0.25f ;		// pi / 180 * 0.25 degree
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	ProcessKeyboardInput();
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}


Camera* ApplicationClass::GetCamera()
{
	return m_FirstPersonCam;
}

ThirdPersonCamera* ApplicationClass::GetCameraTrd()
{
	return m_ThirdPersonCam;
}

void ApplicationClass::SetFPViewMod()
{
	isFirstPerson = true;
}

void ApplicationClass::SetTPViewMod()
{
	isFirstPerson = false;
}

bool ApplicationClass::LoadDDS(ID3D11Device* device, const wchar_t* filename)
{
	DirectX::ScratchImage image;
	//CreateDDSTextureFromFile
	DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, nullptr, image);
	HRESULT hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &mCubeMapSRV);
	return SUCCEEDED(hr);
}

bool ApplicationClass::Render(float ratation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// deltatime
	QueryPerformanceCounter(&t1);
	float deltaTime = float(t1.QuadPart - t0.QuadPart) / freq.QuadPart;
	t0 = t1;
	//m_Camera->ProcessKeyboardInput(deltaTime);

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	//m_Camera->Render();				/// update camera data
	m_Direct3D->GetWorldMatrix(worldMatrix);
	if (isFirstPerson)
	{
		viewMatrix = m_FirstPersonCam->GetViewMatrixXM();
	}else
	{
		viewMatrix = m_ThirdPersonCam->GetViewMatrixXM();
	}
	//m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);			// TODO：这个本来该是相机操控的

	/// Sun rotate matrix && Translate Matrix  
	XMMATRIX  rotateMatrix_sun = XMMatrixRotationY(ratation);  
	//XMMATRIX  translateMatrix = XMMatrixTranslation(0.f, 0.f, 0.f);
	worldMatrix = XMMatrixMultiply(rotateMatrix_sun, worldMatrix);
	m_Model_sun->Render(m_Direct3D->GetDeviceContext());	/// set VBO & IBO for context
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model_sun->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model_sun->GetTexture());
	if (!result)
	{
		return false;
	}


	/// Render earth
	XMMATRIX scaleMatrix_earth = XMMatrixScaling(0.75f, 0.75f, 0.75f);
	XMMATRIX rotateMatrix_earth = XMMatrixRotationY(ratation);
	XMMATRIX translateMatrix_earth = XMMatrixTranslation(175.0f, 0.0f, 0.0f);  // Build the translation matrix.
	XMMATRIX SRMatrix = XMMatrixMultiply(scaleMatrix_earth, rotateMatrix_earth);
	XMMATRIX orbitRotate_earth = XMMatrixRotationY(ratation);			/// rotate around sun
	XMMATRIX SRTMatrix  = XMMatrixMultiply(SRMatrix, translateMatrix_earth);

	XMMATRIX worldMatrix_earth = XMMatrixMultiply(SRTMatrix, orbitRotate_earth);
	m_Model_earth->Render(m_Direct3D->GetDeviceContext());
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model_earth->GetIndexCount(),
		worldMatrix_earth, viewMatrix, projectionMatrix, m_Model_earth->GetTexture());
	if (!result)
	{
		return false;
	}


	/// Render Moon  local_translation   scale -> self rotate ->  tanslate
	XMMATRIX scaleMatrix_moon = XMMatrixScaling(0.35f, 0.35f, 0.35f);
	XMMATRIX rotateMatrix_moon = XMMatrixRotationY(ratation);  // 月亮自转
	XMMATRIX translateMatrix_moon = XMMatrixTranslation(50.0f, 10.0f, 0.0f); // 月亮轨道相较于地球的距离
	XMMATRIX orbitRotate_moon = XMMatrixRotationY(ratation);    // 月亮公转轨道

	XMMATRIX SRMatrix_moon = XMMatrixMultiply(scaleMatrix_moon, rotateMatrix_moon);
	XMMATRIX SRTMatrix_moon = XMMatrixMultiply(SRMatrix_moon, translateMatrix_moon);
	XMMATRIX localMatrix_moon = XMMatrixMultiply(SRTMatrix_moon, orbitRotate_moon);		/// local坐标变化到地球为原点的坐标系，再执行一次旋转，从而形成轨道
	XMMATRIX worldMatrix_moon = XMMatrixMultiply(localMatrix_moon, worldMatrix_earth);  ///  先转移到地球轨道 -> 世界空间  【为什么这里是local 先 world后呢】

	m_Model_moon->Render(m_Direct3D->GetDeviceContext());
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model_moon->GetIndexCount(),
		worldMatrix_moon, viewMatrix, projectionMatrix, m_Model_moon->GetTexture());
	if (!result)
	{
		return false;
	}


	/// Render Ship
	XMMATRIX scaleMatrix_ship = XMMatrixScaling(0.05f, 0.05f, 0.05f);				// 飞船较小
	XMMATRIX rotateMatrix_ship = XMMatrixIdentity();
	/// 飞船在月球轨道运行
	XMMATRIX translateMatrix_moonShip = XMMatrixTranslation(50.0f, 15.0f, 10.0f); // 月亮轨道相较于地球的距离
	XMMATRIX toMoonOrbitMatrix = XMMatrixMultiply(scaleMatrix_ship, translateMatrix_moonShip);		   /// 缩放+转移到月球轨道(稍微偏移一点)
	toMoonOrbitMatrix = XMMatrixMultiply(toMoonOrbitMatrix, orbitRotate_moon);					       /// 绕地球旋转、形成轨道
	toMoonOrbitMatrix = XMMatrixMultiply(toMoonOrbitMatrix, worldMatrix_earth);					       /// 最重要的一点，转移到相对于地球的坐标系

	/// 飞船在地球轨道运行
	XMMATRIX translateMatrix_earthShip = XMMatrixTranslation(175.0f, 15.0f, 25.0f);;
	XMMATRIX toEarthOrbitMatrix = XMMatrixMultiply(scaleMatrix_ship, translateMatrix_earthShip);	   /// 转移到地球位置
	XMMATRIX orbitRotate_earthShipSpeed = XMMatrixRotationY(ratation*15);						   /// 轨道提速
	toEarthOrbitMatrix = XMMatrixMultiply(toEarthOrbitMatrix, orbitRotate_earthShipSpeed);			   /// 进行绕太阳旋转，模拟进入地球轨道

	/// 飞船在地月轨道运行
	float theta = fmod(ratation * 35.f, 2 * XM_PI); // 控制飞船的移动速度
	float a = 35.0f; // 半长轴
	float b = 135.0f; // 半短轴
	float x = a * cos(theta);
	float y = 10.0f;   // 假设轨道在XY平面上，y=0
	float z = b * sin(theta);
	XMMATRIX translateMatrix_EarthMoonShip = XMMatrixTranslation(x, y, z);
	XMMATRIX SRTMatrix_EarthMoonShip = XMMatrixMultiply(scaleMatrix_ship, translateMatrix_EarthMoonShip);
	SRTMatrix_EarthMoonShip = XMMatrixMultiply(SRTMatrix_EarthMoonShip, worldMatrix_earth);

	// 通过按键切换轨道
	XMMATRIX cur_Matrix = SRTMatrix_EarthMoonShip;
	m_Model_ship->Render(m_Direct3D->GetDeviceContext());
	switch (orbitID)
	{
		case 0:
			cur_Matrix = SRTMatrix_EarthMoonShip;
			break;
		case 1:
			cur_Matrix = toEarthOrbitMatrix;
			break;
		case 2:
			cur_Matrix = toMoonOrbitMatrix;
			break;
		default:
			break;
	}

	/// 相机始终跟随飞船
	XMVECTOR worldPosVec = cur_Matrix.r[3];
	float worldX = XMVectorGetX(worldPosVec);
	float worldY = XMVectorGetY(worldPosVec);
	float worldZ = XMVectorGetZ(worldPosVec);
	//m_Camera->SetPosition(worldX, worldY+10, worldZ);


	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model_ship->GetIndexCount(),
		cur_Matrix, viewMatrix, projectionMatrix, m_Model_ship->GetTexture());
	if (!result)
	{
		return false;
	}

	/// SkyBOX model render
	m_Model_SkyBox->RenderGeo(m_Direct3D->GetDeviceContext());	/// primative + vertexBuffer + indexbuffer
	/// layout + Drawindex 放在shader中执行
	//XMMATRIX MVPMatrix = XMMatrixMultiply(toEarthOrbitMatrix, viewMatrix );
	//MVPMatrix = XMMatrixMultiply(MVPMatrix, projectionMatrix);
	XMMATRIX V = viewMatrix;
	V.r[3] = g_XMIdentityR3;
	XMMATRIX MVPMatrix = XMMatrixMultiply(V, projectionMatrix);
	MVPMatrix = XMMatrixTranspose(MVPMatrix);


	result = m_SkyBoxShader->Render(m_Direct3D->GetDeviceContext(), m_Model_SkyBox->GetIndexCount(),
		MVPMatrix, mCubeMapSRV);
	if (!result)
	{
		return false;
	}

	/// 第一人称在飞船头部一点点的位置
	m_FirstPersonCam->SetPosition({ worldX , worldY , worldZ+10 });
	m_ThirdPersonCam->SetPosition({ worldX, worldY, worldZ });
	m_ThirdPersonCam->SetTarget({ worldX, worldY, worldZ });

	m_Direct3D->EndScene();
	return true;
}
 

