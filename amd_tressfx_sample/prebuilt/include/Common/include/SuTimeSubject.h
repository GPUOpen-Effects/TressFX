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

#ifndef _SU_TIMESUBJECT_H_
#define _SU_TIMESUBJECT_H_

#include "SuList.h"

class SuObserver;

typedef SuList<SuObserver *> SuObserverList;

//=========================================================================================================
/// \ingroup Common
/// \brief Base class for timing objects.
///
/// Provides support for registering observers to be notified of time changes, and provides an interface
/// for getting the time elapsed during the last frame.
//=========================================================================================================
class SuTimeSubject
{
public:
   virtual SU_DLL ~SuTimeSubject();

   /// Add an observer to notify of clock events
   void SU_DLL AddObserver( SuObserver* pObserver );

   /// Removes an observer
   void SU_DLL RemoveObserver( SuObserver* pObserver );

   /// Returns duration of previous frame
   virtual SU_DLL SuTime GetFrameDuration() const = 0;

protected:
   /// Notifies all registered observers of an update
   void SU_DLL NotifyObservers() const;

private:
   SuObserverList m_observers;
};

#endif // _SU_TIMESUBJECT_H_
