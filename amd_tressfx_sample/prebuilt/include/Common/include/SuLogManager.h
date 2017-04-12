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

#ifndef _SULOGMANAGER_H
#define _SULOGMANAGER_H

#include "SuTypes.h"
#include "SuList.h"
#include "SuHashMap.h"

/// \ingroup Common
typedef enum _SuLogMessageType
{
   LOG_INFO = 0,
   LOG_DEBUG = 1,
   LOG_WARNING = 2,
   LOG_ERROR = 3,
   LOG_CRITICAL = 4
} SuLogMessageType;

class SuFile;
class SuLogListener;

/// \brief Manager for handling outputs to a message log
///
/// \ingroup Common
/// SuLogManager takes messages and logs them to a file. The messages
/// are augmented with timestamps, message severity indicators, and optional filename
/// and line number information.
/// Messages are not only written to a file, but also to stderr and to a debug console
/// using OutputDebugString. Moreover, it is possible to register objects that implement
/// the SuLogListener interface, which will be notified of any message sent to the log. This
/// can be useful for implementing a seperate log UI.
///
/// For the actual logging of messages, the application should use the SuLogInfo(), SuLogDebug(),
/// SuLogWarning(), SuLogError(), and SuLogCritical() macros.
// tolua_begin
class SuLogManager
{
// tolua_end
protected:
   /// Constructor
   SU_DLL SuLogManager( bool bUseLogFile = true, bool bUseStdout = true, const SuString& logFileName = _T("SushiLog.txt") );

public:

   /// Destructor
   virtual SU_DLL ~SuLogManager();

   /// Specifies the behavior of the log manager and constructs the singleton.
   static SU_DLL void Init( bool bUseLogFile, bool bUseStderr, const SuString& logFileName );

   /// Destroys the singleton
   static SU_DLL void ShutDown();

   /// Singleton access
   static SU_DLL SuLogManager* GetPtr();

   /// Singleton access
   static SU_DLL SuLogManager& GetRef();

   /// Writes a message to the log
   SU_DLL void LogMessage( const SuString& rMsg, SuLogMessageType type = LOG_INFO,
                            const char8* pFileName = NULL, int32 nLineNumber = -1 );

   /// Writes a string to the log
   SU_DLL void WriteString( const SuString& rStr, SuLogMessageType type = LOG_INFO );

   /// Adds a listener that gets notified with each logged message
   SU_DLL void AddLogListener( SuLogListener* pListener );

   /// Removes a log message listener
   SU_DLL void RemoveLogListener( SuLogListener* pListener );

   /// Opens the current error log in an external text editor
   SU_DLL void ShowErrorLogInEditor();

   void ResetErrorMap();

   static SU_DLL void ResetSushiErrorLog() { SuLogManager::GetRef ().ResetErrorMap(); } // tolua_export

private:
   void OpenLog();
   void CloseLog();
   SuString FilenameAndLineNumberToString( const SuString& rFileName, int32 nLineNumber, bool bUseShortFilename = true );

   // data members

   static SuLogManager* ms_pInstance;
   static const TCHAR*  m_typeStrings[5];

   typedef SuList<SuLogListener *> SuLogListenerList;

   SuLogListenerList m_logListeners;
   SuString          m_logFileName;
   SuFile*           m_pLogFile;
   
   bool m_bSevereErrorOccured;
   bool m_bUseLogFile;
   bool m_bUseStdout;

   typedef SuHashMap<SuString, uint32, SuStringHasher, SuHashComparer<SuString> > ErrorLogMap;
   ErrorLogMap m_errorLog;

// tolua_begin
};
// tolua_end

#endif
