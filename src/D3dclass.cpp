#include "D3dclass.h"

#include <iostream>
#include <ostream>

D3DClass::D3DClass():m_swapChain(nullptr),
                     m_device(nullptr),m_deviceContext(nullptr),
                     m_renderTargetView(nullptr),m_depthStencilBuffer(nullptr),m_depthStencilState(nullptr),
                     m_depthStencilView(nullptr),m_rasterState(nullptr)
{

}

D3DClass::D3DClass(const D3DClass& other)
{
}

D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;			/// GPU INFO
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView, screenAspect; /// for projection Matrix
	m_vsync_enabled = vsync;

	/// 1. Get Refresh Rate
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, &adapterOutput);		/// find monitor
	if (FAILED(result))
	{
		return false;
	}

	/// GPU output
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		return false;
	}
	displayModeList = new DXGI_MODE_DESC[numModes];		/// allocate Size
	if (!displayModeList)
	{
		return false;
	}
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	for ( i = 0 ; i<numModes ;++i)
	{
		if ( displayModeList[i].Width == static_cast<unsigned int>(screenWidth) 
			&& displayModeList[i].Height == static_cast<unsigned int>(screenHeight))
		{
			numerator = displayModeList[i].RefreshRate.Numerator;			/// 60000
			denominator = displayModeList[i].RefreshRate.Denominator;		/// 1001
		}
	}

	/// GPU INFO
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}
	m_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, 
		adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	/// 2. DX Init set swapChain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}else
	{
		swapChainDesc.Windowed = true;
	}

	/// legacy draw line by line or draw jump line
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	/// 2. DX create devcice | swapChain | device context
	//	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
	//	&featureLevel, 1, D3D11_SDK_VERSION,
	//	&swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 
		0, &featureLevel, 1, 
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

	if (FAILED(result)) {
		std::cout << "Failed to create device and swap chain. HRESULT: " << std::hex << result << std::endl;
	}

	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBufferPtr));
	if (FAILED(result))
	{
		return false;
	}

	/// backbuffer --> RenderTargetView
	result = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = nullptr;

	/// depthbuffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;					/// can not access by CPU
	depthBufferDesc.MiscFlags = 0;						// extra function
	result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	/// Depth-stencil buffer£º describe whether use depth or not £¬ how £¬rules
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;				/// how to operate depth buffer
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;							/// current less write to depth buffer

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;									/// 8bits
	depthStencilDesc.StencilWriteMask = 0xFF;

	/// front-facing pixels
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;			/// stencil Pass depth fail
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;				/// stencil always pass

	/// back-face pixels
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;			///  ???useage?
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	/// Create depth & stencil state
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);		/// set to context

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	///3.  DX-rasterizer
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;						/// BACK feature
	rasterDesc.DepthBias = 0;									/// for shadow
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;		
	rasterDesc.SlopeScaledDepthBias = 0.0f;						/// 	

	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->RSSetState(m_rasterState);

	///4. DX-viewport 
	m_viewport.Width = static_cast<float>(screenWidth);
	m_viewport.Height = static_cast<float>(screenHeight);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_deviceContext->RSSetViewports(1, &m_viewport);

	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	m_worldMatrix = XMMatrixIdentity()*0.1f;
	m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), screenNear, screenDepth);
	return true;
}

void D3DClass::Shutdown()
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, nullptr);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	return;
}

/// clear color | stencil | depth
void D3DClass::BeginScene(float red , float green , float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene()
{
	if (m_vsync_enabled)
	{
		m_swapChain->Present(1,0);		/// set syncInterval -> syc to fresh rate
	}else
	{
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3DClass::SetBackBufferRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	return;
}

void D3DClass::ResetViewport()
{
	m_deviceContext->RSSetViewports(1, &m_viewport);
	return;
}




