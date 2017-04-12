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

#ifndef _SU_DIRECTIONALLIGHT_H_
#define _SU_DIRECTIONALLIGHT_H_

#include "SuPointLight.h"
#include "SuVector3.h"

//=========================================================================================================
/// \ingroup Engine
/// \brief Directional light
///
//=========================================================================================================
// tolua_begin
class SuDirectionalLight : public SuLight
{
public:
    /// Constructor
    SuDirectionalLight(const SuString& rName);

    /// Destructor
    virtual ~SuDirectionalLight();

    /// Sets the light direction
    void SetDirection(const SuVector3& rDir) { m_dir = rDir; }

    /// Returns the light direction
    const SuVector3& GetDirection() const { return m_dir; }

    /// Tests whether the directional light casts any light on a bounding box
    virtual bool IlluminatesBoundingBox(const SuBoundingBox& rBV) const;

    // tolua_end

private:
    SuVector3 m_dir;

    // tolua_begin
};
// tolua_end


#endif  // _SU_DIRECTIONALLIGHT_H_
