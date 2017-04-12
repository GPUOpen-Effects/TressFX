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

#ifndef _SULOG_H
#define _SULOG_H

#include "SuLogManager.h"

#ifndef __GCCE__
/// \ingroup Common
#define SuLogError(...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( __VA_ARGS__ ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_ERROR, __FILE__, __LINE__); \
}

/// \ingroup Common
#define SuLogCritical(...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( __VA_ARGS__ ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_CRITICAL, __FILE__, __LINE__); \
}
/// \ingroup Common
#define SuLogWarning(...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( __VA_ARGS__ ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_WARNING, __FILE__, __LINE__); \
}
/// \ingroup Common
#define SuLogDebug(...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( __VA_ARGS__); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_DEBUG, __FILE__, __LINE__); \
}
/// \ingroup Common
#define SuLogInfo(...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( __VA_ARGS__ ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_INFO); \
}
#else
/// \ingroup Common
#define SuLogError(args...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( args ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_ERROR, __FILE__, __LINE__); \
}

/// \ingroup Common
#define SuLogCritical(args...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( args ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_CRITICAL, __FILE__, __LINE__); \
}
/// \ingroup Common
#define SuLogWarning(args...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( args ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_WARNING, __FILE__, __LINE__); \
}
/// \ingroup Common
#define SuLogDebug(args...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( args ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_DEBUG, __FILE__, __LINE__); \
}
/// \ingroup Common
#define SuLogInfo(args...) \
{ \
   SuString _logTmpStr_; \
   _logTmpStr_.Format ( args ); \
   SuLogManager::GetRef ().LogMessage (_logTmpStr_ , LOG_INFO); \
}
#endif

#endif // _SULOG_H
