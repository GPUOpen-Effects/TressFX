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

#ifndef _SUVERTEXSTREAMDECL_H
#define _SUVERTEXSTREAMDECL_H

#include "SuTypes.h"
#include "SuVertexElement.h"
#include "SuAnnotationMap.h"
#include "SuStreamBinder.h"

/// Array of Vertex elements
/// \ingroup EffectManager
typedef SuArray <SuVertexElement *> SuVertexElementArray;

class SuGPUVertexBuffer;

/// \brief Vertex Stream Declaration
///
/// \ingroup EffectManager
/// A class representing a vertex stream declaration read from an effect file.
/// It contains an array of SuVertexElements
///
/// \sa SuVertexElement
class SuVertexStreamDecl
{
public:
   /// Constructor
   SU_DLL SuVertexStreamDecl (const SuString& rName, SuAnnotationMap* pAnnotations = NULL);

   /// Constructor
   SU_DLL SuVertexStreamDecl (const SuString& rName, const SuVertexElementArray& rElems,
                               SuAnnotationMap* pAnnotations = NULL);

   /// Destructor
   virtual SU_DLL ~SuVertexStreamDecl ();

   /// Adds a vertex element to the end of the stream
   virtual SU_DLL void AppendVertexElement ( SuVertexElement* pElem );

   /// Returns the stride of the stream in bytes
   SU_DLL uint32 GetStride () const;

   /// Locates vertex elements with a particular semantic
   SU_DLL void GetElementsBySemantic( const SuString& rSemantic, SuArray<const SuVertexElement*>& rElementsOut ) const;

   /// Determines the offset of a particular stream element from the start of a vertex
   SU_DLL uint32 GetElementOffset( const SuVertexElement* pElement ) const;

   /// Returns the declaration name
   inline const SuString& GetName () const { return m_name; }

   /// Returns the annotations
   inline const SuAnnotationMap* GetAnnotations () const { return m_pAnnotations; }

   /// Returns a reference to the vertex elements in the stream
   inline const SuVertexElementArray& GetElements () const { return m_elements; }

   /// Returns the number of vertex elements in the stream
   inline uint32 GetNumElements () const { return static_cast<uint32> (m_elements.size ()); }

   /// Binds a vertex buffer to a vertex stream
   inline void BindVertexBuffer( const SuGPUVertexBuffer* pBoundVB, uint32 nBaseIndex ) const 
   { 
      m_pBoundVB = pBoundVB;
      m_nBaseIndex = nBaseIndex;
   };

   /// Returns the vertex buffer bound to a vertex stream
   inline const SuGPUVertexBuffer* GetBoundVertexBuffer() const { return m_pBoundVB; };

   /// Returns the base offset that the vertex buffer was bound with
   inline uint32 GetBaseVertexIndex() const { return m_nBaseIndex; };

   /// Returns the vertex stream binder associated with this stream, if any
   inline SuStreamBinderPtr GetBinder() const { return m_pBinder; };

   /// Removes the binder that is associated with this slot
   inline void RemoveBinder() { m_pBinder = SuStreamBinderPtr(0); };

private:

   friend class SuEffectBuilder; // So that SuEffectBuilder may call 'SetBinder()'

   /// Sets the vertex stream binder associated with this stream
   inline void SetBinder( SuStreamBinderPtr pBinder ) { m_pBinder = pBinder; };


   SuVertexElementArray m_elements;
   SuString m_name;
   SuAnnotationMap* m_pAnnotations;
   SuStreamBinderPtr m_pBinder;                 ///< Stream binder attached with this stream, if any

   mutable uint32 m_nBaseIndex;                 ///< Offset base index for bound vertex buffer
   mutable const SuGPUVertexBuffer* m_pBoundVB; ///< Vertex buffer currently bound to the stream

   mutable uint32 m_nStride;
   mutable bool   m_bStrideValid;
};

#endif
