//--------------------------------------------------------------------------------------
// File: DX11Mesh.cpp
//
// Mesh model loading
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

#include "..\\..\\DXUT\\Core\\DXUT.h"
#include "..\\..\\DXUT\\Core\\WICTextureLoader.h"
#include "..\\..\\DXUT\\Optional\\SDKmisc.h"
#include "DX11Mesh.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using namespace DirectX;

//--------------------------------------------------------------------------------------
//
// Constructor
//
//--------------------------------------------------------------------------------------
CDX11Mesh::CDX11Mesh(void)
{
    this->pIndexBuffer = NULL;
    this->pVertexBuffer = NULL;
}

//--------------------------------------------------------------------------------------
//
// Destructor
//
//--------------------------------------------------------------------------------------
CDX11Mesh::~CDX11Mesh(void)
{
}

//--------------------------------------------------------------------------------------
//
// CreateMeshFromFile
//
// Reads a binary file containing the scene model mesh. Creates the DirectX asssets
// necessary for rendering.
//
//--------------------------------------------------------------------------------------
void CDX11Mesh::CreateMeshFromFile(ID3D11Device* pd3dDevice, const WCHAR* strFilename)
{
    OnDestroy();

    ifstream binaryFile(strFilename, ios::binary);

    ReadBinaryFile(strFilename);

    HRESULT hr;
    // Create vertex buffer from vertex data read from file
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.BindFlags        = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.ByteWidth        = (UINT)(sizeof( StandardVertex ) * m_model.vertexBuffer.size());
    bufferDesc.Usage            = D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags   = 0;
    bufferDesc.MiscFlags        = 0;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = &((m_model.vertexBuffer)[0]);
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    // Create the vertex buffer.
    hr = pd3dDevice->CreateBuffer( &bufferDesc, &InitData, &(this->pVertexBuffer) );
    if ( FAILED( hr ) )
    {
        ::MessageBoxA(0, "fail to create vb for obj", "d3dError", 0);
    }

    // Create indices.
    // Fill in a buffer description.
    bufferDesc.Usage           = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth       = (UINT)(sizeof( unsigned int ) * m_model.indexBuffer.size());
    bufferDesc.BindFlags       = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags  = 0;
    bufferDesc.MiscFlags       = 0;

    // Define the resource data.
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = &((m_model.indexBuffer)[0]);
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    // Create the buffer with the device.
    hr = pd3dDevice->CreateBuffer( &bufferDesc, &InitData, &(this->pIndexBuffer) );
    if ( FAILED( hr ) )
    {
        ::MessageBoxA(0, "fail to create vb for obj", "d3dError", 0);
    }

    // set directory for loading textures
    WCHAR wstrOldDir[MAX_PATH] = {0};
    GetCurrentDirectory( MAX_PATH, wstrOldDir );
    // Directory where the mesh was found
    WCHAR strMediaDir[MAX_PATH] = {0};
    wcscpy_s( strMediaDir, MAX_PATH - 1, strFilename );
    WCHAR* pch = wcsrchr( strMediaDir, L'\\' );
    if ( pch )
    {
        *pch = NULL;
    }
    SetCurrentDirectory( strMediaDir );

    // load textures required in .mtl file
    for ( UINT iMaterial = 0; iMaterial < m_model.materials.size(); ++iMaterial )
    {
        Material *pMaterial = &m_model.materials[ iMaterial ];

        if ( pMaterial->strTexture[0] )
        {
            // Avoid loading the same texture twice
            bool bFound = false;

            // loop through previous materials to see if texture is already loaded.
            for ( int x = 0; x < (int)iMaterial; x++ )
            {
                Material* pCur = &m_model.materials[x];
                if ( 0 == wcscmp( pCur->strTexture, pMaterial->strTexture ) )
                {
                    pMaterial->pTextureRV11 = pCur->pTextureRV11;
                    bFound = true;
                    break;
                }
            }

            // Not found, load the texture
            if (!bFound)
            {
                WCHAR wstr[ MAX_PATH ] = {0};
                if ( SUCCEEDED(DXUTFindDXSDKMediaFileCch( wstr, MAX_PATH, pMaterial->strTexture) ) )
                {
                    hr = CreateWICTextureFromFile( pd3dDevice, nullptr, wstr, nullptr, &pMaterial->pTextureRV11 );
                    m_TextureSRVs.push_back(pMaterial->pTextureRV11);
                    if (FAILED(hr)) { ::MessageBoxW(0, L"fail to load material texture", L"D3D11 Error", 0); }

                }
            }
        }
    }

    // Restore the original current directory
    SetCurrentDirectory( wstrOldDir );
}

