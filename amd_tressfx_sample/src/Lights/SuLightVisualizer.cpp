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

#include "PCH.h"
#include "SuAreaLight.h"
#include "SuArray.h"
#include "SuEffect.h"
#include "SuEffectManager.h"
#include "SuEffectParameter.h"
#include "SuGPUIndexBuffer.h"
#include "SuGPUResourceManager.h"
#include "SuGPUVertexBuffer.h"
#include "SuLightVisualizer.h"
#include "SuPoint3.h"
#include "SuPointLight.h"
#include "SuRenderManager.h"
#include "SuSpotLight.h"
#include "SuVector3.h"
#include "SuVector4.h"
#include "SuVolumeLight.h"


SuEffectPtr          SuLightVisualizer::ms_pEffect;
SuGPUVertexBufferPtr SuLightVisualizer::ms_pSpotLightVB;
SuGPUIndexBufferPtr  SuLightVisualizer::ms_pSpotLightIB;
SuGPUVertexBufferPtr SuLightVisualizer::ms_pPointLightVB;
SuGPUIndexBufferPtr  SuLightVisualizer::ms_pPointLightIB;
SuGPUVertexBufferPtr SuLightVisualizer::ms_pVolumeLightSphereVB;
SuGPUIndexBufferPtr  SuLightVisualizer::ms_pVolumeLightSphereIB;
SuGPUVertexBufferPtr SuLightVisualizer::ms_pVolumeLightConeVB;
SuGPUIndexBufferPtr  SuLightVisualizer::ms_pVolumeLightConeIB;
SuGPUVertexBufferPtr SuLightVisualizer::ms_pAreaLightVB;
SuGPUIndexBufferPtr  SuLightVisualizer::ms_pAreaLightIB;
uint32               SuLightVisualizer::ms_nResourceRefCount = 0;

const SuString SuLightVisualizer::EFFECT_FILE_NAME("Global/gLightVisualizer.sufx");
const SuString SuLightVisualizer::SPOT_LIGHT_TECHNIQUE_NAME("RenderSpotLight");
const SuString SuLightVisualizer::SPOT_LIGHT_STREAM_NAME("SpotLightStream");
const SuString SuLightVisualizer::POINT_LIGHT_TECHNIQUE_NAME("RenderPointLight");
const SuString SuLightVisualizer::POINT_LIGHT_STREAM_NAME("PointLightStream");
const SuString SuLightVisualizer::VOLUME_LIGHT_SPHERE_TECHNIQUE_NAME("RenderVolumeLightSphere");
const SuString SuLightVisualizer::VOLUME_LIGHT_SPHERE_STREAM_NAME("VolumeLightSphereStream");
const SuString SuLightVisualizer::VOLUME_LIGHT_CONE_TECHNIQUE_NAME("RenderVolumeLightCone");
const SuString SuLightVisualizer::VOLUME_LIGHT_CONE_STREAM_NAME("VolumeLightConeStream");
const SuString SuLightVisualizer::AREA_LIGHT_TECHNIQUE_NAME("RenderAreaLight");
const SuString SuLightVisualizer::AREA_LIGHT_STREAM_NAME("AreaLightStream");

const uint32 SuLightVisualizer::SPOT_LIGHT_NUM_LINE_SEGMENTS            = 6;
const uint32 SuLightVisualizer::SPOT_LIGHT_NUM_CIRCLE_SEGMENTS          = 32;
const uint32 SuLightVisualizer::POINT_LIGHT_NUM_LINE_SEGMENTS           = 6;
const uint32 SuLightVisualizer::VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS = 32;
const uint32 SuLightVisualizer::VOLUME_LIGHT_CONE_NUM_CIRCLE_SEGMENTS   = 32;

static bool InitVBAndIB(const SuArray<SuVector3>& rVBData,
                        const SuArray<uint16>&    rIBData,
                        SuGPUVertexBufferPtr*     ppVB,
                        SuGPUIndexBufferPtr*      ppIB);

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuLightVisualizer::SuLightVisualizer()
{
    if (ms_nResourceRefCount == 0)
    {
        InitResources();
    }

    SuEffectManager::GetRef().AddReloadListener(this);
    ms_nResourceRefCount++;
}

