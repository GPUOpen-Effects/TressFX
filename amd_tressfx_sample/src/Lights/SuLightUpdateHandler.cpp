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
#include "SuDirectionalLight.h"
#include "SuLightManager.h"
#include "SuLightUpdateHandler.h"
#include "SuObjectLightTable.h"
#include "SuObjectManager.h"
#include "SuPointLight.h"
#include "SuSpotLight.h"
#include "SuVolumeLight.h"

/// \ingroup Engine
/// \brief Templatized helper function to fill effect parameter arrays with light data
template <typename BufferT, typename LightDataFunc>
bool SuFillArrayWithLightData(const SuList<const SuLight*>& rLights,
                              BufferT*                      pBuffer,
                              uint32                        nParamArraySize,
                              LightDataFunc                 func);

/// \ingroup Engine
/// \brief Returns the position of a light
static SuVector4 SuGetLightPosition(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the cutoff distance of a light
static SuVector4 SuGetLightCutoff(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the color of a light
static SuVector4 SuGetLightColor(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the additional parameters of a light
static SuVector4 SuGetLightMiscParams(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the direction of a light
static SuVector4 SuGetLightDirection(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the cone angles of a light
static SuVector4 SuGetLightConeAngles(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the view-projection matrix of a light
static SuMatrix4 SuGetLightViewProjectionMatrix(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the view-projection-texture matrix of a light
static SuMatrix4 SuGetLightViewProjTextureMatrix(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the view matrix of a light
static SuMatrix4 SuGetLightViewMatrix(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the projection matrix of a light
static SuMatrix4 SuGetLightProjectionMatrix(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the scale vector of a light
static SuVector4 SuGetLightScale(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the orientation of a light
static SuQuat SuGetLightOrientation(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the shape of a light
static int32 SuGetLightShape(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the intensity of a light
static float32 SuGetLightIntensity(const SuLight* pLight);

/// \ingroup Engine
/// \brief Returns the index (handle) of a light
static int32 SuGetLightIndex(const SuLight* pLight);

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuLightUpdateHandler::SuLightUpdateHandler(AppUpdateType             type,
                                           uint32                    nParamArraySize,
                                           const SuObjectLightTable& rObjectLightMapping,
                                           const SuString&           rLightName)
    : m_appUpdateType(type)
    , m_nParamArraySize(nParamArraySize)
    , m_rObjectLightMapping(rObjectLightMapping)
    , m_lightName(rLightName)
{
}


//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Callback method which is used to retrieve the value of an app-updatable parameter.
///
/// \param pBuffer Destination buffer for the appupdate values
/// \param nBufferSize Destination buffer size
///
/// \return true on success, false otherwise
//=================================================================================================================================
bool SuLightUpdateHandler::GetUpdateValue(void* pBuffer, uint32 nBufferSize)
{
    switch (m_appUpdateType)
    {
    case SU_NUM_LIGHTS:
        return GetNumLights(pBuffer, nBufferSize);

    case SU_LIGHT_POSITION:
        return GetLightPosition(pBuffer, nBufferSize);

    case SU_LIGHT_POSITION_ARRAY:
        return GetLightPositionArray(pBuffer, nBufferSize);

    case SU_LIGHT_CUTOFF:
        return GetLightCutoff(pBuffer, nBufferSize);

    case SU_LIGHT_CUTOFF_ARRAY:
        return GetLightCutoffArray(pBuffer, nBufferSize);

    case SU_LIGHT_COLOR:
        return GetLightColor(pBuffer, nBufferSize);

    case SU_LIGHT_COLOR_ARRAY:
        return GetLightColorArray(pBuffer, nBufferSize);

    case SU_LIGHT_PARAMS:
        return GetMiscLightParams(pBuffer, nBufferSize);

    case SU_LIGHT_PARAMS_ARRAY:
        return GetMiscLightParamsArray(pBuffer, nBufferSize);

    case SU_LIGHT_DIRECTION:
        return GetLightDirection(pBuffer, nBufferSize);

    case SU_LIGHT_DIRECTION_ARRAY:
        return GetLightDirectionArray(pBuffer, nBufferSize);

    case SU_LIGHT_CONE_ANGLES:
        return GetLightConeAngles(pBuffer, nBufferSize);

    case SU_LIGHT_CONE_ANGLES_ARRAY:
        return GetLightConeAnglesArray(pBuffer, nBufferSize);

    case SU_LIGHT_VIEW_PROJ_MATRIX:
        return GetLightViewProjectionMatrix(pBuffer, nBufferSize);

    case SU_LIGHT_VIEW_PROJ_MATRIX_ARRAY:
        return GetLightViewProjectionMatrixArray(pBuffer, nBufferSize);

    case SU_LIGHT_VIEW_MATRIX:
        return GetLightViewMatrix(pBuffer, nBufferSize);

    case SU_LIGHT_VIEW_MATRIX_ARRAY:
        return GetLightViewMatrixArray(pBuffer, nBufferSize);

    case SU_LIGHT_PROJ_MATRIX:
        return GetLightProjectionMatrix(pBuffer, nBufferSize);

    case SU_LIGHT_PROJ_MATRIX_ARRAY:
        return GetLightProjectionMatrixArray(pBuffer, nBufferSize);

    case SU_LIGHT_SCALE:
        return GetLightScale(pBuffer, nBufferSize);

    case SU_LIGHT_SCALE_ARRAY:
        return GetLightScaleArray(pBuffer, nBufferSize);

    case SU_LIGHT_ORIENTATION:
        return GetLightOrientation(pBuffer, nBufferSize);

    case SU_LIGHT_ORIENTATION_ARRAY:
        return GetLightOrientationArray(pBuffer, nBufferSize);

    case SU_LIGHT_SHAPE:
        return GetLightShape(pBuffer, nBufferSize);

    case SU_LIGHT_SHAPE_ARRAY:
        return GetLightShapeArray(pBuffer, nBufferSize);

    case SU_LIGHT_INTENSITY:
        return GetLightIntensity(pBuffer, nBufferSize);

    case SU_LIGHT_INTENSITY_ARRAY:
        return GetLightIntensityArray(pBuffer, nBufferSize);

    case SU_LIGHT_INDEX:
        return GetLightIndex(pBuffer, nBufferSize);

    case SU_LIGHT_INDEX_ARRAY:
        return GetLightIndexArray(pBuffer, nBufferSize);

    default:
        SU_ASSERT(!"Unknown light appupdate type");
    }
    return false;
}

//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// \param rLights Lights to put in the buffer
/// \param pBuffer Destination buffer for lighting data
/// \param nParamArraySize Number of elements in the effect parameter array (determines how many
/// entries will be written into
///                        the buffer)
/// \param func Function that returns light data to be written into the buffer. Must implement the
/// function-call signature
///             BufferT func( const SuLight* pLight )
///
/// \return true on success, false otherwise
//=================================================================================================================================
template <typename BufferT, typename LightDataFunc>
bool SuFillArrayWithLightData(const SuList<const SuLight*>& rLights,
                              BufferT*                      pBuffer,
                              uint32                        nParamArraySize,
                              LightDataFunc                 func)
{
    uint32   nNumLightsToFill = SuMin(nParamArraySize, static_cast<uint32>(rLights.size()));
    BufferT* pData            = reinterpret_cast<BufferT*>(pBuffer);
    SuList<const SuLight*>::const_iterator it = rLights.begin();

    for (uint32 i = 0; i < nNumLightsToFill; i++)
    {
        *pData = func(*it);
        pData++;
        ++it;
    }

    // fill the rest of the array with 0's
    memset(pData, 0, sizeof(BufferT) * (nParamArraySize - nNumLightsToFill));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the number of lights hitting an object
//=================================================================================================================================
bool SuLightUpdateHandler::GetNumLights(void* pBuffer, uint32 nBufferSize) const
{
    SU_ASSERT(nBufferSize >= sizeof(int32));

    const SuObject* pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    int32*          pDest       = reinterpret_cast<int32*>(pBuffer);
    *pDest =
        static_cast<int32>(m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj).size());
    return true;
}


//=================================================================================================================================
/// Fills a buffer with the position of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightPosition(void* pBuffer, uint32 nBufferSize) const
{
    SuVector4* pData = reinterpret_cast<SuVector4*>(pBuffer);
    *pData           = SuGetLightPosition(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the positions of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightPositionArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuVector4*>(pBuffer), m_nParamArraySize, SuGetLightPosition);
}

//=================================================================================================================================
/// Fills a buffer with the cutoff distance of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightCutoff(void* pBuffer, uint32 nBufferSize) const
{
    SuVector4* pData = reinterpret_cast<SuVector4*>(pBuffer);
    *pData           = SuGetLightCutoff(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the cutoff distances of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightCutoffArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuVector4*>(pBuffer), m_nParamArraySize, SuGetLightCutoff);
}

//=================================================================================================================================
/// Fills a buffer with the color of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightColor(void* pBuffer, uint32 nBufferSize) const
{
    SuVector4* pData = reinterpret_cast<SuVector4*>(pBuffer);
    *pData           = SuGetLightColor(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the colors of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightColorArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuVector4*>(pBuffer), m_nParamArraySize, SuGetLightColor);
}

//=================================================================================================================================
/// Fills a buffer with the parameters of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetMiscLightParams(void* pBuffer, uint32 nBufferSize) const
{
    SuVector4* pData = reinterpret_cast<SuVector4*>(pBuffer);
    *pData           = SuGetLightMiscParams(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the parameters of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetMiscLightParamsArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuVector4*>(pBuffer), m_nParamArraySize, SuGetLightMiscParams);
}

//=================================================================================================================================
/// Fills a buffer with the direction of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightDirection(void* pBuffer, uint32 nBufferSize) const
{
    SuVector4* pData = reinterpret_cast<SuVector4*>(pBuffer);
    *pData           = SuGetLightDirection(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the directions of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightDirectionArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuVector4*>(pBuffer), m_nParamArraySize, SuGetLightDirection);
}

//=================================================================================================================================
/// Fills a buffer with the cone angles of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightConeAngles(void* pBuffer, uint32 nBufferSize) const
{
    SuVector4* pData = reinterpret_cast<SuVector4*>(pBuffer);
    *pData           = SuGetLightConeAngles(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the cone angles of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightConeAnglesArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuVector4*>(pBuffer), m_nParamArraySize, SuGetLightConeAngles);
}

//=================================================================================================================================
/// Fills a buffer with the view-projection matrix of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightViewProjectionMatrix(void* pBuffer, uint32 nBufferSize) const
{
    SuMatrix4* pData = reinterpret_cast<SuMatrix4*>(pBuffer);
    *pData = SuGetLightViewProjectionMatrix(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the view-projection matrix of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightViewProjectionMatrixArray(void*  pBuffer,
                                                             uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(rLights,
                                    reinterpret_cast<SuMatrix4*>(pBuffer),
                                    m_nParamArraySize,
                                    SuGetLightViewProjectionMatrix);
}

//=================================================================================================================================
/// Fills a buffer with the view-projection matrix for texturing of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightViewProjTextureMatrix(void* pBuffer, uint32 nBufferSize) const
{
    SuMatrix4* pData = reinterpret_cast<SuMatrix4*>(pBuffer);
    *pData = SuGetLightViewProjTextureMatrix(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the view-projection matrix for texturing all lights hitting the current
/// object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightViewProjTextureMatrixArray(void*  pBuffer,
                                                              uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(rLights,
                                    reinterpret_cast<SuMatrix4*>(pBuffer),
                                    m_nParamArraySize,
                                    SuGetLightViewProjTextureMatrix);
}

//=================================================================================================================================
/// Fills a buffer with the view matrix of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightViewMatrix(void* pBuffer, uint32 nBufferSize) const
{
    SuMatrix4* pData = reinterpret_cast<SuMatrix4*>(pBuffer);
    *pData           = SuGetLightViewMatrix(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the view matrix of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightViewMatrixArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuMatrix4*>(pBuffer), m_nParamArraySize, SuGetLightViewMatrix);
}

//=================================================================================================================================
/// Fills a buffer with the projection matrix of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightProjectionMatrix(void* pBuffer, uint32 nBufferSize) const
{
    SuMatrix4* pData = reinterpret_cast<SuMatrix4*>(pBuffer);
    *pData = SuGetLightProjectionMatrix(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the projection matrix array of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightProjectionMatrixArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(rLights,
                                    reinterpret_cast<SuMatrix4*>(pBuffer),
                                    m_nParamArraySize,
                                    SuGetLightProjectionMatrix);
}

//=================================================================================================================================
/// Fills a buffer with the scale of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightScale(void* pBuffer, uint32 nBufferSize) const
{
    SuVector4* pData = reinterpret_cast<SuVector4*>(pBuffer);
    *pData           = SuGetLightScale(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the scale of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightScaleArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuVector4*>(pBuffer), m_nParamArraySize, SuGetLightScale);
}

//=================================================================================================================================
/// Fills a buffer with the orientation of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightOrientation(void* pBuffer, uint32 nBufferSize) const
{
    SuQuat* pData = reinterpret_cast<SuQuat*>(pBuffer);
    *pData        = SuGetLightOrientation(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the orientation of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightOrientationArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<SuQuat*>(pBuffer), m_nParamArraySize, SuGetLightOrientation);
}

//=================================================================================================================================
/// Fills a buffer with the shape of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightShape(void* pBuffer, uint32 nBufferSize) const
{
    int32* pData = reinterpret_cast<int32*>(pBuffer);
    *pData       = SuGetLightShape(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the shape of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightShapeArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<int32*>(pBuffer), m_nParamArraySize, SuGetLightShape);
}

//=================================================================================================================================
/// Fills a buffer with intensity of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightIntensity(void* pBuffer, uint32 nBufferSize) const
{
    float32* pData = reinterpret_cast<float32*>(pBuffer);
    *pData         = SuGetLightIntensity(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the intensity of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightIntensityArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<float32*>(pBuffer), m_nParamArraySize, SuGetLightIntensity);
}

//=================================================================================================================================
/// Fills a buffer with intensity of a named light source
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightIndex(void* pBuffer, uint32 nBufferSize) const
{
    int32* pData = reinterpret_cast<int32*>(pBuffer);
    *pData       = SuGetLightIndex(SuLightManager::GetRef().GetLightByName(m_lightName));
    return true;
}

//=================================================================================================================================
/// Fills a buffer with the intensity of all lights hitting the current object
//=================================================================================================================================
bool SuLightUpdateHandler::GetLightIndexArray(void* pBuffer, uint32 nBufferSize) const
{
    const SuObject*               pCurrentObj = SuObjectManager::GetRef().GetObjectBeingDrawn();
    const SuList<const SuLight*>& rLights =
        m_rObjectLightMapping.GetActiveLightsForObject(pCurrentObj);

    return SuFillArrayWithLightData(
        rLights, reinterpret_cast<int32*>(pBuffer), m_nParamArraySize, SuGetLightIndex);
}

//=================================================================================================================================
/// \return The light position in a vec4 (x, y, z, 1)
//=================================================================================================================================
SuVector4 SuGetLightPosition(const SuLight* pLight)
{
    SuPoint3 pos(0.0f, 0.0f, 0.0f);

    if (pLight != NULL)
    {
        switch (pLight->GetType())
        {
        case SuLight::POINT_LIGHT:
            pos = static_cast<const SuPointLight*>(pLight)->GetPosition();
            break;

        case SuLight::SPOT_LIGHT:
            pos = static_cast<const SuSpotLight*>(pLight)->GetPosition();
            break;

        case SuLight::DIRECTIONAL_LIGHT:
            // directional lights don't have a position, keep it at 0
            break;

        case SuLight::VOLUME_LIGHT:
            pos = static_cast<const SuVolumeLight*>(pLight)->GetPosition();
            break;

        default:
            SU_ASSERT(!"Unknown light type");
            break;
        }
    }

    return SuVector4(pos.x, pos.y, pos.z, 1.0f);
}

//=================================================================================================================================
/// \return The light cutoff distance parameters in a vec4 (light type, falloff exponent, distance,
/// unused)
//=================================================================================================================================
SuVector4 SuGetLightCutoff(const SuLight* pLight)
{
    if (pLight == NULL)
    {
        return SuVector4::ZERO;
    }

    float32 cutoff = 0.0f;

    if (pLight->GetType() == SuLight::POINT_LIGHT)
        cutoff = static_cast<const SuPointLight*>(pLight)->GetRadius();

    return SuVector4(static_cast<float32>(pLight->GetType()),
                     static_cast<float32>(pLight->GetFalloff()),
                     cutoff,
                     pLight->IsSpecular() ? 1.0f : 0.0f);
}

//=================================================================================================================================
/// \return The light color
//=================================================================================================================================
SuVector4 SuGetLightColor(const SuLight* pLight)
{
    if (pLight == NULL)
    {
        return SuVector4::ZERO;
    }

    return pLight->GetColor() * pLight->GetIntensity();
}

//=================================================================================================================================
/// \return The additional light parameters in a vec4: (light type, falloff exponent, diffuse mask,
/// specular mask)
//=================================================================================================================================
SuVector4 SuGetLightMiscParams(const SuLight* pLight)
{
    if (pLight == NULL)
    {
        return SuVector4::ZERO;
    }

    return SuVector4(static_cast<float32>(pLight->GetType()),
                     static_cast<float32>(pLight->GetFalloff()),
                     pLight->IsDiffuse() ? 1.0f : 0.0f,
                     pLight->IsSpecular() ? 1.0f : 0.0f);
}

//=================================================================================================================================
/// \return The light direction in a vec4: (x, y, z, 0)
//=================================================================================================================================
SuVector4 SuGetLightDirection(const SuLight* pLight)
{
    SuVector3 dir(0.0f, 0.0f, 0.0f);

    if (pLight != NULL)
    {
        switch (pLight->GetType())
        {
        case SuLight::POINT_LIGHT:
            // point lights don't have a direction, keep it a 0
            break;

        case SuLight::SPOT_LIGHT:
            dir = static_cast<const SuSpotLight*>(pLight)->GetDirection();
            break;

        case SuLight::DIRECTIONAL_LIGHT:
            dir = static_cast<const SuDirectionalLight*>(pLight)->GetDirection();
            break;

        case SuLight::VOLUME_LIGHT:
            // volume lights don't have a direction, keep it a 0
            break;

        default:
            SU_ASSERT(!"Unknown light type");
            break;
        }
    }

    return SuVector4(dir, 0.0f);
}

//=================================================================================================================================
/// \return The light cone angles in a vec4: (cos(cone angle), cos(hotspot angle), cone angle,
/// hotspot angle). The angles
///         are in degrees.
//=================================================================================================================================
SuVector4 SuGetLightConeAngles(const SuLight* pLight)
{
    if (pLight == NULL || pLight->GetType() != SuLight::SPOT_LIGHT)
    {
        return SuVector4::ZERO;
    }

    return SuVector4(
        SuCos(0.5f * SuDegToRad(static_cast<const SuSpotLight*>(pLight)->GetConeAngle())),
        SuCos(0.5f * SuDegToRad(static_cast<const SuSpotLight*>(pLight)->GetHotSpotAngle())),
        static_cast<const SuSpotLight*>(pLight)->GetDropOff(),
        0.0f);
}

//=================================================================================================================================
/// \return The light view-projection matrix
//=================================================================================================================================
SuMatrix4 SuGetLightViewProjectionMatrix(const SuLight* pLight)
{
    if (pLight == NULL || pLight->GetType() != SuLight::SPOT_LIGHT)
    {
        return SuMatrix4::IDENTITY;
    }

    return static_cast<const SuSpotLight*>(pLight)->GetViewProjectionMatrix();
}

//=================================================================================================================================
/// \return The light view-projection-texture matrix
//=================================================================================================================================
SuMatrix4 SuGetLightViewProjTextureMatrix(const SuLight* pLight)
{
    if (pLight == NULL || pLight->GetType() != SuLight::SPOT_LIGHT)
    {
        return SuMatrix4::IDENTITY;
    }

    // Matrix to scale texture coord to [0..1] range and flip y axis in texture space
    static const SuMatrix4 flipScaleBiasMat(0.5f,
                                            0.0f,
                                            0.0f,
                                            0.5f,
                                            0.0f,
                                            -0.5f,
                                            0.0f,
                                            0.5f,
                                            0.0f,
                                            0.0f,
                                            1.0f,
                                            0.0f,
                                            0.0f,
                                            0.0f,
                                            0.0f,
                                            1.0f);

    return flipScaleBiasMat * static_cast<const SuSpotLight*>(pLight)->GetViewProjectionMatrix();
}

//=================================================================================================================================
/// \return The light view matrix
//=================================================================================================================================
SuMatrix4 SuGetLightViewMatrix(const SuLight* pLight)
{
    if (pLight == NULL || pLight->GetType() != SuLight::SPOT_LIGHT)
    {
        return SuMatrix4::IDENTITY;
    }

    return static_cast<const SuSpotLight*>(pLight)->GetViewMatrix();
}

//=================================================================================================================================
/// \return The light projection matrix
//=================================================================================================================================
SuMatrix4 SuGetLightProjectionMatrix(const SuLight* pLight)
{
    if (pLight == NULL || pLight->GetType() != SuLight::SPOT_LIGHT)
    {
        return SuMatrix4::IDENTITY;
    }

    return static_cast<const SuSpotLight*>(pLight)->GetProjectionMatrix();
}

//=================================================================================================================================
/// \return The light scale in a vec3 (sx, sy, sz)
//=================================================================================================================================
SuVector4 SuGetLightScale(const SuLight* pLight)
{
    if (pLight == NULL || pLight->GetType() != SuLight::VOLUME_LIGHT)
    {
        return SuVector4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    SuVector3 scale = static_cast<const SuVolumeLight*>(pLight)->GetScale();
    return SuVector4(scale, 0.0f);
}

//=================================================================================================================================
/// \return The light orientation in a quaternion
//=================================================================================================================================
SuQuat SuGetLightOrientation(const SuLight* pLight)
{
    if (pLight == NULL || pLight->GetType() != SuLight::VOLUME_LIGHT)
    {
        return SuQuat(0.0f, 0.0f, 0.0f, 0.0f);
    }

    SuQuat ori = static_cast<const SuVolumeLight*>(pLight)->GetOrientation();

    // SuMath.shl uses quaternion(x,y,z,w) so change the order!!
    return SuQuat(ori.Y(), ori.Z(), ori.W(), ori.X());
}

//=================================================================================================================================
/// \return The light shape in an integer
//=================================================================================================================================
int32 SuGetLightShape(const SuLight* pLight)
{
    if (pLight == NULL || pLight->GetType() != SuLight::VOLUME_LIGHT)
    {
        return -1;
    }

    return static_cast<const SuVolumeLight*>(pLight)->GetLightShape();
}

//=================================================================================================================================
/// \return The light shape in an integer
//=================================================================================================================================
float32 SuGetLightIntensity(const SuLight* pLight)
{
    if (pLight == NULL)
        return -1;

    return pLight->GetIntensity();
}


//=================================================================================================================================
/// \return The light index in an integer
//=================================================================================================================================
int32 SuGetLightIndex(const SuLight* pLight)
{
    if (pLight == NULL)
        return -1;

    return pLight->GetIndex();
}
