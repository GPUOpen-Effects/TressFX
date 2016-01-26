//-------------------------------------------------------------------------------------
// ExportXmlParser.h
//
// A simple non-validating XML parser.
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

namespace ATG
{

//-----------------------------------------------------------------------------
// error returns from XMLParse
//-----------------------------------------------------------------------------
#define _ATGFAC 0x61B
#define E_COULD_NOT_OPEN_FILE   MAKE_HRESULT(1, _ATGFAC, 0x0001 )
#define E_INVALID_XML_SYNTAX    MAKE_HRESULT(1, _ATGFAC, 0x0002 )


const UINT XML_MAX_ATTRIBUTES_PER_ELEMENT  =   32;
const UINT XML_MAX_NAME_LENGTH             =   128;
const UINT XML_READ_BUFFER_SIZE            =   2048;
const UINT XML_WRITE_BUFFER_SIZE           =   2048;   

// No tag can be longer than XML_WRITE_BUFFER_SIZE - an error will be returned if 
// it is

//-------------------------------------------------------------------------------------
struct XMLAttribute
{
    WCHAR*  strName;
    UINT    NameLen;
    WCHAR*  strValue;
    UINT    ValueLen;       
};

//-------------------------------------------------------------------------------------
class ISAXCallback
{
friend class XMLParser;
public:
    ISAXCallback() {};
    ~ISAXCallback() {};

    virtual HRESULT  StartDocument() = 0;
    virtual HRESULT  EndDocument() = 0;

    virtual HRESULT  ElementBegin( const WCHAR* strName, UINT NameLen, 
                                   const XMLAttribute *pAttributes, UINT NumAttributes ) = 0;
    virtual HRESULT  ElementContent( const WCHAR *strData, UINT DataLen, bool More ) = 0;
    virtual HRESULT  ElementEnd( const WCHAR *strName, UINT NameLen ) = 0;

    virtual HRESULT  CDATABegin( ) = 0;
    virtual HRESULT  CDATAData( const WCHAR *strCDATA, UINT CDATALen, bool bMore ) = 0;
    virtual HRESULT  CDATAEnd( ) = 0;

    virtual void     Error( HRESULT hError, const CHAR *strMessage ) = 0;

    virtual void     SetParseProgress( DWORD dwProgress ) { }

    const CHAR*      GetFilename() { return m_strFilename; }
    UINT             GetLineNumber() { return m_LineNum; }
    UINT             GetLinePosition() { return m_LinePos; }

private:
    const CHAR *m_strFilename;
    UINT        m_LineNum;
    UINT        m_LinePos;
};


//-------------------------------------------------------------------------------------
class XMLParser
{
public:    
    XMLParser();
    ~XMLParser();
   
    //      Register an interface inheiriting from ISAXCallback
    void            RegisterSAXCallbackInterface( ISAXCallback *pISAXCallback );
    
    //      Get the registered interface
    ISAXCallback*   GetSAXCallbackInterface();    

    //      ParseXMLFile returns one of the following:
    //         E_COULD_NOT_OPEN_FILE - couldn't open the file    
    //         E_INVALID_XML_SYNTAX - bad XML syntax according to this parser
    //         E_NOINTERFACE - RegisterSAXCallbackInterface not called
    //         E_ABORT - callback returned a fail code
    //         S_OK - file parsed and completed   

    HRESULT    ParseXMLFile( const CHAR *strFilename );                              
    
    //      Parses from a buffer- if you pass a WCHAR buffer (and cast it), it will 
    //         correctly detect it and use unicode instead.  Return codes are the
    //         same as for ParseXMLFile

    HRESULT    ParseXMLBuffer( const CHAR* strBuffer, UINT uBufferSize );    

private:      
    HRESULT    MainParseLoop();

    HRESULT    AdvanceCharacter( bool bOkToFail = false ); 
    void       SkipNextAdvance();           

    HRESULT    ConsumeSpace();            
    HRESULT    ConvertEscape();           
    HRESULT    AdvanceElement();           
    HRESULT    AdvanceName();            
    HRESULT    AdvanceAttrVal();           
    HRESULT    AdvanceCDATA();           
    HRESULT    AdvanceComment();          

    void    FillBuffer();
    
    void    Error( HRESULT hRet, const CHAR* strFormat, ... );

    ISAXCallback*   m_pISAXCallback;    

    HANDLE          m_hFile;    
    const CHAR*     m_pInXMLBuffer; 
    UINT            m_uInXMLBufferCharsLeft;
    DWORD           m_dwCharsTotal;
    DWORD           m_dwCharsConsumed;

    BYTE            m_pReadBuf[ XML_READ_BUFFER_SIZE + 2 ]; // room for a trailing nullptr
    WCHAR           m_pWriteBuf[ XML_WRITE_BUFFER_SIZE ];    

    BYTE*           m_pReadPtr;
    WCHAR*          m_pWritePtr;        // write pointer within m_pBuf      

    bool            m_bUnicode;         // true = 16-bits, false = 8-bits
    bool            m_bReverseBytes;    // true = reverse bytes, false = don't reverse
    
    bool            m_bSkipNextAdvance;
    WCHAR           m_Ch;               // Current character being parsed
};

}  // namespace ATG
