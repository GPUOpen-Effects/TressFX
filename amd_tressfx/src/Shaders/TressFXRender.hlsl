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
// File: TressFXRender.hlsl
//
// Hair rendering functions.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//
//      Defines for TressFX rendering
//
//--------------------------------------------------------------------------------------
#define PI 3.1415926
#define e 2.71828183
#define FLOAT_EPSILON 1e-7

#define KBUFFER_SIZE    8

// techniques: has to match the define in cpp
#define SHADOW_SDSM     2
#define SHADOW_NONE     0

#define SM_EPSILON 0.01
#define KERNEL_SIZE 5
#define NULLPOINTER 0xFFFFFFFF

#define MIN_SHADOW_VALUE 0.15f

// These defines are used for conditionally including code
#define SIMPLESHADING           // no specular term
#define SIMPLESHADOWING         // no filtering except PCF
#define SUPERSIMPLESHADING      // constant diffuse (doesn't change with light angle)
//#define COLORDEBUG            // shows the number of overlapping hair fragments with color

#if (KBUFFER_SIZE <= 16)
#define ALU_INDEXING            // avoids using an indexed array for better performance
#endif


#define SHORTCUT_MIN_ALPHA 0.02

// Constants must match in TressFXShortCut.cpp

// Clear value for depths resource
#define SHORTCUT_INITIAL_DEPTH 0x3f800000

// Number of depth layers to use.  2 or 3 supported.
#define SHORTCUT_NUM_DEPTHS 3

// Compute source color as weighted average of front fragments, vs blending in order.
#define SHORTCUT_WEIGHTED_AVERAGE 1

// Output color deterministically when fragments have the same depth.  Requires additional clear of colors resource.
#define SHORTCUT_DETERMINISTIC 1


//--------------------------------------------------------------------------------------
// TressFX Structures
//--------------------------------------------------------------------------------------
struct KBuffer_STRUCT
{
    uint3   depthTangentColor;
};

struct HAIR_VERTEX
{
    float3 pos;
    float3 tangent;
    float4 texcoord;
};

