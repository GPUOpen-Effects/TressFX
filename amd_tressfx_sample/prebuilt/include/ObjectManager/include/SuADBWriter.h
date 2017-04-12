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

#ifndef _SU_ADBWRITER_H_
#define _SU_ADBWRITER_H_

#include "SuModelADB.h"

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A utility class which can be used to serialize binary data into an ADB
/// 
///  The SuADBWriter class provides an easy to use interface for encoding variable-sized binary data structures into model ADBs.
///   To use this class, simply create an instance, and pass data to it using the 'WriteData' method.  After all of the data
///   has been written, call the 'FinalizeADB' method to create an ADB instance containing the data.
///
//=================================================================================================================================
class SuADBWriter
{
public:

   SU_DLL SuADBWriter(  );

   SU_DLL ~SuADBWriter( );

   /// Generic method for writing objects to the ADB
   template< class T > inline void Write( const T* pObjectArray, uint32 nCount = 1 )  {  WriteData( pObjectArray, nCount*sizeof(T) ); }

   /// Writes raw binary data to the ADB
   SU_DLL void WriteData( const void* pData, uint32 nSize );

   /// Finalizes the ADB and returns an ADB instance
   SU_DLL SuModelADBPtr FinalizeADB( const SuString& rName, const SuString& rUsage, uint32 nContentVersion );

private:

   uint8* m_pBuffer;
   uint32 m_nBufferSize;
   uint32 m_nBytesWritten;

};


#endif // _SU_ADBWRITER_H_
