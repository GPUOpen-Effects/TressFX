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

#ifndef _SU_ANIMATIONSET_H_
#define _SU_ANIMATIONSET_H_

#include "SuHashMap.h"
#include "SuAnimationCurve.h"

class SuArchive;

#if 0
// tolua_begin
// this counted pointer typedef needs to be repeated here for tolua's benefit because tolua does not chase #includes
class SuAnimationCurve;
typedef SuCountedPtr<SuAnimationCurve> SuAnimationCurvePtr;
// tolua_end
#endif

class SuAnimationSet;
typedef SuCountedPtr<SuAnimationSet> SuAnimationSetPtr;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A set of named animation curves
/// 
/// An animation set is a container for animation curves.  
///
//=================================================================================================================================

// tolua_begin
class SuAnimationSet
{
public:

   /// Constructs an empty animation set
   SU_DLL SuAnimationSet();

   /// Destroys the animation set and all of its animations
   virtual SU_DLL ~SuAnimationSet();

   /// Clear the animation set
   SU_DLL void Clear();

   /// Loads an animation set from an animation file
   static SU_DLL SuAnimationSet* CreateFromFile( const SuString& rFileName );

   /// Reads the specified animation file and adds all of its animations to this set
   SU_DLL bool LoadFile( const SuString& rFileName );

   /// Saves an animation set to a file
   SU_DLL bool SaveToFile( const SuString& rFileName, bool bPacked=true ) const;

   /// Adds a shared animation curve to the animation set
   SU_DLL bool AddCurve( const SuString& rCurveName, SuAnimationCurvePtr pCurve );

   /// Adds all curves from one animation set to another
   SU_DLL void AddCurvesFromSet( const SuAnimationSet* pSet );

   /// Removes a curve from the animation set, by name.
   SU_DLL bool RemoveCurve( const SuString& rCurveName );

   /// Retrieves an animation curve from the set
   SU_DLL SuAnimationCurvePtr GetCurve( const SuString& rCurveName ) const;

   /// Returns a list containing the names of all curves in this animation set
   SU_DLL SuStringList GetAllCurveNames() const;

   /// Returns the number of curves in the set
   inline uint32 GetNumCurves() const { return m_curves.size(); }

   /// Returns the length of the longest curve in the animation set (in seconds)
   SU_DLL float32 GetMaxAnimationLength() const;

// tolua_end
private:

   /// Name of the manifest file to create in the zip archive
   static const SuString MANIFEST_FILE_NAME;

   /// Version string to write into the manifest file
   static const SuString MANIFEST_VERSION_STRING;

   /// Version string to write into the manifest file for a 'packed' curve file
   static const SuString MANIFEST_VERSION_STRING_PACKED;

   /// Name of the file used to store packed animations in an animation set
   static const SuString PACKED_ANIMATION_FILENAME;

   /// Helper function which creates a manifest file for this animation set
   bool CreateManifest( SuArchive* pArchive, bool bPacked ) const;

   /// List of animation curves, paired with curve names
   typedef SuList< SuPair<SuString, SuAnimationCurvePtr> > CurveNameList;

   /// Parses a manifest from an 'unpacked' animation set and loads the curves
   bool ReadUnpackedManifest( const SuArchive& rArchive, const SuStringList& rManifestLines, CurveNameList& rLoadedAnimations );

   /// Parses a manifest from a 'packed' animation set and loads the curves
   bool ReadPackedManifest( SuFile* pPackedCurveFile, const SuString& rArchiveName, const SuStringList& rManifestLines, CurveNameList& rLoadedAnimations );

   /// Helper function which creates a curve file name for a curve name
   SuString GenerateCurveFilename( const SuString& rCurveName ) const;

   typedef SuHashMap< SuString, SuAnimationCurvePtr, SuStringHasher, SuHashComparer<SuString> > CurveNameMap;
   
   CurveNameMap m_curves;

// tolua_begin
};
// tolua_end


#endif // _SU_ANIMATIONSET_H_