struct PS_INPUT_HAIR
{
    float4 Position : SV_POSITION;
    float4 Tangent  : Tangent;
    float4 p0p1     : TEXCOORD0;
    float3 strandColor : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Constant Buffer
//--------------------------------------------------------------------------------------

cbuffer cbPerFrame : register( b1 )
{
    matrix      g_mWorld                        : packoffset( c0  );
    matrix      g_mViewProj                     : packoffset( c4  );
    matrix      g_mInvViewProj                  : packoffset( c8  );
    matrix      g_mViewProjLight                : packoffset( c12 );

    float3      g_vEye                          : packoffset( c16 );
    float       g_fvFov                         : packoffset( c16.w );

    float4      g_AmbientLightColor             : packoffset( c17 );
    float4      g_PointLightColor               : packoffset( c18 );
    float4      g_PointLightPos                 : packoffset( c19 );
    float4      g_MatBaseColor                  : packoffset( c20 );
    float4      g_MatKValue                     : packoffset( c21 ); // Ka, Kd, Ks, Ex

    float       g_FiberAlpha                    : packoffset( c22.x );
    float       g_HairShadowAlpha               : packoffset( c22.y );
    float       g_bExpandPixels                 : packoffset( c22.z );
    float       g_FiberRadius                   : packoffset( c22.w );

    float4      g_WinSize                       : packoffset( c23 ); // screen size

    float       g_FiberSpacing                  : packoffset( c24.x ); // average spacing between fibers
    float       g_bThinTip                      : packoffset( c24.y );
    float       g_fNearLight                    : packoffset( c24.z );
    float       g_fFarLight                     : packoffset( c24.w );

    int         g_iTechSM                       : packoffset( c25.x );
    int         g_bUseCoverage                  : packoffset( c25.y );
    int         g_iStrandCopies                 : packoffset( c25.z ); // strand copies that the transparency shader will produce
    int         g_iMaxFragments                 : packoffset( c25.w );

    float       g_alphaThreshold                : packoffset( c26.x );
    float       g_fHairKs2                      : packoffset( c26.y ); // for second highlight
    float       g_fHairEx2                      : packoffset( c26.z ); // for second highlight
    int         g_OptionalSRVs                  : packoffset( c26.w );

    matrix      g_mInvViewProjViewport          : packoffset( c27 );

    int         g_NumVerticesPerStrand          : packoffset( c31.x );
    int         g_NumFollowHairsPerGuideHair    : packoffset( c31.y );
    int         g_bSingleHeadTransform          : packoffset( c31.z );
};

// Bit fields for optional SRVs
#define PER_STRAND_TEX_COORDS   0x0001
#define PER_VERTEX_TEX_COORDS   0x0002
#define PER_VERTEX_COLORS       0x0004

//--------------------------------------------------------------------------------------
// Per-Pixel Linked List (PPLL) structure
//--------------------------------------------------------------------------------------
struct PPLL_STRUCT
{
    uint    TangentAndCoverage;
    uint    depth;
    uint    strandColor;
    uint    uNext;
};

//--------------------------------------------------------------------------------------
// Transforms structure
//--------------------------------------------------------------------------------------
struct Transforms
{
    row_major float4x4  tfm;
    float4              quat;
};

//--------------------------------------------------------------------------------------
// SRV buffers
//--------------------------------------------------------------------------------------
StructuredBuffer<PPLL_STRUCT>           LinkedListSRV               : register(t0);
Texture2D<uint>                         LinkedListHeadSRV           : register(t1);

Texture2D                               g_txSMScene                 : register( t2 );
Texture2D                               g_txSMHair                  : register( t3 );
Texture2D                               g_txHairColor               : register( t4 );
Texture2D                               g_txNoise                   : register( t5 );

Buffer<float>                           g_HairThicknessCoeffs       : register( t6 );

StructuredBuffer<float4>                g_HairVertexPositions       : register( t7 );
StructuredBuffer<float4>                g_HairVertexTangents        : register( t8 );
StructuredBuffer<Transforms>            g_Transforms                : register( t9 );

StructuredBuffer<float2>                g_txHairStrandTexCd         : register( t10 );

Texture2DArray<uint>                    FragmentDepthsTexture       : register( t13 );
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
Texture2D<float4>                       FragmentColors              : register( t14 );
#else
#if SHORTCUT_NUM_DEPTHS == 3
StructuredBuffer<uint3>                 FragmentColors              : register( t14 );
#else
StructuredBuffer<uint2>                 FragmentColors              : register( t14 );
#endif
#endif
Texture2D<float>                        tAccumInvAlpha              : register( t15 );



//--------------------------------------------------------------------------------------
// Unordered Access
//--------------------------------------------------------------------------------------
//#if SHORTCUT_LAYERS == 0
RWTexture2D<uint>                       LinkedListHeadUAV           : register(u1);
RWStructuredBuffer<PPLL_STRUCT>         LinkedListUAV               : register(u2); // store fragment linked list

RWTexture2DArray<uint>                  RWFragmentDepthsTexture     : register(u1);
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
RWStructuredBuffer<uint2>               RWFragmentColors             : register(u0);
#else
#if SHORTCUT_NUM_DEPTHS == 3
RWStructuredBuffer<uint3>               RWFragmentColors             : register(u0);
#else
RWStructuredBuffer<uint2>               RWFragmentColors             : register(u0);
#endif
#endif

//--------------------------------------------------------------------------------------
// Samplers
//--------------------------------------------------------------------------------------
SamplerState                            g_samLinearWrap             : register( s0 );
SamplerState                            g_samPointClamp             : register( s1 );
SamplerComparisonState                  g_samShadow                 : register( s2 );


//--------------------------------------------------------------------------------------
// common shader input/output structure
//--------------------------------------------------------------------------------------

struct VS_OUTPUT_SM
{
    float4 Position     : SV_POSITION; // vertex position
    float3 WorldPos     : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Screen quad
//--------------------------------------------------------------------------------------
struct VS_INPUT_SCREENQUAD
{
    float3 Position     : POSITION;     // vertex position
    float3 Normal       : NORMAL;       // this normal comes in per-vertex
    float2 Texcoord     : TEXCOORD0;    // vertex texture coords
};

struct VS_OUTPUT_SCREENQUAD
{
    float4 vPosition : SV_POSITION;
    float2 vTex      : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Simple vertex shader for rendering a full screen quad
//--------------------------------------------------------------------------------------
VS_OUTPUT_SCREENQUAD VS_ScreenQuad(VS_INPUT_SCREENQUAD input)
{
    VS_OUTPUT_SCREENQUAD output;

    output.vPosition = float4(input.Position.xyz, 1.0);
    output.vTex = input.Texcoord.xy;

    return output;
}

//--------------------------------------------------------------------------------------
//
//      Vertex Shader Code
//
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//
//      GetVertex
//
//--------------------------------------------------------------------------------------
inline float4 GetVertex(int index) 
{
    return g_HairVertexPositions[index];
}

//--------------------------------------------------------------------------------------
//
//      StrandTextureColor
//
//--------------------------------------------------------------------------------------
float3 StrandColor(int index)
{

    if (g_OptionalSRVs & PER_STRAND_TEX_COORDS)
    {
        float2 texCd = g_txHairStrandTexCd[(uint)index / (uint)g_NumVerticesPerStrand];
        float3 color = g_txHairColor.SampleLevel(g_samLinearWrap, texCd, 0).xyz * g_MatBaseColor.xyz;
        return (color);
    }
    else
    {
        return g_MatBaseColor.xyz;
    }
}

//--------------------------------------------------------------------------------------
//
//      Safe_normalize-float2
//
//--------------------------------------------------------------------------------------
float2 Safe_normalize(float2 vec)
{
    float len = length(vec);
    return len >= FLOAT_EPSILON ? (vec * rcp(len)) : float2(0,0);
}

//--------------------------------------------------------------------------------------
//
//      Safe_normalize-float3
//
//--------------------------------------------------------------------------------------
float3 Safe_normalize(float3 vec)
{
    float len = length(vec);
    return len >= FLOAT_EPSILON ? (vec * rcp(len)) : float3(0,0,0);
}

//--------------------------------------------------------------------------------------
// VS_GenerateHairSM
//
// Vertex shader for generating the shadow map
//--------------------------------------------------------------------------------------
VS_OUTPUT_SM VS_GenerateHairSM( uint vertexId : SV_VertexID )
{
    VS_OUTPUT_SM Output;

    float4 pos = GetVertex(vertexId);

    // Transform the position from object space to homogeneous projection space
    Output.Position = mul( float4(pos.xyz, 1), g_mViewProjLight );

    Output.WorldPos = pos.xyz;

    return Output;
}

//--------------------------------------------------------------------------------------
// VS_RenderHair
//
// Vertex shader for rendering the hair strands without AA
//--------------------------------------------------------------------------------------
PS_INPUT_HAIR VS_RenderHair( uint vertexId : SV_VertexID )
{

    // Access the current line segment
    uint index = vertexId / 2;  // vertexId is actually the indexed vertex id when indexed triangles are used

    // Get updated positions and tangents from simulation result
    float3 t = g_HairVertexTangents[index].xyz;
    float3 v = GetVertex(index).xyz;

    // Get hair strand thickness
    float ratio = ( g_bThinTip > 0 ) ? g_HairThicknessCoeffs[index] : 1.0;

    // Calculate right and projected right vectors
    float3 right      = Safe_normalize( cross( t, Safe_normalize(v - g_vEye) ) );
    float2 proj_right = Safe_normalize( mul( float4(right, 0), g_mViewProj ).xy );

    // g_bExpandPixels should be set to 0 at minimum from the CPU side; this would avoid the below test
    float expandPixels = (g_bExpandPixels < 0 ) ? 0.0 : 0.71;

    PS_INPUT_HAIR Output = (PS_INPUT_HAIR)0;

    float fDirCoef = (vertexId & 0x01) ? -1.0 : 1.0;
    float3 temp     = v + fDirCoef * right * ratio * g_FiberRadius;
    Output.Position = mul(float4(temp, 1.f), g_mViewProj);
    Output.Position = Output.Position + fDirCoef * float4(proj_right * expandPixels / g_WinSize.y, 0.f, 0.f) * Output.Position.w;
    Output.Tangent  = float4(t, ratio);
    Output.p0p1     = float4( v.xy, GetVertex(index+1).xy );
    Output.strandColor = StrandColor(index);

    return Output;
}

//--------------------------------------------------------------------------------------
// VS_RenderHair_StrandCopies
//
// Vertex shader for rendering the hair strands with randomized copies and without AA
//--------------------------------------------------------------------------------------
PS_INPUT_HAIR VS_RenderHair_StrandCopies( uint vertexId : SV_VertexID, uint instanceId : SV_InstanceID )
{
    // Access the current line segment
    uint index = vertexId / 2;  // vertexId is actually the indexed vertex id when indexed triangles are used

    // Identifies which copy of the strand this is (0 is the original, 1 is the first copy, etc.)
    uint randOffsetIndex = instanceId;

    // Getting a random offset value
    uint seedRand = ((index / 64 + 1) * (randOffsetIndex + 1)) % 512;
    float2 seedTexcorrd = float2( float(seedRand % 512), float(seedRand / 512) ) * (1.f/512.f);
    float3 randOffset = float(randOffsetIndex) * g_txNoise.SampleLevel(g_samLinearWrap, seedTexcorrd, 0).xyz;
    randOffset = 2.0f * randOffset - 1.0f; // so we have random offsets between -1 and 1

    // Get updated positions and tangents from simulation result
    float3 t = g_HairVertexTangents[index].xyz;
    float3 v = randOffset + GetVertex(index).xyz;  // We apply a random offset to each vertex when multiple strands are requested

    // Get hair strand thickness
    float ratio = ( g_bThinTip > 0 ) ? g_HairThicknessCoeffs[index] : 1.0;

    // Calculate right and projected right vectors
    float3 right      = Safe_normalize( cross( t, Safe_normalize(v - g_vEye) ) );
    float2 proj_right = Safe_normalize( mul( float4(right, 0), g_mViewProj ).xy );

    // g_bExpandPixels should be set to 0 at minimum from the CPU side; this would avoid the below test
    float expandPixels = (g_bExpandPixels < 0 ) ? 0.0 : 0.71;

    PS_INPUT_HAIR Output = (PS_INPUT_HAIR)0;

    float fDirCoef = (vertexId & 0x01) ? -1.0 : 1.0;
    float3 temp     = v +  fDirCoef * right * ratio * g_FiberRadius;
    Output.Position = mul(float4(temp, 1.f), g_mViewProj);
    Output.Position = Output.Position + fDirCoef * float4(proj_right * expandPixels/ g_WinSize.y, 0.f, 0.f) * Output.Position.w;
    Output.Tangent  = float4(t, ratio);
    Output.p0p1     = float4( v.xy, randOffset.xy + GetVertex(index+1).xy );
    Output.strandColor = StrandColor(index);

    return Output;
}

//--------------------------------------------------------------------------------------
// VS_RenderHair_AA
//
// Vertex shader for rendering the hair strands with AA
//--------------------------------------------------------------------------------------
PS_INPUT_HAIR VS_RenderHair_AA( uint vertexId : SV_VertexID )
{
    // Access the current line segment
    uint index = vertexId / 2;  // vertexId is actually the indexed vertex id when indexed triangles are used

    // Get updated positions and tangents from simulation result
    float3 t = g_HairVertexTangents[index].xyz;
    float3 v = GetVertex(index).xyz;

    // Get hair strand thickness
    float ratio = ( g_bThinTip > 0 ) ? g_HairThicknessCoeffs[index] : 1.0;

    // Calculate right and projected right vectors
    float3 right      = Safe_normalize( cross( t, Safe_normalize(v - g_vEye) ) );
    float2 proj_right = Safe_normalize( mul( float4(right, 0), g_mViewProj ).xy );

    // g_bExpandPixels should be set to 0 at minimum from the CPU side; this would avoid the below test
    float expandPixels = (g_bExpandPixels < 0 ) ? 0.0 : 0.71;

    // Calculate the negative and positive offset screenspace positions
    float4 hairEdgePositions[2]; // 0 is negative, 1 is positive
    hairEdgePositions[0] = float4(v +  -1.0 * right * ratio * g_FiberRadius, 1.0);
    hairEdgePositions[1] = float4(v +   1.0 * right * ratio * g_FiberRadius, 1.0);
    hairEdgePositions[0] = mul(hairEdgePositions[0], g_mViewProj);
    hairEdgePositions[1] = mul(hairEdgePositions[1], g_mViewProj);

    // Write output data
    PS_INPUT_HAIR Output = (PS_INPUT_HAIR)0;
    float fDirIndex = (vertexId & 0x01) ? -1.0 : 1.0;
    Output.Position = ((vertexId & 0x01) ? hairEdgePositions[0] : hairEdgePositions[1]) + fDirIndex * float4(proj_right * expandPixels / g_WinSize.y, 0.0f, 0.0f) * ((vertexId & 0x01) ? hairEdgePositions[0].w : hairEdgePositions[1].w);
    Output.Tangent  = float4(t, ratio);
    Output.p0p1     = float4( hairEdgePositions[0].xy / max(hairEdgePositions[0].w, FLOAT_EPSILON), hairEdgePositions[1].xy / max(hairEdgePositions[1].w, FLOAT_EPSILON) );
    Output.strandColor = StrandColor(index);

    return Output;
}

//--------------------------------------------------------------------------------------
// VS_RenderHair_AA_StrandCopies
//
// Vertex shader for rendering the hair strands with randomized copies and AA
//--------------------------------------------------------------------------------------
PS_INPUT_HAIR VS_RenderHair_AA_StrandCopies( uint vertexId : SV_VertexID, uint instanceId : SV_InstanceID )
{
    // Access the current line segment
    uint index = vertexId / 2;  // vertexId is actually the indexed vertex id when indexed triangles are used

    // Identifies which copy of the strand this is (0 is the original, 1 is the first copy, etc.)
    uint randOffsetIndex = instanceId;

    // Getting a random offset value
    uint seedRand = ((index / 64 + 1) * (randOffsetIndex + 1)) % 512;
    float2 seedTexcorrd = float2( float(seedRand % 512), float(seedRand / 512) ) * (1.f/512.f);
    float3 randOffset = float(randOffsetIndex) * g_txNoise.SampleLevel(g_samLinearWrap, seedTexcorrd, 0).xyz;
    randOffset = 2.0f * randOffset - 1.0f; // so we have random offsets between -1 and 1

    // Get updated positions and tangents from simulation result
    float3 t = g_HairVertexTangents[index].xyz;
    float3 v = randOffset + GetVertex(index).xyz;  // We apply a random offset to each vertex when multiple strands are requested

    // Get hair strand thickness
    float ratio = ( g_bThinTip > 0 ) ? g_HairThicknessCoeffs[index] : 1.0;

    // Calculate right and projected right vectors
    float3 right      = Safe_normalize( cross( t, Safe_normalize(v - g_vEye) ) );
    float2 proj_right = Safe_normalize( mul( float4(right, 0), g_mViewProj ).xy );

    // g_bExpandPixels should be set to 0 at minimum from the CPU side; this would avoid the below test
    float expandPixels = (g_bExpandPixels < 0 ) ? 0.0 : 0.71;

    // Calculate the negative and positive offset screenspace positions
    float4 hairEdgePositions[2]; // 0 is negative, 1 is positive
    hairEdgePositions[0] = float4(v +  -1.0 * right * ratio * g_FiberRadius, 1.0);
    hairEdgePositions[1] = float4(v +   1.0 * right * ratio * g_FiberRadius, 1.0);
    hairEdgePositions[0] = mul(hairEdgePositions[0], g_mViewProj);
    hairEdgePositions[1] = mul(hairEdgePositions[1], g_mViewProj);

    // Write output data
    PS_INPUT_HAIR Output = (PS_INPUT_HAIR)0;
    float fDirIndex = (vertexId & 0x01) ? -1.0 : 1.0;
    Output.Position = ((vertexId & 0x01) ? hairEdgePositions[0] : hairEdgePositions[1]) + fDirIndex * float4(proj_right * expandPixels / g_WinSize.y, 0.0f, 0.0f) * ((vertexId & 0x01) ? hairEdgePositions[0].w : hairEdgePositions[1].w);
    Output.Tangent  = float4(t, ratio);
    Output.p0p1     = float4( hairEdgePositions[0].xy / max(hairEdgePositions[0].w, FLOAT_EPSILON), hairEdgePositions[1].xy / max(hairEdgePositions[1].w, FLOAT_EPSILON) );
    Output.strandColor = StrandColor(index);

    return Output;
}

//--------------------------------------------------------------------------------------
//
//      Pixel Shader Code
//
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// ComputeHairShading
//
// Hair shading using dual highlight approach and Kajiya lighting model
// dual highlight: marschner 03
// kajiya model: kajiya 84
//--------------------------------------------------------------------------------------
float3 ComputeHairShading(float3 iPos, float3 iTangent, float4 iTex, float amountLight, float3 baseColor)
{
    float rand_value = 1;

    if(abs(iTex.x) + abs(iTex.y) >1e-5) // if texcoord is available, use texture map
        rand_value = g_txNoise.SampleLevel(g_samLinearWrap, iTex.xy, 0).x;

    // define baseColor and Ka Kd Ks coefficient for hair
    float Ka = g_MatKValue.x, Kd = g_MatKValue.y,
          Ks1 = g_MatKValue.z, Ex1 = g_MatKValue.w,
          Ks2 = g_fHairKs2, Ex2 = g_fHairEx2;

    float3 lightPos = g_PointLightPos.xyz;
    float3 vLightDir = normalize(lightPos - iPos.xyz);
    float3 vEyeDir = normalize(g_vEye.xyz - iPos.xyz);
    float3 tangent = normalize(iTangent);

    // in Kajiya's model: diffuse component: sin(t, l)
    float cosTL = (dot(tangent, vLightDir));
    float sinTL = sqrt(1 - cosTL*cosTL);
    float diffuse = sinTL; // here sinTL is apparently larger than 0

    float alpha = (rand_value * 10) * PI/180; // tiled angle (5-10 dgree)

    // in Kajiya's model: specular component: cos(t, rl) * cos(t, e) + sin(t, rl)sin(t, e)
    float cosTRL = -cosTL;
    float sinTRL = sinTL;
    float cosTE = (dot(tangent, vEyeDir));
    float sinTE = sqrt(1- cosTE*cosTE);

    // primary highlight: reflected direction shift towards root (2 * Alpha)
    float cosTRL_root = cosTRL * cos(2 * alpha) - sinTRL * sin(2 * alpha);
    float sinTRL_root = sqrt(1 - cosTRL_root * cosTRL_root);
    float specular_root = max(0, cosTRL_root * cosTE + sinTRL_root * sinTE);

    // secondary highlight: reflected direction shifted toward tip (3*Alpha)
    float cosTRL_tip = cosTRL*cos(-3*alpha) - sinTRL*sin(-3*alpha);
    float sinTRL_tip = sqrt(1 - cosTRL_tip * cosTRL_tip);
    float specular_tip = max(0, cosTRL_tip * cosTE + sinTRL_tip * sinTE);

    // completely in shadow looks very flat, so don't go below some min value
    amountLight = max(amountLight, MIN_SHADOW_VALUE);

    float3 vColor = Ka * g_AmbientLightColor.xyz * baseColor + // ambient
                    amountLight * g_PointLightColor.xyz * (
                    Kd * diffuse * baseColor + // diffuse
                    Ks1 * pow(specular_root, Ex1)  + // primary hightlight r
                    Ks2 * pow(specular_tip, Ex2) * baseColor); // secondary highlight rtr

   return vColor;
}

//--------------------------------------------------------------------------------------
// SimpleHairShading
//
// Low quality, but faster hair shading
//--------------------------------------------------------------------------------------
float3 SimpleHairShading(float3 iPos, float3 iTangent, float4 iTex, float amountLight, float3 baseColor)
{
    // completely in shadow looks very flat, so don't go below some min value
    amountLight = max(amountLight, MIN_SHADOW_VALUE);

    float Kd = g_MatKValue.y;

#ifdef SUPERSIMPLESHADING
    float3 vColor = amountLight * Kd * baseColor;
#else
    // define baseColor and Ka Kd Ks coefficient for hair
    float Ka = g_MatKValue.x;
    float Ks1 = g_MatKValue.z;
    float Ex1 = g_MatKValue.w;
    float Ks2 = g_fHairKs2;
    float Ex2 = g_fHairEx2;

    float3 lightPos = g_PointLightPos.xyz;
    float3 vLightDir = normalize(lightPos - iPos.xyz);
    float3 tangent = normalize(iTangent);

    // in Kajiya's model: diffuse component: sin(t, l)
    float cosTL = (dot(tangent, vLightDir));
    float sinTL = sqrt(1 - cosTL*cosTL);
    float diffuse = sinTL; // here sinTL is apparently larger than 0

    float3 vColor = Ka * g_AmbientLightColor.xyz * baseColor +                          // ambient
                    amountLight * g_PointLightColor.xyz * (Kd * diffuse * baseColor);   // diffuse
#endif

    return vColor;
}

//--------------------------------------------------------------------------------------
// ComputeCoverage
//
// Calculate the pixel coverage of a hair strand by computing the hair width
//--------------------------------------------------------------------------------------
float ComputeCoverage(float2 p0, float2 p1, float2 pixelLoc)
{
    // p0, p1, pixelLoc are in d3d clip space (-1 to 1)x(-1 to 1)

    // Scale positions so 1.f = half pixel width
    p0 *= g_WinSize.xy;
    p1 *= g_WinSize.xy;
    pixelLoc *= g_WinSize.xy;

    float p0dist = length(p0 - pixelLoc);
    float p1dist = length(p1 - pixelLoc);
    float hairWidth = length(p0 - p1);

    // will be 1.f if pixel outside hair, 0.f if pixel inside hair
    float outside = any( float2(step(hairWidth, p0dist), step(hairWidth, p1dist)) );

    // if outside, set sign to -1, else set sign to 1
    float sign = outside > 0.f ? -1.f : 1.f;

    // signed distance (positive if inside hair, negative if outside hair)
    float relDist = sign * saturate( min(p0dist, p1dist) );

    // returns coverage based on the relative distance
    // 0, if completely outside hair edge
    // 1, if completely inside hair edge
    return (relDist + 1.f) * 0.5f;
}

//--------------------------------------------------------------------------------------
// ComputeShadow
//
// Computes the shadow using a simplified deep shadow map technique for the hair and
// PCF for scene objects. It uses multiple taps to filter over a (KERNEL_SIZE x KERNEL_SIZE)
// kernel for high quality results.
//--------------------------------------------------------------------------------------
float ComputeShadow(float3 worldPos, float alpha, int iTechSM)
{

    if( iTechSM == SHADOW_NONE )
        return 1;

    float4 projPosLight = mul(float4(worldPos,1), g_mViewProjLight);
    float2 texSM = float2(projPosLight.x/projPosLight.w+1, -projPosLight.y/projPosLight.w+1)*0.5;
    float depth = projPosLight.z/projPosLight.w;
    float epsilon = depth * SM_EPSILON;
    float depth_fragment = projPosLight.w;

    // for shadow casted by scene objs, use PCF shadow
    float total_weight = 0;
    float amountLight_hair = 0;

    total_weight = 0;
    [unroll] for (int dx = (1-KERNEL_SIZE)/2; dx <= KERNEL_SIZE/2; dx++)
    {
        [unroll] for (int dy = (1-KERNEL_SIZE)/2; dy <= KERNEL_SIZE/2; dy++)
        {
            float size = 2.4;
            float sigma = (KERNEL_SIZE/2.0)/size; // standard deviation, when kernel/2 > 3*sigma, it's close to zero, here we use 1.5 instead
            float exp = -1* (dx*dx + dy*dy)/ (2* sigma * sigma);
            float weight = 1/(2*PI*sigma*sigma) * pow(e, exp);

            // shadow casted by hair: simplified deep shadow map
            float depthSMHair = g_txSMHair.SampleLevel( g_samPointClamp, texSM, 0, int2(dx, dy) ).x; //z/w

            float depth_smPoint = g_fNearLight/(1 - depthSMHair*(g_fFarLight - g_fNearLight)/g_fFarLight);

            float depth_range = max(0, depth_fragment-depth_smPoint);
            float numFibers =  depth_range/(g_FiberSpacing*g_FiberRadius);

            // if occluded by hair, there is at least one fiber
            [flatten]if (depth_range > 1e-5)
                numFibers += 1;
            amountLight_hair += pow(abs(1-alpha), numFibers)*weight;

            total_weight += weight;
        }
    }
    amountLight_hair /= total_weight;

    float amountLight_scene = g_txSMScene.SampleCmpLevelZero(g_samShadow, texSM, depth-epsilon);

    return (amountLight_hair * amountLight_scene);

}

//--------------------------------------------------------------------------------------
// ComputeSimpleShadow
//
// Computes the shadow using a simplified deep shadow map technique for the hair and
// PCF for scene objects. This function only uses one sample, so it is faster but
// not as good quality as ComputeShadow
//--------------------------------------------------------------------------------------
float ComputeSimpleShadow(float3 worldPos, float alpha, int iTechSM)
{
    if( iTechSM == SHADOW_NONE )
    {
        return 1;
    }

    float4 projPosLight = mul(float4(worldPos,1), g_mViewProjLight);

    float2 texSM = 0.5 * float2(projPosLight.x/projPosLight.w+1.0, -projPosLight.y/projPosLight.w+1.0);
    float depth = projPosLight.z/projPosLight.w;
    float epsilon = depth * SM_EPSILON;
    float depth_fragment = projPosLight.w;

    // shadow casted by scene
    float amountLight_scene = g_txSMScene.SampleCmpLevelZero(g_samShadow, texSM, depth-epsilon);

    // shadow casted by hair: simplified deep shadow map
    float depthSMHair = g_txSMHair.SampleLevel( g_samPointClamp, texSM, 0 ).x; //z/w

    float depth_smPoint = g_fNearLight/(1 - depthSMHair*(g_fFarLight - g_fNearLight)/g_fFarLight);

    float depth_range = max(0, depth_fragment-depth_smPoint);
    float numFibers =  depth_range/(g_FiberSpacing*g_FiberRadius);

    // if occluded by hair, there is at least one fiber
    [flatten]if (depth_range > 1e-5)
        numFibers += 1.0;
    float amountLight_hair = pow(abs(1-alpha), numFibers);

    return amountLight_scene * amountLight_hair;
}

//--------------------------------------------------------------------------------------
// Helper functions for packing and unpacking the stored tangent and coverage
//--------------------------------------------------------------------------------------
uint PackFloat4IntoUint(float4 vValue)
{
    return (uint)( ( ( (uint)(vValue.x*255.0f) & 0xFFu ) << 24 ) | ( ( (uint)(vValue.y*255.0f) & 0xFFu ) << 16 ) | ( ( (uint)(vValue.z*255.0f) & 0xFFu ) << 8) | ( (uint)(vValue.w * 255.0f) & 0xFFu ) );
}

float4 UnpackUintIntoFloat4(uint uValue)
{
    return float4( (float)( (uint)(uValue & 0xFF000000u) >> 24 ) / 255.0f, (float)( (uint)(uValue & 0x00FF0000u) >> 16 ) / 255.0f, (float)( (uint)(uValue & 0x0000FF00u) >> 8 ) / 255.0f, (float)( (uint)(uValue & 0x000000FFu) ) / 255.0f );
}

uint PackTangentAndCoverage(float3 tangent, float coverage)
{
    return PackFloat4IntoUint( float4(tangent.xyz*0.5f + 0.5f, coverage) );
}

float3 GetTangent(uint packedTangent)
{
    return 2.0f * UnpackUintIntoFloat4(packedTangent).xyz - 1.0f;
}

float GetCoverage(uint packedCoverage)
{
    return UnpackUintIntoFloat4(packedCoverage).w;
}

//--------------------------------------------------------------------------------------
// StoreFragments_Hair
//
// Stores the necessary fragment data into the per pixel linked list
//      LinkedListHeadUAV is the UAV that stores an offset to the head of the pixel's linked list
//      LinkedListUAV is the UAV that contains the actual linked list
//
//--------------------------------------------------------------------------------------
void StoreFragments_Hair(uint2 address, float3 tangent, float coverage, float depth, float3 color)
{
    // Retrieve current pixel count and increase counter
    uint uPixelCount = LinkedListUAV.IncrementCounter();
    uint uOldStartOffset;

    // Exchange indices in LinkedListHead texture corresponding to pixel location
    InterlockedExchange(LinkedListHeadUAV[address], uPixelCount, uOldStartOffset);  // link head texture

    // Append new element at the end of the Fragment and Link Buffer
    PPLL_STRUCT Element;
    Element.TangentAndCoverage = PackTangentAndCoverage(tangent, coverage);
    Element.depth = asuint(depth);
    Element.strandColor = PackFloat4IntoUint( float4(color,0) );
    Element.uNext = uOldStartOffset;
    LinkedListUAV[uPixelCount] = Element; // buffer that stores the fragments
}

//--------------------------------------------------------------------------------------
// PS_ABuffer_Hair
//
// This is the first pass pixel shader for rendering hair geometry into an A-buffer.
// It keeps all of the layers of the transparent hair in a UAV per-pixel linked list.
//
// Coverage for antialiasing is calculated and stored in the linked list along with geometry
// and attributes necessary for lighting and shadows which are calculated in a subsequent
// pass by rendering a full screen quad.
//
//--------------------------------------------------------------------------------------
[earlydepthstencil]
float4 PS_ABuffer_Hair( PS_INPUT_HAIR In) : SV_Target
{
    // Render AA Line, calculate pixel coverage
    float4 proj_pos = float4(   2*In.Position.x*g_WinSize.z - 1.0,  // g_WinSize.z = 1.0/g_WinSize.x
                                1 - 2*In.Position.y*g_WinSize.w,    // g_WinSize.w = 1.0/g_WinSize.y
                                1,
                                1);

    float4 original_pos = mul(proj_pos, g_mInvViewProj);

    float curve_scale = 1;
    if (g_bThinTip > 0 )
        curve_scale = In.Tangent.w;

    float fiber_radius = curve_scale * g_FiberRadius;

    float coverage = 1.f;
    if(g_bUseCoverage)
    {
        coverage = ComputeCoverage(In.p0p1.xy, In.p0p1.zw, proj_pos.xy);
    }

    coverage *= g_FiberAlpha;

    // only store fragments with non-zero alpha value
    if (coverage > g_alphaThreshold) // ensure alpha is at least as much as the minimum alpha value
    {
        StoreFragments_Hair(In.Position.xy, In.Tangent.xyz, coverage, In.Position.z, In.strandColor);
    }
    // output a mask RT for final pass
    return float4(1, 0, 0, 0);
}


#ifdef ALU_INDEXING
//--------------------------------------------------------------------------------------
//
// Helper functions for storing the k-buffer into non-indexed registers for better
// performance. For this code to work, KBUFFER_SIZE must be <= 16.
//
//--------------------------------------------------------------------------------------

uint GetUintFromIndex_Size16(uint4 V03, uint4 V47, uint4 V811, uint4 V1215, uint uIndex)
{
    uint u;
    u = uIndex==1 ? V03.y : V03.x;
    u = uIndex==2 ? V03.z : u;
    u = uIndex==3 ? V03.w : u;
    u = uIndex==4 ? V47.x : u;
    u = uIndex==5 ? V47.y : u;
    u = uIndex==6 ? V47.z : u;
    u = uIndex==7 ? V47.w : u;
    u = uIndex==8 ? V811.x : u;
    u = uIndex==9 ? V811.y : u;
    u = uIndex==10 ? V811.z : u;
    u = uIndex==11 ? V811.w : u;
    u = uIndex==12 ? V1215.x : u;
    u = uIndex==13 ? V1215.y : u;
    u = uIndex==14 ? V1215.z : u;
    u = uIndex==15 ? V1215.w : u;
    return u;
}

void StoreUintAtIndex_Size16(inout uint4 V03, inout uint4 V47, inout uint4 V811, inout uint4 V1215, uint uIndex, uint uValue)
{
    V03.x = (uIndex==0) ? uValue : V03.x;
    V03.y = (uIndex==1) ? uValue : V03.y;
    V03.z = (uIndex==2) ? uValue : V03.z;
    V03.w = (uIndex==3) ? uValue : V03.w;
    V47.x = (uIndex==4) ? uValue : V47.x;
    V47.y = (uIndex==5) ? uValue : V47.y;
    V47.z = (uIndex==6) ? uValue : V47.z;
    V47.w = (uIndex==7) ? uValue : V47.w;
    V811.x = (uIndex==8) ? uValue : V811.x;
    V811.y = (uIndex==9) ? uValue : V811.y;
    V811.z = (uIndex==10) ? uValue : V811.z;
    V811.w = (uIndex==11) ? uValue : V811.w;
    V1215.x = (uIndex==12) ? uValue : V1215.x;
    V1215.y = (uIndex==13) ? uValue : V1215.y;
    V1215.z = (uIndex==14) ? uValue : V1215.z;
    V1215.w = (uIndex==15) ? uValue : V1215.w;
}
#endif

//--------------------------------------------------------------------------------------
// PS_KBuffer_Hair
//
// Second pass pixel shader which selects the nearest k fragments (k-buffer) and renders
// then in depth order for correct transparent blending of the hair. It uses the per-pixel
// linked list to access all of the layers of hair fragment so that the nearest k fragments
// are rendered at a higher quality than the remaining fragments. The lighting and shadows
// are calculated in screen space using the tangent and depth stored in the per-pixel linked
// list, similar to deferred shading.
//
//--------------------------------------------------------------------------------------
[earlydepthstencil]
float4 PS_KBuffer_Hair(VS_OUTPUT_SCREENQUAD In): SV_Target
{
    float4 fcolor = float4(0,0,0,1);
    float amountLight;
    float lightIntensity;
    float4 fragmentColor;
    float4 vWorldPosition;
    float3 vTangent;
    float coverage;
    uint tangentAndCoverage;

    // get the start of the linked list from the head pointer
    uint pointer = LinkedListHeadSRV[In.vPosition.xy];

    // A local Array to store the top k fragments(depth and color), where k = KBUFFER_SIZE
#ifdef ALU_INDEXING

    uint4 kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215;
    uint4 kBufferPackedTangentV03, kBufferPackedTangentV47, kBufferPackedTangentV811, kBufferPackedTangentV1215;
    uint4 kBufferStrandColorV03, kBufferStrandColorV47, kBufferStrandColorV811, kBufferStrandColorV1215;
    kBufferDepthV03 = uint4(asuint(100000.0f), asuint(100000.0f), asuint(100000.0f), asuint(100000.0f));
    kBufferDepthV47 = uint4(asuint(100000.0f), asuint(100000.0f), asuint(100000.0f), asuint(100000.0f));
    kBufferDepthV811 = uint4(asuint(100000.0f), asuint(100000.0f), asuint(100000.0f), asuint(100000.0f));
    kBufferDepthV1215 = uint4(asuint(100000.0f), asuint(100000.0f), asuint(100000.0f), asuint(100000.0f));
    kBufferPackedTangentV03 = uint4(0,0,0,0);
    kBufferPackedTangentV47 = uint4(0,0,0,0);
    kBufferPackedTangentV811 = uint4(0,0,0,0);
    kBufferPackedTangentV1215 = uint4(0,0,0,0);
    kBufferStrandColorV03 = uint4(0,0,0,0);
    kBufferStrandColorV47 = uint4(0,0,0,0);
    kBufferStrandColorV811 = uint4(0,0,0,0);
    kBufferStrandColorV1215 = uint4(0,0,0,0);

    // Get the first k elements in the linked list
    int nNumFragments = 0;
    for(int p=0; p<KBUFFER_SIZE; p++)
    {
        if (pointer != NULLPOINTER)
        {
            StoreUintAtIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215, p, LinkedListSRV[pointer].depth);
            StoreUintAtIndex_Size16(kBufferPackedTangentV03, kBufferPackedTangentV47, kBufferPackedTangentV811, kBufferPackedTangentV1215, p, LinkedListSRV[pointer].TangentAndCoverage);
            StoreUintAtIndex_Size16(kBufferStrandColorV03, kBufferStrandColorV47, kBufferStrandColorV811, kBufferStrandColorV1215, p, LinkedListSRV[pointer].strandColor);
            pointer = LinkedListSRV[pointer].uNext;
#ifdef COLORDEBUG
            nNumFragments++;
#endif
        }
    }

#else

