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

#ifndef _SU_CLOCK_H_
#define _SU_CLOCK_H_

#include "SuTypes.h"
#include "SuArray.h"
#include "SuTimeSubject.h"

class SuTimeSource;

//=========================================================================================================
/// \ingroup Common
/// \brief The time pulse that drives all timers
/// \sa SuObserver, SuTimeSource, SuTimer
//=========================================================================================================

// tolua_begin
class SuClock : public SuTimeSubject
{
public:
   /// Constructor
   SU_DLL SuClock( const SuTimeSource* pSource = NULL );

   /// Destructor
   virtual SU_DLL ~SuClock();

   /// Sets the time source to use
   SU_DLL void SetTimeSource( const SuTimeSource* pSource );
   
   /// Updates clock time from time source
   SU_DLL void FrameStep();
   
   /// Returns current clock time
   SuTime GetTime() const { return m_currentTime; }

   /// Returns clock time at previous frame
   SuTime GetPreviousTime() const { return m_lastTime; }

   /// Returns duration of last frame
   virtual SuTime GetFrameDuration() const { return m_frameTime; }

   /// Returns min and max frame duration during smooting interval
   SU_DLL void GetMinMaxFrameDuration( SuTime *dMin, SuTime *dMax ) const;
   
   /// Returns number of frames elapsed
   inline int32 GetFrameNumber() const { return m_nFrameNumber; }

   /// Returns current frame rate
   SU_DLL SuTime GetFrameRate() const;
   
   /// Enable/disable time smoothing
   SU_DLL void SetSmoothTime( bool bEnable ) { m_bUseSmoothTime = bEnable; }

   /// Query time smoothing state
   SU_DLL bool IsSmoothTimeEnabled() const { return m_bUseSmoothTime; }

   /// Set the time filtering window
   SU_DLL void SetFiltering( int32 nFrameWindow, SuTime frameDefault = 0.030 );
   
   /// clears the smoothing history
   SU_DLL void ClearFrameHistory();

// tolua_end
   
private:
   /// Get exact unsmoothed duration of last frame
   SuTime GetExactLastFrameDuration();

   /// Add a frametime to the smoothing history
   void AddToFrameHistory( SuTime exactFrameDuration );

   /// Get average frame duration from frame history
   SuTime GetAverageFrameDuration() const;
   
   /// The time source used for the clock to get time
   const SuTimeSource* m_pTimeSource;

   /// Current time
   SuTime m_currentTime;

   /// unsmoothed time
   SuTime m_rawCurrentTime;

   /// Last time that was retrieved
   SuTime m_lastTime;

   /// Current frame time that was retrieved   
   SuTime m_frameTime;

   /// Running average of the frame times
   SuTime m_avgFrameTime;

   /// Count of the current frame number
   int32 m_nFrameNumber;
   
   /// Has the clock been setup with a time source
   bool   m_bTimeSourceInitialized;

   /// Start time from the time source
   SuTime m_sourceStartValue;

   /// Previous time value retrieved from source
   SuTime m_sourceLastValue;
      
   /// Smooth time enable/disable
   bool   m_bUseSmoothTime;

   /// Number of frames to filter
   int32   m_nFrameFilteringWindow;
   
   /// Default frame time
   SuTime m_frameDefaultTime;

   /// History of frame times
   SuList<SuTime> m_frameDurationHistory;

// tolua_begin
};
// tolua_end

#endif // _SU_CLOCK_H_
