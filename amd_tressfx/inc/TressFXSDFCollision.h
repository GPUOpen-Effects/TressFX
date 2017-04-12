// ----------------------------------------------------------------------------
// This wraps a single signed distance field, used for TressFX collision.
// it provides the interface to generate the signed distance field, as well
// as apply that signed distance field to a TressFX simulation.
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

#ifndef TRESSFXSDFCOLLIION_H_
#define TRESSFXSDFCOLLIION_H_

#include "TressFXCommon.h"
#include "TressFXHairObject.h"

class TressFXHairObject;
class AMD::TressFXAsset;
class TressFXSDFInputMeshInterface;

using namespace AMD::TRESSFX;

class TressFXSDFCollisionSystem
{
public:
    void Initialize(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
    {
        m_InitializeSignedDistanceFieldPSO = EI_CreateComputeShaderPSO(pDevice, layoutManager, TRESSFX_STRING_HASH("InitializeSignedDistanceField"));
        m_ConstructSignedDistanceFieldPSO = EI_CreateComputeShaderPSO(pDevice, layoutManager, TRESSFX_STRING_HASH("ConstructSignedDistanceField"));
        m_FinalizeSignedDistanceFieldPSO = EI_CreateComputeShaderPSO(pDevice, layoutManager, TRESSFX_STRING_HASH("FinalizeSignedDistanceField"));
        m_CollideHairVerticesWithSdfPSO = EI_CreateComputeShaderPSO(pDevice, layoutManager, TRESSFX_STRING_HASH("CollideHairVerticesWithSdf_forward"));
    }

    void Shutdown(EI_Device* pDevice)
    {
        EI_DestroyPSO(pDevice, m_InitializeSignedDistanceFieldPSO);
        EI_DestroyPSO(pDevice, m_ConstructSignedDistanceFieldPSO);
        EI_DestroyPSO(pDevice, m_FinalizeSignedDistanceFieldPSO);
        EI_DestroyPSO(pDevice, m_CollideHairVerticesWithSdfPSO);
    }

    EI_PSO* m_InitializeSignedDistanceFieldPSO;
    EI_PSO* m_ConstructSignedDistanceFieldPSO;
    EI_PSO* m_FinalizeSignedDistanceFieldPSO;
    EI_PSO* m_CollideHairVerticesWithSdfPSO;
};

class TressFXSDFCollision : private TressFXNonCopyable
{
public:
    TressFXSDFCollision();
    ~TressFXSDFCollision();  // Don't need to be virtual. Just don't make a derived class inherited
                             // from this one.

    // Initialize effects and buffers. It should be called after calling LoadCollisionMesheData
    // function.
    void Initialize(EI_Device*              pDevice,
                    TressFXSDFInputMeshInterface* pCollMesh,
                    EI_StringHash modelName);

    // Update and animate the collision mesh
    void Update(EI_CommandContextRef commandContext, TressFXSDFCollisionSystem& system);

    // Run collision checking and response with hair
    void CollideWithHair(EI_CommandContextRef          commandContext,
                         TressFXSDFCollisionSystem& system,
                         TressFXHairObject&         hairObject);

    void SetSDFCollisionMargin(float collisionMargin) { m_CollisionMargin = collisionMargin; }
    float                            GetSDFCollisionMargin() const { return m_CollisionMargin; }


    const EI_StructuredBuffer& GetSDFDataGPUBuffer() const { return *m_SignedDistanceFieldUAV; }
    EI_StructuredBuffer&       GetSDFDataGPUBuffer() { return *m_SignedDistanceFieldUAV; }

    // Grid
    void SetNumCellsInXAxis(int numCellsInX) { m_NumCellsInXAxis = numCellsInX; }
    float                       GetGridCellSize() const { return m_CellSize; }
    AMD::tressfx_vec3           GetGridOrigin() const { return m_Origin; }
    void GetGridNumCells(int& x, int& y, int& z) const
    {
        x = m_NumCellsX;
        y = m_NumCellsY;
        z = m_NumCellsZ;
    }
    int GetGridNumTotalCells() const { return m_NumTotalCells; }

    void Destroy(EI_Device* pDevice);

private:
    AMD::TressFXBoneSkinningData* boneSkinningData;
    TressFXSDFCollisionConstantBuffer m_ConstBuffer;
    TressFXSDFInputMeshInterface* m_pInputCollisionMesh;

    // UAV
    EI_StructuredBuffer* m_SignedDistanceFieldUAV;

    EI_BindSet* m_pSimBindSet;

    // SDF grid
    AMD::tressfx_vec3 m_Origin;
    float             m_CellSize;
    int               m_NumCellsX;
    int               m_NumCellsY;
    int               m_NumCellsZ;
    int               m_NumTotalCells;
    AMD::tressfx_vec3 m_Min;
    AMD::tressfx_vec3 m_Max;
    AMD::tressfx_vec3 m_PaddingBoundary;
    float             m_GridAllocationMutliplier;

    // number of cells in X axis
    int m_NumCellsInXAxis;

    // SDF collision margin.
    float m_CollisionMargin;

    void UpdateSDFGrid(const AMD::tressfx_vec3& tight_bbox_min,
                       const AMD::tressfx_vec3& tight_bbox_max);
};

#endif  // TRESSFXSDFCOLLIION_H_
