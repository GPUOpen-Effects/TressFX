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

#ifndef TRESSFXSDFMARCHINGCUBES_H_
#define TRESSFXSDFMARCHINGCUBES_H_

#include "TressFXCommon.h"
#include "TressFXGPUInterface.h"

// Sushi
#include "SuBatchLineRenderer.h"
#include "SuEffectManager.h"
#include "SuGPUResourceManager.h"

class TressFXSDFCollision;


class SuTressFXSDFMarchingCubes : private TressFXNonCopyable
{
public:
    SuTressFXSDFMarchingCubes();
    ~SuTressFXSDFMarchingCubes();  // Don't need to be virtual. Just don't make a derived class
                                   // inherited from this one.

    void Initialize(EI_StringHash name);
    void Destroy();

    // Draw the SDF using marching cubes for debug purpose
    void Draw();

    // Draw the grid
    void DrawGrid();

    // Update mesh by running marching cubes
    void Update(EI_CommandContextRef commandContext);

    void SetSDF(TressFXSDFCollision* SDF)
    {
        SU_ASSERT(SDF);
        m_pSDF = SDF;
    }

    // Setting the SDF ISO level for drawing.
    void SetSDFIsoLevel(float isoLevel) { m_SDFIsoLevel = isoLevel; }

private:
    // SDF grid
    SuVector3 m_Origin;
    float     m_CellSize;
    int       m_NumCellsX;
    int       m_NumCellsY;
    int       m_NumCellsZ;
    int       m_NumTotalCells;

    TressFXSDFCollision* m_pSDF;

    // For drawing lines
    SuBatchLineRenderer m_LineRenderer;

    struct VertexData
    {
        float position[3];
        float normal[3];
    };

    // SDF ISO level. This value will be multiplied with the cell size before be passed to the
    // compute shader.
    float m_SDFIsoLevel;

    const int m_MaxMarchingCubesVertices;
    int       m_NumMCVertices;

    EI_StructuredBuffer* m_MarchingCubesTriangleVerticesUAV;
    EI_StructuredBuffer* m_NumMarchingCubesVerticesUAV;

    EI_StructuredBuffer* m_MarchingCubesEdgeTableSRV;
    EI_StructuredBuffer* m_MarchingCubesTriangleTableSRV;

    // compute shader
    SuEffectPtr m_pComputeEffect;

    // shader for rendering
    SuEffectPtr m_pRenderEffect;
};


#endif  // TRESSFXSDFMARCHINGCUBES_H_