//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuLightVisualizer::~SuLightVisualizer()
{
    ms_nResourceRefCount--;
    if (ms_nResourceRefCount == 0)
    {
        DestroyResources();
    }

    SuEffectManager::GetRef().RemoveReloadListener(this);
}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Draws a light
//=================================================================================================================================
void SuLightVisualizer::DrawLight(const SuLight* pLight) const
{
    if (!pLight)
    {
        return;
    }

    if (!pLight->IsVisible())
    {
        return;
    }

    switch (pLight->GetType())
    {
    case SuLight::SPOT_LIGHT:
        DrawSpotLight(static_cast<const SuSpotLight*>(pLight));
        return;

    case SuLight::POINT_LIGHT:
        DrawPointLight(static_cast<const SuPointLight*>(pLight));
        return;

    case SuLight::VOLUME_LIGHT:
        DrawVolumeLight(static_cast<const SuVolumeLight*>(pLight));
        return;

    case SuLight::AREA_LIGHT:
        DrawAreaLight(static_cast<const SuAreaLight*>(pLight));
        return;
    }
}

//=================================================================================================================================
//=================================================================================================================================
void SuLightVisualizer::OnEffectReload(SuEffectPtr pOld, SuEffectPtr pNew)
{
    if (ms_pEffect == pOld)
        ms_pEffect = pNew;
}

//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Initializes all required GPU resources
//=================================================================================================================================
void SuLightVisualizer::InitResources()
{
    InitEffect();
    InitSpotLightResources();
    InitPointLightResources();
    InitVolumeLightResources();
    InitAreaLightResources();
}

//=================================================================================================================================
/// Loads light visualization effect file
//=================================================================================================================================
void SuLightVisualizer::InitEffect()
{
    ms_pEffect = SuEffectManager::GetRef().LoadEffect(EFFECT_FILE_NAME);
    SU_ASSERT(ms_pEffect);
}

//=================================================================================================================================
/// Initializes VB and IB for rendering spot lights
//=================================================================================================================================
void SuLightVisualizer::InitSpotLightResources()
{
    SuArray<SuVector3> vbData;
    SuArray<uint16>    ibData;

    vbData.push_back(SuVector3::ZERO);


    for (size_t i = 0; i < SPOT_LIGHT_NUM_LINE_SEGMENTS; i++)
    {
        // angle, distance, cone flag
        vbData.push_back(SuVector3(
            (float32)(2.0f * SU_PI * i) / (float32)SPOT_LIGHT_NUM_LINE_SEGMENTS, 2.0f, 0.0f));
        ibData.push_back(0);
        ibData.push_back(static_cast<uint16>(vbData.size()) - 1);
    }

    // two rings (near and far plane)
    size_t nOffset = vbData.size();
    for (size_t i = 0; i < SPOT_LIGHT_NUM_CIRCLE_SEGMENTS; i++)
    {
        // angle, distance, cone flag
        vbData.push_back(SuVector3(
            (float32)(2.0f * SU_PI * i) / (float32)SPOT_LIGHT_NUM_CIRCLE_SEGMENTS, 1.0f, 0.0f));

        ibData.push_back(static_cast<uint16>(nOffset + i));
        ibData.push_back(static_cast<uint16>(nOffset + (i + 1) % SPOT_LIGHT_NUM_CIRCLE_SEGMENTS));
    }

    nOffset = vbData.size();
    for (size_t i = 0; i < SPOT_LIGHT_NUM_CIRCLE_SEGMENTS; i++)
    {
        // angle, distance, cone flag
        vbData.push_back(SuVector3(
            (float32)(2.0f * SU_PI * i) / (float32)SPOT_LIGHT_NUM_CIRCLE_SEGMENTS, 2.0f, 0.0f));

        ibData.push_back(static_cast<uint16>(nOffset + i));
        ibData.push_back(
            static_cast<uint16>(nOffset + ((i + 1) % SPOT_LIGHT_NUM_CIRCLE_SEGMENTS)));
    }

    InitVBAndIB(vbData, ibData, &ms_pSpotLightVB, &ms_pSpotLightIB);
}

