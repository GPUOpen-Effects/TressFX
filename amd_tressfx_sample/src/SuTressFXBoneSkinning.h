// ----------------------------------------------------------------------------
// Compute-based skinning.
// ----------------------------------------------------------------------------
//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef TRESSFXBONESKINNING_H_
#define TRESSFXBONESKINNING_H_

// Sushi
#include "SuAnimatedModel.h"
#include "SuEffectManager.h"
#include "SuGPUResourceManager.h"

// TressFX interfaces
#include "SushiGPUInterface.h"
#include "TressFXSDFInputMeshInterface.h"

// TressFX
#include "TressFXAsset.h"
#include "TressFXCommon.h"

class SuTressFXBoneSkinning : public TressFXSDFInputMeshInterface
{
public:
    SuTressFXBoneSkinning();
    virtual ~SuTressFXBoneSkinning();

    // Initialize effects and buffers.
    void Initialize(EI_Device* pDevice, EI_CommandContextRef commandContext, EI_StringHash name);

    // Destroy resources
    void Destroy(EI_Device* pDevice);

    // Draw the mesh for debug purpose
    void DrawMesh(EI_CommandContextRef commandContext);

    // Update and animate the mesh
    void Update(EI_CommandContextRef commandContext, SuTime fTime);

    // Set the associated Sushi model.
    void SetModel(const SuString& modelname);

    inline SuAnimatedModel* GetModel() { return m_pModel; }

    bool LoadTressFXCollisionMeshData(TressFXSkeletonInterface* skeletonInterface,
                                      const char*               filePath);

    // TressFXSDFInputMeshInterface
    virtual EI_StructuredBuffer& GetMeshBuffer() { return *m_CollMeshVertPositionsUAV; }
    virtual EI_StructuredBuffer&  GetTrimeshVertexIndicesBuffer()
    {
        return *m_TrimeshVertexIndicesSRV;
    }
    virtual int  GetNumMeshVertices() { return m_NumVertices; }
    virtual int  GetNumMeshTriangle() { return m_NumTriangles; }
    virtual void GetBoundingBox(AMD::tressfx_vec3& min, AMD::tressfx_vec3& max);
    virtual size_t GetSizeOfMeshElement()
    {
        return 3 * sizeof(float) + 3 * sizeof(float);
    }  // position + normal

private:
    SuArray<AMD::TRESSFX::float3> m_pTempVertices;
    SuArray<AMD::TRESSFX::float3> m_pTempNormals;
    SuArray<int>                  m_pTempIndices;

    AMD::TressFXBoneSkinningData* boneSkinningData;

    // shader for rendering
    SuEffectPtr m_pRenderEffect;

    // compute shader for bone skinning
    SuEffectPtr m_pComputeEffectSkinning;

    // color to render the mesh for debug purpose
    SuVector3 m_MeshColor;

    // Stats for mesh topology
    uint32 m_NumVertices;
    uint32 m_NumTriangles;  // the size of indices buffer is 3 time of this.

    // UAV
    EI_StructuredBuffer* m_CollMeshVertPositionsUAV;

    // SRV
    EI_StructuredBuffer* m_TrimeshVertexIndicesSRV;
    EI_StructuredBuffer* m_BoneSkinningDataSRV;
    EI_StructuredBuffer* m_InitialVertexPositionsSRV;

    // Binding
    EI_BindSet* m_pBindSet;

    // GPU buffers
    SuGPUIndexBufferPtr  m_pIndexBuffer;

    // animated model associated to this hair.
    SuAnimatedModel* m_pModel;
};

#endif  // TRESSFXBONESKINNING_H_