    KBuffer_STRUCT kBuffer[KBUFFER_SIZE];

    [unroll]for(int t=0; t<KBUFFER_SIZE; t++)
    {
        kBuffer[t].depthTangentColor.x = asuint(100000.0f); // must be larger than the maximum possible depth value
        kBuffer[t].depthTangentColor.y = 0;
        kBuffer[t].depthTangentColor.z = 0;
    }

    // Get the first k elements in the linked list
    int nNumFragments = 0;
    for(int p=0; p<KBUFFER_SIZE; p++)
    {
        if (pointer != NULLPOINTER)
        {
            kBuffer[p].depthTangentColor.x  = LinkedListSRV[pointer].depth;
            kBuffer[p].depthTangentColor.y  = LinkedListSRV[pointer].TangentAndCoverage;
            kBuffer[p].depthTangentColor.z =  LinkedListSRV[pointer].strandColor;
            pointer                             = LinkedListSRV[pointer].uNext;
#ifdef COLORDEBUG
            nNumFragments++;
#endif
        }
    }

#endif

    // Go through the rest of the linked list, and keep the closest k fragments, but not in sorted order.
    [allow_uav_condition]
    for(int l=0; l < g_iMaxFragments; l++)
    {
        if(pointer == NULLPOINTER)  break;

#ifdef COLORDEBUG
        nNumFragments++;
#endif

        int id = 0;
        float max_depth = 0;

        // find the furthest node in array
        [unroll]for(int i=0; i<KBUFFER_SIZE; i++)
        {
#ifdef ALU_INDEXING
            float fDepth = asfloat(GetUintFromIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215, i));
#else
            float fDepth = asfloat(kBuffer[i].depthTangentColor.x);
#endif
            if(max_depth < fDepth)
            {
                max_depth = fDepth;
                id = i;
            }
        }