//=================================================================================================================================
/// Initializes VB and IB for rendering point lights
//=================================================================================================================================
void SuLightVisualizer::InitPointLightResources()
{
    SuArray<SuVector3> vbData;
    SuArray<uint16>    ibData;

    vbData.push_back(SuVector3::ZERO);

    for (size_t j = 1; j < POINT_LIGHT_NUM_LINE_SEGMENTS; j++)
    {
        float32 fAngle =
            (float32)((SU_PI * j) / (float32)POINT_LIGHT_NUM_LINE_SEGMENTS - 0.5f * SU_PI);
        SuVector3 vTmp(SuCos(fAngle), SuSin(fAngle), 0.0f);

        for (size_t i = 0; i < POINT_LIGHT_NUM_LINE_SEGMENTS; i++)
        {
            // angle, distance, cone flag
            float32 fAngle2 = (float32)(2.0f * SU_PI * i) / (float32)POINT_LIGHT_NUM_LINE_SEGMENTS;

            SuMatrix4 mRot;
            mRot.MakeFromArbitraryAxis(SuVector3::UNIT_Y, fAngle2);
            SuVector3 vPos = mRot * vTmp;

            ibData.push_back(0);
            ibData.push_back(static_cast<uint16>(vbData.size()));
            vbData.push_back(vPos);
        }
    }

    // vertical line
    ibData.push_back(static_cast<uint16>(vbData.size()));
    vbData.push_back(SuVector3::UNIT_Y);
    ibData.push_back(static_cast<uint16>(vbData.size()));
    vbData.push_back(-SuVector3::UNIT_Y);

    InitVBAndIB(vbData, ibData, &ms_pPointLightVB, &ms_pPointLightIB);
}

//=================================================================================================================================
/// Initializes VB and IB for rendering volume lights
//=================================================================================================================================
void SuLightVisualizer::InitVolumeLightResources()
{
    InitVolumeLightSphereResources();
    InitVolumeLightConeResources();
}

//=================================================================================================================================
/// Initializes VB and IB for rendering sphere volume lights
//=================================================================================================================================
void SuLightVisualizer::InitVolumeLightSphereResources()
{
    SuArray<SuVector3> vbData;
    SuArray<uint16>    ibData;

    ///////////////////////////////
    // a unit circle on the xy plane
    size_t nOffset = vbData.size();
    for (size_t i = 0; i < VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS; i++)
    {
        // angle, distance, cone flag
        float32 fAngle =
            (float32)((2.0f * SU_PI * i) / (float32)VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS);
        SuVector3 vTmp(SuCos(fAngle), SuSin(fAngle), 0.0f);
        vbData.push_back(vTmp);

        ibData.push_back(static_cast<uint16>(nOffset + i));
        ibData.push_back(
            static_cast<uint16>(nOffset + (i + 1) % VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS));
    }

    // a line along the x axis (1,0,0) (-1,0,0)
    size_t nStartOffset = nOffset;
    nOffset             = vbData.size();

    size_t nCenterIndex = (nStartOffset + nOffset) * (size_t)0.5;
    ibData.push_back(static_cast<uint16>(nCenterIndex));
    ibData.push_back(static_cast<uint16>(nStartOffset));

    ///////////////////////////////
    // a unit circle on the xz plane
    for (size_t i = 0; i < VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS; i++)
    {
        // angle, distance, cone flag
        float32 fAngle =
            (float32)((2.0f * SU_PI * i) / (float32)VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS);
        SuVector3 vTmp(SuSin(fAngle), 0.0f, SuCos(fAngle));
        vbData.push_back(vTmp);

        ibData.push_back(static_cast<uint16>(nOffset + i));
        ibData.push_back(
            static_cast<uint16>(nOffset + (i + 1) % VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS));
    }

    // a line along the z axis
    nStartOffset = nOffset;
    nOffset      = vbData.size();

    nCenterIndex = (nStartOffset + nOffset) >> 1;
    ibData.push_back(static_cast<uint16>(nCenterIndex));
    ibData.push_back(static_cast<uint16>(nStartOffset));

    ///////////////////////////////
    // a unit circle on the yz plane
    nOffset = vbData.size();
    for (size_t i = 0; i < VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS; i++)
    {
        // angle, distance, cone flag
        float32 fAngle =
            (float32)((2.0f * SU_PI * i) / (float32)VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS);
        SuVector3 vTmp(0.0f, SuCos(fAngle), SuSin(fAngle));
        vbData.push_back(vTmp);

        ibData.push_back(static_cast<uint16>(nOffset + i));
        ibData.push_back(
            static_cast<uint16>(nOffset + (i + 1) % VOLUME_LIGHT_SPHERE_NUM_CIRCLE_SEGMENTS));
    }

    // a line along the y axis
    nStartOffset = nOffset;
    nOffset      = vbData.size();

    nCenterIndex = (nStartOffset + nOffset) >> 1;
    ibData.push_back(static_cast<uint16>(nCenterIndex));
    ibData.push_back(static_cast<uint16>(nStartOffset));

    InitVBAndIB(vbData, ibData, &ms_pVolumeLightSphereVB, &ms_pVolumeLightSphereIB);
}


