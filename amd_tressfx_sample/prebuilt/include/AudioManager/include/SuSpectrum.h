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

#ifndef _SU_SPECTRUM_H
#define _SU_SPECTRUM_H

#include "SuTypes.h"

//=========================================================================================================
/// \ingroup Audio
/// \brief A class representing an spectrum of audio frequencies
/// 
//=========================================================================================================
// tolua_begin
class SuSpectrum
{
public:
   /// Constructor
   SuSpectrum( float32 *pfData, int32 nSize ) : m_pfData( pfData ), m_nSize( nSize ) {}

   /// Get the number of frequencies
   int32 GetSize() { return m_nSize; }

   /// Get the pointer to the data 
   void CopyBuffer( const float32 *pfBuffer ) { memcpy( (void*)pfBuffer, (void*)m_pfData, m_nSize*sizeof(float32) ); };

   /// Destructor
   virtual ~SuSpectrum() { delete[] m_pfData; }

// tolua_end

private:

   int32    m_nSize;    //# float values
   float32* m_pfData;   //pointer to spectrum data

// tolua_begin
};
// tolua_end

#endif // _SU_SPECTRUM_H