        uint nodePackedTangent = LinkedListSRV[pointer].TangentAndCoverage;
        uint nodeDepth         = LinkedListSRV[pointer].depth;
        float fNodeDepth       = asfloat(nodeDepth);
        uint strandColor     = LinkedListSRV[pointer].strandColor;

        // If the node in the linked list is nearer than the furthest one in the local array, exchange the node
        // in the local array for the one in the linked list.
        if (max_depth > fNodeDepth)
        {
#ifdef ALU_INDEXING
            uint tmp                                = GetUintFromIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215, id);
            StoreUintAtIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811,  kBufferDepthV1215, id, nodeDepth);
            fNodeDepth                              = asfloat(tmp);
            tmp                                     = GetUintFromIndex_Size16(kBufferPackedTangentV03, kBufferPackedTangentV47, kBufferPackedTangentV811, kBufferPackedTangentV1215, id);
            StoreUintAtIndex_Size16(kBufferPackedTangentV03, kBufferPackedTangentV47, kBufferPackedTangentV811, kBufferPackedTangentV1215, id, nodePackedTangent);
            nodePackedTangent                       = tmp;
            uint tmpColor                           = GetUintFromIndex_Size16(kBufferStrandColorV03, kBufferStrandColorV47, kBufferStrandColorV811, kBufferStrandColorV1215, id);
            StoreUintAtIndex_Size16(kBufferStrandColorV03, kBufferStrandColorV47, kBufferStrandColorV811,  kBufferDepthV1215, id, strandColor);
            strandColor                             = tmpColor;
#else
            uint tmp                                = kBuffer[id].depthTangentColor.x;
            kBuffer[id].depthTangentColor.x = nodeDepth;
            fNodeDepth                              = asfloat(tmp);
            tmp                                     = kBuffer[id].depthTangentColor.y;
            kBuffer[id].depthTangentColor.y = nodePackedTangent;
            nodePackedTangent                       = tmp;
            uint tmpColor                           = kBuffer[id].depthTangentColor.z;
            kBuffer[id].depthTangentColor.z         = strandColor;
            strandColor                             = tmpColor;
#endif
        }

