#pragma once
#include <d3d11.h>
#include <stdio.h>
#include <string>
#include <vector>

class TextureClass
{
private:
    struct TargaHeader
    {
        unsigned char data1[12];
        unsigned short width;
        unsigned short height;
        unsigned char bpp;
        unsigned char data2;
    };

public:
    TextureClass();
    TextureClass(const TextureClass&);
    ~TextureClass();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
    bool InitializeCubeMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
        const std::vector<std::string>& filenames);

    void Shutdown();

    ID3D11ShaderResourceView* GetTexture();

    int GetWidth();
    int GetHeight();

private:
    bool LoadTarga32Bit(char*);

private:
    unsigned char* m_targaData;     /// raw texture (ON CPU RAM)
    ID3D11Texture2D* m_texture;     /// DX use to render (ON GPU RAM)
    ID3D11ShaderResourceView* m_textureView;    /// be used for shader
    int m_width, m_height;

};

