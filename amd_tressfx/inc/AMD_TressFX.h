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

#ifndef AMD_TRESSFX_H
#define AMD_TRESSFX_H

#include <d3d11.h>
#include <DirectXMath.h>

#define AMD_TRESSFX_VERSION_MAJOR                    3
#define AMD_TRESSFX_VERSION_MINOR                    1
#define AMD_TRESSFX_VERSION_PATCH                    1

// default to static lib
#ifndef AMD_TRESSFX_COMPILE_DYNAMIC_LIB
#define AMD_TRESSFX_COMPILE_DYNAMIC_LIB   0
#endif

#if AMD_TRESSFX_COMPILE_DYNAMIC_LIB
#    ifdef AMD_DLL_EXPORTS
#        define AMD_TRESSFX_DLL_API __declspec(dllexport)
#    else // AMD_DLL_EXPORTS
#        define AMD_TRESSFX_DLL_API __declspec(dllimport)
#    endif // AMD_DLL_EXPORTS
#else // AMD_TRESSFX_COMPILE_DYNAMIC_LIB
#    define AMD_TRESSFX_DLL_API
#endif // AMD_TRESSFX_COMPILE_DYNAMIC_LIB

#include "AMD_Types.h"

#define MAX_SHAPE_PARAMS    16
#define MAX_SCENE_MESHES    32

#define NONE_SHADOW_INDEX   0x00000000
#define SDSM_SHADOW_INDEX   0x00000002

#define ALPHA_THRESHOLD     0.99f/255.0f    // minimum alpha value required to render a hair fragment

// shader resource view slots
#define IDSRV_PPLL                              0
#define IDSRV_HEAD_PPLL                         1
#define IDSRV_SCENESM                           2
#define IDSRV_HAIRSM                            3
#define IDSRV_HAIR_COLOR_TEXTURE                4
#define IDSRV_NOISEMAP                          5
#define IDSRV_HAIR_THICKNESSES                  6
#define IDSRV_HAIR_VERTEX_POSITIONS             7
#define IDSRV_HAIR_TANGENTS                     8
#define IDSRV_HAIR_TRANSFORMS                   9
#define IDSRV_HAIR_STRAND_TEX_COORDS            10
#define IDSRV_HAIR_VERTEX_TEX_COORDS            11
#define IDSRV_HAIR_VERTEX_COLORS                12

// Shortcut resources.
#define IDSRV_HAIR_FRAGMENT_DEPTHS              13
#define IDSRV_HAIR_FRAGMENT_COLORS              14
#define IDSRV_HAIR_ACCUM_INV_ALPHA              15


#if defined(DEBUG) || defined(_DEBUG)
#define AMD_TRESSFX_DEBUG                       1
#endif

namespace AMD
{

// The Return codes
enum TRESSFX_RETURN_CODE
{
    TRESSFX_RETURN_CODE_SUCCESS,
    TRESSFX_RETURN_CODE_FAIL,
    TRESSFX_RETURN_CODE_INVALID_DEVICE,
    TRESSFX_RETURN_CODE_INVALID_DEVICE_CONTEXT,
    TRESSFX_RETURN_CODE_INVALID_ARGUMENT,
    TRESSFX_RETURN_CODE_INVALID_POINTER,
    TRESSFX_RETURN_CODE_D3D11_CALL_FAILED,

    TRESSFX_RETURN_CODE_COUNT,
};

struct TressFX_HairParams
{
    float               Ka;                         // ambient
    float               Kd;                         // diffuse
    float               Ks1;                        // specular intensity 1
    float               Ex1;                        // specular exponent 1
    float               Ks2;                        // specular intensity 2
    float               Ex2;                        // specular exponent 2
    float               thickness;                  // hair strand radius (thickness)
    float               density;                    // density of hair strands (between [0, 1])
    float               alpha;                      // hair transparency
    float               alphaThreshold;             // cutoff when hair fragment is rendered based on transparency
    bool                bTransparency;              // enable/disable hair transparency
    int                 strandCopies;              // number of strands to duplicate
    float               duplicateStrandSpacing;     // distance between hairs duplicated hairs
    bool                bThinTip;                   // if true, the end of the hair is tapered
    DirectX::XMFLOAT3   color;                      // hair color
    int                 shadowTechnique;            // hair self shadowing technique
    float               shadowMapAlpha;             // shadow map transparency value
    bool                bSimulation;                // enable/disable simulation
    float               targetFrameRateForSimulation; // i.e 1/60
    bool                bAntialias;                 // enable/disable antialiasing
    int                 maxFragments;              // maximum number of overlapping fragments to render per pixel
    DirectX::XMFLOAT4   ambientLightColor;          // ambient light color
    DirectX::XMFLOAT4   pointLightColor;            // color of the point light
};

struct TressFX_ShapeParams
{
    float               damping;
    float               stiffnessForLocalShapeMatching;
    float               stiffnessForGlobalShapeMatching;
    float               globalShapeMatchingEffectiveRange;
};

struct TressFX_SimulationParams
{
    float                       gravityMagnitude;
    bool                        bCollision;
    bool                        bGuideFollowSimulation;
    DirectX::XMFLOAT3           windDir;
    float                       windMag;
    int                         numLengthConstraintIterations;
    int                         numLocalShapeMatchingIterations;

