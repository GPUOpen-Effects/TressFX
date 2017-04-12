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

#ifndef _SU_VOLUMELIGHT_H_
#define _SU_VOLUMELIGHT_H_

#include "SuLight.h"
#include "SuPoint3.h"
#include "SuQuat.h"
#include "SuVector3.h"

//=========================================================================================================
/// \ingroup Engine
/// \brief Volume light source
///
//=========================================================================================================
// tolua_begin
class SuVolumeLight : public SuLight
{
public:
    /// Constructor
    SuVolumeLight(const SuString& rName);

    /// Destructor
    virtual ~SuVolumeLight();

    /// Sets the light position
    void SetPosition(const SuPoint3& rPos) { m_position = rPos; }

    /// Returns the light position
    const SuPoint3& GetPosition() const { return m_position; }

    /// Sets the light orientation
    void SetOrientation(const SuQuat& rOrientation) { m_orientation = rOrientation; }

    /// Returns the light orientation
    const SuQuat& GetOrientation() const { return m_orientation; }

    /// Sets the light shape scale
    void SetScale(const SuVector3& rScale) { m_scale = rScale; }

    /// Returns the light scale
    const SuVector3 GetScale() const { return m_scale; }

    /// Sets the light shape
    void SetLightShape(LightShapeType rLightShape) { m_lightShape = rLightShape; }

    /// Returns the light shape
    LightShapeType GetLightShape() const { return m_lightShape; }

    /// Sets the light shape
    void SetSushiLightIndirect(bool bSushiLightIndirect)
    {
        m_sushiLightIndirect = bSushiLightIndirect;
    }

    /// Returns the light shape
    bool GetSushiLightIndirect() const { return m_sushiLightIndirect; }

    /// Tests whether the volume light casts any light on a bounding box
    virtual bool IlluminatesBoundingBox(const SuBoundingBox& rBV) const;

    // tolua_end

protected:
    SuPoint3       m_position;
    SuQuat         m_orientation;
    SuVector3      m_scale;
    LightShapeType m_lightShape;
    bool           m_sushiLightIndirect;

    // tolua_begin
};
// tolua_end

#endif  // _SU_VOLUMELIGHT_H_