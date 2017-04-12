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

#ifndef _SUSERIALIZER_H
#define _SUSERIALIZER_H


#include "SuCommon.h"
#include "SuFile.h"

class SuMemoryBuffer;

// ====================================================================================
/// \ingroup Common
///
/// \brief Sushi Serialization base class.  Use this as a base class for building serializers.
///
/// Provides basic serialization routines for importing and exporting Sushi data files.
/// Sushi serializer classes inherit from this base class.  You must set the file handle
/// before using any read or write methods.  Be sure to open a file with read access if
/// you plan to use read methods or write access if you plan to use write methods.  Also
/// keep in mind that certain kinds of files (such as zip files) may only be opened for
/// read *or* for write, they may not be opened for read and write.
///
// ====================================================================================
class SuSerializer
{

public :

   /// SuSerializer Constructor
   SU_DLL SuSerializer ( void );

   /// SuSerializer Destructor
   virtual SU_DLL ~SuSerializer ( void );

protected :

   /// Serializer version
   SuString m_strVersion;

   /// Set the file handle for serializing to/from.  Make sure the file was opened with the correct access settings (read or write).
   SU_DLL bool SetFileHandle ( SuFile* pFile );

   //=========//
   // Writing //
   //=========//

   /// Writes an array of float64s
   SU_DLL bool WriteFloat64 ( const float64* const pFloats, uint32 nCount );

   /// Writes an array of float32s
   SU_DLL bool WriteFloat32 ( const float32* const pFloats, uint32 nCount );

   /// Writes an array of uint64s
   SU_DLL bool WriteUInt64 ( const uint64* const pInts, uint32 nCount ); 

   /// Writes an array of uint32s
   SU_DLL bool WriteUInt32 ( const uint32* const pInts, uint32 nCount ); 

   /// Writes an array of uint16s
   SU_DLL bool WriteUInt16 ( const uint16* const pInts, uint32 nCount );

   /// Writes an array of uint8s
   SU_DLL bool WriteUInt8 ( const uint8* const pInts, uint32 nCount );

   /// Writes an array of bools
   SU_DLL bool WriteBool ( const bool* const pBools, uint32 nCount );

   /// Writes a string (writes a uint32 with number of characters followed by that number of single byte characters)
   SU_DLL bool WriteString ( const SuString& rString );

   /// Writes a string (doesn't preceed string with length as does WriteString)
   SU_DLL bool WriteLine ( const SuString& rString );

   /// Writes an array of typeless data
   SU_DLL bool WriteData ( const void* const pData, uint32 nSize, uint32 nCount );

   /// Writes an array of typeless data stored in an SuMemoryBuffer
   SU_DLL bool WriteData( const SuMemoryBuffer& rBuffer );

   //=========//
   // Reading //
   //=========//

   /// Reads an array of float64s
   SU_DLL bool ReadFloat64 ( float64* pFloats, uint32 nCount );

   /// Reads an array of float32s
   SU_DLL bool ReadFloat32 ( float32* pFloats, uint32 nCount );

   /// Reads an array of uint64s
   SU_DLL bool ReadUInt64 ( uint64* pInts, uint32 nCount );

   /// Reads an array of uint32s
   SU_DLL bool ReadUInt32 ( uint32* pInts, uint32 nCount ); 

   /// Reads an array of uint16s
   SU_DLL bool ReadUInt16 ( uint16* pInts, uint32 nCount );

   /// Reads an array of uint8s
   SU_DLL bool ReadUInt8 ( uint8* pInts, uint32 nCount );

   /// Reads an array of bools
   SU_DLL bool ReadBool ( bool* pBools, uint32 nCount );

   /// Reads a string (reads a uint32 with number of characters followed by that number of single byte characters)
   SU_DLL bool ReadString ( SuString& rString );

   /// Reads a string (reads single byte characters until it encounters a delimiter, doesn't use a preceeding count like ReadString does)
   SU_DLL bool ReadLine ( SuString& rString, const SuString& rDelimiter = "\n", uint32 nMaxCharacters = 2048, bool* pEOF = NULL );

   /// Reads an array of typeless data
   SU_DLL bool ReadData ( void* pData, uint32 nSize, uint32 nCount );

private :

   /// File object pointer, use SetFileHandle to set.  Pointer is NULL by default.
   SuFile* m_pFile;

};
 

#endif