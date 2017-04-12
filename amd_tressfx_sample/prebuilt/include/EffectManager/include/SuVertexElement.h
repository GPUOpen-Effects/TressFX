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

#ifndef _SUVERTEXELEMENT_H
#define _SUVERTEXELEMENT_H

#include "SuTypes.h"
#include "SuAnnotationMap.h"

/// \brief Element in a vertex stream
///
/// \ingroup EffectManager
/// A class representing a data element in a vertex stream.
///
/// \sa SuVertexStreamDecl
class SuVertexElement
{
public:
   /// The possible data types of the vertex stream element
   enum DataType
   {
      FLOAT = 0,        ///< IEEE float
      FLOAT2 = 1,
      FLOAT3 = 2,
      FLOAT4 = 3,
      UBYTE4 = 4,       ///< Four-component, unsigned byte (0-255)
      DEC3N = 5,        ///< Three-component, 10-bit (-1,1).  NOTE: sushi supports Dec3N in D3D10 by using D3D10's 10_10_10_2_UNORM format, meaning that dec3n data must be scaled and biased in DX10 shaders
      HALF = 6,
      HALF2 = 7,
      HALF3 = 8,
      HALF4 = 9,
      
      UDEC3 = 10,       ///< 3-channels, 10-bits, interpretted as unsigned integers (0-1023)

      SHORT = 11,       ///< 16-bit signed integer (-32768,32767)
      SHORT2 = 12,
      SHORT3 = 13,
      SHORT4 = 14,

      UBYTE4N = 15,     ///< Four-channel unsigned byte, normalized (0-1)
      USHORT2N = 16,    ///< Two-channel unsigned short, normalized (0-1)
      USHORT4N = 17,    ///< Four-chhannel unsigned short, normalized (0-1)
      SHORT2N =  18,    ///< Signed short, normalized (-1,1)
      SHORT4N = 19,     ///< Signed short, normalized (-1,1)

      // The following are not available in D3D9:

      USHORT2 = 20,     ///< 2-channel unsigned short (0-65535)   (DX10 ONLY)
      USHORT4 = 21,     ///< 4-channel unsigned short (0-65535)   (DX10 ONLY)
      INT4  = 22,       ///< 4-channel signed int (-2147483648 to 2147483647)  (DX10 ONLY)
      UINT4 = 23,       ///< 4-channel unsigned int (0 to 4294967295)   (DX10 ONLY)

      BYTE2  = 24,      ///< 2-channel signed byte (-128,127)   (DX10 ONLY)
      BYTE2N = 25,      ///< 2-channel signed byte, normalized (-1,1)   (DX10 ONLY)
      UBYTE2 = 26,      ///< 2-channel unsigned byte, (0-255)   (DX10 ONLY)
      UBYTE2N = 27,     ///< 2-channel unsigned byte, normalized (0-1)   (DX10 ONLY)

      USHORT = 28,       ///< 16-bit unsigned integer (0,65535)
      USHORTN = 29,      ///< unsigned short, normalized (0,1)
      SHORTN  = 30,      ///< signed short, normalized (-1,1)

      UINT  = 31,    ///< Unsigned 32-bit integer
      INT   = 32,    ///< Signed 32-bit integer
      UINTN = 33,    ///< Unsigned 32-bit normalized int (0-1)
      INTN  = 34,    ///< Signed 32-bit normalized int (-1,1)
      UINT2 = 35,
      INT2  = 36,
      UINT2N   = 37,
      INT2N    = 38,

      // etc.
      NUM_VERTEX_DATA_TYPES = 39
   };

   /// Constructor
   SU_DLL SuVertexElement ( DataType type, const SuString& rName, const SuString& rSemantic,
                    SuAnnotationMap* pAnnotations = NULL);

   /// Destructor
   virtual SU_DLL ~SuVertexElement ();

   /// Returns the name of the vertex element
   inline const SuString& GetName () const { return m_name; }

   /// Returns the vertex element semantic (e.g. Position, Normal, etc.)
   inline const SuString& GetSemantic () const { return m_semantic; }

   /// Returns the vertex element data type
   inline DataType GetDataType () const { return m_type; }

   /// Returns the vertex element annotations
   /// \return A pointer to the annotations for this element or NULL if there are none.
   inline const SuAnnotationMap* GetAnnotations () const { return m_pAnnotations; }

   /// Returns the element size in bytes
   inline uint32 GetSize () const;

   /// Returns the number of component values in the vertex element
   inline uint8 GetComponentCount() const { return ms_dataTypeComponentCount[ GetDataType() ]; };


private:
   DataType m_type;
   SuString m_name;
   SuString m_semantic;
   SuAnnotationMap *m_pAnnotations;

   static SU_DLL const uint32 ms_dataTypeSize[NUM_VERTEX_DATA_TYPES];
   static SU_DLL const uint8 ms_dataTypeComponentCount[NUM_VERTEX_DATA_TYPES];
};

// inlined implementation of GetSize
uint32 SuVertexElement::GetSize () const
{
   SU_ASSERT ( m_type >= 0 && m_type < NUM_VERTEX_DATA_TYPES );
   return ms_dataTypeSize[m_type];
}

#endif
