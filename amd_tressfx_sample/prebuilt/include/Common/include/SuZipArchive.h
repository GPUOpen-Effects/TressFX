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

#ifndef _SUZIPARCHIVE_H
#define _SUZIPARCHIVE_H

#include "SuArchive.h"
#include "SuHashMap.h"

class SuZippedInputFile;
class SuZippedOutputFile;

/// \brief Archive implementation for zip files
/// \ingroup Common
///
class SuZipArchive : public SuArchive
{
public:
   /// Constructor
   SU_DLL SuZipArchive( const SuString& rZipFileName );

   /// Destructor
   virtual SU_DLL ~SuZipArchive();

   /// Checks whether a file with a particular name exists in the archive.
   virtual SU_DLL bool  FileExists( const SuString& rFileName, SuString* pFullFileName = NULL ) const;

   /// Opens a file from the archive
   virtual SU_DLL SuFile*  OpenFile( const SuString& rFileName, const SuString& rMode ) const;

   /// Returns the name of the archive
   virtual SU_DLL const SuString&  GetName() const { return m_zipFileName; };

   /// Creates a new empty zip file
   static SU_DLL SuZipArchive*  CreateNewZipFile( const SuString rZipFileName );

   /// Retrieves the timestamp of a file in the archive.  
   virtual SU_DLL bool  GetFileTime( const SuString& rFileName, time_t& rTimeOut ) const ;

private:

   /// Returns an input file handle to the zip archive
   void* GetInputHandle() const;

   /// Returns an output file handle to the zip archive
   void* GetOutputHandle() const;

   /// Helper method to open a zipped input file
   SuZippedInputFile* OpenInputFile( const SuString& rFileName, const SuString& rMode ) const;

   /// Helper method to open a zipped output file
   SuZippedOutputFile* OpenOutputFile( const SuString& rFileName, const SuString& rMode ) const;

   /// Looks for a directory in a zip file
   bool DirectoryExists( const SuString& rDirName ) const;

   /// Initializes the directory cache
   bool InitDirectoryCache( void* pZipHandle ) const;

   mutable bool m_bDirCacheInitialized;

   struct ZipDirFilePos    // must match unz_file_pos struct from zip library
   {
      uint32 pos_in_zip_directory;
      uint32 num_of_file;
   };

   typedef SuHashMap< SuString, ZipDirFilePos, SuStringHasher, SuHashComparer<SuString> > DirCacheHashMap;
   mutable DirCacheHashMap m_dirCache;

   SuString m_zipFileName;
};

#endif
