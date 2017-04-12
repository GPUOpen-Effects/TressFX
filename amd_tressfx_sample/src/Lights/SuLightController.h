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

#ifndef _SU_LIGHTCONTROLLER_H_
#define _SU_LIGHTCONTROLLER_H_

#include "SuAnimationCurve.h"
#include "SuCountedPtr.h"
#include "SuTypes.h"

class SuLight;

//=========================================================================================================
/// \ingroup Engine
/// \brief Animation controller for lights
///
//=========================================================================================================
// tolua_begin
class SuLightController
{
public:
    enum CurveIndex
    {
        COLOR_INDEX          = 0,
        INTENSITY_INDEX      = 1,
        DIFFUSE_INDEX        = 2,
        SPECULAR_INDEX       = 3,
        FALLOFF_INDEX        = 4,
        POS_INDEX            = 5,
        RADIUS_INDEX         = 6,
        ORIENTATION_INDEX    = 7,
        SCALE_INDEX          = 8,
        CONE_ANGLE_INDEX     = 9,
        PENUMBRA_ANGLE_INDEX = 10,
        DROP_OFF_INDEX       = 11,
        VISIBILITY_INDEX     = 12,
        SHAPE_INDEX          = 13,
        INDIRECT_INDEX       = 14,
        NUM_CURVE_TYPES      = 15
    };

    /// One bit for each kind of curve.
    enum CurveMask
    {
        COLOR_MASK          = 1 << COLOR_INDEX,
        INTENSITY_MASK      = 1 << INTENSITY_INDEX,
        DIFFUSE_MASK        = 1 << DIFFUSE_INDEX,
        SPECULAR_MASK       = 1 << SPECULAR_INDEX,
        FALLOFF_MASK        = 1 << FALLOFF_INDEX,
        POS_MASK            = 1 << POS_INDEX,
        RADIUS_MASK         = 1 << RADIUS_INDEX,
        ORIENTATION_MASK    = 1 << ORIENTATION_INDEX,
        SCALE_MASK          = 1 << SCALE_INDEX,
        CONE_ANGLE_MASK     = 1 << CONE_ANGLE_INDEX,
        PENUMBRA_ANGLE_MASK = 1 << PENUMBRA_ANGLE_INDEX,
        DROP_OFF_MASK       = 1 << DROP_OFF_INDEX,
        VISIBILITY_MASK     = 1 << VISIBILITY_INDEX,
        SHAPE_MASK          = 1 << SHAPE_INDEX,
        INDIRECT_MASK       = 1 << INDIRECT_INDEX,
        ALL_MASK            = (1 << (INDIRECT_INDEX + 1)) - 1
    };

    /// Constructor
    SuLightController(SuAnimationCurvePtr pColorCurve,
                      SuAnimationCurvePtr pIntensityCurve,
                      SuAnimationCurvePtr pDiffuseCurve,
                      SuAnimationCurvePtr pSpecularCurve,
                      SuAnimationCurvePtr pFalloffCurve,
                      SuAnimationCurvePtr pPosCurve,
                      SuAnimationCurvePtr pRadiusCurve,
                      SuAnimationCurvePtr pOrientationCurve,
                      SuAnimationCurvePtr pScaleCurve,
                      SuAnimationCurvePtr pConeAngleCurve,
                      SuAnimationCurvePtr pPenumbraAngleCurve,
                      SuAnimationCurvePtr pDropOffCurve,
                      SuAnimationCurvePtr pVisibilityCurve,
                      SuAnimationCurvePtr pLightShapeCurve,
                      SuAnimationCurvePtr pSushiLightIndirectCurve);

    /// Destructor
    virtual ~SuLightController();

    /// Updates the controlled light
    virtual void Update(SuTime fTime, SuLight* pLight);

    //=================================================================================================================================
    /// \returns whether any of the specified curves changed value in the last call to Update.
    ///
    /// Returns true if
    /// <li>Any curve whose curveMask bit is on changed value in the last call to Update.</li>
    /// <li>Update has only been called once, and in that call, the controller set a value in the
    /// light.</li>
    /// <li>Update has not yet been called.</li>
    ///
    /// Whether a value has changed in the controller is independent of the light used in the last
    /// call to update.
    /// It also doesn't depend on whether the light was visible, which is something that might be
    /// reconsidered.
    //=================================================================================================================================
    bool HasChanged(CurveMask curveMask) const
    {
        return (curveMask & m_nChangedMask & ALL_MASK) != 0;
    }

    /// \returns whether any of the specified curves change value over time.
    bool IsDynamic(CurveMask curveMask) const
    {
        return (curveMask & m_nDynamicMask & ALL_MASK) != 0;
    }

    // tolua_end

private:
    /// Returns the maximum length of any animation curve
    float32 ComputeAnimationLength() const;

    /// Precompuatation to find which curves change over time.
    uint32 ComputeDynamicMask() const;

    /// Used by caching system to register when a value changes.
    void ChangeCheck(uint32 index, void* newValue);

    float32 m_fAnimLength;  ///< Animation length in seconds
    SuTime  m_fLastTime;    ///< Argument to last call of Update

    SuAnimationCurvePtr m_pCurves[NUM_CURVE_TYPES];

    uint32 m_nDynamicMask;  ///< Which curves change over time.
    uint32 m_nChangedMask;  ///< Which curves changed in last call to Update.

    static const uint32 CACHE_SIZE = 512;
    uint8               m_pCache[CACHE_SIZE];
    uint32              m_pCacheOffsets[NUM_CURVE_TYPES];
    bool                m_bFirstUpdate;
    // tolua_begin
};
// tolua_end

#endif  // _SU_LIGHTCONTROLLER_H_
