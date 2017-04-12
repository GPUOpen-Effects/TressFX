// ----------------------------------------------------------------------------
// Interface to strands of hair.  Both in terms of rendering, and the data 
// required for simulation.
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

#ifndef TRESSFXHAIROBJECT_H_
#define TRESSFXHAIROBJECT_H_

// AMD
#include "AMD_Types.h"

// TressFX
#include "TressFXAsset.h"
#include "TressFXConstantBuffers.h"
#include "TressFXGPUInterface.h"
#include "TressFXSettings.h"

// Contains the position and tangent data,
// which is used by 3 modules:
// simulation, sigend distance field, and rendering.
// Rendering uses current position and tangent as SRVs in VS.
class PosTanCollection
{
public:
    PosTanCollection();
    ~PosTanCollection();

    void CreateGPUResources(EI_Device* pDevice, int numVertices, EI_StringHash name);
    void UploadGPUData(EI_CommandContextRef commandContext, void* pos, void* tan, int numVertices);

    // The following are roughly called in order.

    // UAV Barriers on buffers used in simulation.
    void UAVBarrier(EI_CommandContextRef commandContext);

    // Transitions to move between simulation (UAV) and rendering (SRV).
    void TransitionSimToRendering(EI_CommandContextRef commandContext);
    void TransitionRenderingToSim(EI_CommandContextRef commandContext);

    EI_BindSet& GetSimBindSet() { return *m_pSimBindSets; }
    EI_BindSet& GetApplySDFBindSet() 
    { 
        return *m_pApplySDFBindSets; 
    }
    EI_BindSet& GetRenderBindSet() { return *m_pRenderBindSets; }

    void Destroy(EI_Device* pDevice);

private:
    EI_StructuredBuffer* m_Positions;
    EI_StructuredBuffer* m_Tangents;
    EI_StructuredBuffer* m_PositionsPrev;
    EI_StructuredBuffer* m_PositionsPrevPrev;

    EI_BindSet* m_pSimBindSets;
    EI_BindSet* m_pApplySDFBindSets;  // updated by
    EI_BindSet* m_pRenderBindSets;    // used for rendering.
};

class TressFXHairObject : private TressFXNonCopyable
{
    friend class TressFXSimulation;

public:
    TressFXHairObject();
    ~TressFXHairObject();

    void Create(AMD::TressFXAsset* asset,
                EI_Device*   pDevice,
        EI_CommandContextRef  commandContext,
                EI_StringHash      name,
                EI_SRV             hairColorTextureSRV);
    void Destroy(EI_Device* pDevice);

    // pBoneMatricesInWS constains array of column major bone matrices in world space.
    void UpdateBoneMatrices(EI_CommandContextRef  commandContext, const float* pBoneMatricesInWS, size_t sizeOfBoneMatrices);

    // update collision capsules
    void UpdateCapsuleCollisions();

    void UpdateSimulationParameters(const TressFXSimulationSettings& parameters);

    // Rendering
    void DrawStrands(EI_CommandContextRef commandContext,
                     EI_PSO&            pso);

    PosTanCollection& GetPosTanCollection() { return mPosTanCollection; }

    // Get hair asset info
    int GetNumTotalHairVertices() { return m_NumTotalVertice; }
    int GetNumTotalHairStrands() { return m_NumTotalStrands; }
    int GetNumVerticesPerStrand() { return m_NumVerticePerStrand; }

private:
    // Turn raw data into GPU resources for rendering.
    void CreateRenderingGPUResources(EI_Device*   pDevice,
                                     AMD::TressFXAsset& asset,
                                     EI_StringHash      name);
    void UploadRenderingGPUResources(EI_CommandContextRef commandContext, AMD::TressFXAsset& asset);

    void PopulateDrawStrandsBindSet(EI_Device* pDevice, EI_SRV hairColorTextureSRV);

    void UpdatePerObjectRenderParams();

    // set wind parameters for simulation
    void SetWind(const AMD::tressfx_vec3& windDir, float windMag, int frame);

    // hair asset information
    int m_NumTotalVertice;
    int m_NumTotalStrands;
    int m_NumVerticePerStrand;
    int m_CPULocalShapeIterations;

    // simulation control params
    TressFXSimulationConstantBuffer mSimCB;

    // position and tangent
    PosTanCollection mPosTanCollection;

    EI_StructuredBuffer* mInitialHairPositionsBuffer;
    EI_StructuredBuffer* mGlobalRotationsBuffer;
    EI_StructuredBuffer* mHairRestLengthSRVBuffer;
    EI_StructuredBuffer* mHairStrandTypeBuffer;
    EI_StructuredBuffer* mHairRefVecsInLocalFrameBuffer;
    EI_StructuredBuffer* mFollowHairRootOffsetBuffer;
    EI_StructuredBuffer* mBoneSkinningDataBuffer;

    // Rendering Inputs

    // SRVs for rendering
    EI_StructuredBuffer* mHairVertexRenderParams;
    EI_StructuredBuffer* mHairTexCoords;

    TressFXRenderingConstantBuffer mRenderCB;

    EI_BindSet* m_pDrawStrandsBindSet;

    // For simulation compute shader
    EI_BindSet* m_pSimBindSet;

    // index buffer
    EI_IndexBuffer* mpIndexBuffer;
    AMD::uint32     mtotalIndices;

    // frame counter for wind effect
    int mSimulationFrame;

    int          m_NumOfVerticesInStrand;
    int          m_NumFollowHairsPerGuideHair;
};

#endif  // TRESSFXHAIROBJECT_H_
