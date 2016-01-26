//--------------------------------------------------------------------------------------
// File: DX11Mesh.h
//
// Basic mesh class for DX11
// mesh contains a single vertex buffer and index buffer
// the m_subsets is grouped according to material information
// all the triangles with the same material are grouped into one subsets
// This mesh can be created using objImpoart class.
//
//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//--------------------------------------------------------------------------------------

#pragma once
#include <vector>
#include <d3d11.h>

//// the vertex layout for the mesh will be the following:
// This lay out has to match the shader input
//static const D3D11_INPUT_ELEMENT_DESC layout_mesh[] =
//{
//    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//    { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//};

struct StandardVertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 texcoord;
};


struct StrandVertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 tangent;
    DirectX::XMFLOAT4 texcoord;
};

struct DynamicVertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 texmass;
    DirectX::XMFLOAT3 velocity;
};

// define a 3D ellipsoid for collision constrain
// (x/a)^2 + (y/b)^2 + (z/c)^2 = 1
struct Ellipsoid
{
    DirectX::XMFLOAT3 center;
    DirectX::XMFLOAT3 radius;
    DirectX::XMFLOAT3 rotate;
};

class Material
{
public:
    Material();
    ~Material();
    WCHAR   strName[MAX_PATH];

    DirectX::XMFLOAT3 vAmbient;
    DirectX::XMFLOAT3 vDiffuse;
    DirectX::XMFLOAT3 vSpecular;

    int nShininess;
    float fAlpha;

    bool bSpecular;

    WCHAR   strTexture[MAX_PATH];
    ID3D11ShaderResourceView* pTextureRV11; // for d3d11
};

struct Subset
{
    int startIndex;
    int triangleCount;
    int materialID;
};

class BSphere
{
public:
    DirectX::XMFLOAT3 center;
    float radius;
    BSphere()
    {
        center = DirectX::XMFLOAT3(0,0,0);
        radius = 0;
    }

};
struct SceneModel
{
    std::vector<Subset>         subsets;            // all the sub meshes, each with different material property
    std::vector<Material>       materials;
    std::vector<StandardVertex> vertexBuffer;
    std::vector<unsigned int>   indexBuffer;
    BSphere                     bSphere;
};


class CDX11Mesh
{
public:
    CDX11Mesh(void);
    ~CDX11Mesh(void);
    void CreateMeshFromFile(ID3D11Device* pd3dDevice, const WCHAR* strFilename);
    void OnFrameRender(ID3D11DeviceContext* pd3dContext);
    void OnDestroy();
private:
    bool ReadBinaryFile(const WCHAR *fileName);

public:
    std::vector<ID3D11ShaderResourceView*> m_TextureSRVs;
    SceneModel      m_model;
    ID3D11Buffer*   pVertexBuffer;
    ID3D11Buffer*   pIndexBuffer;
};
