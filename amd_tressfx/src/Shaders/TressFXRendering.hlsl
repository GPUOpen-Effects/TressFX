//---------------------------------------------------------------------------------------
// Shader code related to lighting and shadowing for TressFX
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

StartHLSL TressFXRendering



#define AMD_PI 3.1415926
#define AMD_e 2.71828183

#define AMD_TRESSFX_KERNEL_SIZE 5

// We might break this down further.
cbuffer tressfxShadeParameters
{
    //float       g_FiberAlpha        ; // Is this redundant with g_MatBaseColor.a?
    float       g_HairShadowAlpha   ;
    float       g_FiberRadius       ;
    float       g_FiberSpacing      ;

    float4      g_MatBaseColor      ;
    float4      g_MatKValue         ;

    float       g_fHairKs2          ;
    float       g_fHairEx2          ;
    int g_NumVerticesPerStrand      ;  

    //float padding_ 
}


struct HairShadeParams
{
    float3 cColor;
    float fRadius;
    float fSpacing;
    float fAlpha;
};


// fDepthDistanceWS is the world space distance between the point on the surface and the point in the shadow map.
// fFiberSpacing is the assumed, average, world space distance between hair fibers.
// fFiberRadius in the assumed average hair radius.
// fHairAlpha is the alpha value for the hair (in terms of translucency, not coverage.)
// Output is a number between 0 (totally shadowed) and 1 (lets everything through)
float ComputeShadowAttenuation(float fDepthDistanceWS, float fFiberSpacing, float fFiberRadius, float fHairAlpha)
{
    float numFibers = fDepthDistanceWS / (fFiberSpacing*fFiberRadius);

    // if occluded by hair, there is at least one fiber
    [flatten]if (fDepthDistanceWS > 1e-5)
        numFibers = max(numFibers, 1);
    return pow(abs(1 - fHairAlpha), numFibers);
}

float ComputeShadowAttenuation(float fDepthDistanceWS, HairShadeParams params)
{
    return ComputeShadowAttenuation(fDepthDistanceWS, params.fSpacing, params.fRadius, params.fAlpha);
}


float ComputeHairShadowAttenuation(float fDepthDistanceWS, float fFiberSpacing, float fFiberRadius, float fHairAlpha)
{
    float numFibers =  fDepthDistanceWS/(fFiberSpacing*fFiberRadius);

    fHairAlpha*=0.5;

    // if occluded by hair, there is at least one fiber
    [flatten]if (fDepthDistanceWS > 1e-5)
        numFibers = max(numFibers,1);
    return pow(abs(1-fHairAlpha), numFibers);	
}


float ComputeHairShadowAttenuation(float fDepthDistanceWS)
{
    return ComputeHairShadowAttenuation(fDepthDistanceWS, g_FiberRadius, g_FiberSpacing, g_HairShadowAlpha);
}


// Returns a float3 which is the scale for diffuse, spec term, and colored spec term.
//
// The diffuse term is from Kajiya.
// 
// The spec term is what Marschner calls "R", reflecting directly off the surface of the hair, 
// taking the color of the light like a dielectric specular term.  This highlight is shifted 
// towards the root of the hair.
//
// The colored spec term is caused by light passing through the hair, bouncing off the back, and 
// coming back out.  It therefore picks up the color of the light.  
// Marschner refers to this term as the "TRT" term.  This highlight is shifted towards the 
// tip of the hair.
//
// vEyeDir, vLightDir and vTangentDir are all pointing out.
// coneAngleRadians explained below.
//
// 
// hair has a tiled-conical shape along its lenght.  Sort of like the following.
// 
// \    /
//  \  /
// \    /
//  \  /  
//
// The angle of the cone is the last argument, in radians.  
// It's typically in the range of 5 to 10 degrees
float3 TressFX_ComputeDiffuseSpecFactors(float3 vEyeDir, float3 vLightDir, float3 vTangentDir, float coneAngleRadians = 10*AMD_PI/180 )
{

    // From old code.
    float Kd = g_MatKValue.y;
    float Ks1 = g_MatKValue.z;
    float Ex1 = g_MatKValue.w;
    float Ks2 = g_fHairKs2;
    float Ex2 = g_fHairEx2;

    // in Kajiya's model: diffuse component: sin(t, l)
    float cosTL = (dot(vTangentDir, vLightDir));
    float sinTL = sqrt(1 - cosTL*cosTL);
    float diffuse = sinTL; // here sinTL is apparently larger than 0

    float cosTRL = -cosTL;
    float sinTRL = sinTL;
    float cosTE = (dot(vTangentDir, vEyeDir));
    float sinTE = sqrt(1 - cosTE*cosTE);

    // primary highlight: reflected direction shift towards root (2 * coneAngleRadians)
    float cosTRL_root = cosTRL * cos(2 * coneAngleRadians) - sinTRL * sin(2 * coneAngleRadians);
    float sinTRL_root = sqrt(1 - cosTRL_root * cosTRL_root);
    float specular_root = max(0, cosTRL_root * cosTE + sinTRL_root * sinTE);

    // secondary highlight: reflected direction shifted toward tip (3*coneAngleRadians)
    float cosTRL_tip = cosTRL*cos(-3 * coneAngleRadians) - sinTRL*sin(-3 * coneAngleRadians);
    float sinTRL_tip = sqrt(1 - cosTRL_tip * cosTRL_tip);
    float specular_tip = max(0, cosTRL_tip * cosTE + sinTRL_tip * sinTE);

    return float3(Kd*diffuse, Ks1 * pow(specular_root, Ex1), Ks2 * pow(specular_tip, Ex2));
}


//--------------------------------------------------------------------------------------
// ComputeCoverage
//
// Calculate the pixel coverage of a hair strand by computing the hair width
//--------------------------------------------------------------------------------------
float ComputeCoverage(float2 p0, float2 p1, float2 pixelLoc, float2 winSize)
{
    // p0, p1, pixelLoc are in d3d clip space (-1 to 1)x(-1 to 1)

    // Scale positions so 1.f = half pixel width
    p0 *= winSize;
    p1 *= winSize;
    pixelLoc *= winSize;

    float p0dist = length(p0 - pixelLoc);
    float p1dist = length(p1 - pixelLoc);
    float hairWidth = length(p0 - p1);

    // will be 1.f if pixel outside hair, 0.f if pixel inside hair
    float outside = any(float2(step(hairWidth, p0dist), step(hairWidth, p1dist)));

    // if outside, set sign to -1, else set sign to 1
    float sign = outside > 0.f ? -1.f : 1.f;

    // signed distance (positive if inside hair, negative if outside hair)
    float relDist = sign * saturate(min(p0dist, p1dist));

    // returns coverage based on the relative distance
    // 0, if completely outside hair edge
    // 1, if completely inside hair edge
    return (relDist + 1.f) * 0.5f;
}







EndHLSL

