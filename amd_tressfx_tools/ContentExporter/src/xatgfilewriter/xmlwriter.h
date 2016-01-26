//-------------------------------------------------------------------------------------
// xmlwriter.h
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
#pragma once

#define XMLWRITER_NAME_STACK_SIZE 255

namespace ATG
{
    class XMLWriter
    {
    public:
        XMLWriter();
        XMLWriter( CHAR* strBuffer, UINT uBufferSize );
        XMLWriter( const CHAR* strFileName );
        ~XMLWriter();

        void Initialize( CHAR* strBuffer, UINT uBufferSize );
        void Initialize( const CHAR* strFileName );
        void Close();

        bool IsValid() const { return m_bValid; }

        void SetIndentCount( UINT uSpaces );
        void EnableNewlines( bool bWriteNewLines ) { m_bWriteNewlines = bWriteNewLines; }

        bool StartElement( const CHAR* strName );
        bool EndElement();
        bool WriteElement( const CHAR* strName, const CHAR* strBody );
        bool WriteElement( const CHAR* strName, INT iBody );
        bool WriteElement( const CHAR* strName, float fBody );
        bool WriteElementFormat( const CHAR* strName, const CHAR* strFormat, ... );

        bool StartCDATA();
        bool EndCDATA();
        bool WriteCDATA( const CHAR* strData, DWORD dwDataLength );

        bool StartComment( bool bInline = false );
        bool EndComment();
        bool WriteComment( const CHAR* strComment, bool bInline = false );

        bool AddAttributeFormat( const CHAR* strName, const CHAR* strFormat, ... );
        bool AddAttribute( const CHAR* strName, const CHAR* strValue );
        bool AddAttribute( const CHAR* strName, const WCHAR* wstrValue );
        bool AddAttribute( const CHAR* strName, INT iValue );
        bool AddAttribute( const CHAR* strName, float fValue );

        bool WriteString( const CHAR* strText );
        bool WriteStringFormat( const CHAR* strFormat, ... );

    private:

        void PushName( const CHAR* strName );
        const CHAR* PopName();

        inline bool EndOpenTag();
        inline bool WriteNewline();
        inline bool WriteIndent();

        inline bool OutputString( const CHAR* strText );
        inline bool OutputStringFast( const CHAR* strText, UINT uLength );
        void FlushBufferToFile();

        HANDLE          m_hFile;
        CHAR*           m_strBuffer;
        CHAR*           m_strBufferStart;
        UINT            m_uBufferSizeRemaining;

        CHAR            m_strNameStack[XMLWRITER_NAME_STACK_SIZE];
        CHAR*           m_strNameStackTop;
        UINT            m_uNameStackSize;
        std::vector<UINT>    m_NameStackPositions;
        UINT            m_uIndentCount;
        CHAR            m_strIndent[9];
        bool            m_bWriteNewlines;

        bool            m_bOpenTagFinished;
        bool            m_bWriteCloseTagIndent;
        bool            m_bInlineComment;

        bool            m_bValid;
    };

    class IXMLSerializable
    {
    public:
        virtual bool Serialize( XMLWriter* pXMLWriter ) = 0;
    };
}