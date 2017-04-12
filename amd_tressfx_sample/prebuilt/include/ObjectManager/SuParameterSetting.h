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

#ifndef _SU_PARAMETERSETTING_H_
#define _SU_PARAMETERSETTING_H_

#include "SuEffectParameter.h"
#include "SuHashMap.h" // for SuPair
#include "SuList.h"

class SuParameterSetting;
typedef SuCountedPtr< SuParameterSetting > SuParameterSettingPtr;

//=========================================================================================================
/// \ingroup ObjectManager
/// \brief A class to encapsulate a value that is assigned to an effect parameter
/// 
///
//=========================================================================================================
class SuParameterSetting
{
public:

   /// Constructor 
   SuParameterSetting( const SuString& rParamName,  SuEffectParameter::DataType eType, uint32 nArraySize,  
                       const SuString& rAnimationName, const uint8* pDefaultValue );

   /// Destructor 
   ~SuParameterSetting();

   /// Returns the name of the effect parameter that is to be set
   const SuString& GetParameterName() const;

   /// Returns the data type of this parameter
   SuEffectParameter::DataType GetType() const;

   /// Returns the number of elements in the array (or 1 if the parameter is not an array)
   uint32 GetArraySize() const;

   /// Returns the number of scalar elements (floats, ints, or bools) in the parameter's value array
   uint32 GetScalarCount() const;

   /// Returns the size of the parameter value, in bytes
   uint32 GetValueSize() const;

   /// Returns the default value to assign to the parameter.  If NULL, the value declared in the effect file should be used
   const void* GetDefaultValue() const;

   /// Accessor for the name of the parameter's animation
   const SuString& GetAnimationName() const;

   /// @materialedit - Mutator function to set this parameter's value in-engine
   /// This function is to be used judiciously and carefully. It is provided
   /// to support live material editing capability and will remain here until we 
   /// think of a better way
   bool SetValue( 
		SuEffectParameter::DataType eType, 
		const uint32 nArraySize, 
		const uint8* pValue );


private:

   SuString m_parameterName;

   SuEffectParameter::DataType m_eType;

   uint32 m_nArraySize;

   SuString m_animationName;

   const uint8* m_pValue;


};

/// An array of counted parameter setting pointers
typedef SuArray<SuParameterSettingPtr> SuParameterSettingArray;

class SuEffectParameter;


/// An association between an effect parameter and a corresponding parameter setting
typedef SuPair< SuEffectParameter*, SuParameterSettingPtr > SuParameterBinding;

/// An array of parameter bindings
typedef SuList< SuParameterBinding > SuParameterBindingList;



#endif // _SU_PARAMETERSETTING_H_
