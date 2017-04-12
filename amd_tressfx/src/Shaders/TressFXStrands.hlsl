//---------------------------------------------------------------------------------------
// Shader code related to hair strands in the graphics pipeline.
//-------------------------------------------------------------------------------------
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

// This is code decoration for our sample.  
// Remove this and the EndHLSL at the end of this file 
// to turn this into valid HLSL

StartHLSL TressFXStrands

#define TRESSFX_FLOAT_EPSILON 1e-7


static const bool g_bExpandPixels = false;
bool g_bThinTip;
float g_Ratio;


texture2D<float3> g_txHairColor;
sampler g_samLinearWrap;



//--------------------------------------------------------------------------------------
//
//     Controls whether you do mul(M,v) or mul(v,M)
//     i.e., row major vs column major
//
//--------------------------------------------------------------------------------------
float4 MatrixMult(float4x4 m, float4 v)
{
    return mul(m, v);
}



//--------------------------------------------------------------------------------------
//
//      Safe_normalize-float2
//
//--------------------------------------------------------------------------------------
float2 Safe_normalize(float2 vec)
{
    float len = length(vec);
    return len >= TRESSFX_FLOAT_EPSILON ? (vec * rcp(len)) : float2(0, 0);
}

//--------------------------------------------------------------------------------------
//
//      Safe_normalize-float3
//
//--------------------------------------------------------------------------------------
float3 Safe_normalize(float3 vec)
{
    float len = length(vec);
    return len >= TRESSFX_FLOAT_EPSILON ? (vec * rcp(len)) : float3(0, 0, 0);
}

//--------------------------------------------------------------------------------------
//
//      Structured buffers with hair vertex info
//      Accessors to allow changes to how they are accessed.
//
//--------------------------------------------------------------------------------------
StructuredBuffer<float4> g_GuideHairVertexPositions;
StructuredBuffer<float4> g_GuideHairVertexTangents;
StructuredBuffer<float> g_HairThicknessCoeffs;
StructuredBuffer<float2> g_HairStrandTexCd;

inline float4 GetPosition(int index)
{
    return g_GuideHairVertexPositions[index];
}
inline float4 GetTangent(int index)
{
    return g_GuideHairVertexTangents[index];
}
inline float GetThickness(int index)
{
    return g_HairThicknessCoeffs[index];
}


float3 GetStrandColor(int index)
{
    float2 texCd = g_HairStrandTexCd[(uint)index / (uint)g_NumVerticesPerStrand];
    float3 color = g_txHairColor.SampleLevel(g_samLinearWrap, texCd, 0).xyz;// * g_MatBaseColor.xyz;
    return (color);
}

struct TressFXVertex
{
    float4 Position;
    float4 Tangent;
    float4 p0p1;
    float3 strandColor;
};

TressFXVertex GetExpandedTressFXVert(uint vertexId, float3 eye, float2 winSize, float4x4 viewProj)
{

    // Access the current line segment
    uint index = vertexId / 2;  // vertexId is actually the indexed vertex id when indexed triangles are used

                                // Get updated positions and tangents from simulation result
    float3 v = g_GuideHairVertexPositions[index].xyz;
    float3 t = g_GuideHairVertexTangents[index].xyz;

    // Get hair strand thickness
    float ratio = (g_bThinTip > 0) ? g_Ratio : 1.0;

    // Calculate right and projected right vectors
    float3 right = Safe_normalize(cross(t, Safe_normalize(v - eye)));
    float2 proj_right = Safe_normalize(MatrixMult(viewProj, float4(right, 0)).xy);

    // g_bExpandPixels should be set to 0 at minimum from the CPU side; this would avoid the below test
    float expandPixels = (g_bExpandPixels < 0) ? 0.0 : 0.71;

    // Calculate the negative and positive offset screenspace positions
    float4 hairEdgePositions[2]; // 0 is negative, 1 is positive
    hairEdgePositions[0] = float4(v + -1.0 * right * ratio * g_FiberRadius, 1.0);
    hairEdgePositions[1] = float4(v + 1.0 * right * ratio * g_FiberRadius, 1.0);
    hairEdgePositions[0] = MatrixMult(viewProj, hairEdgePositions[0]);
    hairEdgePositions[1] = MatrixMult(viewProj, hairEdgePositions[1]);

    // Write output data
    TressFXVertex Output = (TressFXVertex)0;
    float fDirIndex = (vertexId & 0x01) ? -1.0 : 1.0;
    Output.Position = ((vertexId & 0x01) ? hairEdgePositions[0] : hairEdgePositions[1]) + fDirIndex * float4(proj_right * expandPixels / winSize.y, 0.0f, 0.0f) * ((vertexId & 0x01) ? hairEdgePositions[0].w : hairEdgePositions[1].w);
    Output.Tangent = float4(t, ratio);
    Output.p0p1 = float4(hairEdgePositions[0].xy / max(hairEdgePositions[0].w, TRESSFX_FLOAT_EPSILON), hairEdgePositions[1].xy / max(hairEdgePositions[1].w, TRESSFX_FLOAT_EPSILON));
    Output.strandColor = GetStrandColor(index);
    //Output.PosCheck = MatrixMult(g_mView, float4(v,1));

    return Output;

}

EndHLSL

