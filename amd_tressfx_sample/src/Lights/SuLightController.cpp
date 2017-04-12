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
#include "SuAlgorithm.h"
#include "SuAreaLight.h"
#include "SuDirectionalLight.h"
#include "SuKeyframeCurve.h"
#include "SuLightController.h"
#include "SuPointLight.h"
#include "SuQuat.h"
#include "SuSpotLight.h"
#include "SuVolumeLight.h"


//=================================================================================================================================
//
//          Static functions and values
//
//=================================================================================================================================


//=================================================================================================================================
/// \return Return whether the animation curve ever changes value
/// \todo This should probably be moved into the SuAnimationCurve class.
//=================================================================================================================================
static bool IsCurveDynamic(SuAnimationCurvePtr pCurve)
{
    if (!pCurve)
        return false;

    SU_ASSERT(pCurve->GetCurveClass() == SuAnimationCurve::KEYFRAME_CURVE);

    // We don't handle anything else right now, so just say it's not constant - a safe, but
    // inefficient thing to do.
    if (pCurve->GetCurveClass() != SuAnimationCurve::KEYFRAME_CURVE)
        return true;

    SuKeyframeCurve* pKeyframeCurve = (SuKeyframeCurve*)pCurve.get();

    if (pKeyframeCurve->GetNumKeys() == 1)
        return false;

    uint32 valueSize = pKeyframeCurve->GetValueSize();  // Just in case the compiler is stupid.
    SU_ASSERT(SuAnimationCurve::MAX_DATA_SIZE >= valueSize);

    // Once we find a single case of a key differing from previous keys, we know the
    // curve is dynamic and can stop looking.
    for (uint32 i = 0; i < pKeyframeCurve->GetNumKeys() - 2; ++i)
    {
        uint8* value0[SuAnimationCurve::MAX_DATA_SIZE];
        uint8* value1[SuAnimationCurve::MAX_DATA_SIZE];
        pKeyframeCurve->GetKeyValueRaw(i, value0);
        pKeyframeCurve->GetKeyValueRaw(i + 1, value1);

        if (memcmp(value0, value1, valueSize) != 0)
        {
            return true;
        }
    }
    return false;
}


//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuLightController::SuLightController(SuAnimationCurvePtr pColorCurve,
                                     SuAnimationCurvePtr pIntensityCurve,
                                     SuAnimationCurvePtr pDiffuseCurve,
                                     SuAnimationCurvePtr pSpecularCurve,
                                     SuAnimationCurvePtr pFalloffCurve,
                                     SuAnimationCurvePtr pRadiusCurve,
                                     SuAnimationCurvePtr pPosCurve,
                                     SuAnimationCurvePtr pOrientationCurve,
                                     SuAnimationCurvePtr pScaleCurve,
                                     SuAnimationCurvePtr pConeAngleCurve,
                                     SuAnimationCurvePtr pPenumbraAngleCurve,
                                     SuAnimationCurvePtr pDropOffCurve,
                                     SuAnimationCurvePtr pVisibilityCurve,
                                     SuAnimationCurvePtr pLightShapeCurve,
                                     SuAnimationCurvePtr pSushiLightIndirectCurve)
    : m_fAnimLength(0.0f)
    , m_fLastTime(-SU_FLT_MAX)
    , m_nDynamicMask(ALL_MASK)
    , m_nChangedMask(ALL_MASK)
    , m_bFirstUpdate(true)
{
    m_pCurves[COLOR_INDEX]          = pColorCurve;
    m_pCurves[INTENSITY_INDEX]      = pIntensityCurve;
    m_pCurves[DIFFUSE_INDEX]        = pDiffuseCurve;
    m_pCurves[SPECULAR_INDEX]       = pSpecularCurve;
    m_pCurves[FALLOFF_INDEX]        = pFalloffCurve;
    m_pCurves[RADIUS_INDEX]         = pRadiusCurve;
    m_pCurves[POS_INDEX]            = pPosCurve;
    m_pCurves[ORIENTATION_INDEX]    = pOrientationCurve;
    m_pCurves[SCALE_INDEX]          = pScaleCurve;
    m_pCurves[CONE_ANGLE_INDEX]     = pConeAngleCurve;
    m_pCurves[PENUMBRA_ANGLE_INDEX] = pPenumbraAngleCurve;
    m_pCurves[DROP_OFF_INDEX]       = pDropOffCurve;
    m_pCurves[VISIBILITY_INDEX]     = pVisibilityCurve;
    m_pCurves[SHAPE_INDEX]          = pLightShapeCurve;
    m_pCurves[INDIRECT_INDEX]       = pSushiLightIndirectCurve;

    uint32 offset = 0;
    for (int i = 0; i < NUM_CURVE_TYPES; ++i)
    {
        m_pCacheOffsets[i] = offset;
        if (m_pCurves[i])
        {
            offset += m_pCurves[i]->GetValueSize();
        }
    }

    m_fAnimLength  = ComputeAnimationLength();
    m_nDynamicMask = ComputeDynamicMask();
}