        // Do simple shading and out of order blending for nodes that are not part of the k closest fragments
        vWorldPosition = mul(float4(In.vPosition.xy, fNodeDepth, 1), g_mInvViewProjViewport);
        vWorldPosition.xyz /= vWorldPosition.www;

#ifdef SIMPLESHADOWING
        amountLight = ComputeSimpleShadow(vWorldPosition.xyz, g_HairShadowAlpha, g_iTechSM);
#else
        amountLight = ComputeShadow(vWorldPosition.xyz, g_HairShadowAlpha, g_iTechSM);
#endif

        fragmentColor.w = GetCoverage(nodePackedTangent);
        vTangent = GetTangent(nodePackedTangent);

#ifdef SIMPLESHADING
        fragmentColor.xyz = SimpleHairShading( vWorldPosition.xyz, vTangent, float4(0,0,0,0), amountLight, UnpackUintIntoFloat4(strandColor).xyz);
#else
        fragmentColor.xyz = ComputeHairShading( vWorldPosition.xyz, vTangent, float4(0,0,0,0), amountLight, UnpackUintIntoFloat4(strandColor).xyz);
#endif

        // Blend the fragment color
        fcolor.xyz = mad(-fcolor.xyz, fragmentColor.w, fcolor.xyz) + fragmentColor.xyz * fragmentColor.w;
        fcolor.w = mad(-fcolor.w, fragmentColor.w, fcolor.w);