//=================================================================================================================================
/// Initializes VB and IB for rendering cone volume lights
//=================================================================================================================================
void SuLightVisualizer::InitVolumeLightConeResources()
{
    SuArray<SuVector3> vbData;
    SuArray<uint16>    ibData;

    // Light Position
    vbData.push_back(SuVector3::ZERO);

    // Test - xyz axis
    vbData.push_back(SuVector3(0.1f, 0, 0));
    vbData.push_back(SuVector3(0, 0.1f, 0));
    vbData.push_back(SuVector3(0, 0, 0.1f));

    ibData.push_back(0);
    ibData.push_back(1);
    ibData.push_back(0);
    ibData.push_back(2);
    ibData.push_back(0);
    ibData.push_back(3);

    ///////////////////////////////////////////
    // a unit circle on the xz plane, y=-1
    size_t nOffset = vbData.size();
    for (size_t i = 0; i < VOLUME_LIGHT_CONE_NUM_CIRCLE_SEGMENTS; i++)
    {
        // angle, distance, cone flag
        float32 fAngle =
            (float32)((2.0f * SU_PI * i) / (float32)VOLUME_LIGHT_CONE_NUM_CIRCLE_SEGMENTS);
        SuVector3 vTmp(SuCos(fAngle), -1.0f, SuSin(fAngle));
        vbData.push_back(vTmp);

        ibData.push_back(static_cast<uint16>(nOffset + i));
        ibData.push_back(
            static_cast<uint16>(nOffset + (i + 1) % VOLUME_LIGHT_CONE_NUM_CIRCLE_SEGMENTS));
    }

    // 4 lines
    size_t nIndexOffset = (int)(VOLUME_LIGHT_CONE_NUM_CIRCLE_SEGMENTS / 4.0f);

    for (size_t i = 0; i < 4; i++)
    {
        ibData.push_back(0);
        ibData.push_back(static_cast<uint16>(nOffset + nIndexOffset * i));
    }

    InitVBAndIB(vbData, ibData, &ms_pVolumeLightConeVB, &ms_pVolumeLightConeIB);
}

void SuLightVisualizer::InitAreaLightResources()
{
    SuArray<SuVector3> vbData;
    SuArray<uint16>    ibData;

    ///////////////////////////////
    // a square on the xy plane
    vbData.push_back(SuVector3(-1, -1, 0));
    vbData.push_back(SuVector3(1, -1, 0));
    vbData.push_back(SuVector3(1, 1, 0));
    vbData.push_back(SuVector3(-1, 1, 0));
    vbData.push_back(SuVector3(0, 0, -0.5));
    vbData.push_back(SuVector3(0, 0, 0));

    unsigned int indices[14] = { 0, 1, 1, 2, 2, 3, 3, 0,  // square
                                 0, 2, 1, 3,              // diagonals
                                 4, 5 };                  // direction

    for (int i = 0; i < 14; i++)
    {
        ibData.push_back(indices[i]);
    }

    InitVBAndIB(vbData, ibData, &ms_pAreaLightVB, &ms_pAreaLightIB);
}

//=================================================================================================================================
/// Releases all GPU resources
//=================================================================================================================================
void SuLightVisualizer::DestroyResources()
{
    ms_pEffect              = SuEffectPtr(NULL);
    ms_pSpotLightVB         = SuGPUVertexBufferPtr(NULL);
    ms_pSpotLightIB         = SuGPUIndexBufferPtr(NULL);
    ms_pPointLightVB        = SuGPUVertexBufferPtr(NULL);
    ms_pPointLightIB        = SuGPUIndexBufferPtr(NULL);
    ms_pVolumeLightSphereVB = SuGPUVertexBufferPtr(NULL);
    ms_pVolumeLightSphereIB = SuGPUIndexBufferPtr(NULL);
    ms_pVolumeLightConeVB   = SuGPUVertexBufferPtr(NULL);
    ms_pVolumeLightConeIB   = SuGPUIndexBufferPtr(NULL);
    ms_pAreaLightVB         = SuGPUVertexBufferPtr(NULL);
    ms_pAreaLightIB         = SuGPUIndexBufferPtr(NULL);
}