//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuLightController::~SuLightController() {}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Updates the controlled light
///
/// \param fTime The time at which to sample the animation curves
//=================================================================================================================================
void SuLightController::Update(SuTime fTime, SuLight* pLight)
{
    SU_ASSERT(pLight != NULL);
    if (pLight == NULL)
    {
        return;
    }

    m_nChangedMask = 0;

    if (m_fLastTime == fTime)
    {
        return;
    }

    m_fLastTime = fTime;

    float32 fWrappedTime = static_cast<float32>(fmod(fTime, static_cast<float64>(m_fAnimLength)));

    // Update common light attributes
    if (m_pCurves[COLOR_INDEX])
    {
        SuVector4 color(0.0f, 0.0f, 0.0f, 1.0f);
        m_pCurves[COLOR_INDEX]->GetValueVec3(fWrappedTime, reinterpret_cast<SuVector3*>(&color));

        ChangeCheck(COLOR_INDEX, &color);

        // COLOR SPACE CONVERSION
        // Maya's colors are in sRGB space. Exponentiate to get to linear space
        color.x = SuPow(color.x, 2.2f);
        color.y = SuPow(color.y, 2.2f);
        color.z = SuPow(color.z, 2.2f);

        pLight->SetColor(color);
    }

    if (m_pCurves[INTENSITY_INDEX])
    {
        float32 fIntensity = 0.0f;
        m_pCurves[INTENSITY_INDEX]->GetValueFloat(fWrappedTime, &fIntensity);
        pLight->SetIntensity(fIntensity);
        ChangeCheck(INTENSITY_INDEX, &fIntensity);
    }

    if (m_pCurves[DIFFUSE_INDEX])
    {
        bool bDiffuse = true;
        m_pCurves[DIFFUSE_INDEX]->GetValueBool(fWrappedTime, &bDiffuse);
        pLight->SetDiffuse(bDiffuse);
        ChangeCheck(DIFFUSE_INDEX, &bDiffuse);
    }

    if (m_pCurves[SPECULAR_INDEX])
    {
        bool bSpecular = true;
        m_pCurves[SPECULAR_INDEX]->GetValueBool(fWrappedTime, &bSpecular);
        pLight->SetSpecular(bSpecular);
        ChangeCheck(SPECULAR_INDEX, &bSpecular);
    }

    if (m_pCurves[FALLOFF_INDEX])
    {
        SuLight::FalloffType falloff;
        m_pCurves[FALLOFF_INDEX]->GetValueInt(fWrappedTime, reinterpret_cast<int32*>(&falloff));
        SU_ASSERT(sizeof(falloff) == m_pCurves[FALLOFF_INDEX]->GetValueSize());
        pLight->SetFalloff(falloff);
        ChangeCheck(FALLOFF_INDEX, &falloff);
    }

    if (m_pCurves[VISIBILITY_INDEX])
    {
        bool bVisible = true;
        m_pCurves[VISIBILITY_INDEX]->GetValueBool(fWrappedTime, &bVisible);
        pLight->SetVisible(bVisible);
        ChangeCheck(VISIBILITY_INDEX, &bVisible);
    }

    // light type-specific attributes
    switch (pLight->GetType())
    {
    case SuLight::POINT_LIGHT:
        if (m_pCurves[POS_INDEX])
        {
            SuPoint3 pos;
            m_pCurves[POS_INDEX]->GetValueVec3(fWrappedTime, reinterpret_cast<SuVector3*>(&pos));
            static_cast<SuPointLight*>(pLight)->SetPosition(pos);

            SU_ASSERT(sizeof(pos) == m_pCurves[POS_INDEX]->GetValueSize());
            ChangeCheck(POS_INDEX, &pos);
        }

        if (m_pCurves[RADIUS_INDEX])
        {
            float32 fRadius = 0.0f;
            m_pCurves[RADIUS_INDEX]->GetValueFloat(fWrappedTime, &fRadius);
            static_cast<SuPointLight*>(pLight)->SetRadius(fRadius);
            ChangeCheck(RADIUS_INDEX, &fRadius);
        }
        break;

    case SuLight::DIRECTIONAL_LIGHT:
        if (m_pCurves[ORIENTATION_INDEX])
        {
            SuQuat    ori;
            SuMatrix4 mat;
            m_pCurves[ORIENTATION_INDEX]->GetValueQuat(fWrappedTime, &ori);
            SU_ASSERT(sizeof(ori) == m_pCurves[ORIENTATION_INDEX]->GetValueSize());
            ChangeCheck(ORIENTATION_INDEX, &ori);

            ori.ToRotationMatrix(mat);

            static_cast<SuDirectionalLight*>(pLight)->SetDirection(mat * SuVector3(0, 0, -1));
        }
        break;

    case SuLight::SPOT_LIGHT:
        if (m_pCurves[POS_INDEX])
        {
            SuPoint3 pos;
            m_pCurves[POS_INDEX]->GetValueVec3(fWrappedTime, reinterpret_cast<SuVector3*>(&pos));
            static_cast<SuSpotLight*>(pLight)->SetPosition(pos);
            SU_ASSERT(sizeof(pos) == m_pCurves[POS_INDEX]->GetValueSize());
            ChangeCheck(POS_INDEX, &pos);
        }

        if (m_pCurves[ORIENTATION_INDEX])
        {
            SuQuat    ori;
            SuMatrix4 mat;
            m_pCurves[ORIENTATION_INDEX]->GetValueQuat(fWrappedTime, &ori);
            SU_ASSERT(sizeof(ori) == m_pCurves[ORIENTATION_INDEX]->GetValueSize());
            ChangeCheck(ORIENTATION_INDEX, &ori);
            ori.ToRotationMatrix(mat);

            static_cast<SuSpotLight*>(pLight)->SetDirection(mat * SuVector3(0, 0, -1));
        }

        if (m_pCurves[DROP_OFF_INDEX])
        {
            float32 fDropOff = 0.0f;
            m_pCurves[DROP_OFF_INDEX]->GetValueFloat(fWrappedTime, &fDropOff);
            ChangeCheck(DROP_OFF_INDEX, &fDropOff);
            static_cast<SuSpotLight*>(pLight)->SetDropOff(fDropOff);
        }

        {
            float32 fConeAngle = static_cast<SuSpotLight*>(pLight)->GetConeAngle();

            if (m_pCurves[CONE_ANGLE_INDEX])
            {
                m_pCurves[CONE_ANGLE_INDEX]->GetValueFloat(fWrappedTime, &fConeAngle);
                ChangeCheck(CONE_ANGLE_INDEX, &fConeAngle);
                static_cast<SuSpotLight*>(pLight)->SetConeAngle(fConeAngle);
            }

            if (m_pCurves[PENUMBRA_ANGLE_INDEX])
            {
                float32 fAngle = 0.0f;
                m_pCurves[PENUMBRA_ANGLE_INDEX]->GetValueFloat(fWrappedTime, &fAngle);
                ChangeCheck(PENUMBRA_ANGLE_INDEX, &fAngle);
                static_cast<SuSpotLight*>(pLight)->SetHotSpotAngle(
                    fConeAngle + fAngle);  // In Maya terms, the penumbra angle is
                                           // usually negative.
            }
        }
        break;

    case SuLight::VOLUME_LIGHT:
        if (m_pCurves[POS_INDEX])
        {
            SuPoint3 pos;
            m_pCurves[POS_INDEX]->GetValueVec3(fWrappedTime, reinterpret_cast<SuVector3*>(&pos));
            SU_ASSERT(sizeof(pos) == m_pCurves[POS_INDEX]->GetValueSize());
            ChangeCheck(POS_INDEX, &pos);
            static_cast<SuVolumeLight*>(pLight)->SetPosition(pos);
        }

        if (m_pCurves[ORIENTATION_INDEX])
        {
            SuQuat ori;
            m_pCurves[ORIENTATION_INDEX]->GetValueQuat(fWrappedTime, &ori);
            SU_ASSERT(sizeof(ori) == m_pCurves[ORIENTATION_INDEX]->GetValueSize());
            ChangeCheck(ORIENTATION_INDEX, &ori);
            static_cast<SuVolumeLight*>(pLight)->SetOrientation(ori);
        }

        if (m_pCurves[SCALE_INDEX])
        {
            SuVector3 scale;
            m_pCurves[SCALE_INDEX]->GetValueVec3(fWrappedTime, &scale);
            ChangeCheck(SCALE_INDEX, &scale);
            static_cast<SuVolumeLight*>(pLight)->SetScale(scale);
        }

        if (m_pCurves[SHAPE_INDEX])
        {
            SuLight::LightShapeType lightShape;
            m_pCurves[SHAPE_INDEX]->GetValueInt(fWrappedTime,
                                                reinterpret_cast<int32*>(&lightShape));
            SU_ASSERT(sizeof(lightShape) == m_pCurves[SHAPE_INDEX]->GetValueSize());
            ChangeCheck(SHAPE_INDEX, &lightShape);
            static_cast<SuVolumeLight*>(pLight)->SetLightShape(lightShape);
        }

        if (m_pCurves[INDIRECT_INDEX])
        {
            bool sushiLightIndirect = false;
            m_pCurves[INDIRECT_INDEX]->GetValueBool(fWrappedTime, &sushiLightIndirect);
            ChangeCheck(INDIRECT_INDEX, &sushiLightIndirect);
            static_cast<SuVolumeLight*>(pLight)->SetSushiLightIndirect(sushiLightIndirect);
        }

        break;

    case SuLight::AREA_LIGHT:
        if (m_pCurves[POS_INDEX])
        {
            SuPoint3 pos;
            m_pCurves[POS_INDEX]->GetValueVec3(fWrappedTime, reinterpret_cast<SuVector3*>(&pos));
            SU_ASSERT(sizeof(pos) == m_pCurves[POS_INDEX]->GetValueSize());
            ChangeCheck(POS_INDEX, &pos);
            static_cast<SuAreaLight*>(pLight)->SetPosition(pos);
        }

        if (m_pCurves[ORIENTATION_INDEX])
        {
            SuQuat ori;
            m_pCurves[ORIENTATION_INDEX]->GetValueQuat(fWrappedTime, &ori);
            SU_ASSERT(sizeof(ori) == m_pCurves[ORIENTATION_INDEX]->GetValueSize());
            ChangeCheck(ORIENTATION_INDEX, &ori);
            static_cast<SuAreaLight*>(pLight)->SetOrientation(ori);
        }

        if (m_pCurves[SCALE_INDEX])
        {
            SuVector3 scale;
            m_pCurves[SCALE_INDEX]->GetValueVec3(fWrappedTime, &scale);
            ChangeCheck(SCALE_INDEX, &scale);
            static_cast<SuAreaLight*>(pLight)->SetScale(scale);
        }

        break;

    default:
        SU_ASSERT(!"Unsupported light type");
        break;
    }

    m_bFirstUpdate = false;
}


