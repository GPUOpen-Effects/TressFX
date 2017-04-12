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

#ifndef _SU_MODELDATAMETAFILE_H_
#define _SU_MODELDATAMETAFILE_H_

#include "SuArchive.h"

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief  A file containing meta-data about the model
/// 
//=================================================================================================================================
class SuModelDataMetaFile
{
public:

   SU_DLL ~SuModelDataMetaFile();

   /// Constructs a meta file from an archive
   static SU_DLL SuModelDataMetaFile* ExtractFromArchive( const SuArchive* pArchive );

   /// Writes the meta file to a text file in an archive
   SU_DLL bool WriteToArchive( SuArchive* pArchive ) const;

   /// Adds an entry to the meta file
   SU_DLL void AddEntry( SuString sEntry ) { m_entries.push_back( sEntry ) ; }

private:
   
   SuArray <SuString> m_entries;

};


#endif // _SU_MODELDATAMETAFILE_H_
