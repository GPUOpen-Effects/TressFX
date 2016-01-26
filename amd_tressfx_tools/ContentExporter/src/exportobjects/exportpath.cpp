//-------------------------------------------------------------------------------------
// ExportPath.cpp
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
#include "ExportPath.h"

const CHAR g_VolumeSeparator = ':';
const CHAR g_DirectorySeparator = '\\';
const CHAR g_AltDirectorySeparator = '/';

ExportPath::ExportPath()
{
    Initialize( "" );
}

ExportPath::ExportPath( const CHAR* strPath )
{
    Initialize( strPath );
}

ExportPath::ExportPath( const ExportPath& OtherPath )
{
    Initialize( (const CHAR*)OtherPath );
}

ExportPath& ExportPath::operator=( const ExportPath& OtherPath )
{
    Initialize( (const CHAR*)OtherPath );
    return *this;
}

void ExportPath::SetPathAndFileName( const CHAR* strPath )
{
    Initialize( strPath );
}

void ExportPath::SetPathOnly( const CHAR* strPath )
{
    const CHAR* strLastSlash = strrchr( strPath, g_DirectorySeparator );
    if( !strLastSlash )
    {
        strLastSlash = strrchr( strPath, g_AltDirectorySeparator );
    }
    if( !strLastSlash || strLastSlash[1] != '\0' )
    {
        CHAR strModifiedPath[MAX_PATH];
        strcpy_s( strModifiedPath, strPath );
        strcat_s( strModifiedPath, "\\" );
        Initialize( strModifiedPath );
    }
    else
    {
        Initialize( strPath );
    }
}

void ExportPath::Initialize( const CHAR* strPath )
{
    // Copy in our new path if one is provided
    if( strPath )
    {
        strcpy_s( m_strPath, strPath );
    }

    // Convert all slashes to the same slash
    CHAR* strSrc = m_strPath;
    while( *strSrc != '\0' )
    {
        if( *strSrc == g_AltDirectorySeparator )
        {
            *strSrc = g_DirectorySeparator;
        }
        ++strSrc;
    }

    // Look for the file extension
    m_strExtension = strrchr( m_strPath, '.' );

    // Look for the file name
    m_strFileName = strrchr( m_strPath, g_DirectorySeparator );
    if( m_strFileName )
    {
        ++m_strFileName;
    }
    else
    {
        m_strFileName = m_strPath;
    }
}

void ExportPath::ChangeExtension( const CHAR* strExtension )
{
    // nullptr extension means empty string
    if( !strExtension )
    {
        strExtension = "";
    }

    // make sure extension does not include a leading period
    while( *strExtension == '.' && *strExtension != '\0' )
    {
        ++strExtension;
    }

    // make sure we have a valid filename
    assert( m_strFileName != nullptr );

    if( !m_strExtension )
    {
        // append a trailing period on the path
        strcat_s( m_strPath, "." );
    }
    else
    {
        // ensure that the path has a trailing period
        assert( *m_strExtension == '.' );
        m_strExtension[1] = '\0';
    }

    // concatenate extension onto path
    strcat_s( m_strPath, strExtension );

    // re-initialize all of our pointers
    Initialize( nullptr );
}

void ExportPath::ChangeFileName( const CHAR* strFileName )
{
    // Ensure that incoming filename has no separators
    const CHAR* strSep = strchr( strFileName, g_DirectorySeparator );
    assert( strSep == nullptr );
    strSep = strchr( strFileName, g_AltDirectorySeparator );
    assert( strSep == nullptr );

    // save current extension
    CHAR strExtension[MAX_PATH] = "";
    if( m_strExtension )
    {
        strcpy_s( strExtension, m_strExtension );
    }

    // trim off current file name
    assert( m_strFileName != nullptr );
    *m_strFileName = '\0';

    // concatenate filename onto path
    strcat_s( m_strPath, strFileName );

    // concatenate original extension
    strcat_s( m_strPath, strExtension );

    // re-initialize all of our pointers
    Initialize( nullptr );
}

void ExportPath::ChangeFileName( const ExportPath& OtherPath )
{
    ChangeFileName( (const CHAR*)OtherPath.GetFileNameWithoutExtension() );
}

void ExportPath::ChangeFileNameWithExtension( const CHAR* strFileName )
{
    // Ensure that incoming filename has no separators
    const CHAR* strSep = strchr( strFileName, g_DirectorySeparator );
    assert( strSep == nullptr );
    strSep = strchr( strFileName, g_AltDirectorySeparator );
    assert( strSep == nullptr );

    // trim off current file name
    assert( m_strFileName != nullptr );
    *m_strFileName = '\0';

    // concatenate filename onto path
    strcat_s( m_strPath, strFileName );

    // re-initialize all of our pointers
    Initialize( nullptr );
}

