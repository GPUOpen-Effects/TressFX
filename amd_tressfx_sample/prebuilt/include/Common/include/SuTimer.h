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

#ifndef _SU_TIMER_H_
#define _SU_TIMER_H_

#include "SuTypes.h"
#include "SuTimeSubject.h"
#include "SuObserver.h"

class SuClock;


//=================================================================================================================================
/// \ingroup Common
/// \brief  Interface to time querying
/// \sa SuClock, SuTimeSource, SuObserver
//=================================================================================================================================

// tolua_begin
class SuTimer : public SuTimeSubject, public SuObserver
{
public:
   /// Constructor
   SU_DLL SuTimer( SuTimeSubject& rTimeUpdateSource );

   /// Destructor
   virtual SU_DLL ~SuTimer();

   /// Returns current time of timer
   inline SuTime GetTime() const { return m_fCurrentTime; }

   /// Set the current time of this timer
   virtual SU_DLL void SetTime( SuTime fNewTime );

   /// Set new time relative to current time
   virtual SU_DLL void SetTimeRelative( SuTime fOffset );

   /// Returns time at previous frame
   inline SuTime GetPreviousTime() const { return m_fPrevTime; }

   /// Returns duration of previous frame
   virtual SU_DLL SuTime GetFrameDuration() const { return m_fFrameTime; }
   
   /// Pause/unpause timer
   SU_DLL void Pause( bool bOn );

   /// Returns pause state
   SU_DLL bool IsPaused() const;

   /// Set time scale for speedup/slow motion
   SU_DLL void SetScale( float32 fScale );
   
   /// Returns current time scale
   SU_DLL float32 GetScale() const;

// tolua_end
   
protected:
   /// Update internal state on ping from clock
   virtual void Notify();

   SuTime m_fCurrentTime;     ///< Current time
   SuTime m_fPrevTime;        ///< Previous frame time
   SuTime m_fFrameTime;       ///< Duration of last frame
      
private:

   SuTimeSubject& m_rTimeUpdateSource; ///< Reference to clock
   
   bool    m_bPaused;         ///< Is the clock currently paused
   float32  m_fScale;         ///< Time scale

// tolua_begin
};
// tolua_end


#endif // _SU_TIMER_H_
