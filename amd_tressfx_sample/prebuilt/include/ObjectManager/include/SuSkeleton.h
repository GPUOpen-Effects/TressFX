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

#ifndef _SU_SKELETON_H_
#define _SU_SKELETON_H_

#include "SuTypes.h"
#include "SuArray.h"
#include "SuHashMap.h"
#include "SuCountedPtr.h"

class SuAnimationCurve;
class SuMatrix4;
class SuBone;

class SuBoneAnimator;
class SuSkeleton;
typedef SuCountedPtr<SuSkeleton> SuSkeletonPtr;

class SuFile;
//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief 
/// 
//=================================================================================================================================
// tolua_begin
class SuSkeleton 
{
public:
  
   /// Constructor
   SU_DLL SuSkeleton();
   
   /// Destructor
   SU_DLL ~SuSkeleton();

   /// Factory method to construct a skeleton from a text file
   static SU_DLL SuSkeleton* ConstructFromFile( SuFile* pFile );

   /// Returns the number of bones in this skeleton
   SU_DLL uint32 GetNumBones() const;

   /// Accessor for bones by index
   SU_DLL const SuBone* GetBoneByIndex( uint32 nIndex ) const;

   /// Accessor for bones by name
   SU_DLL const SuBone* GetBoneByName( const SuString& rBoneName ) const;

   /// Adds a bone to the skeleton
   SU_DLL void AddBone( SuBone* pBone, const SuString& rParentName );

   /// Exports the skeleton to a file
   SU_DLL bool SaveToFile( SuFile* pFile ) const;

   /// Converts bone transformations from bone space to world space
   SU_DLL void ConvertBonesToWorldSpace( SuArray<SuMatrix4>& rBoneTransforms ) const;

// tolua_end
private:

   /// Magic number to indicate a 'null' bone index
   static const int32 INVALID_BONE_INDEX = -1;

   /// Current version string to put into skeleton files
   static const SuString SKELETON_FILE_VERSION_STRING ;

   /// Previous version strings, for backwards compat.
   static const SuString SKELETON_FILE_V1_VERSION_STRING;

   /// Helper method to locate the index of a bone with a particular name
   int32 GetBoneIndexByName( const SuString& rName ) const;

   /// Array of bones
   SuArray<SuBone*> m_bones;

   /// Array containing the index of each bone's parent.  This is used to facilitate
   SuArray<int32> m_parentIndices;


   typedef SuHashMap< SuString, uint32, SuStringHasher, SuHashComparer<SuString> > BoneNameMap;
   BoneNameMap m_boneNameMap;

// tolua_begin
};
// tolua_end


#endif // _SU_SKELETON_H_