//=================================================================================================================================
/// Draws a spot light
///
/// \param pLight The light to draw
//=================================================================================================================================
void SuLightVisualizer::DrawSpotLight(const SuSpotLight* pLight) const
{
    ms_pEffect->BindVertexBuffer(SPOT_LIGHT_STREAM_NAME, ms_pSpotLightVB.get());
    ms_pEffect->BindIndexBuffer(ms_pSpotLightIB.get());

    // effect parameters
    SuVector4 vZeroNearFarPlane(
        0.0f, pLight->GetNearPlaneDistance(), pLight->GetFarPlaneDistance(), 0.0f);
    SuMatrix4 mConeAngleRot;
    mConeAngleRot.MakeFromArbitraryAxis(SuVector3::UNIT_X,
                                        SuDegToRad(0.5f * pLight->GetConeAngle()));

    ms_pEffect->GetParameter("mLightViewToWorld")
        ->SetFloatMatrix(pLight->GetViewMatrix().RigidInverse());
    ms_pEffect->GetParameter("mConeAngleRot")->SetFloatMatrix(mConeAngleRot);
    ms_pEffect->GetParameter("vZeroNearFarPlane")->SetFloatVector(vZeroNearFarPlane);
    ms_pEffect->GetParameter("cColor")->SetFloatVector(pLight->GetColor() *
                                                       pLight->GetIntensity());

    Draw(ms_pSpotLightVB, ms_pSpotLightIB, SPOT_LIGHT_TECHNIQUE_NAME);
}

//=================================================================================================================================
/// Draws a point light
///
/// \param pLight The light to draw
//=================================================================================================================================
void SuLightVisualizer::DrawPointLight(const SuPointLight* pLight) const
{
    ms_pEffect->BindVertexBuffer(POINT_LIGHT_STREAM_NAME, ms_pPointLightVB.get());
    ms_pEffect->BindIndexBuffer(ms_pPointLightIB.get());

    // effect parameters
    SuPoint3  vTmpPos = pLight->GetPosition();
    SuVector4 vLightPos(vTmpPos.x, vTmpPos.y, vTmpPos.z, 1.0f);

    ms_pEffect->GetParameter("vLightPos")->SetFloatVector(vLightPos);
    ms_pEffect->GetParameter("cColor")->SetFloatVector(pLight->GetColor() *
                                                       pLight->GetIntensity());

    Draw(ms_pPointLightVB, ms_pPointLightIB, POINT_LIGHT_TECHNIQUE_NAME);
}

//=================================================================================================================================
/// Draws a volume light
///
/// \param pLight The light to draw
//=================================================================================================================================
void SuLightVisualizer::DrawVolumeLight(const SuVolumeLight* pLight) const
{
    // effect parameters
    SuPoint3  vTmpPos = pLight->GetPosition();
    SuVector4 vLightPos(vTmpPos.x, vTmpPos.y, vTmpPos.z, 1.0f);

    SuLight::LightShapeType nLightShape = pLight->GetLightShape();

    ms_pEffect->GetParameter("vLightPos")->SetFloatVector(vLightPos);
    ms_pEffect->GetParameter("cColor")->SetFloatVector(pLight->GetColor() *
                                                       pLight->GetIntensity());

    // SuMath.shl uses quaternion(x,y,z,w) so change the order!!
    SuQuat ori = pLight->GetOrientation();
    ms_pEffect->GetParameter("qOrientation")
        ->SetFloatVector(SuQuat(ori.Y(), ori.Z(), ori.W(), ori.X()));
    ms_pEffect->GetParameter("vScale")->SetFloatVector(pLight->GetScale());

    switch (nLightShape)
    {
    case SuLight::VOLUMELIGHT_SPHERE:

        ms_pEffect->BindVertexBuffer(VOLUME_LIGHT_SPHERE_STREAM_NAME,
                                     ms_pVolumeLightSphereVB.get());
        ms_pEffect->BindIndexBuffer(ms_pVolumeLightSphereIB.get());

        Draw(ms_pVolumeLightSphereVB, ms_pVolumeLightSphereIB, VOLUME_LIGHT_SPHERE_TECHNIQUE_NAME);

        break;

    case SuLight::VOLUMELIGHT_CONE:

        ms_pEffect->BindVertexBuffer(VOLUME_LIGHT_CONE_STREAM_NAME, ms_pVolumeLightConeVB.get());
        ms_pEffect->BindIndexBuffer(ms_pVolumeLightConeIB.get());

        Draw(ms_pVolumeLightConeVB, ms_pVolumeLightConeIB, VOLUME_LIGHT_CONE_TECHNIQUE_NAME);

        break;

    default:
        break;
    }
}

