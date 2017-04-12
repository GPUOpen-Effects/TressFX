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

#ifndef _SUEFFECTPARAMETER_H
#define _SUEFFECTPARAMETER_H

#include "SuTypes.h"
#include "SuAnnotationMap.h"
#include "SuVector4.h"
#include "SuMatrix4.h"
#include "SuUpdateHandler.h"

// forward declarations
class SuEffectParameter;


/// \brief Effect parameter
/// 
/// \ingroup EffectManager

// tolua_begin
class SuEffectParameter
{
public:
   /// Effect parameter data type
   enum DataType
   {
      FLOAT       = 0,  ///< float scalar
      INT         = 1,  ///< int   scalar
      BOOL        = 2,  ///< bool  scalar
      VECTOR      = 3,  ///< float 4-vector
      VECTOR_INT  = 4,  ///< int   4-vector
      VECTOR_BOOL = 5,  ///< bool  4-vector
      MATRIX      = 6,  ///< float 4x4 matrix
      MAX_PARAMETER_DATA_TYPES = 7
   };

   /// Converts a Sushi data type from a string name into a data type enum.
   static SU_DLL bool DataTypeFromString( const SuString& rType, DataType& eDataType );

   static SU_DLL SuString DataTypeToString( DataType eType );

   /// Constructor
   SU_DLL SuEffectParameter( const SuString& rName, DataType t, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for float
   SU_DLL SuEffectParameter( const SuString& rName, float32 fDefaultVal, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for int parameter
   SU_DLL SuEffectParameter( const SuString& rName, int32 nDefaultVal, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for bool parameter
   SU_DLL SuEffectParameter( const SuString& rName, bool bDefaultVal, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for float vector parameter
   SU_DLL SuEffectParameter( const SuString& rName, const SuVector4& rDefaultVal, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for int vector parameter
   SU_DLL SuEffectParameter( const SuString& rName, const int32 nValues [4], SuAnnotationMap* pAnnotations = NULL );

   /// Constructor for bool vector parameter
   SU_DLL SuEffectParameter( const SuString& rName, const bool bValues [4], SuAnnotationMap* pAnnotations = NULL );

   /// Constructor for matrix parameter
   SU_DLL SuEffectParameter( const SuString& rName, const SuMatrix4& rDefaultVal, SuAnnotationMap* pAnnotations = NULL );

   /// Constructor for parameter array
   SU_DLL SuEffectParameter( const SuString& rName, DataType t, uint32 nArraySize, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for float parameter array
   SU_DLL SuEffectParameter( const SuString& rName, float32 fDefaultVal, uint32 nArraySize, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for int parameter array
   SU_DLL SuEffectParameter( const SuString& rName, int32 nDefaultVal, uint32 nArraySize, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for bool parameter array
   SU_DLL SuEffectParameter( const SuString& rName, bool bDefaultVal, uint32 nArraySize, SuAnnotationMap* pAnnotations = NULL );
   
   /// Constructor for vector parameter array
   SU_DLL SuEffectParameter( const SuString& rName, const SuVector4& rDefaultVal, uint32 nArraySize, SuAnnotationMap* pAnnotations = NULL );

   /// Constructor for int vector parameter array
   SU_DLL SuEffectParameter( const SuString& rName, const int32 nDefaultVals[4], uint32 nArraySize, SuAnnotationMap* pAnnotations = NULL );

   /// Constructor for bool vector parameter array
   SU_DLL SuEffectParameter( const SuString& rName, const bool bDefaultVals[4], uint32 nArraySize, SuAnnotationMap* pAnnotations = NULL );

   /// Constructor for matrix parameter array
   SU_DLL SuEffectParameter( const SuString& rName, const SuMatrix4& rDefaultVal, uint32 nArraySize, SuAnnotationMap* pAnnotations = NULL );


   /// Destructor
   SU_DLL ~SuEffectParameter();

   /// Returns the data type of this parameter
   inline DataType GetDataType() const { return m_dataType; }

   /// Returns the parameter's default value
   SU_DLL float32 GetDefaultValueFloat() const;

   /// Returns the parameter's default value
   SU_DLL int32 GetDefaultValueInt() const;

   /// Returns the parameter's default value
   SU_DLL bool GetDefaultValueBool() const;

   /// Returns the parameter's default value
   SU_DLL const SuVector4& GetDefaultValueVector() const;

   /// Returns the parameter's default value
   SU_DLL const SuMatrix4& GetDefaultValueMatrix() const;

   /// Gets a pointer to the default value of this parameter
   SU_DLL const void* GetDefaultValuePtr() const;

   /// Returns the annotations for this parameter
   inline const SuAnnotationMap* GetAnnotations() const { return m_pAnnotations; }

   /// returns the array size for this parameter array
   inline uint32 GetArraySize() const { return m_nArrayElems; }

   /// Returns whether this parameter is an array
   inline bool IsArray() const { return GetArraySize () > 1; }

// tolua_end

   // don't expose callback stuff to Lua

   /// Returns the update handler for this effect parameter
   SU_DLL SuCountedPtr<SuUpdateHandler> GetUpdateHandler() const;

   /// Executes the update handler (if any) and modifies the value of the effect parameter
   SU_DLL bool ExecuteUpdateHandler() const;

// tolua_begin

   /// Gets a pointer to the current value of this parameter
   inline const void* GetValuePtr() const { return m_pValue; }

   /// Non-const access to the value of this parameter
   inline void* GetValuePtr()  { return m_pValue; }

   /// Sets the current parameter value to a float
   SU_DLL void SetFloat( float32 f );

   /// Sets the current parameter value to an int
   SU_DLL void SetInt( int32 i );

   /// Sets the current parameter value to a bool
   SU_DLL void SetBool( bool b );

   /// Sets the current parameter value to a float vector
   SU_DLL void SetFloatVector( const float32* pFirstValue );

   /// Sets the current parameter value to an int vector
   SU_DLL void SetIntVector( const int32* pFirstValue );

   /// Sets the current parameter value to a bool vector
   SU_DLL void SetBoolVector( const bool* pFirstValue );

   /// Sets the current parameter value to a float matrix
   SU_DLL void SetFloatMatrix( const float32* pFirstValue );

   /// Sets the current parameter value using raw data
   SU_DLL void SetRawValue( const void* pData, uint32 nSizeInBytes );

   /// Sets the parameter value back to its default
   SU_DLL void SetToDefaultValue();

   /// Returns the total size of the parameter data
   SU_DLL uint32 GetParameterSize() const;

   /// Returns the size of a data type in bytes
   static SU_DLL uint32 GetDataTypeSize( DataType t ) ;

   /// Returns the name of the parameter
   inline const SuString& GetName() const { return m_name; };

// tolua_end

   /// Returns true if the parameter has an app-update handler.  False otherwise
   inline bool HasAppUpdateHandler() const { return m_pUpdateHandler.get() != NULL; };

private:

   /// Creates an app update handler for this effect parameter, if appropriate
   void MakeUpdateHandler( SuAnnotationMap* pAnnotations );

   /// Allocates memory to hold the data
   void AllocateValueBackingStore();

   /// Frees data allocated with AllocateValueBackingStore()
   void FreeValueBackingStore();


   SuString m_name;

   DataType m_dataType;
   SuAnnotationMap* m_pAnnotations;
   uint32 m_nArrayElems;
   
   SuCountedPtr<SuUpdateHandler> m_pUpdateHandler;

   uint8* m_pValue;

   union
   {
      float32    m_fDefaultFloat;
      int32      m_nDefaultInt;
      bool       m_bDefaultBool;
      bool       m_bDefaultVecBool[4];
      SuVector4* m_pDefaultVec;
      int32*     m_pDefaultVecInt;
      SuMatrix4* m_pDefaultMat;
   };

   // Color conversion (valid for float and float4 types)
   bool m_bColorConvert; ///< Parameter needs color conversion 
   float32 m_fColorConversionExponent; ///< Color conversion exponent
   bool m_bColorCoversionMask[4]; ///< Color conversion channel mask   

   /// Setup the color conversion values
   void SetupColorConversion( SuAnnotationMap* pAnnotations );

   /// Apply color conversion
   void ApplyColorConversion( bool bDefaultValue, bool bValue ) const; // Making this const so you can call it from ExecuteUpdateHandler

   /// Array containing the size of each parameter data type
   static const uint32 ms_dataTypeSize[];

   /// Array containing string names for each data type
   static const SuString ms_dataTypeNames[];

// tolua_begin
};
// tolua_end

#endif
