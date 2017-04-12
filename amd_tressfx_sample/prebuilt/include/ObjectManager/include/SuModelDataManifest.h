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

#ifndef _SU_MODELDATAMANIFEST_H_
#define _SU_MODELDATAMANIFEST_H_

#include "SuModelDataItem.h"
#include "SuModelDataManifestEntry.h"

class SuModelVB;
class SuModelIB;
class SuModelADB;
class SuModelTexture;

typedef SuCountedPtr<SuModelVB> SuModelVBPtr;
typedef SuCountedPtr<SuModelIB> SuModelIBPtr;
typedef SuCountedPtr<SuModelADB> SuModelADBPtr;
typedef SuCountedPtr<SuModelTexture> SuModelTexturePtr;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief  A collection of data items in a model data file
/// 
//=================================================================================================================================
class SuModelDataManifest
{
public:

   SU_DLL ~SuModelDataManifest();

   /// Constructs a manifest from an archive
   static SU_DLL SuModelDataManifest* ExtractFromArchive( const SuArchive* pArchive );

   /// Returns a manifest entry for an item with a particular name
   const SU_DLL SuModelDataManifestEntry* GetEntryByName( const SuString& rItemName ) const;

   /// Returns a manifest entry for an ADB with a particular usage field.  If more than one such ADB exists, the first ADB is returned
   const SU_DLL SuModelDataManifestEntry* GetEntryByADBUsage( const SuString& rUsage ) const;

   /// Returns the number of entries in the manifest
   inline uint32 GetNumEntries() const { return (uint32) m_entries.size(); };

   /// Accessor for a specific manifest entry
   inline const SuModelDataManifestEntry* GetEntry( uint32 i ) const { return m_entries[i]; };

   /// Writes the manifest to a text file in an archive
   SU_DLL bool WriteToArchive( SuArchive* pArchive ) const;

   /// Adds a data item to the model
   SU_DLL void AddModelDataItem( SuModelVBPtr pItem );
   SU_DLL void AddModelDataItem( SuModelIBPtr pItem );
   SU_DLL void AddModelDataItem( SuModelADBPtr pItem );
   SU_DLL void AddModelDataItem( SuModelTexturePtr pItem );

   /// Removes a data item from the manifest
   SU_DLL void RemoveDataItem( const SuModelDataManifestEntry* pEntry );

 
private:

   /// A helper method which parses a dependency list from a manifest file
   static bool ParseDependencies( SuList< SuString >::iterator& rCurrentLine, SuList<SuString>::iterator& rEnd );

   SuArray<SuModelDataManifestEntry*> m_entries;

};


#endif // _SU_MODELDATAMANIFEST_H_
