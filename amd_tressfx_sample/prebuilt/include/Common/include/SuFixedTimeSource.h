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

#ifndef _SU_FIXEDTIMESOURCE_H_
#define _SU_FIXEDTIMESOURCE_H_

#include "SuTimeSource.h"

//=========================================================================================================
/// \ingroup Common
/// \brief Returns synthetic benchmark time
/// \sa SuTimeSource, SuTimer, SuClock
//=========================================================================================================
// tolua_begin
class SuFixedTimeSource : public SuTimeSource
{
public:
   /// Constructor
   SuFixedTimeSource( SuTime fIncrement )
      : m_fIncrement( fIncrement ),
        m_fCurrentTime( -fIncrement ) // start at this value so that the first call to GetTime() returns 0
   {}

   /// Increases the current time by the increment and returns it
   virtual SuTime GetTime() const { m_fCurrentTime += m_fIncrement; return m_fCurrentTime; }

   /// Returns the fixed time increment
   inline SuTime GetIncrement() const { return m_fIncrement; }

// tolua_end

private:
   SuTime m_fIncrement;
   mutable SuTime m_fCurrentTime;

// tolua_begin
};
// tolua_end


#endif // _SU_FIXEDTIMESOURCE_H_
