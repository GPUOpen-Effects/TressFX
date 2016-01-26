//-------------------------------------------------------------------------------------
// xmlwriter.cpp
//  
// A simple XML writer.
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
#include "xmlwriter.h"

#define WRITE_BUFFER_SIZE 16384

namespace ATG
{
    //----------------------------------------------------------------------------------
    // Name: XMLWriter
    // Desc: Constructor for the XML writer class.
    //----------------------------------------------------------------------------------
    XMLWriter::XMLWriter()
        : m_strBuffer( nullptr ),
          m_uBufferSizeRemaining( 0 ),
          m_hFile( INVALID_HANDLE_VALUE ),
          m_strNameStackTop( m_strNameStack ),
          m_uIndentCount( 0 ),
          m_bValid( false )
    {
    }


    //----------------------------------------------------------------------------------
    // Name: XMLWriter
    // Desc: Constructor for the XML writer class.
    //----------------------------------------------------------------------------------
    XMLWriter::XMLWriter( const CHAR* strFileName )
    {
        Initialize( strFileName );
    }


    //----------------------------------------------------------------------------------
    // Name: XMLWriter
    // Desc: Constructor for the XML writer class.
    //----------------------------------------------------------------------------------
    XMLWriter::XMLWriter( CHAR* strBuffer, UINT uBufferSize )
    {
        Initialize( strBuffer, uBufferSize );
    }


    //----------------------------------------------------------------------------------
    // Name: ~XMLWriter
    // Desc: Destructor for the XML writer class.
    //----------------------------------------------------------------------------------
    XMLWriter::~XMLWriter()
    {
        Close();
    }


    //----------------------------------------------------------------------------------
    // Name: Initialize
    // Desc: Sets up the XML writer to write to a file.
    //----------------------------------------------------------------------------------
    void XMLWriter::Initialize( const CHAR* strFileName )
    {
        m_strBuffer = new CHAR[WRITE_BUFFER_SIZE];
        m_strBufferStart = m_strBuffer;
        m_uBufferSizeRemaining = WRITE_BUFFER_SIZE;
        m_bOpenTagFinished = true;
        m_bWriteCloseTagIndent = false;
        m_hFile = CreateFile( strFileName, FILE_WRITE_DATA, 0, nullptr, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, nullptr );
        m_strNameStack[0] = '\0';
        m_strNameStackTop = m_strNameStack;
        m_NameStackPositions.clear();
        SetIndentCount( 4 );
        m_bWriteNewlines = true;
        if( m_hFile == INVALID_HANDLE_VALUE )
        {
            m_bValid = false;
        }
        else
        {
            m_bValid = true;
        }
    }


    //----------------------------------------------------------------------------------
    // Name: Initialize
    // Desc: Sets up the XML writer to write to a string buffer.
    //----------------------------------------------------------------------------------
    void XMLWriter::Initialize( CHAR* strBuffer, UINT uBufferSize )
    {
        m_strBuffer = strBuffer;
        m_strBufferStart = m_strBuffer;
        m_uBufferSizeRemaining = uBufferSize;
        m_hFile = INVALID_HANDLE_VALUE;
        m_bOpenTagFinished = true;
        m_bWriteCloseTagIndent = false;
        m_strNameStack[0] = '\0';
        m_strNameStackTop = m_strNameStack;
        m_NameStackPositions.clear();
        SetIndentCount( 0 );
        m_bWriteNewlines = false;
        m_bValid = true;
    }


    //----------------------------------------------------------------------------------
    // Name: Close
    // Desc: Cleans up the output of the XML writing operation.
    //----------------------------------------------------------------------------------
    void XMLWriter::Close()
    {
        if( m_hFile != INVALID_HANDLE_VALUE )
        {
            FlushBufferToFile();
            CloseHandle( m_hFile );
            delete[] m_strBufferStart;
            m_strBufferStart = nullptr;
            m_strBuffer = nullptr;
            m_hFile = INVALID_HANDLE_VALUE;
        }
        if( m_strBuffer )
        {
            m_strBuffer = nullptr;
            m_strBufferStart = nullptr;
            m_uBufferSizeRemaining = 0;
        }
    }


    void XMLWriter::FlushBufferToFile()
    {
        if( m_uBufferSizeRemaining >= WRITE_BUFFER_SIZE || m_hFile == INVALID_HANDLE_VALUE )
            return;
        DWORD dwBytesWritten = 0;
        WriteFile( m_hFile, m_strBufferStart, WRITE_BUFFER_SIZE - m_uBufferSizeRemaining, &dwBytesWritten, nullptr );
        m_uBufferSizeRemaining = WRITE_BUFFER_SIZE;
        m_strBuffer = m_strBufferStart;
    }


