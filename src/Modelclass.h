#pragma once
#include <d3d11.h>
#include <fstream>
#include <directxmath.h>
#include "Textureclass.h"

using namespace DirectX;
using namespace  std;
/// data prepare
class Modelclass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Modelclass();
	Modelclass(const Modelclass&);
	~Modelclass();

	//bool Initialize(ID3D11Device*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, const wchar_t*);

	void Shutdown();
	void Render(ID3D11DeviceContext*);	/// REAL: upload data to GPU

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();


private:
	bool InitializeBuffers(ID3D11Device*);		/// set VBO IBO data¡¾FIX Data¡¿
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);	/// to set the vertex buffer and index buffer as active on the input assembler in the GPU.
	bool LoadModel(char*);
	void ReleaseModel();

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const wchar_t*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

	/// Resource
	ModelType* m_model;
	TextureClass* m_Texture;
};

