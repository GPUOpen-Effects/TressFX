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
#include "SuBoundingBox.h"
#include "SuFrustum.h"
#include "SuMath.h"

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuAreaLight::SuAreaLight(const SuString& rName)
    : SuLight(SuLight::AREA_LIGHT, rName)
    , m_position(0.0f, 0.0f, 0.0f)
    , m_orientation(0.0f, 0.0f, 0.0f, 0.0f)
    , m_scale(1.0f, 1.0f, 1.0f)
{
}

//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuAreaLight::~SuAreaLight() {}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Tests whether the Area light casts any light on a bounding box
///
/// \param rBB The bounding box to test
///
/// \return true if the bounding box is hit by the light, false otherwise
//=================================================================================================================================
bool SuAreaLight::IlluminatesBoundingBox(const SuBoundingBox& rBB) const
{
    // light orientation
    SuMatrix4 mRotation;
    m_orientation.ToRotationMatrix(mRotation);

    // light direction - (negative z)
    SuVector3 dir = mRotation * SuVector3(0.0, 0.0, -1.0);

    // get each corners of the bounding box
    SuPoint3 minCorner = rBB.GetMinCorner();
    SuPoint3 maxCorner = rBB.GetMaxCorner();

    SuVector3 vCorners[8];
    vCorners[1] = SuVector3(
        minCorner.x - m_position.x, maxCorner.y - m_position.y, minCorner.z - m_position.z);
    vCorners[2] = SuVector3(
        maxCorner.x - m_position.x, minCorner.y - m_position.y, minCorner.z - m_position.z);
    vCorners[3] = SuVector3(
        maxCorner.x - m_position.x, maxCorner.y - m_position.y, minCorner.z - m_position.z);
    vCorners[4] = SuVector3(
        minCorner.x - m_position.x, minCorner.y - m_position.y, maxCorner.z - m_position.z);
    vCorners[5] = SuVector3(
        minCorner.x - m_position.x, maxCorner.y - m_position.y, maxCorner.z - m_position.z);
    vCorners[6] = SuVector3(
        maxCorner.x - m_position.x, minCorner.y - m_position.y, maxCorner.z - m_position.z);
    vCorners[7] = SuVector3(
        maxCorner.x - m_position.x, maxCorner.y - m_position.y, maxCorner.z - m_position.z);

    // check if any corner of the bounding box in front of the light
    for (int i = 0; i < 8; i++)
    {
        if (dir.Dot(vCorners[i]) > 0)
            return true;
    }

    return false;
}