    //----------------------------------------------------------------------------------
    // Name: SetIndentCount
    // Desc: Builds a string with the correct amount of indentation spaces.
    //----------------------------------------------------------------------------------
    void XMLWriter::SetIndentCount( UINT uSpaces )
    {
        m_uIndentCount = ( uSpaces > 8 ) ? 8 : uSpaces;
        if( m_uIndentCount > 0 )
            memset( m_strIndent, ' ', m_uIndentCount );
        m_strIndent[ m_uIndentCount ] = '\0';
    }


    //----------------------------------------------------------------------------------
    // Name: StartElement
    // Desc: Writes the beginning of an XML open tag.
    //----------------------------------------------------------------------------------
    bool XMLWriter::StartElement( const CHAR* strName )
    {
        if( !m_bOpenTagFinished )
        {
            if( !EndOpenTag() ) 
                return false;
            if( !WriteNewline() )
                return false;
        }
        bool result = true;
        result &= WriteIndent();
        PushName( strName );
        result &= OutputStringFast( "<", 1 );
        result &= OutputString( strName );
        m_bOpenTagFinished = false;
        m_bWriteCloseTagIndent = false;
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: EndElement
    // Desc: Writes an element close tag corresponding with the most recent open tag.
    //----------------------------------------------------------------------------------
    bool XMLWriter::EndElement()
    {
        const CHAR* strName = PopName();
        if( !strName )
            return false;
        bool result = true;
        if( !m_bOpenTagFinished )
        {
            m_bOpenTagFinished = true;
            result &= OutputStringFast( " />", 3 );
            result &= WriteNewline();
            m_bWriteCloseTagIndent = true;
            return result;
        }
        if( m_bWriteCloseTagIndent )
            result &= WriteIndent();
        result &= OutputStringFast( "</", 2 );
        result &= OutputString( strName );
        result &= OutputStringFast( ">", 1 );
        result &= WriteNewline();
        m_bWriteCloseTagIndent = true;
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: WriteElement
    // Desc: Convenience function to write an XML element with a body and no attributes.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteElement( const CHAR* strName, const CHAR* strBody )
    {
        bool result = true;
        result &= StartElement( strName );
        result &= WriteString( strBody );
        result &= EndElement();
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: WriteElement
    // Desc: Convenience function to write an XML element with a body and no attributes.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteElement( const CHAR* strName, INT iBody )
    {
        bool result = true;
        result &= StartElement( strName );
        CHAR strTemp[32];
        _itoa_s( iBody, strTemp, 10 );
        result &= WriteString( strTemp );
        result &= EndElement();
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: WriteElement
    // Desc: Convenience function to write an XML element with a body and no attributes.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteElement( const CHAR* strName, float fBody )
    {
        bool result = true;
        result &= StartElement( strName );
        CHAR strTemp[32];
        sprintf_s( strTemp, "%f", fBody );
        result &= WriteString( strTemp );
        result &= EndElement();
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: WriteElementFormat
    // Desc: Convenience function to write an XML element with a body and no attributes.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteElementFormat( const CHAR* strName, const CHAR* strFormat, ... )
    {
        bool result = true;
        result &= StartElement( strName );
        CHAR strTemp[512];
        va_list args;
        va_start( args, strFormat );
        vsprintf_s( strTemp, strFormat, args );
        result &= WriteString( strTemp );
        result &= EndElement();
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: StartCDATA
    // Desc: Starts a CDATA block.
    //----------------------------------------------------------------------------------
    bool XMLWriter::StartCDATA()
    {
        bool bResult = true;
        if( !m_bOpenTagFinished )
        {
            if( !EndOpenTag() ) 
                return false;
            if( !WriteNewline() )
                return false;
        }
        bResult &= WriteIndent();
        bResult &= OutputStringFast( "<![CDATA[", 9 );
        return bResult;
    }


    //----------------------------------------------------------------------------------
    // Name: EndCDATA
    // Desc: Ends a CDATA block.
    //----------------------------------------------------------------------------------
    bool XMLWriter::EndCDATA()
    {
        bool bResult = true;
        bResult &= OutputStringFast( "]]>", 3 );
        bResult &= WriteNewline();
        m_bWriteCloseTagIndent = true;
        return bResult;
    }


    //----------------------------------------------------------------------------------
    // Name: WriteCDATA
    // Desc: Writes a CDATA block.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteCDATA( const CHAR* strData, DWORD dwDataLength )
    {
        bool bResult = StartCDATA();
        bResult &= OutputStringFast( strData, dwDataLength );
        bResult &= EndCDATA();
        return bResult;
    }


    //----------------------------------------------------------------------------------
    // Name: StartComment
    // Desc: Writes the beginning of an XML comment tag.
    //----------------------------------------------------------------------------------
    bool XMLWriter::StartComment( bool bInline )
    {
        if( !m_bOpenTagFinished )
        {
            if( !EndOpenTag() ) 
                return false;
            if( !bInline && !WriteNewline() )
                return false;
        }
        bool result = true;
        if( !bInline )
            result &= WriteIndent();
        result &= OutputStringFast( "<!-- ", 5 );
        m_bOpenTagFinished = true;
        m_bWriteCloseTagIndent = false;
        m_bInlineComment = bInline;
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: EndComment
    // Desc: Writes a comment close tag.
    //----------------------------------------------------------------------------------
    bool XMLWriter::EndComment()
    {
        bool result = true;
        result &= OutputStringFast( " -->", 4 );
        if( !m_bInlineComment )
            result &= WriteNewline();
        m_bWriteCloseTagIndent = !m_bInlineComment;
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: WriteComment
    // Desc: Convenience function to write an entire comment.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteComment( const CHAR* strComment, bool bInline )
    {
        bool result = true;
        result &= StartComment( bInline );
        result &= WriteString( strComment );
        result &= EndComment();
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: AddAttributeFormat
    // Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
    //       after calling StartElement(), but before calling WriteString() or
    //       EndElement().
    //----------------------------------------------------------------------------------
    bool XMLWriter::AddAttributeFormat( const CHAR* strName, const CHAR* strFormat, ... )
    {
        if( m_bOpenTagFinished )
            return false;
        bool result = true;
        result &= OutputStringFast( " ", 1 );
        result &= OutputString( strName );
        result &= OutputStringFast( "=\"", 2 );
        CHAR strTemp[256];
        va_list args;
        va_start( args, strFormat );
        vsprintf_s( strTemp, strFormat, args );
        result &= OutputString( strTemp );
        result &= OutputStringFast( "\"", 1 );
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: AddAttribute
    // Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
    //       after calling StartElement(), but before calling WriteString() or
    //       EndElement().
    //----------------------------------------------------------------------------------
    bool XMLWriter::AddAttribute( const CHAR* strName, const CHAR* strValue )
    {
        if( m_bOpenTagFinished )
            return false;
        bool result = true;
        result &= OutputStringFast( " ", 1 );
        result &= OutputString( strName );
        result &= OutputStringFast( "=\"", 2 );
        result &= OutputString( strValue );
        result &= OutputStringFast( "\"", 1 );
        return result;
    }


    //----------------------------------------------------------------------------------
    // Name: AddAttribute
    // Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
    //       after calling StartElement(), but before calling WriteString() or
    //       EndElement().
    //----------------------------------------------------------------------------------
    bool XMLWriter::AddAttribute( const CHAR* strName, const WCHAR* wstrValue )
    {
        CHAR strTemp[256];
        WideCharToMultiByte( CP_ACP, 0, wstrValue, static_cast<INT>( wcslen( wstrValue ) + 1 ), strTemp, 256, nullptr, nullptr );
        return AddAttribute( strName, strTemp );
    }


    //----------------------------------------------------------------------------------
    // Name: AddAttribute
    // Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
    //       after calling StartElement(), but before calling WriteString() or
    //       EndElement().
    //----------------------------------------------------------------------------------
    bool XMLWriter::AddAttribute( const CHAR* strName, INT iValue )
    {
        CHAR strTemp[20];
        _itoa_s( iValue, strTemp, 10 );
        return AddAttribute( strName, strTemp );
    }


    //----------------------------------------------------------------------------------
    // Name: AddAttribute
    // Desc: Adds a key-value attribute pair to an XML open tag.  This must be called
    //       after calling StartElement(), but before calling WriteString() or
    //       EndElement().
    //----------------------------------------------------------------------------------
    bool XMLWriter::AddAttribute( const CHAR* strName, float fValue )
    {
        CHAR strTemp[20];
        sprintf_s( strTemp, "%f", fValue );
        return AddAttribute( strName, strTemp );
    }


    //----------------------------------------------------------------------------------
    // Name: WriteString
    // Desc: Writes a string after an XML open tag.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteString( const CHAR* strText )
    {
        if( !strText )
            strText = "";
        if( !m_bOpenTagFinished )
        {
            if( !EndOpenTag() )
                return false;
        }
        return OutputString( strText );
    }


    //----------------------------------------------------------------------------------
    // Name: WriteStringFormat
    // Desc: Writes a formatted string after an XML open tag.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteStringFormat( const CHAR* strFormat, ... )
    {
        if( !m_bOpenTagFinished )
        {
            if( !EndOpenTag() )
                return false;
        }
        CHAR strTemp[1024];
        va_list args;
        va_start( args, strFormat );
        vsprintf_s( strTemp, strFormat, args );
        return OutputString( strTemp );
    }


    //----------------------------------------------------------------------------------
    // Name: EndOpenTag
    // Desc: Writes the closing angle bracket of an XML open tag, and sets the proper
    //       state.
    //----------------------------------------------------------------------------------
    bool XMLWriter::EndOpenTag()
    {
        assert( !m_bOpenTagFinished );
        OutputStringFast( ">", 1 );
        m_bOpenTagFinished = true;
        return true;
    }


    //----------------------------------------------------------------------------------
    // Name: WriteNewline
    // Desc: Writes a new line, if that option is enabled.  Writes a hard return to
    //       files, and a soft return to buffers.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteNewline()
    {
        if( !m_bWriteNewlines )
            return true;
        if( m_hFile != INVALID_HANDLE_VALUE )
            return OutputStringFast( "\r\n", 2 );
        return OutputStringFast( "\n", 1 );
    }


    //----------------------------------------------------------------------------------
    // Name: WriteIndent
    // Desc: Writes an indentation using spaces if indentation is enabled.
    //----------------------------------------------------------------------------------
    bool XMLWriter::WriteIndent()
    {
        if( m_uIndentCount == 0 )
            return true;
        for( size_t i = 0; i < m_NameStackPositions.size(); i++ )
        {
            if( !OutputStringFast( m_strIndent, m_uIndentCount ) )
                return false;
        }
        return true;
    }


    //----------------------------------------------------------------------------------
    // Name: PushName
    // Desc: Pushes an XML tag name onto the stack.  This is used to write an open tag.
    //----------------------------------------------------------------------------------
    void XMLWriter::PushName( const CHAR* strName )
    {
        size_t uLen = strlen( strName );
        if( ( m_strNameStackTop - m_strNameStack + uLen ) >= XMLWRITER_NAME_STACK_SIZE )
        {
            assert( false );
            return;
        }
        m_NameStackPositions.push_back( static_cast<UINT>( m_strNameStackTop - m_strNameStack ) );
        *m_strNameStackTop = '\0';
        strcat_s( m_strNameStack, strName );
        m_strNameStackTop += uLen;
    }


    //----------------------------------------------------------------------------------
    // Name: PopName
    // Desc: Pops an XML tag name off the stack.  This is used to write a close tag.
    //----------------------------------------------------------------------------------
    const CHAR* XMLWriter::PopName()
    {
        if( m_NameStackPositions.empty() )
            return nullptr;
        UINT uPos = m_NameStackPositions.back();
        m_NameStackPositions.pop_back();
        *m_strNameStackTop = '\0';
        m_strNameStackTop = m_strNameStack + uPos;
        return m_strNameStackTop;
    }


    //----------------------------------------------------------------------------------
    // Name: OutputString
    // Desc: Sends a null-terminated string to the output.
    //----------------------------------------------------------------------------------
    bool XMLWriter::OutputString( const CHAR* strText )
    {
        assert( strText != nullptr );
        return OutputStringFast( strText, static_cast<UINT>( strlen( strText ) ) );
    }


    //----------------------------------------------------------------------------------
    // Name: OutputStringFast
    // Desc: Sends a string with a supplied length to the output.
    //----------------------------------------------------------------------------------
    bool XMLWriter::OutputStringFast( const CHAR* strText, UINT uLength )
    {
        if( m_hFile != INVALID_HANDLE_VALUE )
        {
            while( uLength >= m_uBufferSizeRemaining )
            {
                memcpy( m_strBuffer, strText, m_uBufferSizeRemaining );
                m_strBuffer += m_uBufferSizeRemaining;
                strText += m_uBufferSizeRemaining;
                uLength -= m_uBufferSizeRemaining;
                m_uBufferSizeRemaining = 0;
                FlushBufferToFile();
            }
            memcpy( m_strBuffer, strText, uLength );
            m_uBufferSizeRemaining -= uLength;
            m_strBuffer += uLength;
            return true;
        }
        else if( m_strBuffer )
        {
            if( ( uLength + 1 ) > m_uBufferSizeRemaining )
                return false;
            memcpy( m_strBuffer, strText, uLength + 1 );
            m_uBufferSizeRemaining -= uLength;
            m_strBuffer += uLength;
            return true;
        }
        return false;
    }
}