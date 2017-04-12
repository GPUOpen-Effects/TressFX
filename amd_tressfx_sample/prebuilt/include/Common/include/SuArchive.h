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

#ifndef _SUARCHIVE_H
#define _SUARCHIVE_H

#include "SuTypes.h"

// forward declarations
class SuFile;

/// \brief Interface for archives (collections) of files
/// \ingroup Common
class SuArchive
{
public:
   /// Destructor
   virtual SU_DLL ~SuArchive();

   /// Factory method to create archive instances
   static SU_DLL SuArchive* Create( const SuString& rArchiveFileName );

   /// Checks whether a file with a particular name exists in the archive.
   ///
   /// \param rFileName Name of the file to check for existance
   /// \param pFullFileName Pointer to a string that will contain the full path/file name of the
   ///                      file found in the archive. Can be NULL.
   ///
   /// \return true if the file exists in the archive, false otherwise
   virtual SU_DLL bool FileExists( const SuString& rFileName, SuString* pFullFileName = NULL ) const = 0;

   /// Opens a file from the archive
   ///
   /// \param rFileName Name of the file to open
   /// \param rMode File access mode string. Same as you would pass into fopen().
   ///
   /// \return A pointer to an SuFile interface on success or NULL if there was an
   ///         error opening the file.
   virtual SU_DLL SuFile* OpenFile( const SuString& rFileName, const SuString& rMode ) const = 0;

   /// Retrieves the timestamp of a file in the archive.  
   ///
   /// \param rFileName  The name of the file in the archive whose time is desired
   /// \param rTimeOut   A reference to a variable that will receive the file time
   /// \return True if the file exists and its time was successfully retrieved.  False otherwise.  
   ///         rTimeOut is not modified in the event of a false return value
   virtual SU_DLL bool GetFileTime( const SuString& rFileName, time_t& rTimeOut ) const = 0;

   /// Returns the name of the archive
   virtual SU_DLL const SuString& GetName() const = 0;
};

/// \ingoup Common
/// \brief Returns the global archive, used as a fallback location to look for files.
SU_DLL const SuArchive* SuGetGlobalArchive();

/// \ingoup Common
/// \brief Sets the global archive
SU_DLL void SuSetGlobalArchive( SuArchive* pArchive );

#endif
