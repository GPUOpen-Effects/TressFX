
//---------------------------------------------------------------------------------------
// Shader code related to ShortCut method.
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

StartHLSL TressFXShortCut_Common

#define SHORTCUT_MIN_ALPHA 0.02

// Constants must match in TressFXShortCut.h

// Clear value for depths resource
#define SHORTCUT_INITIAL_DEPTH 0x3f800000

    float4x4 g_mInvViewProj;
    float4 g_vViewport;
    float3 g_vEye;

    float4 vFragmentBufferSize;

    // Convert 1D address to 2D address
    int2 GetAddress( int nAddress )
    {
        uint nBufferWidth = vFragmentBufferSize.x;
        int2 vAddress = int2( nAddress % nBufferWidth, nAddress / nBufferWidth );
        return vAddress;
    }

    // Convert 2D address to 1D address
    uint GetAddress( int2 vAddress )
    {
        int nAddress = vAddress.y * vFragmentBufferSize.x + vAddress.x;
        return nAddress;
    }

    uint PackFloat4IntoUint(float4 vValue)
    {
        return ( ((uint)(vValue.x*255)) << 24 ) | ( ((uint)(vValue.y*255)) << 16 ) | ( ((uint)(vValue.z*255)) << 8) | (uint)(vValue.w * 255);
    }

    float4 UnpackUintIntoFloat4(uint uValue)
    {
        return float4( ( (uValue & 0xFF000000)>>24 ) / 255.0, ( (uValue & 0x00FF0000)>>16 ) / 255.0, ( (uValue & 0x0000FF00)>>8 ) / 255.0, ( (uValue & 0x000000FF) ) / 255.0);
    }

    float3 ScreenPosToNDC(float3 vScreenPos, float4 viewport)
    {
        float2 xy = vScreenPos.xy;

        // add viewport offset.
        xy += viewport.xy;

        // scale by viewport to put in 0 to 1
        xy /= viewport.zw;

        // shift and scale to put in -1 to 1. y is also being flipped.
        xy.x = (2 * xy.x) - 1;
        xy.y = 1 - (2 * xy.y);

        return float3(xy, vScreenPos.z);
    }

EndHLSL
        

StartHLSL TressFXShortCut_DepthsAlphaPS

    RWTexture2DArray<uint> RWFragmentDepthsTexture;

    struct PS_INPUT_HAIR_AA
    {
        float4 Position : SV_POSITION;
        float4 Tangent : Tangent;
        float4 p0p1 : TEXCOORD0;
        float3 strandColor : TEXCOORD1;
    };

    // First pass of ShortCut.
    // Geometry pass that stores the 3 front fragment depths, and accumulates product of 1-alpha values in the render target.
    [earlydepthstencil]
    float main(PS_INPUT_HAIR_AA input) : SV_Target
    {
        float3 vNDC = ScreenPosToNDC(input.Position.xyz, g_vViewport);
        float coverage = ComputeCoverage(input.p0p1.xy, input.p0p1.zw, vNDC.xy, g_vViewport.zw);
        float alpha = coverage*g_MatBaseColor.a;

        ASSERT(coverage >= 0)
        if (alpha < SHORTCUT_MIN_ALPHA)
            return 1.0;

        int2 vScreenAddress = int2(input.Position.xy);

        uint uDepth = asuint(input.Position.z);
        uint uDepth0Prev, uDepth1Prev;

        // Min of depth 0 and input depth
        // Original value is uDepth0Prev
        InterlockedMin(RWFragmentDepthsTexture[uint3(vScreenAddress, 0)], uDepth, uDepth0Prev);

        // Min of depth 1 and greater of the last compare
        // If fragment opaque, always use input depth (don't need greater depths)
        uDepth = (alpha > 0.98) ? uDepth : max(uDepth, uDepth0Prev);

        InterlockedMin(RWFragmentDepthsTexture[uint3(vScreenAddress, 1)], uDepth, uDepth1Prev);

        uint uDepth2Prev;

        // Min of depth 2 and greater of the last compare
        // If fragment opaque, always use input depth (don't need greater depths)
        uDepth = (alpha > 0.98) ? uDepth : max(uDepth, uDepth1Prev);

        InterlockedMin(RWFragmentDepthsTexture[uint3(vScreenAddress, 2)], uDepth, uDepth2Prev);

        return 1.0 - alpha;
    }

EndHLSL


