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

#ifndef _SU_POINTLIGHT_H_
#define _SU_POINTLIGHT_H_

#include "SuLight.h"
#include "SuPoint3.h"

//=========================================================================================================
/// \ingroup Engine
/// \brief Point light source
///
//=========================================================================================================
// tolua_begin
class SuPointLight : public SuLight
{
public:
    /// Constructor
    SuPointLight(const SuString& rName);

    /// Destructor
    virtual ~SuPointLight();

    /// Sets the light position
    void SetPosition(const SuPoint3& rPos) { m_position = rPos; }

    /// Returns the light position
    const SuPoint3& GetPosition() const { return m_position; }

    /// Sets the light radius
    void SetRadius(const float32 fRadius) { m_fRadius = fRadius; }

    /// Returns the light radius
    float32 GetRadius() const { return m_fRadius; }

    /// Tests whether the point light casts any light on a bounding box
    virtual bool IlluminatesBoundingBox(const SuBoundingBox& rBV) const;

    // tolua_end

private:
    SuPoint3 m_position;
    float32  m_fRadius;

    // tolua_begin
};
// tolua_end

#endif  // _SU_POINTLIGHT_H_
