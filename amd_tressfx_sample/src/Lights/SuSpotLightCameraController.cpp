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
#include "SuCamera.h"
#include "SuSpotLight.h"
#include "SuSpotLightCameraController.h"

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuSpotLightCameraController::SuSpotLightCameraController(SuSpotLight* pSpotLight)
    : m_pSpotLight(pSpotLight), m_fLastTime(-SU_FLT_MAX)
{
}

//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuSpotLightCameraController::~SuSpotLightCameraController() {}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Updates the camera parameters by copying them from the spot light
//=================================================================================================================================
void SuSpotLightCameraController::Update(SuTime t)
{
    if (m_fLastTime == t)
    {
        return;
    }
    m_fLastTime = t;

    SuCamera* pCam = GetCamera();
    if (!pCam)
    {
        return;
    }

    m_pSpotLight->Update(t);

    const SuCamera& rSpotLightCam = m_pSpotLight->GetSpotLightCamera();
    pCam->SetPosition(rSpotLightCam.GetPosition());
    pCam->SetOrientation(
        rSpotLightCam.GetDirection(), rSpotLightCam.GetUpVector(), rSpotLightCam.GetRightVector());
    float32 fFrustum[6];
    rSpotLightCam.GetFrustum(
        &fFrustum[0], &fFrustum[1], &fFrustum[2], &fFrustum[3], &fFrustum[4], &fFrustum[5]);
    pCam->SetFrustum(fFrustum[0], fFrustum[1], fFrustum[2], fFrustum[3], fFrustum[4], fFrustum[5]);
}
