#include "Modelclass.h"

Modelclass::Modelclass():m_vertexBuffer(nullptr) , m_indexBuffer(nullptr), m_model(nullptr),m_Texture(nullptr)
{
	
}

Modelclass::Modelclass(const Modelclass& other)
{
}

Modelclass::~Modelclass()
{
}

bool Modelclass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	char* modelFilename, char* textureFilename)
{
	bool result;

 	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	

	return true;
}


/*
bool Modelclass::Initialize(ID3D11Device* device)
{
	bool result;
	result = InitializeBuffers(device);
	if (!result) 
	{
		return false;
	}
	return true;
}*/

void Modelclass::Shutdown()
{
	ReleaseModel();
	ShutdownBuffers();
	ReleaseTexture();
	return;
}

void Modelclass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
	return;
}

int Modelclass::GetIndexCount()
{
	return  m_indexCount;
}

ID3D11ShaderResourceView* Modelclass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Modelclass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC  vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;					/// real data
	HRESULT result;

	//m_vertexCount = 3;
	//m_indexCount = 3;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex&&index array
	for ( int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}


	/// Create Vertex Buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	/// Create Index Buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;
	return true;
}

void Modelclass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	return;
}

void Modelclass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Modelclass::LoadModel(char* filename)
{
	ifstream fin;
	char input;


	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}
	/// read vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin >> m_vertexCount;
	m_indexCount = m_vertexCount;


	m_model = new ModelType[m_vertexCount];

	///read position , texcoord , normal vector
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (int i=0 ; i<m_vertexCount ;++i)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();
	return true;
}

void Modelclass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

bool Modelclass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	m_Texture = new TextureClass();

	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Modelclass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}
	return;
}
















