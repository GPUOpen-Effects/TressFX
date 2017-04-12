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

#ifndef _SU_BOUNDINGVOLUME_H_
#define _SU_BOUNDINGVOLUME_H_

#include "SuTypes.h"
#include "SuPoint3.h"

class SuRay;

//=========================================================================================================
/// \ingroup Math
/// \brief Bounding volume base class
/// 
//=========================================================================================================
// tolua_begin
class SuBoundingVolume
{
public:
   enum BVType
   {
      SPHERE,  ///< bounding sphere
      BOX      ///< axis-aligned bounding box
   };

   /// Destructor
   virtual SU_DLL ~SuBoundingVolume();

   /// Returns the bounding volume's type
   inline BVType GetType() const { return m_type; }

   /// Checks whether a point is inside the bounding volume
   virtual SU_DLL bool PointInside( const SuPoint3& rPt ) const = 0;

   /// Checks whether a ray intersects the bounding volume
   virtual SU_DLL bool RayIntersects( const SuRay& rRay ) const = 0;

// tolua_end

   /// Creates a duplicate of this bounding volume
   /// \return A duplicate of the bounding volume.  The caller must delete the duplicate
   virtual SuBoundingVolume* Clone() const = 0;

protected:
   /// Constructor
   SU_DLL SuBoundingVolume( BVType type ) : m_type(type) {}

private:
   BVType m_type;
// tolua_begin
};
// tolua_end


#endif // _SU_BOUNDINGVOLUME_H_