//--------------------------------------------------------------------------------------
//
// ReadBinaryFile
//
// Reads a binary file containing the scene mesh. The binary file format is an
// internal format (.objb) used only by this sample.
//
//--------------------------------------------------------------------------------------
bool CDX11Mesh::ReadBinaryFile(const WCHAR *filename)
{
    ifstream inFile(filename, ios::binary);

    if ( !inFile.is_open() )
    {
        return false;
    }

    inFile.read((char *)&m_model.bSphere.center, sizeof(DirectX::XMFLOAT3));
    inFile.read((char *)&m_model.bSphere.radius, sizeof(float));

    m_model.subsets.clear();
    size_t size;
    inFile.read((char *)&size, sizeof(size));
    Subset *pSubsets = new Subset[size];
    inFile.read((char *)pSubsets, size * sizeof(Subset));
    for (unsigned i = 0; i < size; i++)
    {
        m_model.subsets.push_back(pSubsets[i]);
    }
    delete [] pSubsets;

    m_model.materials.clear();
    inFile.read((char *)&size, sizeof(size));
    Material *pMaterials = new Material[size];
    inFile.read((char *)pMaterials, size * sizeof(Material));
    for (unsigned i = 0; i < size; i++)
    {
        m_model.materials.push_back(pMaterials[i]);
    }
    delete [] pMaterials;

    m_model.vertexBuffer.clear();
    inFile.read((char *)&size, sizeof(size));
    StandardVertex *pVertices = new StandardVertex[size];
    inFile.read((char *)pVertices, size * sizeof(StandardVertex));
    for (unsigned i = 0; i < size; i++)
    {
        m_model.vertexBuffer.push_back(pVertices[i]);
    }
    delete [] pVertices;

    m_model.indexBuffer.clear();
    inFile.read((char *)&size, sizeof(size));
    UINT *pIndices = new UINT[size];
    inFile.read((char *)pIndices, size * sizeof(UINT));
    for (unsigned i = 0; i < size; i++)
    {
        m_model.indexBuffer.push_back(pIndices[i]);
    }
    delete [] pIndices;

    inFile.close();

    return true;
}

//--------------------------------------------------------------------------------------
//
// OnFrameRender
//
// Draw all subsets of the mesh
// Including:
// - Setting vertex buffer
// - Setting index buffer
// - Set primitive Topology : triangleList
// Not including:
// - Set viewport
// - Set Shader
// - Set input layout (as it require shader signiture)
//--------------------------------------------------------------------------------------
void CDX11Mesh::OnFrameRender(ID3D11DeviceContext* pd3dContext)
{
    UINT strides = sizeof( StandardVertex );
    UINT offsets = 0;
    pd3dContext->IASetVertexBuffers( 0, 1, &pVertexBuffer, &strides, &offsets );
    pd3dContext->IASetIndexBuffer( pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // loop through all subsets
    int numSubsets = (int)m_model.subsets.size();
    for (int i = 0; i < numSubsets; i++)
    {
        int indexCount = m_model.subsets[i].triangleCount*3;
        int startLocation = m_model.subsets[i].startIndex;
        pd3dContext->DrawIndexed( indexCount, startLocation, 0 );
    }
}

//--------------------------------------------------------------------------------------
//
// OnDestroy
//
//--------------------------------------------------------------------------------------
void CDX11Mesh::OnDestroy()
{
    SAFE_RELEASE(pVertexBuffer);
    SAFE_RELEASE(pIndexBuffer);

    for (UINT i = 0; i < m_TextureSRVs.size(); i++)
    {
        ID3D11Resource* pRes = NULL;
        if (m_TextureSRVs[i])
        {
            m_TextureSRVs[i]->GetResource( &pRes );
            SAFE_RELEASE( pRes );
            SAFE_RELEASE( pRes );   // do this twice, because GetResource adds a ref

            SAFE_RELEASE( m_TextureSRVs[i] );
        }
    }

    std::vector<Subset>().swap(m_model.subsets);            // all the sub meshes, each with different material property
    std::vector<Material>().swap(m_model.materials);
    std::vector<ID3D11ShaderResourceView*>().swap( m_TextureSRVs);

    m_model.bSphere.center = XMFLOAT3(0, 0, 0);
    m_model.bSphere.radius = 0;
}

//--------------------------------------------------------------------------------------
//
// Material Class Implementation
//
//--------------------------------------------------------------------------------------
Material::Material()
{
    // set the entire str buffer to zero, so that when materials are written out to binary files,
    // you don't get unitialized memory values making the output non-determistic
    memset( strName, '\0', sizeof( strName ) );
    memset( strTexture, '\0', sizeof( strTexture ) );

    this->vAmbient = XMFLOAT3( 0.1f*(172/255.0f), 0.1f*(120/255.0f), 0.1f*(83/255.0f) );
    this->vDiffuse = XMFLOAT3( 0.3f*(172/255.0f), 0.3f*(120/255.0f), 0.3f*(83/255.0f) );
    this->vSpecular = XMFLOAT3( 0.02f, 0.02f, 0.02f );

    nShininess = 1;
    fAlpha = 1.0;

    bSpecular = false;

    this->pTextureRV11 = 0;
}
Material::~Material()
{}
