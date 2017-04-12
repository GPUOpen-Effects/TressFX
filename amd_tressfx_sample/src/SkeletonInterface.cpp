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

#include "SkeletonInterface.h"
#include "SuAnimatedModel.h"
#include "SuBone.h"
#include "SuLog.h"
#include "SuObjectManager.h"
#include "SuSkeleton.h"

void SkeletonInterface::SetSkeleton(const SuSkeleton* s) 
{ 
    m_Skeleton = s; 
}

void SkeletonInterface::SetModel(const SuString& modelname)
{
    // Get a model by its name
    SuObject* pObject = SuObjectManager::GetRef().GetObjectByName(modelname);

    if (pObject == nullptr)
    {
        SuLogError("Could not find object called %s", modelname);
        return;
    }

    SuAnimatedModel* pModel = dynamic_cast<SuAnimatedModel*>(pObject);
    m_Skeleton              = pModel->GetSkeleton();
}

const char* SkeletonInterface::GetBoneNameByIndex(unsigned int index) const
{
    if (m_Skeleton == nullptr)
    {
        // You need to set skeleton first.
        return 0;
    }
    return m_Skeleton->GetBoneByIndex(index)->GetName();
}

unsigned int SkeletonInterface::GetNumberOfBones() const
{
    if (m_Skeleton == nullptr)
    {
        // You need to set skeleton first.
        return 0;
    }

    return m_Skeleton->GetNumBones();
}

unsigned int SkeletonInterface::GetBoneIndexByName(const char* name) const
{
    if (m_Skeleton == nullptr)
    {
        // You need to set skeleton first.
        return 0;
    }

    for (unsigned int i = 0; i < m_Skeleton->GetNumBones(); ++i)
    {
        const SuBone* pBone = m_Skeleton->GetBoneByIndex(i);
        if (pBone->GetName() == name)
            return i;
    }

    return 0;
}