//=================================================================================================================================
/// Draws an area light
///
/// \param pLight The light to draw
//=================================================================================================================================
void SuLightVisualizer::DrawAreaLight(const SuAreaLight* pLight) const
{
    // effect parameters
    SuPoint3  vTmpPos = pLight->GetPosition();
    SuVector4 vLightPos(vTmpPos.x, vTmpPos.y, vTmpPos.z, 1.0f);

    ms_pEffect->GetParameter("vLightPos")->SetFloatVector(vLightPos);
    ms_pEffect->GetParameter("cColor")->SetFloatVector(pLight->GetColor() *
                                                       pLight->GetIntensity());

    // SuMath.shl uses quaternion(x,y,z,w) so change the order!!
    SuQuat ori = pLight->GetOrientation();
    ms_pEffect->GetParameter("qOrientation")
        ->SetFloatVector(SuQuat(ori.Y(), ori.Z(), ori.W(), ori.X()));
    ms_pEffect->GetParameter("vScale")->SetFloatVector(pLight->GetScale());

    ms_pEffect->BindVertexBuffer(AREA_LIGHT_STREAM_NAME, ms_pAreaLightVB.get());
    ms_pEffect->BindIndexBuffer(ms_pAreaLightIB.get());

    Draw(ms_pAreaLightVB, ms_pAreaLightIB, AREA_LIGHT_TECHNIQUE_NAME);
}

//=================================================================================================================================
/// Internal draws helper function
///
/// \param pVB The VB used for drawing
/// \param pIB The IB used for drawing
/// \param rTechniqueName The technique to use for drawing
//=================================================================================================================================
void SuLightVisualizer::Draw(SuGPUVertexBufferPtr pVB,
                             SuGPUIndexBufferPtr  pIB,
                             const SuString&      rTechniqueName)
{
    SuEffectManager::GetRef().ForceDisableOverrideTechniques(true);

    uint32 nPasses = 0;
    bool   bOK     = ms_pEffect->Begin(rTechniqueName, &nPasses);
    SU_ASSERT(bOK);

    for (uint32 i = 0; i < nPasses; i++)
    {
        // Begin a render pass (Sets render state)
        bOK = ms_pEffect->BeginPass(i);
        SU_ASSERT(bOK);
        SuRenderManager::GetRef().DrawIndexed(SuRenderManager::LINE_LIST,
                                              0,
                                              pVB->GetNumVertices(),
                                              0,
                                              pIB->GetNumIndices(),
                                              0,
                                              pIB->GetIndexSize());

        bOK = ms_pEffect->EndPass();
        SU_ASSERT(bOK);
    }  // end for nPasses

    bOK = ms_pEffect->End();
    SU_ASSERT(bOK);

    SuEffectManager::GetRef().ForceDisableOverrideTechniques(false);
}

//=================================================================================================================================
//
//          Static helper functions block
//
//=================================================================================================================================

//=================================================================================================================================
//=================================================================================================================================
bool InitVBAndIB(const SuArray<SuVector3>& rVBData,
                 const SuArray<uint16>&    rIBData,
                 SuGPUVertexBufferPtr*     ppVB,
                 SuGPUIndexBufferPtr*      ppIB)
{
    SU_ASSERT(ppVB);
    SU_ASSERT(ppIB);

    SuMemoryBufferPtr pVBData = SuMemoryBufferPtr(
        SuMemoryBuffer::Allocate(sizeof(SuVector3) * static_cast<uint32>(rVBData.size())));
    SU_ASSERT(pVBData);
    memcpy(pVBData->GetData(), &rVBData[0], pVBData->GetSize());

    *ppVB =
        SuGPUResourceManager::GetRef().CreateResourceVertex(SuGPUResource::GPU_RESOURCE_STATIC,
                                                            sizeof(SuVector3),
                                                            static_cast<uint32>(rVBData.size()),
                                                            pVBData);
    SU_ASSERT(ppVB);

    SuMemoryBufferPtr pIBData = SuMemoryBufferPtr(
        SuMemoryBuffer::Allocate(sizeof(uint16) * static_cast<uint32>(rIBData.size())));
    SU_ASSERT(pIBData);
    memcpy(pIBData->GetData(), &rIBData[0], pIBData->GetSize());

    *ppIB = SuGPUResourceManager::GetRef().CreateResourceIndex(SuGPUResource::GPU_RESOURCE_STATIC,
                                                               sizeof(uint16),
                                                               static_cast<uint32>(rIBData.size()),
                                                               pIBData);
    SU_ASSERT(ppIB);

    return *ppVB && *ppIB;
}
