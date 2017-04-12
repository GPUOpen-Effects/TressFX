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

#ifndef _SU_LIGHTUPDATEHANDLER_H_
#define _SU_LIGHTUPDATEHANDLER_H_

#include "SuUpdateHandler.h"

class SuObjectLightTable;
//=========================================================================================================
/// \ingroup Engine
/// \brief  An App update handler for light source attributes
///
//=========================================================================================================
class SuLightUpdateHandler : public SuUpdateHandler
{
public:
    /// Enumeration of all App-Update parameters that the engine supports
    enum AppUpdateType
    {
        SU_NUM_LIGHTS,            ///< Number of lights hitting current object
        SU_LIGHT_POSITION,        ///< Light position for a named light source
        SU_LIGHT_POSITION_ARRAY,  ///< Array of light positions for all lights hitting current
                                  /// object
        SU_LIGHT_CUTOFF,          ///< Cutoff distance
        SU_LIGHT_CUTOFF_ARRAY,    ///< Cutoff distance array for all lights hitting current object
        SU_LIGHT_COLOR,           ///< Light color for a named light source
        SU_LIGHT_COLOR_ARRAY,     ///< Array of light colors for all lights hitting current object
        SU_LIGHT_PARAMS,          ///< vec4: type, falloff exponent, diffuse mask, specular mask
        SU_LIGHT_PARAMS_ARRAY,
        SU_LIGHT_DIRECTION,  ///< Direction that a directional or spot light points in
        SU_LIGHT_DIRECTION_ARRAY,
        SU_LIGHT_CONE_ANGLES,  ///< vec4: cos(cone angle), cos(hotspot angle), cone angle, hotspot
                               /// angle (in degrees)
        SU_LIGHT_CONE_ANGLES_ARRAY,
        SU_LIGHT_VIEW_PROJ_MATRIX,  ///< Viewprojection matrix of a spot light
        SU_LIGHT_VIEW_PROJ_MATRIX_ARRAY,
        SU_LIGHT_VIEW_PROJ_TEX_MATRIX,  ///< Viewprojection matrix of a spot light for projective
                                        /// texturing
        SU_LIGHT_VIEW_PROJ_TEX_MATRIX_ARRAY,
        SU_LIGHT_VIEW_MATRIX,  ///< View matrix of a spot light
        SU_LIGHT_VIEW_MATRIX_ARRAY,
        SU_LIGHT_PROJ_MATRIX,  ///< Projection matrix of a spot light
        SU_LIGHT_PROJ_MATRIX_ARRAY,
        SU_LIGHT_SCALE,  ///< Scale of a volume light
        SU_LIGHT_SCALE_ARRAY,
        SU_LIGHT_ORIENTATION,  ///< Orientation of a volume light
        SU_LIGHT_ORIENTATION_ARRAY,
        SU_LIGHT_SHAPE,  ///< Shape of a volume light
        SU_LIGHT_SHAPE_ARRAY,
        SU_LIGHT_INTENSITY,
        SU_LIGHT_INTENSITY_ARRAY,
        SU_LIGHT_INDEX,
        SU_LIGHT_INDEX_ARRAY
    };

    /// Constructor
    SuLightUpdateHandler(AppUpdateType             type,
                         uint32                    nParamArraySize,
                         const SuObjectLightTable& rObjectLightMapping,
                         const SuString&           rLightName = SuString(""));

    /// Callback method which is used to retrieve the value of an app-updatable parameter.
    virtual bool GetUpdateValue(void* pBuffer, uint32 nBufferSize);

private:
    /// Fills a buffer with the number of lights hitting an object
    bool GetNumLights(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the position of a named light source
    bool GetLightPosition(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the positions of all lights hitting the current object
    bool GetLightPositionArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the cutoff distance of a named light source
    bool GetLightCutoff(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the cutoff distances of all lights hitting the current object
    bool GetLightCutoffArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the color of a named light source
    bool GetLightColor(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the colors of all lights hitting the current object
    bool GetLightColorArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the parameters of a named light source
    bool GetMiscLightParams(void* pBuffer, uint32 nBufferSize)
        const;  // type, falloff exponent, diffuse flag, specular flag

    /// Fills a buffer with the parameters all lights hitting the current object
    bool GetMiscLightParamsArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the direction of a named light source
    bool GetLightDirection(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the direction all lights hitting the current object
    bool GetLightDirectionArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the cone angles of a named light source
    bool GetLightConeAngles(void* pBuffer, uint32 nBufferSize)
        const;  // outer angle, inner angle, cos(outer), cos(inner)

    /// Fills a buffer with the cone angles all lights hitting the current object
    bool GetLightConeAnglesArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the view-projection matrix of a named light source
    bool GetLightViewProjectionMatrix(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the view-projection matrix all lights hitting the current object
    bool GetLightViewProjectionMatrixArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the view-projection matrix for texturing of a named light source
    bool GetLightViewProjTextureMatrix(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the view-projection matrix for texturing all lights hitting the current
    /// object
    bool GetLightViewProjTextureMatrixArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the view matrix of a named light source
    bool GetLightViewMatrix(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the view matrix all lights hitting the current object
    bool GetLightViewMatrixArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the projection matrix of a named light source
    bool GetLightProjectionMatrix(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the projection matrix all lights hitting the current object
    bool GetLightProjectionMatrixArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the scale of a named light source
    bool GetLightScale(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the scales of all lights hitting the current object
    bool GetLightScaleArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the scale of a named light source
    bool GetLightOrientation(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the orientations of all lights hitting the current object
    bool GetLightOrientationArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the shape of a named light source
    bool GetLightShape(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the shapes of all lights hitting the current object
    bool GetLightShapeArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with intensity of a named light source
    bool GetLightIntensity(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the intensity of all lights hitting the current object
    bool GetLightIntensityArray(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with intensity of a named light source
    bool GetLightIndex(void* pBuffer, uint32 nBufferSize) const;

    /// Fills a buffer with the intensity of all lights hitting the current object
    bool GetLightIndexArray(void* pBuffer, uint32 nBufferSize) const;

    AppUpdateType m_appUpdateType;

    uint32 m_nParamArraySize;

    const SuObjectLightTable& m_rObjectLightMapping;

    SuString m_lightName;
};


#endif  // _SU_LIGHTUPDATEHANDLER_H_
