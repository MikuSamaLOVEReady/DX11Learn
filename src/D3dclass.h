#pragma once

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass& other);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	void BeginScene(float, float, float, float);
	void EndScene();
	/// ells the swap chain to display our 3D scene
	/// once all the drawing has completed at the end
	/// of each frame

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);	/// TODO
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);


	void GetVideoCardInfo(char*, int&);
	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;		
	ID3D11RenderTargetView* m_renderTargetView;	/// renderTarget {RT0,backBuffer}

	ID3D11Texture2D* m_depthStencilBuffer;		/// depth stencil Test
	ID3D11DepthStencilState* m_depthStencilState;	
	ID3D11DepthStencilView* m_depthStencilView;		/// more like a 

	ID3D11RasterizerState* m_rasterState;		/// rasterizer

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;						/// render UI? using Imgui
	D3D11_VIEWPORT m_viewport;
};

