#pragma once
#include <d3d11.h>
#include <fstream>
#include <directxmath.h>

using namespace DirectX;
using namespace  std;
/// data prepare
class Modelclass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
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

	bool Initialize(ID3D11Device*);
	//bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);

	void Shutdown();
	void Render(ID3D11DeviceContext*);	/// REAL: upload data to GPU

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);		/// set VBO IBO data¡¾FIX Data¡¿
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);	/// to set the vertex buffer and index buffer as active on the input assembler in the GPU.

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

	/// read in and hold the model data before it is placed in the vertex buffer.
	ModelType* m_model;
};

