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

#ifndef _SU_MEMORYBUFFER_H_
#define _SU_MEMORYBUFFER_H_

#include "SuCountedPtr.h"

// silly tolua doesn't understand what size_t is....
// tolua_begin
#if 0
typedef unsigned int size_t;
#endif
// tolua_end

//=================================================================================================================================
/// \ingroup Common
/// \brief A class to encapsulate a block of unstructured memory.  
///
///   Using this class allows memory buffers to be reference counted.
/// 
//=================================================================================================================================
// tolua_begin
class SuMemoryBuffer
{
public:

   /// Factory method that allocates a memory buffer with the specified size
   static SU_DLL SuMemoryBuffer* Allocate( uint32 nSize );

// tolua_end 

   /// Constructs a memory buffer around a block of data. 
   SU_DLL SuMemoryBuffer( uint8* pData, uint32 nSize );
  
   /// Destructor
   SU_DLL ~SuMemoryBuffer();

// tolua_begin

   /// Accessor for the buffer size
   inline uint32 GetSize() const { return m_nSize; };

   /// Const accessor to the buffer contents
   inline const uint8* GetData() const { return m_pData; };
   
   /// Non-const accessor to the buffer contents
   inline uint8* GetData() { return m_pData; };
   
   inline void SetUINT8    ( size_t nByteOffset,  size_t nIndex, uint8 nValue )  { Set<uint8>   ( nByteOffset, nIndex, nValue );  };
   inline void SetINT8     ( size_t nByteOffset,  size_t nIndex, int8 nValue )   { Set<int8>    ( nByteOffset, nIndex, nValue );  };
   inline void SetUINT16   ( size_t nByteOffset,  size_t nIndex, uint16 nValue ) { Set<uint16>  ( nByteOffset, nIndex, nValue );  };
   inline void SetINT16    ( size_t nByteOffset,  size_t nIndex, int16 nValue )  { Set<int16>   ( nByteOffset, nIndex, nValue );  };
   inline void SetUINT32   ( size_t nByteOffset,  size_t nIndex, uint32 nValue ) { Set<uint32>  ( nByteOffset, nIndex, nValue );  };
   inline void SetINT32    ( size_t nByteOffset,  size_t nIndex, int32 nValue )  { Set<int32>   ( nByteOffset, nIndex, nValue );  };
   inline void SetFloat32  ( size_t nByteOffset,  size_t nIndex, float32 fValue ){ Set<float32> ( nByteOffset, nIndex, fValue );  };
   inline void SetFloat64  ( size_t nByteOffset,  size_t nIndex, float64 fValue ){ Set<float64> ( nByteOffset, nIndex, fValue );  };

   inline uint8   GetUINT8    ( size_t nByteOffset, size_t nIndex ) const { return Get<uint8>   ( nByteOffset, nIndex ); };
   inline int8    GetINT8     ( size_t nByteOffset, size_t nIndex ) const { return Get<int8>    ( nByteOffset, nIndex ); };
   inline uint16  GetUINT16   ( size_t nByteOffset, size_t nIndex ) const { return Get<uint16>  ( nByteOffset, nIndex ); };
   inline int16   GetINT16    ( size_t nByteOffset, size_t nIndex ) const { return Get<int16>   ( nByteOffset, nIndex ); };
   inline uint32  GetUINT32   ( size_t nByteOffset, size_t nIndex ) const { return Get<uint32>  ( nByteOffset, nIndex ); };
   inline int32   GetINT32    ( size_t nByteOffset, size_t nIndex ) const { return Get<int32>   ( nByteOffset, nIndex ); };
   inline float32 GetFloat32  ( size_t nByteOffset, size_t nIndex ) const { return Get<float32> ( nByteOffset, nIndex ); };
   inline float64 GetFloat64  ( size_t nByteOffset, size_t nIndex ) const { return Get<float64> ( nByteOffset, nIndex ); };

// tolua_end

   /// Template method for re-interpretting the buffer as an arbitrary type and writing to it
   template <class T> inline void Set( size_t nByteOffset, size_t nIndex, T value )
   {
      T* pArray = reinterpret_cast<T*>( m_pData + nByteOffset );
      pArray[nIndex] = value;
   };

   /// Template method for re-interpretting the buffer as an arbitrary type and reading from it
   template <class T> inline T Get( size_t nByteOffset, size_t nIndex ) const
   {
      const T* pArray = reinterpret_cast<const T*>( m_pData + nByteOffset );
      return pArray[nIndex];
   };

private:

   /// Private default constructor.
   SuMemoryBuffer();

   uint8* m_pData;
   uint32 m_nSize;

// tolua_begin
};


/// A counted pointer to a memory buffer
typedef SuCountedPtr<SuMemoryBuffer> SuMemoryBufferPtr;

// tolua_end

/// An array of counted pointers to memory buffers
typedef SuArray<SuMemoryBufferPtr> SuMemoryBufferArray;



#endif // _SU_MEMORYBUFFER_H_
