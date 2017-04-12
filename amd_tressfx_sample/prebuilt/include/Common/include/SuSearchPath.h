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

#ifndef _SU_SEARCH_PATH_H_
#define _SU_SEARCH_PATH_H_

#include "SuArchive.h"

/// \brief List of archives that can be searched for a file
/// \ingroup Common
class SuSearchPath : public SuArchive
{
public:
   typedef SuList<SuArchive *> ArchiveList;

   /// Constructor
   SU_DLL SuSearchPath();

   /// Destructor
   virtual SU_DLL  ~SuSearchPath();

   /// Adds an archive to the search path
   void SU_DLL AddToSearchPath( const SuString& rArchiveName );

   /// Clear any and all search paths that have been set
   void SU_DLL ClearSearchPaths();

   /// Checks whether a file with a particular name exists in the search path
   virtual SU_DLL bool FileExists( const SuString& rFileName, SuString* pFullFileName = NULL ) const;

   /// Opens a file from the search path
   virtual SU_DLL SuFile* OpenFile( const SuString& rFileName, const SuString& rMode ) const;

   /// This always returns "SuSearchPath"
   virtual SU_DLL const SuString& GetName() const;

   /// Returns a list with all archives currently in the search path
   inline const ArchiveList& GetArchivesInSearchPath() const { return m_archives; }

   /// Retrieves the timestamp of a file in the archive.  
   virtual SU_DLL bool GetFileTime( const SuString& rFileName, time_t& rTimeOut ) const;

private:

   ArchiveList m_archives;
};

#endif