//--------------------------------------------------------------------------------------
// File: SDKMesh.h
//
// Disclaimer:  
//   The SDK Mesh format (.sdkmesh) is not a recommended file format for shipping titles.  
//   It was designed to meet the specific needs of the SDK samples.  Any real-world 
//   applications should avoid this file format in favor of a destination format that 
//   meets the specific needs of the application.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=226208
//--------------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------------
// Hard Defines for the various structures
//--------------------------------------------------------------------------------------
#define SDKMESH_FILE_VERSION 101
#define MAX_VERTEX_ELEMENTS 32
#define MAX_VERTEX_STREAMS 16
#define MAX_FRAME_NAME 100
#define MAX_MESH_NAME 100
#define MAX_SUBSET_NAME 100
#define MAX_MATERIAL_NAME 100
#define MAX_PARAMETER_NAME 60
#define MAX_PARAMETER_STRING 192
#define MAX_TEXTURE_NAME MAX_PATH
#define MAX_MATERIAL_PATH MAX_PATH
#define INVALID_FRAME ((UINT)-1)
#define INVALID_MESH ((UINT)-1)
#define INVALID_MATERIAL ((UINT)-1)
#define INVALID_SUBSET ((UINT)-1)
#define INVALID_ANIMATION_DATA ((UINT)-1)

//--------------------------------------------------------------------------------------
// Enumerated Types.  These will have mirrors in both D3D9 and D3D10
//--------------------------------------------------------------------------------------
enum SDKMESH_PRIMITIVE_TYPE
{
    PT_TRIANGLE_LIST = 0,
    PT_TRIANGLE_STRIP,
    PT_LINE_LIST,
    PT_LINE_STRIP,
    PT_POINT_LIST,
    PT_TRIANGLE_LIST_ADJ,
    PT_TRIANGLE_STRIP_ADJ,
    PT_LINE_LIST_ADJ,
    PT_LINE_STRIP_ADJ,
    PT_QUAD_PATCH_LIST,
    PT_TRIANGLE_PATCH_LIST,
};

enum SDKMESH_INDEX_TYPE
{
    IT_16BIT = 0,
    IT_32BIT,
};

enum FRAME_TRANSFORM_TYPE
{
    FTT_RELATIVE = 0,
    FTT_ABSOLUTE,		//This is not currently used but is here to support absolute transformations in the future
};

//--------------------------------------------------------------------------------------
// Structures.  Unions with pointers are forced to 64bit.
//--------------------------------------------------------------------------------------
#pragma pack(push,8)

struct SDKMESH_HEADER
{
    //Basic Info and sizes
    UINT   Version;
    BYTE   IsBigEndian;
    UINT64 HeaderSize;
    UINT64 NonBufferDataSize;
    UINT64 BufferDataSize;

    //Stats
    UINT NumVertexBuffers;
    UINT NumIndexBuffers;
    UINT NumMeshes;
    UINT NumTotalSubsets;
    UINT NumFrames;
    UINT NumMaterials;

    //Offsets to Data
    UINT64 VertexStreamHeadersOffset;
    UINT64 IndexStreamHeadersOffset;
    UINT64 MeshDataOffset;
    UINT64 SubsetDataOffset;
    UINT64 FrameDataOffset;
    UINT64 MaterialDataOffset;
};

struct SDKMESH_VERTEX_BUFFER_HEADER
{
    UINT64 NumVertices;
    UINT64 SizeBytes;
    UINT64 StrideBytes;
    D3DVERTEXELEMENT9 Decl[MAX_VERTEX_ELEMENTS];
    union
    {
        UINT64					DataOffset;				//(This also forces the union to 64bits)
    };
};

struct SDKMESH_INDEX_BUFFER_HEADER
{
    UINT64 NumIndices;
    UINT64 SizeBytes;
    UINT IndexType;
    union
    {
        UINT64					DataOffset;				//(This also forces the union to 64bits)
    };
};