        // Retrieve next node pointer
        pointer = LinkedListSRV[pointer].uNext;
    }


    // Blend the k nearest layers of fragments from back to front, where k = KBUFFER_SIZE
    for(int j=0; j<KBUFFER_SIZE; j++)
    {
        int id = 0;
        float max_depth = 0;
        float initialized = 1;

        // find the furthest node in the array
        for(int i=0; i<KBUFFER_SIZE; i++)
        {
#ifdef ALU_INDEXING
            float fDepth = asfloat(GetUintFromIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215, i));
#else
            float fDepth = asfloat(kBuffer[i].depthTangentColor.x);
#endif
            if(max_depth < fDepth)
            {
                max_depth = fDepth;
                id = i;
            }
        }


#ifdef ALU_INDEXING
        uint nodePackedTangent = GetUintFromIndex_Size16(kBufferPackedTangentV03, kBufferPackedTangentV47, kBufferPackedTangentV811, kBufferPackedTangentV1215, id);
        uint nodeDepth         = GetUintFromIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215, id);
        uint strandColor       = GetUintFromIndex_Size16(kBufferStrandColorV03, kBufferStrandColorV47, kBufferStrandColorV811, kBufferStrandColorV1215, id);

        // take this node out of the next search
        StoreUintAtIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215, id, 0);
