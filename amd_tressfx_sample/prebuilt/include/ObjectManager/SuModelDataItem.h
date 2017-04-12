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

#ifndef _SU_MODELDATAITEM_H_
#define _SU_MODELDATAITEM_H_

#include "SuCommon.h"
#include "SuCountedPtr.h"
#include "SuMemoryBuffer.h"

class SuArchive;
class SuMD5Hash;

class SuModelDataItem;
typedef SuCountedPtr< SuModelDataItem > SuModelDataItemPtr;


/// An enum describing the type of a data item
enum SuModelDataType
{
   SU_MODELDATA_VB,
   SU_MODELDATA_IB,
   SU_MODELDATA_TEXTURE,
   SU_MODELDATA_ADB,
}; // NOTE:  These are written to object files in binary form.  Add any new elements at the end


//=============================================================================================================
/// \ingroup ObjectManager
/// \brief  A class to encapsulate a data item which resides in an object file archive.
///
///   Model data items include vertex and index buffers, textures, and arbitrary data buffers (ADBs)
///
///   All SuModelDataItem objects have a name, a filename, and a block of data.
///   The can also store blocks of arbitrary meta-data (assigned by the pre-processor as needed).  
///
///   All model item files share a common format.  The files are binary, containing a variable-length header followed
///   by a data payload.  The details of this format are documented in the wiki
///
//==============================================================================================================
class SuModelDataItem 
{
public:

   /// Constructor
   SU_DLL SuModelDataItem( const SuString& rName, 
                          const SuString& rFileName,
                          SuMemoryBufferPtr pMetaData,
                          SuMemoryBufferPtr pItemData );

   /// Destructor
   virtual SU_DLL ~SuModelDataItem( );

   /// Returns the model data item name
   SU_DLL const SuString& GetName() const;

   /// Returns the filename for the data item
   SU_DLL const SuString& GetFileName() const;

   /// Returns the type of model data item.
   virtual SuModelDataType GetType() const = 0;

   /// Returns the meta-data for this object
   SU_DLL SuMemoryBufferPtr GetMetaData() const;

   /// Returns the size of the meta data for this object
   inline uint32 GetMetaSize() const { return ( m_pMetaData ) ? m_pMetaData->GetSize() : 0; };

   /// Returns a pointer to the raw data contained in this data item
   SU_DLL SuMemoryBufferPtr GetData() const;

   /// Computes an MD5 hash for this data item
   virtual SU_DLL void ComputeHash( SuMD5Hash* pHashOut ) const ;


private:

   SuFile* m_pFile;

   SuString m_name;

   SuString m_fileName;

   SuMemoryBufferPtr m_pMetaData;
   SuMemoryBufferPtr m_pItemData;
};

#endif // _SU_MODELDATAITEM_H_
