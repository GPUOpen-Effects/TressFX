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

#ifndef _SU_BONE_H_
#define _SU_BONE_H_

#include "SuCommon.h"

#include "SuMatrix4.h"

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief This class represents a bone in an SuSkeleton
/// 
//=================================================================================================================================
// tolua_begin
class SuBone
{
public:

   /// Constructs a bone
   SU_DLL SuBone( const SuString& rBoneName, const SuMatrix4& rBoneTransform );

   /// Returns the name of this bone
   inline const SuString& GetName() const { return m_boneName; };
   
   /// Returns the model-to-bone transform for this bone
   inline const SuMatrix4& GetBoneTransform() const { return m_boneTransform; };

   /// Returns the bone-to-model transform for this bone
   inline const SuMatrix4& GetWorldTransform() const { return m_worldTransform; };

   /// Returns a pointer to the parent of this bone, or NULL if it has none
   inline const SuBone* GetParent() const { return m_pParent; };

// tolua_end
   /// Sets the parent of this bone
   inline void SetParent( SuBone* pParent ) { m_pParent = pParent; };

private:


   SuString m_boneName;          ///< Name of this bone
   SuMatrix4 m_boneTransform;    ///< The world to bone transform for this bone
   SuMatrix4 m_worldTransform;   ///< The bone to world transform for this bone
   SuBone* m_pParent;            ///< The parent of this bone



// tolua_begin
};
// tolua_end


#endif // _SU_BONE_H_
