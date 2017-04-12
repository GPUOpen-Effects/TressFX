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

#ifndef _SU_LIGHTVISUALIZER_H_
#define _SU_LIGHTVISUALIZER_H_

#include "SuCountedPtr.h"
#include "SuEffectReloadListener.h"
#include "SuTypes.h"

// forward declarations
class SuGPUVertexBuffer;
class SuGPUIndexBuffer;
class SuEffect;

typedef SuCountedPtr<SuGPUIndexBuffer>  SuGPUIndexBufferPtr;
typedef SuCountedPtr<SuGPUVertexBuffer> SuGPUVertexBufferPtr;
typedef SuCountedPtr<SuEffect>          SuEffectPtr;

class SuLight;
class SuPointLight;
class SuSpotLight;
class SuVolumeLight;
class SuAreaLight;

//=========================================================================================================
/// \ingroup Engine
/// \brief Class to draw light sources
///
//=========================================================================================================
// tolua_begin
class SuLightVisualizer : virtual public SuEffectReloadListener
{
public:
    /// Constructor
    SuLightVisualizer();

    /// Destructor
    virtual ~SuLightVisualizer();

    /// Draws a light
    void DrawLight(const SuLight* pLight) const;

    /// Implements effect reloading
    virtual void OnEffectReload(SuEffectPtr pOld, SuEffectPtr pNew);

    // tolua_end

private:
    // Initializes all resources required for drawing lights
    static void InitResources();

    // Initializes effect
    static void InitEffect();

    // Initializes VB and IB for spot lights
    static void InitSpotLightResources();

    // Initializes VB and IB for point lights
    static void InitPointLightResources();

    // Initializes VB and IB for volume lights
    static void InitVolumeLightResources();
    static void InitVolumeLightSphereResources();
    static void InitVolumeLightConeResources();

    // Initializes VB and IB for volume lights
    static void InitAreaLightResources();

    /// Releases all GPU resources
    static void DestroyResources();

    /// Renders a spot light
    void DrawSpotLight(const SuSpotLight* pLight) const;

    /// Renders a point light
    void DrawPointLight(const SuPointLight* pLight) const;

    /// Renders a volume light
    void DrawVolumeLight(const SuVolumeLight* pLight) const;

    /// Renders an area light
    void DrawAreaLight(const SuAreaLight* pLight) const;

    /// Rendering helper function
    static void Draw(SuGPUVertexBufferPtr pVB,
                     SuGPUIndexBufferPtr  pIB,
                     const SuString&      rTechniqueName);

    static SuEffectPtr          ms_pEffect;
    static SuGPUVertexBufferPtr ms_pSpotLightVB;
    static SuGPUIndexBufferPtr  ms_pSpotLightIB;
    static SuGPUVertexBufferPtr ms_pPointLightVB;
    static SuGPUIndexBufferPtr  ms_pPointLightIB;
    static SuGPUVertexBufferPtr ms_pVolumeLightSphereVB;
    static SuGPUIndexBufferPtr  ms_pVolumeLightSphereIB;
    static SuGPUVertexBufferPtr ms_pVolumeLightConeVB;
    static SuGPUIndexBufferPtr  ms_pVolumeLightConeIB;
    static SuGPUVertexBufferPtr ms_pAreaLightVB;
    static SuGPUIndexBufferPtr  ms_pAreaLightIB;
    static uint32 ms_nResourceRefCount;  // number of visualizers using the VB, IB and effect

    static const SuString EFFECT_FILE_NAME;
    static const SuString SPOT_LIGHT_TECHNIQUE_NAME;
    static const SuString SPOT_LIGHT_STREAM_NAME;
    static const SuString POINT_LIGHT_TECHNIQUE_NAME;
    static const SuString POINT_LIGHT_STREAM_NAME;
    static const SuString VOLUME_LIGHT_SPHERE_TECHNIQUE_NAME;
    static const SuString VOLUME_LIGHT_SPHERE_STREAM_NAME;
    static const SuString VOLUME_LIGHT_CONE_TECHNIQUE_NAME;
    static const SuString VOLUME_LIGHT_CONE_STREAM_NAME;
    static const SuString AREA_LIGHT_TECHNIQUE_NAME;
    static const SuString AREA_LIGHT_STREAM_NAME;

    static const uint32 SPOT_LIGHT_NUM_LINE_SEGMENTS;
    static const uint32 SPOT_LIGHT_NUM_CIRCLE_SEGMENTS;
    static const uint32 POINT_LIGHT_NUM_LINE_SEGMENTS;
    static const uint32 VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS;
    static const uint32 VOLUME_LIGHT_CONE_NUM_CIRCLE_SEGMENTS;
    // tolua_begin
};
// tolua_end

#endif  // _SU_LIGHTVISUALIZER_H_