#else
        uint nodePackedTangent = kBuffer[id].depthTangentColor.y;
        uint nodeDepth         = kBuffer[id].depthTangentColor.x;
        uint strandColor       = kBuffer[id].depthTangentColor.z;

        // take this node out of the next search
        kBuffer[id].depthTangentColor.x = 0;
#endif

        // Use high quality shading for the nearest k fragments
        float fDepth = asfloat(nodeDepth);
        vWorldPosition = mul(float4(In.vPosition.xy, fDepth, 1), g_mInvViewProjViewport);
        vWorldPosition.xyz /= vWorldPosition.www;

        amountLight = ComputeShadow(vWorldPosition.xyz, g_HairShadowAlpha, g_iTechSM);

        // Get tangent and coverage
        vTangent        = GetTangent(nodePackedTangent);
        fragmentColor.w = GetCoverage(nodePackedTangent);

        // Shading
        fragmentColor.xyz = ComputeHairShading( vWorldPosition.xyz, vTangent, float4(0,0,0,0), amountLight, UnpackUintIntoFloat4(strandColor).xyz);

        // Blend the fragment color
        fcolor.xyz = mad(-fcolor.xyz, fragmentColor.w, fcolor.xyz) + fragmentColor.xyz * fragmentColor.w;
        fcolor.w = mad(-fcolor.w, fragmentColor.w, fcolor.w);

    }


#ifdef COLORDEBUG
    fcolor.xyz = float3(0,1,0);
    if (nNumFragments>32) fcolor.xyz = float3(1,1,0);
    if (nNumFragments>64) fcolor.xyz = float3(1,0.5,0);
    if (nNumFragments>128) fcolor.xyz = float3(1,0,0);
#endif

    return fcolor;
}

//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------

// Convert 2D address to 1D address
uint GetAddress(int2 vAddress)
{
    int nAddress = vAddress.y * (int) g_WinSize.x + vAddress.x;
    return nAddress;
}






[earlydepthstencil]
float PS_Depth_Hair(PS_INPUT_HAIR In) : SV_Target
{
    // Render AA Line, calculate pixel coverage
    float4 proj_pos = float4(2 * In.Position.x*g_WinSize.z - 1.0,  // g_WinSize.z = 1.0/g_WinSize.x
    1 - 2 * In.Position.y*g_WinSize.w,    // g_WinSize.w = 1.0/g_WinSize.y
        In.Position.z,
        1);

    float curve_scale = 1;
    if (g_bThinTip > 0)
        curve_scale = In.Tangent.w;
    
    float fiber_radius = curve_scale * g_FiberRadius;
    
    float coverage = 1.f;
    if (g_bUseCoverage)
    {
        coverage = ComputeCoverage(In.p0p1.xy, In.p0p1.zw, proj_pos.xy);
    }
    
    float alpha = coverage * g_FiberAlpha;
    
    if (alpha < g_alphaThreshold)
        return 1.0;
    
    int2 vScreenAddress = int2(In.Position.xy);

    uint uDepth = asuint(In.Position.z);
    uint uDepth0Prev, uDepth1Prev;
    
    // Min of depth 0 and input depth
    // Original value is uDepth0Prev
    InterlockedMin(RWFragmentDepthsTexture[uint3(vScreenAddress, 0)], uDepth, uDepth0Prev);

    // Min of depth 1 and greater of the last compare
    // If fragment opaque, always use input depth (don't need greater depths)
    uDepth = (alpha > 0.98) ? uDepth : max(uDepth, uDepth0Prev);
    
    InterlockedMin(RWFragmentDepthsTexture[uint3(vScreenAddress, 1)], uDepth, uDepth1Prev);

#if SHORTCUT_NUM_DEPTHS == 3
    uint uDepth2Prev;
    
    // Min of depth 2 and greater of the last compare
    // If fragment opaque, always use input depth (don't need greater depths)
    uDepth = (alpha > 0.98) ? uDepth : max(uDepth, uDepth1Prev);
    
    InterlockedMin(RWFragmentDepthsTexture[uint3(vScreenAddress, 2)], uDepth, uDepth2Prev);
#endif
    
    return 1.0 - alpha;
}