struct SDKMESH_MESH
{
    char Name[MAX_MESH_NAME];
    BYTE NumVertexBuffers;
    UINT VertexBuffers[MAX_VERTEX_STREAMS];
    UINT IndexBuffer;
    UINT NumSubsets;
    UINT NumFrameInfluences; //aka bones

    DirectX::XMFLOAT3 BoundingBoxCenter;
    DirectX::XMFLOAT3 BoundingBoxExtents;

    union
    {
        UINT64 SubsetOffset;	//Offset to list of subsets (This also forces the union to 64bits)
        UINT*  pSubsets;	    //Pointer to list of subsets
    };
    union
    {
        UINT64 FrameInfluenceOffset;  //Offset to list of frame influences (This also forces the union to 64bits)
        UINT*  pFrameInfluences;      //Pointer to list of frame influences
    };
};

struct SDKMESH_SUBSET
{
    char		Name[MAX_SUBSET_NAME];
    UINT		MaterialID;
    UINT		PrimitiveType;
    UINT64		IndexStart;
    UINT64		IndexCount;
    UINT64		VertexStart;
    UINT64		VertexCount;
};

struct SDKMESH_FRAME
{
    char Name[MAX_FRAME_NAME];
    UINT Mesh;
    UINT ParentFrame;
    UINT ChildFrame;
    UINT SiblingFrame;
    DirectX::XMFLOAT4X4 Matrix;
    UINT AnimationDataIndex;		//Used to index which set of keyframes transforms this frame
};

struct SDKMESH_MATERIAL
{
    char		Name[MAX_MATERIAL_NAME];

    // Use MaterialInstancePath
    char		MaterialInstancePath[MAX_MATERIAL_PATH];

    // Or fall back to d3d8-type materials
    char		DiffuseTexture[MAX_TEXTURE_NAME];
    char		NormalTexture[MAX_TEXTURE_NAME];
    char		SpecularTexture[MAX_TEXTURE_NAME];

    DirectX::XMFLOAT4 Diffuse;
    DirectX::XMFLOAT4 Ambient;
    DirectX::XMFLOAT4 Specular;
    DirectX::XMFLOAT4 Emissive;
    float		Power;

    union
    {
        UINT64						Force64_1;			//Force the union to 64bits
    };
    union
    {
        UINT64						Force64_2;			//Force the union to 64bits
    };
    union
    {
        UINT64						Force64_3;			//Force the union to 64bits
    };

    union
    {
        UINT64					    Force64_4;			//Force the union to 64bits
    };
    union
    {
        UINT64						Force64_5;		    //Force the union to 64bits
    };
    union
    {
        UINT64						Force64_6;			//Force the union to 64bits
    };

};

struct SDKANIMATION_FILE_HEADER
{
    UINT Version;
    BYTE IsBigEndian;
    UINT FrameTransformType;
    UINT NumFrames;
    UINT NumAnimationKeys;
    UINT AnimationFPS;
    UINT64 AnimationDataSize;
    UINT64 AnimationDataOffset;
};

struct SDKANIMATION_DATA
{
    DirectX::XMFLOAT3 Translation;
    DirectX::XMFLOAT4 Orientation;
    DirectX::XMFLOAT3 Scaling;
};

struct SDKANIMATION_FRAME_DATA
{
    char FrameName[MAX_FRAME_NAME];
    union
    {
        UINT64			   DataOffset;
        SDKANIMATION_DATA* pAnimationData;
    };
};

#pragma pack(pop)

static_assert( sizeof(D3DVERTEXELEMENT9) == 8, "Direct3D9 Decl structure size incorrect" );
static_assert( sizeof(SDKMESH_HEADER)== 104, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKMESH_VERTEX_BUFFER_HEADER) == 288, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKMESH_INDEX_BUFFER_HEADER) == 32, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKMESH_MESH) == 224, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKMESH_SUBSET) == 144, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKMESH_FRAME) == 184, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKMESH_MATERIAL) == 1256, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKANIMATION_FILE_HEADER) == 40, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKANIMATION_DATA) == 40, "SDK Mesh structure size incorrect" );
static_assert( sizeof(SDKANIMATION_FRAME_DATA) == 112, "SDK Mesh structure size incorrect" );
