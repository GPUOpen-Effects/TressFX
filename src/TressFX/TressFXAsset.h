//---------------------------------------------------------------------------------------
// Loads and processes TressFX files.
// Inputs are binary files/streams/blobs
// Outputs are raw data that will mostly end up on the GPU.
//-------------------------------------------------------------------------------------
//
// Copyright (c) 2019 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef AMD_TRESSFX_ASSET_H
#define AMD_TRESSFX_ASSET_H

#include "AMD_Types.h"
#include "AMD_TressFX.h"
#include "Math\\Transform.h"
#include "TressFXCommon.h"

#include <assert.h>
#include <vector>
#include <string>

#define TRESSFX_MAX_INFLUENTIAL_BONE_COUNT 4

struct TressFXBoneSkinningData
{
    float boneIndex[TRESSFX_MAX_INFLUENTIAL_BONE_COUNT];
    float weight[TRESSFX_MAX_INFLUENTIAL_BONE_COUNT];
};

class EI_Scene;

class TressFXAsset
{
public:
    TressFXAsset();
    ~TressFXAsset();

    // Hair data from *.tfx
    std::vector<Vector3> m_positions;
    std::vector<AMD::float2> m_strandUV;
    std::vector<AMD::float4> m_tangents;
    std::vector<Vector3> m_followRootOffsets;
    std::vector<AMD::int32>      m_strandTypes;
    std::vector<AMD::real32>     m_thicknessCoeffs;
    std::vector<AMD::real32>     m_restLengths;
    std::vector<AMD::int32>      m_triangleIndices;

    // Bone skinning data from *.tfxbone
    std::vector<TressFXBoneSkinningData> m_boneSkinningData;

    // counts on hair data
    AMD::int32 m_numTotalStrands;
    AMD::int32 m_numTotalVertices;
    AMD::int32 m_numVerticesPerStrand;
    AMD::int32 m_numGuideStrands;
    AMD::int32 m_numGuideVertices;
    AMD::int32 m_numFollowStrandsPerGuide;
        
    // Loads *.tfx hair data 
    bool LoadHairData(FILE * ioObject);

    //Generates follow hairs procedually.  If numFollowHairsPerGuideHair is zero, then this function won't do anything. 
    bool GenerateFollowHairs(int numFollowHairsPerGuideHair = 0, float tipSeparationFactor = 0, float maxRadiusAroundGuideHair = 0);

    // Computes various parameters for simulation and rendering. After calling this function, data is ready to be passed to hair object. 
    bool ProcessAsset();
        
    // Loads *.tfxbone data
    bool LoadBoneData(FILE * ioObject, int skinNumber, EI_Scene * scene);

    inline AMD::uint32 GetNumHairSegments() { return m_numTotalStrands * (m_numVerticesPerStrand - 1); }
    inline AMD::uint32 GetNumHairTriangleIndices() { return 6 * GetNumHairSegments(); }
    inline AMD::uint32 GetNumHairLineIndices() { return 2 * GetNumHairSegments(); }

private:

    // Helper functions for ProcessAsset. 
    void ComputeThicknessCoeffs();
    void ComputeStrandTangent();
    void ComputeRestLengths();
    void FillTriangleIndexArray();
};

#endif