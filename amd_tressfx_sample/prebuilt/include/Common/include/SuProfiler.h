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

#ifndef _SUPROFILER_H
#define _SUPROFILER_H

#include "SuTypes.h"
#include "SuList.h"
#include "SuHashMap.h"

#ifdef SU_ENABLE_PROFILE
#define SU_PROFILE(x)         SuProfileHelper _profileHelperInstance(x)
#define SU_PROFILE_BEGIN(x)   SuProfiler::BeginProfile(x)
#define SU_PROFILE_END(x)     SuProfiler::EndProfile(x)
#define SU_PROFILE_CALL(id, call) SuProfiler::BeginProfile(id); call; SuProfiler::EndProfile(id);
#else
#define SU_PROFILE(x)
#define SU_PROFILE_BEGIN(x)
#define SU_PROFILE_END(x)
#define SU_PROFILE_CALL(profileID, call) call
#endif

class SuProfiler
{
protected:
   /// Constructor
   SuProfiler ();

public:
   /// Destructor
   ~SuProfiler ();

   /// Singleton access
   static SuProfiler* GetPtr ();

   /// Singleton access
   static SuProfiler& GetRef ();

   /// Starts profiling
   void BeginProfile (const SuString &name);

   /// Stops profiling
   void EndProfile (const SuString &name);

private:
   uint64 GetTicks () { return 0; }

   /// Singleton instance
   static SuProfiler *ms_instance;

   /// Element on the internal profiler stack
   struct StackElement
   {
      /// Active profile name
      SuString name;

      /// Start time of profile
      uint64 startTime;
   };

   /// Information for a particular profile in one frame
   struct PerFrameInfo
   {
      /// Total time taken this frame
      uint64 timeTaken;

      /// Total number of calls to profile this frame
      uint32 numCalls;
   };

   SuList<StackElement> m_profileStack;
   SuHashMap<SuString, PerFrameInfo, SuStringHasher, SuHashComparer<SuString> > m_profileFrameInfo;
};

/// \brief Class to help profile execution time of a C++ scope. Never use directly.
/// 
/// \ingroup Common
/// Instead of using this class, use the macro SU_PROFILE to profile the execution time
/// of a C++ scope (usually a function or method).
class SuProfileHelper
{
public:
   /// Constructor. Starts profiling.
   SuProfileHelper (const SuString &name)
   : m_profileName (name)
   {
      SuProfiler::GetRef ().BeginProfile (m_profileName);
   }
   
   /// Destructor. Ends profiling.
   ~SuProfileHelper ()
   {
      SuProfiler::GetRef ().EndProfile (m_profileName);
   }

private:
   /// Currently active profile name.
   SuString m_profileName;
};

#endif
