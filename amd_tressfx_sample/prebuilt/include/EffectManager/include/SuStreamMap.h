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

#ifndef _SUSTREAMMAP_H
#define _SUSTREAMMAP_H

#include "SuTypes.h"
#include "SuAnnotationMap.h"

// Forward declarations
class SuVertexStreamDecl;

/// \brief Stream map declaration in a shader file
///
/// \ingroup EffectManager
/// A class representing a set of vertex streams to be used in a render pass.
///
/// \sa SuVertexStreamDecl
class SuStreamMap
{
public:
   /// Constructor
   SU_DLL SuStreamMap ( SuAnnotationMap* pAnnotations = NULL );

   /// Destructor
   virtual SU_DLL ~SuStreamMap ();

   /// Returns an array with all the vertex stream names in this stream map
   inline const SuStringArray& GetStreamNames () const { return m_streamNames; }

   /// Returns the stream map's annotations
   inline const SuAnnotationMap* GetAnnotations () const { return m_pAnnotations; }

   /// Returns the number of streams in this stream map
   inline uint32 GetNumStreams () const { return (uint32)m_streamNames.size (); }

   /// Get a pointer to the stream declaration for stream "index"
   SU_DLL const SuVertexStreamDecl* GetStreamDecl (uint32 index) const;
   
   /// Appends a vertex stream name and vertex stream decl to the map
   SU_DLL void AppendVertexStreamName ( const SuString& rName, const SuVertexStreamDecl* pStream );

   /// Determines whether or not a stream in the stream map is instanced
   SU_DLL bool IsStreamInstanced( uint32 nStreamIndex ) const;

private:
   /// Array of stream names
   SuStringArray m_streamNames;

   /// Array of pointers to vertex stream declarations, one for each stream
   SuArray<const SuVertexStreamDecl*> m_streamDecls;

   /// Annotations for the stream map
   SuAnnotationMap *m_pAnnotations;
};

#endif
