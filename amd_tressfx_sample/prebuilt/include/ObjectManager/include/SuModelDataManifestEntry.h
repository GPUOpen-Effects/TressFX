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

#ifndef _SU_MODELDATAMANIFESTENTRY_H_
#define _SU_MODELDATAMANIFESTENTRY_H_

#include "SuModelDataItem.h"
#include "SuModelADB.h"
#include "SuMD5Hash.h"

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief An entry in an SuModelDataManifest
/// 
//=================================================================================================================================
class SuModelDataManifestEntry
{
public:

   /// Returns the type of this data item
   inline SuModelDataType GetItemType() const { return m_eItemType; };

   /// Returns the name of this data item
   inline const SuString& GetItemName() const { return m_itemName; };

   /// Returns the filename in the archive for this data item
   inline const SuString& GetItemFileName() const { return m_itemFileName; };

   /// Returns the MD5 hash for this manifest entry
   inline const SuMD5Hash& GetItemHash() const { return m_itemHash; };
      
   /// Returns the ADB usage string, if the item is an ADB
   inline const SuString& GetADBUsage() const { return m_adbUsage; };

   /// Returns the ADB type, if the item is an ADB
   inline SuModelADBType GetADBType() const { return m_eADBType; };

   /// Returns a pointer to the data item for this manifest entry, if it has been loaded.  Returns NULL if not
   inline SuModelDataItemPtr GetItem() const { return m_pItem; };

   /// Returns the file timestamp for the data item inside the archive
   SU_DLL time_t GetItemTimeStamp( const SuArchive* pArchive ) const;

   /// Loads the data item for this manifest entry
   SU_DLL bool LoadItem( const SuArchive* pArchive ) const;

   /// Writes the manifest entry to a text file
   SU_DLL bool WriteToTextFile( SuFile* pFile ) const;

   /// Replaces the data item in this manifest entry.  This is a temporary hack to facilitate testing of Non-indexed object rendering (for tessellation demo)
   inline void OverrideDataItem( SuModelDataItemPtr pItem ) { m_pItem = pItem; };

private:

   /// Private constructor.  Instantiation is restricted to the SuModelDataManifest class
   SuModelDataManifestEntry(); 

   friend class SuModelDataManifest;

   SuModelDataType      m_eItemType;       ///< Type of data item
   SuString             m_itemName;        ///< Data item name
   SuString             m_itemFileName;    ///< Name of the file in the archive containing this data item
   SuMD5Hash            m_itemHash;        ///< MD5 hash of the data item 
   mutable SuModelDataItemPtr   m_pItem;   ///< A pointer to the model data item, if it has been loaded, NULL otherwise
         
   SuString             m_adbUsage;        ///< ADBUsage string, if the item is an ADB, unused otherwise.
   SuModelADBType       m_eADBType;        ///< ADB type, if the item is an ADB, unused otherwise.
 
};


#endif // _SU_MODELDATAMANIFESTENTRY_H_
