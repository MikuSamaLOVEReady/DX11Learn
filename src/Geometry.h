#pragma once
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include <functional>


 
struct GeometryData
{
    std::vector<DirectX::XMFLOAT3> vertices;
    std::vector<DirectX::XMFLOAT3> normals;
    std::vector<DirectX::XMFLOAT2> texcoords;
    std::vector<DirectX::XMFLOAT4> tangents;
    std::vector<uint32_t> indices32;
    std::vector<uint16_t> indices16;
};


namespace Geometry
{
    // 创建球体网格数据，levels和slices越大，精度越高。
    //GeometryData CreateSphere(float radius = 1.0f, uint32_t levels = 20, uint32_t slices = 20);

    // 创建立方体网格数据
    GeometryData CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f);
}

