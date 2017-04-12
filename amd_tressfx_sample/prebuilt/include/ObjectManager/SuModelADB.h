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

#ifndef _SU_MODELADB_H_
#define _SU_MODELADB_H_

#include "SuModelDataItem.h"
#include "SuCountedPtr.h"

/// A structure describing the properties of an ADB
struct SuModelADBDesc
{
   uint32 nBufferSize;     ///< Size of the data in bytes
   
   /// \brief A version number for the content in the data payload. 
   /// This value is intended to be used by the engine subsystem which uses the ADB
   /// in order to support backwards compatibility in case the data format changes.
   uint32 nContentVersion; 
};

class SuModelADB;
typedef SuCountedPtr<SuModelADB> SuModelADBPtr;

enum SuModelADBType
{
   SU_BINARY_ADB,
   SU_TEXT_ADB
};


//=========================================================================================================
/// \ingroup ObjectManager
/// \brief This class represents a preprocessed data file that contains data other than vertex or index data.
///
///   An ADB (additional data buffer) is an arbitrary binary file which can be included in a pre-processed object file and
///   interpretted by the engine in any way desired.  The purpose of an ADB is to provide a clean
///   mechanism for extending the engine and supplying auxilliary data for objects.
///
//=========================================================================================================
class SuModelADB : public SuModelDataItem
{
public:

   /// Constructor for an ADB
   SU_DLL SuModelADB( const SuString& rName, const SuString& rFileName, const SuString& rUsage, uint8* pMetaData, uint32 nMetaSize,
               uint8* pADBData, const SuModelADBDesc& rDesc );
   
   /// Constructor for a text ADB
   SU_DLL SuModelADB( const SuString& rName, const SuString& rFileName, const SuString& rUsage, const SuString& rTextData );

   /// Returns the type of model data item.  For an ADB the return value is SU_MODELDATA_ADB
   virtual SU_DLL SuModelDataType GetType() const { return SU_MODELDATA_ADB; }

   /// Returns a string describing the intended usage of the buffer. 
   SU_DLL const SuString& GetUsage() const;

   /// Returns a description of the ADB
   inline const SuModelADBDesc& GetDesc() const { return m_desc; }

   /// Returns the type of this ADB (binary or string)
   inline SuModelADBType GetADBType() const { return m_eADBType; }

   /// Returns the ADB's text data. Returns an empty string for a binary ADB.
   inline const SuString& GetTextData() const { return m_textData; }

   /// Computes an MD5 hash for this ADB
   virtual SU_DLL void ComputeHash( SuMD5Hash* pHashOut ) const;

private:

   SuModelADBType m_eADBType;
   SuString m_usage;
   SuModelADBDesc m_desc;  // only used for a binary data ADB
   SuString m_textData;  // only used for a text/string ADB
};


#endif // _SU_MODELADB_H_