//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================


//=================================================================================================================================
/// Compute a mask of which curves are dynamic.
//=================================================================================================================================
uint32 SuLightController::ComputeDynamicMask() const
{
    uint32 nDynamicMask = 0;

    for (uint32 i = 0; i < NUM_CURVE_TYPES; ++i)
    {
        if (IsCurveDynamic(m_pCurves[i]))
        {
            nDynamicMask = nDynamicMask + (1 << i);
        }
    }

    return nDynamicMask;
}

//=================================================================================================================================
/// \return The length of the longest animation curve (in seconds)
//=================================================================================================================================
float32 SuLightController::ComputeAnimationLength() const
{
    float32 fLength = 0.0f;

    for (int i = 0; i < NUM_CURVE_TYPES; ++i)
    {
        if (m_pCurves[i])
        {
            fLength = SuMax(fLength, m_pCurves[i]->GetAnimationLength());
        }
    }

    return fLength;
}
//=================================================================================================================================
/// Checks if the newValue is different from what was recorded in the cache.
/// If different, set the appropriate mask, and update the cache.
//=================================================================================================================================

void SuLightController::ChangeCheck(uint32 index, void* newValue)
{
    if (!m_pCurves[index])
    {
        return;
    }

    uint32 mask     = 1 << index;
    bool   bDynamic = (m_nDynamicMask & mask) != 0;

    if (bDynamic)
    {
        void*  oldValue  = m_pCache + m_pCacheOffsets[index];
        size_t valueSize = m_pCurves[index]->GetValueSize();

        bool changed = true;
        if (!m_bFirstUpdate)
        {
            changed = memcmp(oldValue, newValue, valueSize) != 0;
        }

        if (changed)
        {
            m_nChangedMask += mask;
            memcpy(oldValue, newValue, valueSize);
        }
    }
    else if (m_bFirstUpdate)
    {
        m_nChangedMask += mask;
    }
}
