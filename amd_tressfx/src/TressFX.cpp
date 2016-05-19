//--------------------------------------------------------------------------------------
// File: TresssFX.cpp
//
// Hair asset structure
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

#if AMD_TRESSFX_COMPILE_DYNAMIC_LIB
#define AMD_DLL_EXPORTS
#endif
#include "TressFXOpaque.h"

static AMD::TressFX_OpaqueDesc gTressFXOpaqueDesc;

int g_TressFXNumVerticesPerStrand = 16; // must be 2^n (n is integer and greater and 3) and less than or equal to THREAD_GROUP_SIZE. i.e. 8, 16, 32 or 64

namespace AMD
{
//--------------------------------------------------------------------------------------
//
// TressFX_GetVersion
//
// Gets the TressFX library version number.
// This can be called before TressFX_Initialize.
//
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_GetVersion(uint* major, uint* minor, uint* patch)
{
    if (major == NULL || minor == NULL || patch == NULL)
    {
        return TRESSFX_RETURN_CODE_INVALID_POINTER;
    }

    *major = AMD_TRESSFX_VERSION_MAJOR;
    *minor = AMD_TRESSFX_VERSION_MINOR;
    *patch = AMD_TRESSFX_VERSION_PATCH;

    return TRESSFX_RETURN_CODE_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// TressFX_Initialize
//
// Initializes TressFX rendering and simulation. Allocates the needed resources.
// The opaque initialization happens only once for the lifetime of the program, and
// descriptors share the same opaque pointer.
//
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_Initialize(TressFX_Desc & desc)
{
    desc.pOpaque = &gTressFXOpaqueDesc;
    desc.pOpaque->Initialize(desc);

    desc.pTressFXMesh = NULL;
    desc.groupID = 0;
    desc.tressFXHair.pHair = NULL;
    desc.tressFXHair.size = 0;

    return TRESSFX_RETURN_CODE_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// TressFX_LoadRawAsset
//
// Reads the contents of a binary .tfx file. The data in pRawHairBlob is what was
// read from the .tfx file without any processing. Data from multiple .tfx files
// can be loaded by calling TressFXLoadRawAsset multiple times.
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_LoadRawAsset(TressFX_Desc & desc,   const TressFX_GuideFollowParams& guideFollowParams, TressFX_HairBlob *pRawHairBlob)
{
    bool success = desc.pOpaque->LoadAppendAsset(pRawHairBlob, guideFollowParams, desc.groupID);
    if (success)
    {
        desc.groupID++;
    }
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

//--------------------------------------------------------------------------------------
//
// TressFX_LoadProcessedAsset
//
// Loads a processed hair asset. The data in pHairBlob was created by calling
// TressFX_CreateProcessedAsset(). Since it's a preprocessed asset it loads faster
// than a raw asset.
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_LoadProcessedAsset(TressFX_Desc & desc,
                                                            TressFX_HairBlob *pHairBlob, TressFX_SceneMesh *sceneMesh,
                                                            ID3D11ShaderResourceView *pTressFXTexture)
{
    TressFXMesh *pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
    if (pTressFXMesh != NULL)
    {
       delete pTressFXMesh;
    }
    pTressFXMesh = new TressFXMesh();
    memcpy(desc.tressFXHair.pHair, pHairBlob->pHair, pHairBlob->size);
    desc.tressFXHair.size = pHairBlob->size;
    pTressFXMesh->OnCreate(desc.pd3dDevice, &desc.tressFXHair, sceneMesh, pTressFXTexture);
    desc.numTotalHairStrands = pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
    desc.numTotalHairVertices = pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
    desc.pTressFXMesh = (void *)pTressFXMesh;

    return TRESSFX_RETURN_CODE_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// TressFX_CreateProcessedAsset
//
// Creates a processed hair asset from previous loads of raw hair asset data.
// A pointer to the processed hair asset is returned in pHairBlob. The caller of this
// function should copy the contents of pHairBlob instead of using it directly, since
// the memory for it will be released when a new file is loaded.
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_CreateProcessedAsset(TressFX_Desc & desc,
                                                  TressFX_HairBlob **ppHairBlob, TressFX_SceneMesh *sceneMesh,
                                                  ID3D11ShaderResourceView *hairTexture)
{
    bool success = desc.pOpaque->CreateProcessedAsset(desc, ppHairBlob, sceneMesh, hairTexture);
    desc.groupID = 0;
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

//--------------------------------------------------------------------------------------
//
// TressFX_Simulate
//
// Runs the hair simulation
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_Simulate(TressFX_Desc & desc, float elapsedTime)
{
    bool success = desc.pOpaque->Simulate(desc, elapsedTime);
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

//--------------------------------------------------------------------------------------
//
// TressFX_Begin
//
// Indicates that hair simulation and rendering will begin
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_Begin(TressFX_Desc & desc)
{
    bool success = desc.pOpaque->Begin(desc);
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

//--------------------------------------------------------------------------------------
//
// TressFX_RenderShadowMap
//
// Render the hair into a shadow map. Returns the shadow map in the descriptor
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_RenderShadowMap(TressFX_Desc & desc)
{
    bool success = desc.pOpaque->RenderShadowMap(desc);
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

//--------------------------------------------------------------------------------------
//
// TressFX_Render
//
// Render the hair model. Multiple render calls can be made for different hair models
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_Render(TressFX_Desc & desc)
{
    bool success = desc.pOpaque->RenderHair(desc);
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}


//--------------------------------------------------------------------------------------
//
// TressFX_End
//
// Indicates that hair rendering is finished. Final post-process rendering can be
// done at this point
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_End(TressFX_Desc & desc)
{
    bool success = desc.pOpaque->End(desc);
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

//--------------------------------------------------------------------------------------
//
// TressFX_Resize
//
// Called when the back buffer is resized so internal buffers can also be resized
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_Resize(TressFX_Desc & desc)
{
    bool success = desc.pOpaque->Resize(desc);
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

//--------------------------------------------------------------------------------------
//
// TressFX_Release
//
// Releases any memory allocated by the library
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_Release(TressFX_Desc & desc)
{
    if ( !desc.pOpaque )
    {
        return TRESSFX_RETURN_CODE_SUCCESS;
    }

    desc.pOpaque->Release();
    TressFXMesh *pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
    pTressFXMesh->OnDestroy();
    delete pTressFXMesh;
    delete desc.tressFXHair.pHair;
    desc.tressFXHair.pHair = NULL;
    desc.tressFXHair.size = 0;
    return TRESSFX_RETURN_CODE_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// TressFX_GenerateTransforms
//
// Calculates the transformations for each strand.
// Used for fur skinning.
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_GenerateTransforms(TressFX_Desc & desc, TressFX_SceneMesh &sceneMesh)
{
    bool success = desc.pOpaque->GenerateTransforms(desc, sceneMesh);
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

//--------------------------------------------------------------------------------------
//
// TressFX_ApplyRigidTransforms
//
// Apply skin transforms to all hair vertices so that hair would be transformed in a rigid motion.
// Used for fur skinning.
//
//--------------------------------------------------------------------------------------
TRESSFX_RETURN_CODE AMD_TRESSFX_DLL_API TressFX_ApplyRigidTransforms(TressFX_Desc & desc)
{
    bool success = desc.pOpaque->ApplyRigidTransforms(desc);
    return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
}

} // namespace AMD
