// ----------------------------------------------------------------------------
// This is an example TressFXSkeletonInterface implementation. 
//
// TressFXSkeletonInterface is optionally used to remap bones to engine ordering
// if it differs from Maya. 
// ----------------------------------------------------------------------------
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

#ifndef _SKELETONINTERFACE_H
#define _SKELETONINTERFACE_H

#include "TressFXBoneSkeletonInterface.h"

#include "SuString.h"

class SuSkeleton;

class SkeletonInterface : public TressFXSkeletonInterface
{
public:
    virtual unsigned int GetBoneIndexByName(const char* name) const;

    virtual const char* GetBoneNameByIndex(unsigned int index) const;

    virtual  unsigned int GetNumberOfBones() const;

    void SetSkeleton(const SuSkeleton* s);

    void SetModel(const SuString& modelname);

private:
    const SuSkeleton* m_Skeleton;
};

#endif