void ExportPath::ChangeFileNameWithExtension( const ExportPath& OtherPath )
{
    ChangeFileNameWithExtension( (const CHAR*)OtherPath.GetFileName() );
}

ExportPath ExportPath::GetFileName() const
{
    return ExportPath( m_strFileName );
}

bool ExportPath::HasFileName() const
{
    // If there is no filename, the filename pointer will point to the end of the string
    return ( *m_strFileName != '\0' );
}

ExportPath ExportPath::GetFileNameWithoutExtension() const
{
    // if we have no extension, just return the whole filename
    if( !m_strExtension )
    {
        return GetFileName();
    }

    CHAR strFileName[MAX_PATH];

    // copy only the chars between the filename and the extension period
    size_t dwCount = ( m_strExtension - m_strFileName );
    assert( dwCount < MAX_PATH );
    strncpy_s( strFileName, m_strFileName, dwCount );
    strFileName[dwCount] = '\0';

    return ExportPath( strFileName );
}

ExportPath ExportPath::GetDirectory() const
{
    CHAR strDirectory[MAX_PATH];

    // copy only the chars between the path and the start of the filename
    size_t dwCount = ( m_strFileName - m_strPath );
    assert( dwCount < MAX_PATH );
    strncpy_s( strDirectory, m_strPath, dwCount );
    strDirectory[dwCount] = '\0';

    return ExportPath( strDirectory );
}

const CHAR* ExportPath::GetExtension() const
{
    if( !m_strExtension )
        return nullptr;

    // return the extension without the period
    return m_strExtension + 1;
}

bool ExportPath::IsAbsolutePath() const
{
    // look for volume separator in the path
    const CHAR* strVolumeSep = strchr( m_strPath, g_VolumeSeparator );
    if( strVolumeSep )
    {
        return true;
    }

    // look for two leading slashes
    return ( m_strPath[0] == g_DirectorySeparator && m_strPath[1] == g_DirectorySeparator );
}

void ExportPath::Append( const ExportPath& OtherPath )
{
    // the other path can't be an absolute path
    assert( !OtherPath.IsAbsolutePath() );

    // if we don't have a filename, just concatenate the strings
    if( !HasFileName() )
    {
        strcat_s( m_strPath, (const CHAR*)OtherPath );

        Initialize( nullptr );
        return;
    }

    // we have a filename, and we're appending a path.  assume a directory append.
    // save the existing filename
    CHAR strFileName[MAX_PATH];
    strcpy_s( strFileName, m_strFileName );

    // trim off the filename
    *m_strFileName = '\0';

    // append the other path
    strcat_s( m_strPath, (const CHAR*)OtherPath );

    // if the other path has a filename, we'll use that
    if( OtherPath.HasFileName() )
    {
        Initialize( nullptr );
        return;
    }

    // otherwise, use our filename
    strcat_s( m_strPath, strFileName );

    // re-initialize all of our pointers
    Initialize( nullptr );
}

void ExportPath::AppendToFileName( const CHAR* strText )
{
    if( !strText )
        return;

    if( !HasFileName() )
    {
        strcat_s( m_strPath, strText );
        Initialize( nullptr );
        return;
    }

    // save current extension and then trim it off
    CHAR strExtension[MAX_PATH] = "";
    if( m_strExtension )
    {
        strcpy_s( strExtension, m_strExtension );
        *m_strExtension = '\0';
    }

    // concatenate filename onto path
    strcat_s( m_strPath, strText );

    // concatenate original extension
    strcat_s( m_strPath, strExtension );

    // re-initialize all of our pointers
    Initialize( nullptr );
}

void ExportPath::TrimOffFileName()
{
    // trim off the filename
    *m_strFileName = '\0';

    // re-initialize all of our pointers
    Initialize( nullptr );
}

ExportPath ExportPath::GetTempPath()
{
    CHAR strPath[MAX_PATH];
    ::GetTempPathA( MAX_PATH, strPath );
    return ExportPath( strPath );
}

ExportPath ExportPath::GetCurrentPath()
{
    CHAR strPath[MAX_PATH];
    ::GetCurrentDirectoryA( MAX_PATH, strPath );
    strcat_s( strPath, "\\" );
    return ExportPath( strPath );
}
