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

#ifndef _SUANNOTATIONMAP_H
#define _SUANNOTATIONMAP_H

#include "SuTypes.h"
#include "SuString.h"
#include "SuVector4.h"
#include "SuHashMap.h"
#include "SuCountedPtr.h"

// forward declarations
class SuAnnotationValue;

/// \brief A reference-counted smart pointer to an annotation value
///
/// \ingroup EffectManager
typedef SuCountedPtr<SuAnnotationValue> SuAnnotationValueSharedPtr;

class SuAnnotationMap;

/// \brief A value in a shader annotation map
///
/// \ingroup EffectManager
/// SuAnnotationValue can store either a bool, float, string, vector4
/// or a full SuAnnotationMap. SuAnnotationValues are immutable: Once one is
/// constructed, it cannot be changed.
class SuAnnotationValue
{
public:
   /// Constructs a float value
   explicit SU_DLL SuAnnotationValue( float32 f );
   /// Constructs a bool value
   explicit SU_DLL SuAnnotationValue( bool b );
   /// Constructs a string value
   explicit SU_DLL SuAnnotationValue( const SuString& s );
   /// Constructs a string value
   explicit SU_DLL SuAnnotationValue( const char8* pStr );
   /// Constructs a vector value
   explicit SU_DLL SuAnnotationValue( const SuVector4& vec );
   /// Constructs a value that contains another SuAnnotationMap
   explicit SU_DLL SuAnnotationValue( SuAnnotationMap* map );
   /// Copy constructor
   explicit SU_DLL SuAnnotationValue( const SuAnnotationValue& rValue );
   /// Copy constructor
   explicit SU_DLL SuAnnotationValue( const SuAnnotationValueSharedPtr& rValue );
   /// Destructor
   SU_DLL ~SuAnnotationValue ();

   /// Value type
   enum Type
   {
      BOOL,
      FLOAT,
      STRING,
      VECTOR,
      MAP
   };

   /// Returns the value type
   inline Type GetType() const { return m_type; }

   /// Returns the float value
   SU_DLL float32 GetFloat() const;
   /// Returns the bool value
   SU_DLL bool GetBool() const;
   /// Returns the string value
   SU_DLL const SuString* GetString() const;
   /// Returns the vector value
   SU_DLL const SuVector4* GetVector() const;
   /// Returns the map value
   SU_DLL const SuAnnotationMap* GetMap() const;

private:
   /// Assignment is forbidden because that would violate the object's immutablility
   SuAnnotationValue& operator=( const SuAnnotationValue& );

   /// Copy constructor helper function
   void CopyConstructorImpl( const SuAnnotationValue& rValue );

   Type m_type;

   union
   {
      bool m_bool;
      float32 m_float;
      SuString *m_pString;
      SuVector4 *m_pVector;
      SuAnnotationMap *m_pMap;
   };
};

/// \brief A string -> shader annotations value map
///
/// \ingroup EffectManager
/// This map uses reference counted pointers in order to simplify cleanup of
/// annotation maps. Simply deleting the map will in turn delete all the
/// contained annotation values pointed to by the smart pointers stored in
/// the map.
class SuAnnotationMap
{
public:
   typedef SuHashMap<SuString, SuAnnotationValueSharedPtr, SuStringHasher, SuHashComparer<SuString> > AnnotationMapData;
   typedef AnnotationMapData::iterator iterator;
   typedef AnnotationMapData::const_iterator const_iterator;
   
   /// Constructor
   inline SuAnnotationMap() : m_map() {}

   /// Copy constructor
   inline SuAnnotationMap( const SuAnnotationMap& rMap ) : m_map( rMap.m_map ) {}

   /// Destructor
   virtual SU_DLL ~SuAnnotationMap() {}

   /// Assignment operator
   inline SuAnnotationMap& operator=( const SuAnnotationMap& rMap ) { m_map = rMap.m_map; return *this; }

   // The following methods are here for backwards-compatibility with existing code
   // (Which was written when SuAnnotationMap was just a typedef for a hash map

   inline iterator begin() { return m_map.begin(); }
   inline const_iterator begin() const { return m_map.begin(); }

   inline iterator end() { return m_map.end(); }
   inline const_iterator end() const { return m_map.end(); }

   inline iterator find( const SuString& rKey ) { return m_map.find( rKey ); }
   inline const_iterator find( const SuString& rKey ) const { return m_map.find( rKey ); }

   inline size_t size() const { return m_map.size(); }
   inline bool empty() const { return m_map.empty(); }
   inline void clear() { m_map.clear(); }

   inline void insert( const SuString& rKey, SuAnnotationValueSharedPtr pValue ) { m_map.insert( rKey, pValue ); }
   inline SuAnnotationValueSharedPtr& operator[]( const SuString& rKey ) { return m_map[rKey]; }

   // Convenience functions for accessing data in the annotation map

   /// Checks whether a value with a given key exists
   SU_DLL bool ValueExists( const SuString& rKey ) const;

   /// Returns a value with a given key
   SU_DLL SuAnnotationValueSharedPtr LookupValue( const SuString& rKey ) const;

   /// Look up a string value and return a default value if it isn't found
   SU_DLL SuString LookupString( const SuString& rKey, const SuString& rDefaultValue ) const;

   /// Look up a string value and return a default value if it isn't found
   SU_DLL bool LookupBool( const SuString& rKey, bool rbDefaultValue ) const;

   /// Look up a string value and return a default value if it isn't found
   SU_DLL float32 LookupFloat( const SuString& rKey, float32 rfDefaultValue ) const;

   /// Look up a string value and return a default value if it isn't found
   SU_DLL SuVector4 LookupVector( const SuString& rKey, const SuVector4& rDefaultValue ) const;

private:
   AnnotationMapData m_map;
};

#endif
