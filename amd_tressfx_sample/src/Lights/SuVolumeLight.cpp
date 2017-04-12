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
#include "SuBoundingBox.h"
#include "SuFrustum.h"
#include "SuMath.h"
#include "SuVolumeLight.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <algorithm>

bool SphereAABBIntersection(const SuPoint3& center,
                            const float     radius,
                            const SuPoint3& min,
                            const SuPoint3& max);
SuFrustum MakeConeFrustum(const SuVector3& scale,
                          const SuQuat&    orientation,
                          const SuPoint3&  position);

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuVolumeLight::SuVolumeLight(const SuString& rName)
    : SuLight(SuLight::VOLUME_LIGHT, rName)
    , m_position(0.0f, 0.0f, 0.0f)
    , m_orientation(0.0f, 0.0f, 0.0f, 0.0f)
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_lightShape(SuLight::VOLUMELIGHT_SPHERE)
    , m_sushiLightIndirect(false)
{
}

//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuVolumeLight::~SuVolumeLight() {}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Tests whether the volume light casts any light on a bounding box
///
/// \param rBB The bounding box to test
///
/// \return true if the bounding box is hit by the light, false otherwise
//=================================================================================================================================
bool SuVolumeLight::IlluminatesBoundingBox(const SuBoundingBox& rBB) const
{
    // check if the bounding box is in the volume light
    if (m_lightShape == SuLight::VOLUMELIGHT_SPHERE)
    {
        // Radius for bounding sphere - will be used for SphereAABBIntersection test
        float tempMax = std::max(m_scale.x, m_scale.y);
        float radius  = std::max(tempMax, m_scale.z);

        bool test =
            SphereAABBIntersection(m_position, radius, rBB.GetMinCorner(), rBB.GetMaxCorner());
        if (test == true)
            int dummy = 0;
        return SphereAABBIntersection(m_position, radius, rBB.GetMinCorner(), rBB.GetMaxCorner());
    }
    else if (m_lightShape == SuLight::VOLUMELIGHT_CONE)
    {
        SuFrustum frustum = MakeConeFrustum(m_scale, m_orientation, m_position);

        return (frustum.CullAxisAlignedBox(rBB.GetMinCorner(), rBB.GetMaxCorner()) !=
                SuFrustum::OUTSIDE);
    }

    return false;
}

bool SphereAABBIntersection(const SuPoint3& center,
                            const float     radius,
                            const SuPoint3& min,
                            const SuPoint3& max)
{
    float dmin = 0;
    float e;

    if (center.x < min.x - radius || center.x > max.x + radius)
        return false;
    if (center.y < min.y - radius || center.y > max.y + radius)
        return false;
    if (center.z < min.z - radius || center.z > max.z + radius)
        return false;

    // x-axis
    if (center.x < min.x)
    {
        e = center.x - min.x;
        dmin += e * e;
    }
    else if (center.x > max.x)
    {
        e = center.x - max.x;
        dmin += e * e;
    }

    // y-axis
    if (center.y < min.y)
    {
        e = center.y - min.y;
        dmin += e * e;
    }
    else if (center.y > max.y)
    {
        e = center.y - max.y;
        dmin += e * e;
    }

    // z-axis
    if (center.z < min.z)
    {
        e = center.z - min.z;
        dmin += e * e;
    }
    else if (center.z > max.z)
    {
        e = center.z - max.z;
        dmin += e * e;
    }

    if (dmin <= radius * radius)
        return true;

    return false;
}

SuFrustum MakeConeFrustum(const SuVector3& scale,
                          const SuQuat&    orientation,
                          const SuPoint3&  position)
{
    SuVector3 vert[7];
    vert[0] = SuVector3(0, 0, 0);
    vert[1] = SuVector3(-1, -1, 1);
    vert[2] = SuVector3(-1, -1, -1);
    vert[3] = SuVector3(1, -1, -1);
    vert[4] = SuVector3(1, -1, 1);
    // temporary vertices to define top plane
    vert[5] = SuVector3(1, 0, -1);
    vert[6] = SuVector3(1, 0, 1);

    // transform
    SuMatrix4 mTransform;
    mTransform.MakeTransform(SuVector3(position.x, position.y, position.z), orientation, scale);

    for (int i = 0; i < 7; i++)
    {
        SuVector4 tempVert = mTransform * SuVector4(vert[i], 1);
        vert[i]            = tempVert.XYZ();
    }

    SuPlane plane[6];
    plane[SuFrustum::LEFT]   = SuPlane(vert[0], vert[2], vert[1]);
    plane[SuFrustum::RIGHT]  = SuPlane(vert[0], vert[4], vert[3]);
    plane[SuFrustum::BOTTOM] = SuPlane(vert[1], vert[2], vert[4]);
    plane[SuFrustum::TOP]    = SuPlane(vert[6], vert[0], vert[5]);
    plane[SuFrustum::HITHER] = SuPlane(vert[1], vert[4], vert[0]);
    plane[SuFrustum::YON]    = SuPlane(vert[3], vert[2], vert[0]);

    SuFrustum frustum(plane);
    return frustum;

    /*	Half-cube, instead of Pyramid

        SuPlane planes[6];
        SuVector3 p[7];
        p[0] = SuVector3(-1, 0, 0); //left
        p[1] = SuVector3( 1, 0, 0); //right
        p[2] = SuVector3( 0,-1, 0); //bottom
        p[3] = SuVector3( 0, 1, 0); //top
        p[4] = SuVector3( 0, 0,-1); //hither
        p[5] = SuVector3( 0, 0, 1); //yon
        p[6] = SuVector3( 0, 0, 0); //origin

        //transform
        SuMatrix4 mTransform;
        mTransform.MakeTransform(SuVector3(position.x, position.y, position.z), orientation,
       scale);

        for(int i=0; i<7; i++)
        {
            SuVector4 tempVert = mTransform*SuVector4(p[i],1);
            p[i] = tempVert.XYZ();
        }

        SuVector3 o = p[6];
        for(int i=0; i<6; i++) {
            planes[i] = SuPlane(p[i]-o, p[i]);
        }

        // remake top plane
        planes[SuFrustum::TOP] = SuPlane(p[SuFrustum::TOP]-o, o);

        SuFrustum frustum=SuFrustum(planes);
        return frustum;*/
}