[earlydepthstencil]
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
float4 PS_FillColors_Hair(PS_INPUT_HAIR In) : SV_Target
#else
void PS_FillColors_Hair(PS_INPUT_HAIR In)
#endif
{
    // Render AA Line, calculate pixel coverage
    float4 proj_pos = float4(2 * In.Position.x*g_WinSize.z - 1.0,  // g_WinSize.z = 1.0/g_WinSize.x
    1 - 2 * In.Position.y*g_WinSize.w,    // g_WinSize.w = 1.0/g_WinSize.y
        In.Position.z,
        1);

    float4 world_pos = mul(proj_pos, g_mInvViewProj);
    world_pos.xyz = world_pos.xyz / world_pos.w;
    world_pos.w = 1.0;
    
    float curve_scale = 1;
    if (g_bThinTip > 0)
    curve_scale = In.Tangent.w;
    
    float fiber_radius = curve_scale * g_FiberRadius;
    
    float coverage = 1.f;
    if (g_bUseCoverage)
    {
        coverage = ComputeCoverage(In.p0p1.xy, In.p0p1.zw, proj_pos.xy);
    }
    
    float alpha = coverage * g_FiberAlpha;
    
    if (alpha < g_alphaThreshold)
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
        return float4(0, 0, 0, 0);
#else
        return;
#endif

    int2 vScreenAddress = int2(In.Position.xy);
    uint fragmentIndex = GetAddress(vScreenAddress);
    
    uint uDepth = asuint(In.Position.z);

    uint uDepth0 = FragmentDepthsTexture[uint3(vScreenAddress, 0)];
    uint uDepth1 = FragmentDepthsTexture[uint3(vScreenAddress, 1)];
#if SHORTCUT_NUM_DEPTHS == 3
    uint uDepth2 = FragmentDepthsTexture[uint3(vScreenAddress, 2)];
#endif

    // Shade regardless of depth, since ResolveDepth pass writes one of the near depths

    float amountLight = ComputeShadow(world_pos.xyz, g_HairShadowAlpha, g_iTechSM);
    float3 color = ComputeHairShading(world_pos.xyz, In.Tangent.xyz, float4(0, 0, 0, 0), amountLight, In.strandColor.xyz);
    uint uColor = PackFloat4IntoUint(float4(color, alpha));

#if SHORTCUT_DETERMINISTIC  // deterministic result when fragments match in depth, with added cost

#if SHORTCUT_WEIGHTED_AVERAGE
    return float4(color * alpha, alpha);
#else
    // For fragments of equal depth, keep colors with greatest uint value
    if (uDepth == uDepth0)
    {
        uint uColorPrev;
        InterlockedMax(RWFragmentColors[fragmentIndex].x, uColor, uColorPrev);
        uColor = min(uColor, uColorPrev);
    }
    if (uDepth == uDepth1)
    {
        uint uColorPrev;
        InterlockedMax(RWFragmentColors[fragmentIndex].y, uColor, uColorPrev);
        uColor = min(uColor, uColorPrev);
    }
#if SHORTCUT_NUM_DEPTHS == 3
    if (uDepth == uDepth2)
    {
        uint uColorPrev;
        InterlockedMax(RWFragmentColors[fragmentIndex].z, uColor, uColorPrev);
        uColor = min(uColor, uColorPrev);
    }
#endif
#endif

#else // !SHORTCUT_DETERMINISTIC

    if (uDepth == uDepth0)
    {
        RWFragmentColors[fragmentIndex].x = uColor;
        uColor = 0;
    }
    if (uDepth == uDepth1)
    {
        RWFragmentColors[fragmentIndex].y = uColor;
        uColor = 0;
    }
#if SHORTCUT_NUM_DEPTHS == 3
    if (uDepth == uDepth2)
    {
        RWFragmentColors[fragmentIndex].z = uColor;
    }
#endif

#endif
}


float PS_ResolveDepth_Hair(VS_OUTPUT_SCREENQUAD In) : SV_DEPTH
{
        int2 vScreenAddress = int2(In.vPosition.xy);

        uint uDepth = 0;

        // NOTE: A bug in FXC in the Win 8.x SDKs prevents
        // this shader from compiling if depth is read from a 
        // structured buffer.  A texture array is used instead.

#if SHORTCUT_NUM_DEPTHS == 3
        uDepth = FragmentDepthsTexture[uint3(vScreenAddress, 2)];
#else
        uDepth = FragmentDepthsTexture[uint3(vScreenAddress, 1)];
#endif

        return asfloat(uDepth);
}


[earlydepthstencil]
float4 PS_ResolveColor_Hair(VS_OUTPUT_SCREENQUAD In) : SV_TARGET
{
    int2 vScreenAddress = int2(In.vPosition.xy);
    uint fragmentIndex = GetAddress(vScreenAddress);

    float fInvAlpha = tAccumInvAlpha[vScreenAddress];
    float fAlpha = 1.0 - fInvAlpha;

    if (fAlpha < SHORTCUT_MIN_ALPHA)
        return float4(0, 0, 0, 1);

#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
    float4 fcolor;
    float colorSumX = FragmentColors[vScreenAddress].x;
    float colorSumY = FragmentColors[vScreenAddress].y;
    float colorSumZ = FragmentColors[vScreenAddress].z;
    float colorSumW = FragmentColors[vScreenAddress].w;
    fcolor.x = colorSumX / colorSumW;
    fcolor.y = colorSumY / colorSumW;
    fcolor.z = colorSumZ / colorSumW;
    fcolor.xyz *= fAlpha;
#else
    uint uDepth0 = FragmentDepthsTexture[uint3(vScreenAddress, 0)];
    uint uDepth1 = FragmentDepthsTexture[uint3(vScreenAddress, 1)];
    uint uColor0 = FragmentColors[fragmentIndex].x;
    uint uColor1 = FragmentColors[fragmentIndex].y;

    float4 color0 = UnpackUintIntoFloat4(uColor0);
    float alpha0 = color0.w;
    float invAlpha0 = 1.0 - alpha0;

    float4 color1 = (uDepth1 == SHORTCUT_INITIAL_DEPTH) ? float4(0, 0, 0, 0) : UnpackUintIntoFloat4(uColor1);
    float alpha1 = color1.w;
    float invAlpha1 = 1.0 - alpha1;

#if SHORTCUT_NUM_DEPTHS == 3
    uint uDepth2 = FragmentDepthsTexture[uint3(vScreenAddress, 2)];
    uint uColor2 = FragmentColors[fragmentIndex].z;

    float4 color2 = (uDepth2 == SHORTCUT_INITIAL_DEPTH) ? float4(0, 0, 0, 0) : UnpackUintIntoFloat4(uColor2);
    float alpha2 = color2.w;
#endif

    float4 fcolor;

#if SHORTCUT_WEIGHTED_AVERAGE
#if SHORTCUT_NUM_DEPTHS == 3
    fcolor.xyz = fAlpha * (color0.xyz * alpha0 + color1.xyz * alpha1 + color2.xyz * alpha2) / (alpha0 + alpha1 + alpha2);
#else
    fcolor.xyz = fAlpha * (color0.xyz * alpha0 + color1.xyz * alpha1) / (alpha0 + alpha1);
#endif
#else
#if SHORTCUT_NUM_DEPTHS == 3
    float invAlpha01 = invAlpha0 * invAlpha1;

    // Get cumulative alpha for all fragments after front two; apply to back fragment
    float fInvAlphaBack = invAlpha01 < 0.0001 ? fInvAlpha : fInvAlpha / invAlpha01;
    float fAlphaBack = 1.0 - fInvAlphaBack;

    float3 backColor = (uColor2 == 0) ? ((uColor1 == 0) ? color0.xyz : color1.xyz) : color2.xyz;

    // Blend is:
    // alpha0 * color0 + (1 - alpha0) * (alpha1 * color1 + (1 - alpha1) * (fAlphaBack * backColor + fInvAlphaBack * dstcolor))
    fcolor.xyz = color0.xyz * alpha0 + color1.xyz * (1 - alpha0) * alpha1 + backColor.xyz * (1 - alpha0) * (1 - alpha1) * fAlphaBack;
#else
    // Get cumulative alpha for all fragments after front; apply to back fragment
    float fInvAlphaBack = invAlpha0 < 0.0001 ? fInvAlpha : fInvAlpha / invAlpha0;
    float fAlphaBack = 1.0 - fInvAlphaBack;

    float3 backColor = (uColor1 == 0) ? color0.xyz : color1.xyz;

    // Blend is:
    // alpha0 * color0 + (1 - alpha0) * (fAlphaBack * backColor + fInvAlphaBack * dstcolor))
    fcolor.xyz = color0.xyz * alpha0 + backColor * (1 - alpha0) * fAlphaBack;
#endif
#endif

#endif

    fcolor.w = fInvAlpha;
    return fcolor;

}








