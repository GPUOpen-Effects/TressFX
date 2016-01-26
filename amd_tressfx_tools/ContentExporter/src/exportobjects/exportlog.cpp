//-------------------------------------------------------------------------------------
// ExportLog.cpp
//  
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=226208
//-------------------------------------------------------------------------------------

#include "stdafx.h"
#include "exportlog.h"

namespace ATG
{
    bool g_bLoggingEnabled = true;
    UINT g_uLogLevel = 10;
    typedef std::list< ILogListener* > LogListenerList;
    LogListenerList g_Listeners;

    size_t g_dwWarningCount = 0;
    size_t g_dwErrorCount = 0;

    typedef std::list<const CHAR*> StringList;
    StringList      g_WarningsList;
    StringList      g_ErrorsList;

    CHAR g_strBuf[4096];
    void BroadcastMessage( UINT uMessageType, const CHAR* strMsg );

    void ExportLog::AddListener( ILogListener* pListener )
    {
        g_Listeners.push_back( pListener );
    }

    void ExportLog::ClearListeners()
    {
        g_Listeners.clear();
    }

    void ExportLog::EnableLogging( bool bEnable )
    {
        g_bLoggingEnabled = bEnable;
    }

    void ExportLog::SetLogLevel( UINT uLevel )
    {
        g_uLogLevel = uLevel;
    }

    UINT ExportLog::GetLogLevel()
    {
        return g_uLogLevel;
    }

    bool ExportLog::GenerateLogReport( bool bEchoWarningsAndErrors )
    {
        LogMsg( 0, "%Iu warning(s), %Iu error(s).", g_dwWarningCount, g_dwErrorCount );
        if( !bEchoWarningsAndErrors )
            return (g_dwErrorCount > 0);

        StringList::iterator iter = g_WarningsList.begin();
        StringList::iterator end = g_WarningsList.end();
        while( iter != end )
        {
            BroadcastMessage( 1, *iter );
            ++iter;
        }

        iter = g_ErrorsList.begin();
        end = g_ErrorsList.end();
        while( iter != end )
        {
            BroadcastMessage( 2, *iter );
            ++iter;
        }

        return (g_dwErrorCount > 0);
    }

    void ExportLog::ResetCounters()
    {
        StringList::iterator iter = g_WarningsList.begin();
        StringList::iterator end = g_WarningsList.end();
        while( iter != end )
        {
            delete[] *iter;
            ++iter;
        }
        g_WarningsList.clear();

        iter = g_ErrorsList.begin();
        end = g_ErrorsList.end();
        while( iter != end )
        {
            delete[] *iter;
            ++iter;
        }
        g_ErrorsList.clear();

        g_dwWarningCount = 0;
        g_dwErrorCount = 0;
    }

    void BroadcastMessage( UINT uMessageType, const CHAR* strMsg )
    {
        LogListenerList::iterator iter = g_Listeners.begin();
        LogListenerList::iterator end = g_Listeners.end();

        while( iter != end )
        {
            switch( uMessageType )
            {
            case 1:
                (*iter)->LogWarning( strMsg );
                break;
            case 2:
                (*iter)->LogError( strMsg );
                break;
            default:
                (*iter)->LogMessage( strMsg );
                break;
            }
            ++iter;
        }
    }


    void ExportLog::LogCommand( DWORD dwCommand, void* pData )
    {
        LogListenerList::iterator iter = g_Listeners.begin();
        LogListenerList::iterator end = g_Listeners.end();

        while( iter != end )
        {
            (*iter)->LogCommand( dwCommand, pData );
            ++iter;
        }
    }


    void ExportLog::LogMsg( UINT uImportance, _In_z_ _Printf_format_string_ const CHAR* strFormat, ... )
    {
        if( !g_bLoggingEnabled || ( uImportance > g_uLogLevel ) )
            return;
        va_list args;
        va_start( args, strFormat );
        vsprintf_s( g_strBuf, strFormat, args );

        BroadcastMessage( 0, g_strBuf );
    }


    void RecordMessage( StringList& DestStringList, const CHAR* strMessage )
    {
        size_t dwLength = strlen( strMessage );
        CHAR* strCopy = new CHAR[ dwLength + 1 ];
        strcpy_s( strCopy, dwLength + 1, strMessage );
        DestStringList.push_back( strCopy );
    }


    void ExportLog::LogError( _In_z_ _Printf_format_string_ const CHAR* strFormat, ... )
    {
        if( !g_bLoggingEnabled )
            return;

        ++g_dwErrorCount;

        strcpy_s( g_strBuf, "ERROR: " );
        va_list args;
        va_start( args, strFormat );
        vsprintf_s( g_strBuf + 7, ARRAYSIZE( g_strBuf ) - 7, strFormat, args );

        RecordMessage( g_ErrorsList, g_strBuf );
        BroadcastMessage( 2, g_strBuf );
    }

    void ExportLog::LogWarning( _In_z_ _Printf_format_string_ const CHAR* strFormat, ... )
    {
        if( !g_bLoggingEnabled )
            return;

        ++g_dwWarningCount;

        strcpy_s( g_strBuf, "WARNING: " );
        va_list args;
        va_start( args, strFormat );
        vsprintf_s( g_strBuf + 9, ARRAYSIZE( g_strBuf ) - 9, strFormat, args );

        RecordMessage( g_WarningsList, g_strBuf );
        BroadcastMessage( 1, g_strBuf );
    }

    FileListener::FileListener()
        : m_hFileHandle( INVALID_HANDLE_VALUE )
    {
    }

    FileListener::~FileListener()
    {
        StopLogging();
    }

    void FileListener::StartLogging( const CHAR* strFileName )
    {
        assert( m_hFileHandle == INVALID_HANDLE_VALUE );
        m_hFileHandle = CreateFile( strFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr );
    }

    void FileListener::StopLogging()
    {
        if( m_hFileHandle != INVALID_HANDLE_VALUE )
            CloseHandle( m_hFileHandle );
        m_hFileHandle = INVALID_HANDLE_VALUE;
    }

    void FileListener::LogMessage( const CHAR* strMessage )
    {
        if( m_hFileHandle == INVALID_HANDLE_VALUE )
            return;
        DWORD dwByteCount = 0;
        WriteFile( m_hFileHandle, strMessage, static_cast<DWORD>( strlen( strMessage ) ), &dwByteCount, nullptr );
        const CHAR* strNewline = "\r\n";
        WriteFile( m_hFileHandle, strNewline, 2, &dwByteCount, nullptr );
    }
}