StartHLSL TressFXShortCut_ResolveDepthPS

    Texture2DArray<uint> FragmentDepthsTexture;

    struct VS_OUTPUT_SCREENQUAD
    {
        float4 vPosition : SV_POSITION;
        float2 vTex      : TEXCOORD;
    };

    // Second pass of ShortCut.
    // Full-screen pass that writes the farthest of the near depths for depth culling.
    float main(VS_OUTPUT_SCREENQUAD input) : SV_Depth
    {
        // Blend the layers of fragments from back to front
        int2 vScreenAddress = int2(input.vPosition.xy);

        // Write farthest depth value for culling in the next pass.
        // It may be the initial value of 1.0 if there were not enough fragments to write all depths, but then culling not important.
        uint uDepth = FragmentDepthsTexture[uint3(vScreenAddress, 2)];

        return asfloat(uDepth);
    }

EndHLSL


StartHLSL TressFXShortCut_FillColorsPS

#ifndef HEAD_SHADING

    float4 TressFXHeadColor(float2 pixelCoord, float depth, float4 vTangentCoverage, float3 baseColor)
    {
        return float4(baseColor, vTangentCoverage.w);
    }

#define HEAD_SHADING TressFXHeadColor

#endif

    struct PS_INPUT_HAIR_AA
    {
        float4 Position : SV_POSITION;
        float4 Tangent : Tangent;
        float4 p0p1 : TEXCOORD0;
        float3 strandColor : TEXCOORD1;
    };

    // Third pass of ShortCut.
    // Geometry pass that shades pixels passing early depth test.  Limited to near fragments due to previous depth write pass.
    // Colors are accumulated in render target for a weighted average in final pass.
    [earlydepthstencil]
    float4 main(PS_INPUT_HAIR_AA input) : SV_Target
    {
        float3 vNDC = ScreenPosToNDC(input.Position.xyz, g_vViewport);
        float coverage = ComputeCoverage(input.p0p1.xy, input.p0p1.zw, vNDC.xy, g_vViewport.zw);
        float alpha = coverage*g_MatBaseColor.a;

        ASSERT(coverage >= 0)
        if(alpha < SHORTCUT_MIN_ALPHA)
            return float4(0, 0, 0, 0);

        int2 vScreenAddress = int2(input.Position.xy);

        uint uDepth = asuint(input.Position.z);

        // Shade regardless of depth, since ResolveDepth pass writes one of the near depths

        float3 vPositionWS = NDCToWorld(vNDC, g_mInvViewProj);

        float4 rgbaColor = HEAD_SHADING(input.Position.xy, input.Position.z, float4(input.Tangent.xyz*0.5 + float3(0.5, 0.5, 0.5), alpha), input.strandColor.xyz);
        float3 color = rgbaColor.xyz;
        uint uColor = PackFloat4IntoUint(float4(color, alpha));

        return float4(color * alpha, alpha);
    }

EndHLSL


StartHLSL TressFXShortCut_ResolveColorPS

    Texture2DArray<uint> FragmentDepthsTexture;
    Texture2D<float4> FragmentColorsTexture;
    Texture2D<float> tAccumInvAlpha;

    struct VS_OUTPUT_SCREENQUAD
    {
        float4 vPosition : SV_POSITION;
        float2 vTex      : TEXCOORD;
    };

    // Fourth pass of ShortCut.
    // Full-screen pass that finalizes the weighted average, and blends using the accumulated 1-alpha product.
    [earlydepthstencil]
    float4 main(VS_OUTPUT_SCREENQUAD input) : SV_Target
    {
        int2 vScreenAddress = int2(input.vPosition.xy);
        uint fragmentIndex = GetAddress(vScreenAddress);

        float fInvAlpha = tAccumInvAlpha[vScreenAddress];
        float fAlpha = 1.0 - fInvAlpha;

       if (fAlpha < SHORTCUT_MIN_ALPHA)
            return float4(0, 0, 0, 1);

        float4 fcolor;
        float colorSumX = FragmentColorsTexture[vScreenAddress].x;
        float colorSumY = FragmentColorsTexture[vScreenAddress].y;
        float colorSumZ = FragmentColorsTexture[vScreenAddress].z;
        float colorSumW = FragmentColorsTexture[vScreenAddress].w;
        fcolor.x = colorSumX / colorSumW;
        fcolor.y = colorSumY / colorSumW;
        fcolor.z = colorSumZ / colorSumW;
        fcolor.xyz *= fAlpha;
        fcolor.w = fInvAlpha;
        return fcolor;
    }

EndHLSL
