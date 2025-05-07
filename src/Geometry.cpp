#include "Geometry.h"

GeometryData Geometry::CreateBox(float width, float height, float depth)
{
    using namespace DirectX;

    GeometryData geoData;

    geoData.vertices.resize(24);
    geoData.normals.resize(24);
    geoData.tangents.resize(24);
    geoData.texcoords.resize(24);

    float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

    // ����(+X��)
    geoData.vertices[0] = XMFLOAT3(w2, -h2, -d2);
    geoData.vertices[1] = XMFLOAT3(w2, h2, -d2);
    geoData.vertices[2] = XMFLOAT3(w2, h2, d2); 
    geoData.vertices[3] = XMFLOAT3(w2, -h2, d2);
    // ����(-X��)
    geoData.vertices[4] = XMFLOAT3(-w2, -h2, d2);
    geoData.vertices[5] = XMFLOAT3(-w2, h2, d2);
    geoData.vertices[6] = XMFLOAT3(-w2, h2, -d2);
    geoData.vertices[7] = XMFLOAT3(-w2, -h2, -d2);
    // ����(+Y��)
    geoData.vertices[8] = XMFLOAT3(-w2, h2, -d2);
    geoData.vertices[9] = XMFLOAT3(-w2, h2, d2);
    geoData.vertices[10] = XMFLOAT3(w2, h2, d2);
    geoData.vertices[11] = XMFLOAT3(w2, h2, -d2);
    // ����(-Y��)
    geoData.vertices[12] = XMFLOAT3(w2, -h2, -d2);
    geoData.vertices[13] = XMFLOAT3(w2, -h2, d2);
    geoData.vertices[14] = XMFLOAT3(-w2, -h2, d2);
    geoData.vertices[15] = XMFLOAT3(-w2, -h2, -d2);
    // ����(+Z��)
    geoData.vertices[16] = XMFLOAT3(w2, -h2, d2);
    geoData.vertices[17] = XMFLOAT3(w2, h2, d2);
    geoData.vertices[18] = XMFLOAT3(-w2, h2, d2);
    geoData.vertices[19] = XMFLOAT3(-w2, -h2, d2);
    // ����(-Z��)
    geoData.vertices[20] = XMFLOAT3(-w2, -h2, -d2);
    geoData.vertices[21] = XMFLOAT3(-w2, h2, -d2);
    geoData.vertices[22] = XMFLOAT3(w2, h2, -d2);
    geoData.vertices[23] = XMFLOAT3(w2, -h2, -d2);

    for (size_t i = 0; i < 4; ++i)
    {
        // ����(+X��)
        geoData.normals[i] = XMFLOAT3(1.0f, 0.0f, 0.0f);
        geoData.tangents[i] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
        // ����(-X��)
        geoData.normals[i + 4] = XMFLOAT3(-1.0f, 0.0f, 0.0f);
        geoData.tangents[i + 4] = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
        // ����(+Y��)
        geoData.normals[i + 8] = XMFLOAT3(0.0f, 1.0f, 0.0f);
        geoData.tangents[i + 8] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
        // ����(-Y��)
        geoData.normals[i + 12] = XMFLOAT3(0.0f, -1.0f, 0.0f);
        geoData.tangents[i + 12] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // ����(+Z��)
        geoData.normals[i + 16] = XMFLOAT3(0.0f, 0.0f, 1.0f);
        geoData.tangents[i + 16] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
        // ����(-Z��)
        geoData.normals[i + 20] = XMFLOAT3(0.0f, 0.0f, -1.0f);
        geoData.tangents[i + 20] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    for (size_t i = 0; i < 6; ++i)
    {
        geoData.texcoords[i * 4] = XMFLOAT2(0.0f, 1.0f);
        geoData.texcoords[i * 4 + 1] = XMFLOAT2(0.0f, 0.0f);
        geoData.texcoords[i * 4 + 2] = XMFLOAT2(1.0f, 0.0f);
        geoData.texcoords[i * 4 + 3] = XMFLOAT2(1.0f, 1.0f);
    }

    geoData.indices16.resize(36);

    uint16_t indices[] = {
        0, 1, 2, 2, 3, 0,		// ����(+X��)
        4, 5, 6, 6, 7, 4,		// ����(-X��)
        8, 9, 10, 10, 11, 8,	// ����(+Y��)
        12, 13, 14, 14, 15, 12,	// ����(-Y��)
        16, 17, 18, 18, 19, 16, // ����(+Z��)
        20, 21, 22, 22, 23, 20	// ����(-Z��)
    };
    memcpy_s(geoData.indices16.data(), sizeof indices, indices, sizeof indices);

    return geoData;
}