    static const int            MAX_NUM_HAIR_SECTIONS = 4;
    TressFX_ShapeParams         perSectionShapeParams[MAX_NUM_HAIR_SECTIONS];
    int                         numHairSections;
};

struct TressFX_CollisionCapsule
{
    DirectX::XMFLOAT4   cc0_center1AndRadius;
    DirectX::XMFLOAT4   cc0_center2AndRadiusSquared;
    DirectX::XMFLOAT4   cc1_center1AndRadius;
    DirectX::XMFLOAT4   cc1_center2AndRadiusSquared;
    DirectX::XMFLOAT4   cc2_center1AndRadius;
    DirectX::XMFLOAT4   cc2_center2AndRadiusSquared;
};

struct TressFX_HairBlob
{
    unsigned    size;
    void*       pHair;
    unsigned    animationSize;
    void*       pAnimation;
};

struct TressFX_SceneMesh
{
    ID3D11ShaderResourceView*   pMeshVertices;     // untransformed vertices
    unsigned                    numMeshes;         // number of meshes
    unsigned*                   meshOffsets;       // offset to the start of each mesh
    ID3D11ShaderResourceView*   pTransformedVerts; // transformed vertices
};

struct TressFX_HairTransform
{
    float   matrix[4][4];
    float   quaternion[4];
};

struct TressFX_GuideFollowParams
{
    int numFollowHairsPerGuideHair; // follow-to-guide ratio. It could be zero.
    float radiusForFollowHair;      // radius around guide strand to grow follow strands
    float tipSeparationFactor;      // follow strand tip separation factor from guide strand.
    // If zero, follow hair would be parallel to its guide. If one, the tip will
    // be away from its guide's tip by twice of the distance between their roots.
};

struct TressFX_OpaqueDesc;

struct TressFX_Desc
{
    TressFX_Desc() : pOpaque(NULL) {}

    TressFX_OpaqueDesc*             pOpaque;

    int                             backBufferWidth;
    int                             backBufferHeight;

    TressFX_HairParams              hairParams;
    TressFX_SimulationParams        simulationParams;
    TressFX_CollisionCapsule        collisionCapsule;
    TressFX_HairBlob                tressFXHair;

    void *                          pTressFXMesh;
    unsigned                        groupID;

    DirectX::XMMATRIX               mWorld;
    DirectX::XMMATRIX               mViewProj;
    DirectX::XMMATRIX               mInvViewProj;

    // this gets initialized by the library in AMD_TressFXBegin
    DirectX::XMMATRIX               mViewProjLightFromLibrary;

    DirectX::XMVECTOR               eyePoint;
    DirectX::XMVECTOR               lightPosition;
    DirectX::XMMATRIX               modelTransformForHead;
    float                           targetFrameRate;
    int                             numTotalHairStrands;
    int                             numTotalHairVertices;
    bool                            bEnableSkinning;
    bool                            bSingleHeadTransform;
    bool                            bShortCutOn;

    // Buffer of transformations (one transform per strand) for hair skinning
    // This UAV is used as a structured buffer where each element is a TressFX_HairTransform.
    // The number of elements in the buffer is numTotalHairStrands.
    ID3D11UnorderedAccessView*      pSkinningTransformationsUAV;

    // hair shadow map
    ID3D11ShaderResourceView*       pHairShadowMapSRV;

    ID3D11Device*                   pd3dDevice;
    ID3D11DeviceContext*            pd3dDeviceContext;
    ID3D11ShaderResourceView*       pd3dDepthSRV;
    ID3D11RenderTargetView*         pd3dOutputRTV;
};

extern "C"
{
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_GetVersion(uint* major, uint* minor, uint* patch);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_Initialize(TressFX_Desc & desc);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_LoadRawAsset(TressFX_Desc & desc,   const TressFX_GuideFollowParams& guideFollowParams, TressFX_HairBlob *pRawHairBlob);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_LoadProcessedAsset(TressFX_Desc & desc, TressFX_HairBlob *pHairBlob, TressFX_SceneMesh *sceneMesh, ID3D11ShaderResourceView *pTextureSRV);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_CreateProcessedAsset(TressFX_Desc & desc, TressFX_HairBlob **ppHairBlob, TressFX_SceneMesh *sceneMesh, ID3D11ShaderResourceView *pTextureSRV);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_Begin(TressFX_Desc & desc);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_End(TressFX_Desc & desc);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_GenerateTransforms(TressFX_Desc & desc, TressFX_SceneMesh &sceneMesh);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_ApplyRigidTransforms(TressFX_Desc & desc);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_Simulate(TressFX_Desc & desc, float elapsedTime);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_RenderShadowMap(TressFX_Desc & desc);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_Render(TressFX_Desc & desc);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_Resize(TressFX_Desc & desc);
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_Release(TressFX_Desc & desc);
}

} // namespace AMD

#endif // AMD_TRESSFX_H
