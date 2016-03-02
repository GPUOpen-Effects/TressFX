//--------------------------------------------------------------------------------------
// File: Util.cpp
//
//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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
//--------------------------------------------------------------------------------------

#pragma once
#include "AMD_Types.h"
#include "Util.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace DirectX;

typedef double StatsCounterType;

#define RAY_EPSILON 1e-3f

#if defined(WIN32)
#define memalign(a, b) _aligned_malloc(b, a)
#elif defined(__APPLE__)
#define memalign(a, b) valloc(b)
#elif defined(__OpenBSD__)
#define memalign(a, b) malloc(b)
#endif


void *AllocAligned(size_t size)
{
#ifndef L1_CACHE_LINE_SIZE
#define L1_CACHE_LINE_SIZE 64
#endif
    return memalign(L1_CACHE_LINE_SIZE, size);
}


void FreeAligned(void *ptr)
{
#ifdef WIN32 // NOBOOK
    _aligned_free(ptr);
#else // NOBOOK
    free(ptr);
#endif // NOBOOK
}

float Log2(float x)
{
    static float invLog2 = 1.f / logf(2.f);
    return logf(x) * invLog2;
}

BBox Union(const BBox &b, const Float3 &p)
{
    BBox ret = b;
    ret.pMin.x = min(b.pMin.x, p.x);
    ret.pMin.y = min(b.pMin.y, p.y);
    ret.pMin.z = min(b.pMin.z, p.z);
    ret.pMax.x = max(b.pMax.x, p.x);
    ret.pMax.y = max(b.pMax.y, p.y);
    ret.pMax.z = max(b.pMax.z, p.z);
    return ret;
}

BBox Union(const BBox &b, const BBox &b2)
{
    BBox ret;
    ret.pMin.x = min(b.pMin.x, b2.pMin.x);
    ret.pMin.y = min(b.pMin.y, b2.pMin.y);
    ret.pMin.z = min(b.pMin.z, b2.pMin.z);
    ret.pMax.x = max(b.pMax.x, b2.pMax.x);
    ret.pMax.y = max(b.pMax.y, b2.pMax.y);
    ret.pMax.z = max(b.pMax.z, b2.pMax.z);
    return ret;
}

inline Float3 Normalize(const Float3 &v)
{
    return v / v.Length();
}

inline float Distance(const Float3 &p1, const Float3 &p2)
{
    return (p1-p2).Length();
}

inline Float3 Cross(const Float3 &v1, const Float3 &v2)
{
    return Float3((v1.y * v2.z) - (v1.z * v2.y),
                  (v1.z * v2.x) - (v1.x * v2.z),
                  (v1.x * v2.y) - (v1.y * v2.x));
}

inline float Dot(const Float3 &v1, const Float3 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Float3 TransformPoint(const Matrix4x4 &m, const Float3 &pt)
{
    float x = pt.x, y = pt.y, z = pt.z;
    float xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
    float yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
    float zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
    float wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];

    assert(wp != 0);

    return Float3(xp, yp, zp)/wp;
}

void BBox::BoundingSphere(Float3 *c, float *rad) const
{
    *c = pMin * .5f + pMax * .5f;
    *rad = Inside(*c) ? Distance(*c, pMax) : 0.f;
}
