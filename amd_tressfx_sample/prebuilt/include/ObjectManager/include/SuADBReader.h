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

#ifndef _SU_ADBREADER_H_
#define _SU_ADBREADER_H_

#include "SuModelADB.h"

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A utility class to extract serialized objects from an ADB
/// 
///   To use this class, simply pass an ADB instance to its constructor, and then call the 'ReadData' method to
///    extract data from it as though it were a binary file.  
///
//=================================================================================================================================
class SuADBReader
{
public:

   /// Constructs a reader for an ADB
   SU_DLL SuADBReader( SuModelADBPtr pADB );

   /// Generic method for reading objects from an ADB
   /// \param pObjects  Pointer to an array of objects to be filled with objects from the ADB
   /// \param nObjects  Number of objects to read
   /// \return True if successful, false if fewer than 'nObjects' objects were read
   template < class T > inline bool Read( T* pObjects, uint32 nObjects = 1) { return ReadData( pObjects, nObjects*sizeof(T) ) == (nObjects*sizeof(T)); };

   /// Reads a block of binary data from the ADB.
   SU_DLL size_t ReadData( void* pDataOut, size_t nSize );

private:

   SuMemoryBufferPtr m_pADBData;
   size_t m_nDataSize;
   size_t m_nBytesRead;
};


#endif // _SU_ADBREADER_H_
