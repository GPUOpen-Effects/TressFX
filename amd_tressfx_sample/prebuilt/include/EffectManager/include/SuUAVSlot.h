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

#ifndef _SU_UAV_SLOT_H_
#define _SU_UAV_SLOT_H_

#include "SuCountedPtr.h"
#include "SuUAVSlotBinder.h"
#include "SuGPUUnorderedAccessView.h"

class SuAnnotationMap;

/// \brief Texture slot description
///
/// \ingroup EffectManager
class SuUAVSlot
{
public:   
   enum Type
   {
      TEXTURE_2D,
      TEXTURE_3D,
      BUFFER, // Typed R/W buffer
      BUFFER_RAW,
      BUFFER_APPEND_RAW,
      BUFFER_STRUCTURED,
      BUFFER_APPEND_STRUCTURED
   };

   /// Constructor
   SU_DLL SuUAVSlot( const SuString& rName, SuUAVSlot::Type type, SuAnnotationMap *annotations);

   /// Destructor
   virtual SU_DLL ~SuUAVSlot();

   /// Returns the texture type
   inline const Type& GetType() const { return m_type; }

   /// Returns the texture name
   inline const SuString& GetName() const { return m_name; }

   /// Returns the texture annotations
   inline const SuAnnotationMap* GetAnnotations() const { return m_annotations; }

   /// Binds a unordered access view to the slot
   inline void BindResource( const SuCountedPtr< SuGPUUnorderedAccessView >& rpUAV ) const { m_pUAV = rpUAV; };

   /// Returns the sampling view that is bound to the slot.
   inline const SuCountedPtr< SuGPUUnorderedAccessView >& GetBoundResource() const { return m_pUAV; };

   /// Returns the binder that is associated with this slot
   inline const SuUAVSlotBinderPtr GetBinder() const { return m_pBinder; };

   /// Removes the binder that is associated with this slot
   inline void RemoveBinder() { m_pBinder = SuUAVSlotBinderPtr(0); };

private:

   friend class SuEffectBuilder; // So that SuEffectBuilder may call 'SetBinder()'

   /// Sets the binder that is associated with this slot
   inline void SetBinder( SuUAVSlotBinderPtr pBinder ) { m_pBinder = pBinder; };

   /// Texture name
   SuString m_name;

   /// Texture type
   Type m_type;

   /// Texture annotations
   SuAnnotationMap *m_annotations; 

   /// Currently bound texture
   mutable SuCountedPtr<SuGPUUnorderedAccessView> m_pUAV;

   /// Associated binder object
   SuUAVSlotBinderPtr m_pBinder;

};


#endif   // End of _SU_UAV_SLOT_H_
