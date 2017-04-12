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

#ifndef _SUFILE_H
#define _SUFILE_H

#include "SuTypes.h"
#include "SuMemoryBuffer.h"

#include <ctime>

typedef SuCountedPtr<SuMemoryBuffer> SuMemoryBufferPtr;

/// \brief File I/O interface
/// \ingroup Common
// tolua_begin
class SuFile 
{
public:
   /// Opens a file with the given access mode
   static SU_DLL SuFile* Open( const SuString& rFileName, const SuString& rMode );

   /// Determines when the file with the given name was last modified. 
   static SU_DLL bool GetTimeStamp( const SuString& rFileName, time_t& rTimeOut );

   /// Destructor
   virtual SU_DLL ~SuFile();

   /// Checks whether the file object is valid (i.e. if the file is still open)
   inline bool IsValid() const { return m_bValid; }

   inline const SuString& GetFileName() const { return m_fileName; }

   /// Closes the file. The file object becomes useless afterwards and must be deleted.
   virtual SU_DLL void Close() = 0;

   /// Reads data from the file
   ///
   /// \param pBuffer The buffer to put the data in
   /// \param nMaxBytes The maximum number of bytes to read. Must not be larger than the buffer size.
   ///
   /// \return The number of bytes actually read, which can be less than maxNumBytes.
   ///         If retr return value is 0, there is no more data to read in the file.
   ///         -1 indicates a read error.
   virtual SU_DLL int32 Read( void* pBuffer, uint32 nMaxBytes ) = 0;

   /// Writes data to the file
   ///
   /// \param pBuffer The data to write
   /// \param nBytes The number of bytes to write
   ///
   /// \return true on success, false if something went wrong
   virtual SU_DLL bool Write( const void* pBuffer, uint32 nBytes ) = 0;

   /// writes a string to the file (convenience function)
   SU_DLL bool Write( const SuString& rStr );

   /// Seeks to a position in the file
   ///
   /// \param nOffset number of bytes to move relative to nOrigin
   /// \param nOrigin Must be either SEEK_CURR (offset is relative to the current position of the
   ///                file pointer), SEEK_SET (offset relative to beginning of file), or SEEK_END
   ///                (offset relative to end of file).
   ///
   /// \return true on success, false otherwise
   virtual SU_DLL bool Seek( int32 nOffset, int32 nOrigin ) = 0;

   /// Returns the file size
   virtual SU_DLL uint32 GetSize() const = 0;

   /// Flushes filesystem buffers
   ///
   /// \return true on success, false otherwise
   virtual SU_DLL bool Flush() = 0;

   /// Reads an entire file and returns a buffer with the file contents
   static SU_DLL void* ReadFileToBuffer( SuFile& rFile, /* out */ uint32* pnBufferSize );

   static SU_DLL SuMemoryBuffer* ReadFileToMemoryBuffer( SuFile& rFile );

   /// Reads an entire file and returns a buffer with the file contents
   static SU_DLL SuString ReadFileToString( SuFile& rFile );

   /// Scans a text file and breaks it into a collection of text lines. Supports any type of line ending
   SU_DLL bool ReadTextLines( SuStringList& rFileLines );

   /// Checks if a file of a given name exists on disk
   static SU_DLL bool Exists( const SuString& rFileName );

   /// Enables logging all file open operations to a log file
   static SU_DLL void EnableFileAccessLogging( const SuString& rLogFilename );

   /// Closes the file access log
   static SU_DLL void CloseFileAccessLog();

   /// Writes a file access to the log
   static SU_DLL void LogFileAccess( const SuString& rFilename, const SuString& rMode, bool bSuccess = true );

   /// Opens a binary file, copies its contents to an SuMemoryBuffer, and returns it.  This method is provided to ease file loading from script
   static SU_DLL SuMemoryBufferPtr LoadBinaryFile( const SuString& rFileName );

   // tolua_end

protected:
   /// Constructor
   SU_DLL SuFile( const SuString& rFileName );

   /// Sets the file object's valid flag
   inline void SetValid( bool bValid ) { m_bValid = bValid; }

private:
   bool m_bValid;
   SuString m_fileName;

   static const uint32 READ_CHUNK_SIZE;

// tolua_begin
};
// tolua_end

#endif //_SUFILE_H
