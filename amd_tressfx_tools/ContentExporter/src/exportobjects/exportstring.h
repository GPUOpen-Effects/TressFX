//-------------------------------------------------------------------------------------
// ExportString.h
//
// A pooled string class that makes string manipulation easier within the export code.
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
#pragma once

// Change EXPORTSTRING_COMPARE to strcmp to do case-sensitive string pooling.
#define EXPORTSTRING_COMPARE _stricmp
// EXPORTSTRING_HASHSIZE must be a power of 2, so just change the shift value.
#define EXPORTSTRING_HASHSIZE (1 << 4)

namespace ATG
{

class ExportMutableString
{
public:
    ExportMutableString()
        : m_strValue( nullptr )
    { }
    ExportMutableString( const CHAR* strCopy )
    {
        SetAsCopy( strCopy );
    }
    ExportMutableString( const ExportMutableString& strCopy )
    {
        SetAsCopy( strCopy );
    }
    ~ExportMutableString()
    {
    }
    operator const CHAR* () const { return m_strValue.get(); }
    ExportMutableString& operator=( const CHAR* strRHS )
    {
        SetAsCopy( strRHS );
        return *this;
    }
    ExportMutableString& operator=( const ExportMutableString& strRHS )
    {
        SetAsCopy( strRHS );
        return *this;
    }
protected:
    void SetAsCopy( const CHAR* strCopy )
    {
        if( !strCopy )
        {
            m_strValue.reset();
            return;
        }
        size_t dwSize = strlen( strCopy ) + 1;
        m_strValue.reset( new CHAR[dwSize] );
        strcpy_s( m_strValue.get(), dwSize, strCopy );
    }
    std::unique_ptr<CHAR[]> m_strValue;
};

class ExportString
{
public:
    ExportString() : m_strString( nullptr ) {}
    ExportString( const CHAR* strString ) { m_strString = AddString( strString ); }
    ExportString( const ExportString& other ) : m_strString( other.m_strString ) {}

    ExportString& operator= ( const ExportString& RHS ) { m_strString = RHS.m_strString; return *this; }
    ExportString& operator= ( const CHAR* strRHS ) { m_strString = AddString( strRHS ); return *this; }

    bool operator== ( const ExportString& RHS ) const { return m_strString == RHS.m_strString; }
    inline bool operator== ( const CHAR* strRHS ) const;

    operator const CHAR* () const { return m_strString; }
    inline const CHAR* SafeString() const;
    inline static BYTE HashString( const CHAR* strString );
protected:
    inline static const CHAR* AddString( const CHAR* strString );
protected:
    const CHAR* m_strString;
};

const CHAR* ExportString::AddString( const CHAR* strString )
{
    if( !strString )
        return nullptr;
    typedef std::list< const CHAR* > StringList;
    static StringList s_StringLists[ EXPORTSTRING_HASHSIZE ];

    BYTE uBucketIndex = HashString( strString ) & ( EXPORTSTRING_HASHSIZE - 1 );
    StringList& CurrentList = s_StringLists[ uBucketIndex ];

    StringList::iterator iter = CurrentList.begin();
    StringList::iterator end = CurrentList.end();

    while( iter != end )
    {
        const CHAR* strTest = *iter;
        if( EXPORTSTRING_COMPARE( strTest, strString ) == 0 )
            return strTest;
        ++iter;
    }

    size_t dwSize = strlen( strString ) + 1;
    CHAR* strCopy = new CHAR[ dwSize ];
    strcpy_s( strCopy, dwSize, strString );
    CurrentList.push_back( strCopy );
    return strCopy;
}

bool ExportString::operator== ( const CHAR* strRHS ) const
{
    if( !strRHS )
    {
        if( !m_strString )
            return true;
        return false;
    }
    else if( !m_strString )
        return false;

    if( m_strString == strRHS )
        return true;

    return EXPORTSTRING_COMPARE( m_strString, strRHS ) == 0;
}

const CHAR* ExportString::SafeString() const
{
    if( !m_strString )
        return "";
    return m_strString;
}

BYTE ExportString::HashString( const CHAR* strString )
{
    BYTE sum = 0;
    auto p = reinterpret_cast<const BYTE*>( strString );
    if( *p ) sum += (*p++ & 0x1F); else return sum;
    if( *p ) sum += (*p++ & 0x1F); else return sum;
    if( *p ) sum += (*p++ & 0x1F); else return sum;
    if( *p ) sum += (*p++ & 0x1F); else return sum;
    if( *p ) sum += (*p++ & 0x1F); else return sum;
    if( *p ) sum += (*p++ & 0x1F); else return sum;
    if( *p ) sum += (*p++ & 0x1F); else return sum;
    if( *p ) sum += (*p++ & 0x1F);
    return sum;
}

};
