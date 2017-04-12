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

#ifndef _SU_SPOTLIGHTCAMERACONTROLLER_H_
#define _SU_SPOTLIGHTCAMERACONTROLLER_H_

#include "SuCameraController.h"

class SuSpotLight;

//=========================================================================================================
/// \ingroup Engine
/// \brief Camera controller that keeps a camera lined up with a spot light
///
//=========================================================================================================
// tolua_begin
class SuSpotLightCameraController : public SuCameraController
{
public:
    /// Constructor
    SuSpotLightCameraController(SuSpotLight* pSpotLight);

    /// Destructor
    virtual ~SuSpotLightCameraController();

    /// Returns the controlling spot light
    SuSpotLight* GetSpotLight() const { return m_pSpotLight; }

    /// Updates the camera parameters by copying them from the spot light
    virtual void Update(SuTime t);

    // tolua_end
private:
    SuSpotLight* m_pSpotLight;
    SuTime       m_fLastTime;

    // tolua_begin
};
// tolua_end


#endif  // _SU_SPOTLIGHTCAMERACONTROLLER_H_
