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

#ifndef _SUSTRING_H
#define _SUSTRING_H

#include "SuTypes.h"
#include "SuAssert.h"
#include "SuList.h"
#include "SuArray.h"
#include <ctype.h>
#include <string.h>
#ifdef MACOSX
#include <stdio.h>
#endif
#include <stdarg.h>  // for var_arg stuff

//=============================================================================
/// \brief Templated String class
/// \ingroup Common
///
/// Templated string class.
/// Use SuStringA for ASCII string
/// Use SuStringW for Unicode string
/// SuString is automatically defined to be either an ASCII or a Unicode string, 
/// depending on the Unicode compilation settings.
/// 
/// ATTENTION: Do not use SuStringT<char16> directly. For wide character strings,
/// use SuStringW, which is derived from SuStringT<char16> and implements additional
/// casting functions to and from ASCII strings.
//=============================================================================
template < class SuCharType > 
class  SuStringT    
{
   
private :
   
   /// Private data structure to hold string related data
   ///
   /// - We want to pass this string class directly to printf and other similar functions (without casting)
   ///   To do so, this class must contain a pointer to characters as the first and only data member.
   ///
   ///   So we can not store extra data such as length, buffersize etc. To workaround this, when we 
   ///   allocate buffer for the string, we allocate extra bytes to hold the extra info ( StringData ) 
   ///   BEFORE the beginning of string.
   ///
   ///   [-----]["strings........................"]
   ///   ^      ^
   ///   |      |
   ///   |       -------------|
   ///   |                    |
   ///   Ptr to StringData    m_pBuffer
   struct StringData
   {
       uint32  iBufferSize;
       uint32  iSize;

   }; // End of StringData

   /// Returns the address of the StringData associated with this string. The address returned is
   /// the address pointing to the actual character buffer stored in this string (m_pBuffer)     
   StringData*       GetData();

   /// Returns the address of the StringData associated with this string. The address returned is
   /// the address pointing to the actual character buffer stored in this string (m_pBuffer)     
   const StringData* GetData() const;

public :

   /// Constructor
   SuStringT();

   /// Creates a new string as a copy of the input string:
   SuStringT( /*[in]*/ const SuStringT<SuCharType> &src );

   /// Creates a new string as a copy of the input character string:
   SuStringT( /*[in]*/ const SuCharType* srcStr );

   /// Creates a new empty input string of the specified size:
   explicit SuStringT( /*[in]*/ const int32 reserveSize );

   /// Destructor
   ~SuStringT();

   /// Retrieves string length
   ///
   /// \return Length of string ( number of characters )
   uint32 GetLength()         const    { return GetData()->iSize;       };

   /// Retrieves string buffer size
   ///
   /// \return Size of buffer ( number of characters )
   uint32 GetBufferSize()     const    { return GetData()->iBufferSize; };

   /// Retrieves a pointer to the character buffer for this string
   ///
   /// \return Buffer allocated by string
   const SuCharType *GetBuffer() const   { return m_pBuffer;              };

   //==========================================================================
   // Reserve/Clear from memory
   //==========================================================================

   /// Reserves memory for a string with the 'reserveSize' size
   ///
   /// \param reserveSize Number of characters to reserve memory for
   void Reserve( /*[in]*/ const int32 reserveSize );

   /// Clear memory associated with this string:
   void Clear();

   //==========================================================================
   // Comparison
   //==========================================================================
    
   /// Compare two input strings. Returns value < 0 if source string 'src' is less than
   /// this string, returns 0 if the strings are identical, and returns > 0 if 'src' is
   /// greater than this string
   ///
   /// \param src Source string to compare
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return -1 if source string is less than this string
   ///          0 if they are equal
   ///          1 if source string is bigger
   int32 Compare( /*[in]*/ const SuStringT<SuCharType> &src,
                  /*[in]*/ bool bCaseSensitive = true ) const;

   /// Compare two input strings. Returns value < 0 if source string 'src' is less than
   /// this string, returns 0 if the strings are identical, and returns > 0 if 'src' is
   /// greater than this string
   ///
   /// \param src Source string to compare
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return -1 if source string is less than this string
   ///          0 if they are equal
   ///          1 if source string is bigger
   int32 Compare( /*[in]*/ const SuCharType* srcStr,
                  /*[in]*/ bool bCaseSensitive = true ) const;

   /// Search methods. Return the index of the character found
   /// in the string and -1 if the input string or character
   /// are not found in the queried string.
   ///
   /// 'startIndex' specifies the index where to begin search 
   /// from, if that value is -1, then the search starts from the 
   /// default location.
   /// Default search starts at 0th character, except for 
   /// 'FindFromEnd' method.   
   /// For FindFromEnd the default search location is at the end 
   /// of string                
   ///
   /// \param src Source string
   /// \param startIndex Location of comparison starts
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return Location of search result.  -1 if not found
   int32 Find( /*[in]*/ const SuStringT<SuCharType> &src,
               /*[in]*/ const int32 startIndex = 0,
               /*[in]*/ bool bCaseSensitive = true) const;

   /// Search methods. Return the index of the character found
   /// in the string and -1 if the input string or character
   /// are not found in the queried string.
   ///
   /// 'startIndex' specifies the index where to begin search 
   /// from, if that value is -1, then the search starts from the 
   /// default location.
   /// Default search starts at 0th character, except for 
   /// 'FindFromEnd' method.   
   /// For FindFromEnd the default search location is at the end 
   /// of string                
   ///
   /// \param srcStr Source string
   /// \param startIndex Location of comparison starts
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return Location of search result.  -1 if not found
   int32 Find( /*[in]*/ const SuCharType* srcStr,
               /*[in]*/ const int32 startIndex = 0,
               /*[in]*/ bool bCaseSensitive = true) const;

   /// Search methods. Return the index of the character found
   /// in the string and -1 if the input string or character
   /// are not found in the queried string.
   ///
   /// 'startIndex' specifies the index where to begin search 
   /// from, if that value is -1, then the search starts from the 
   /// default location.
   /// Default search starts at 0th character, except for 
   /// 'FindFromEnd' method.   
   /// For FindFromEnd the default search location is at the end 
   /// of string                
   ///
   /// \param Char Source character
   /// \param startIndex Location of comparison starts
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return Location of search result.  -1 if not found
   int32 Find( /*[in]*/ const SuCharType Char,
               /*[in]*/ const int32 startIndex = 0,
               /*[in]*/ bool bCaseSensitive = true) const;

   /// Search methods. Return the index of the character found
   /// in the string and -1 if the input string or character
   /// are not found in the queried string.
   ///
   /// 'startIndex' specifies the index where to begin search 
   /// from, if that value is -1, then the search starts from the 
   /// default location.
   /// Default search starts at 0th character, except for 
   /// 'FindFromEnd' method.   
   /// For FindFromEnd the default search location is at the end 
   /// of string                
   ///
   /// \param src Source character
   /// \param startIndex Location of comparison starts
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return Location of search result.  -1 if not found
   int32 FindOneOf( /*[in]*/ const SuStringT<SuCharType> &src,
                    /*[in]*/ const int32 startIndex = 0,
                    /*[in]*/ bool bCaseSensitive = true) const;

   /// Search methods. Return the index of the character found
   /// in the string and -1 if the input string or character
   /// are not found in the queried string.
   ///
   /// 'startIndex' specifies the index where to begin search 
   /// from, if that value is -1, then the search starts from the 
   /// default location.
   /// Default search starts at 0th character, except for 
   /// 'FindFromEnd' method.   
   /// For FindFromEnd the default search location is at the end 
   /// of string                
   ///
   /// \param src Source character
   /// \param startIndex Location of comparison starts
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return Location of search result.  -1 if not found
   int32 FindOneOf( /*[in]*/ const SuCharType* srcStr,
                    /*[in]*/ const int32 startIndex = 0,
                    /*[in]*/ bool bCaseSensitive = true) const;

   /// Search methods. Return the index of the character found
   /// in the string and -1 if the input string or character
   /// are not found in the queried string.
   ///
   /// 'startIndex' specifies the index where to begin search 
   /// from, if that value is -1, then the search starts from the 
   /// default location.
   /// Default search starts at 0th character, except for 
   /// 'FindFromEnd' method.   
   /// For FindFromEnd the default search location is at the end 
   /// of string                
   ///
   /// \param src Source character
   /// \param startIndex Location of comparison starts
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return Location of search result.  -1 if not found
   int32 FindFromEnd( /*[in]*/ const SuStringT<SuCharType> &src,
                      /*[in]*/ const int32 startIndex = -1,
                      /*[in]*/ bool bCaseSensitive = true) const;

   /// Search methods. Return the index of the character found
   /// in the string and -1 if the input string or character
   /// are not found in the queried string.
   ///
   /// 'startIndex' specifies the index where to begin search 
   /// from, if that value is -1, then the search starts from the 
   /// default location.
   /// Default search starts at 0th character, except for 
   /// 'FindFromEnd' method.   
   /// For FindFromEnd the default search location is at the end 
   /// of string                
   ///
   /// \param src Source character
   /// \param startIndex Location of comparison starts
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return Location of search result.  -1 if not found
   int32 FindFromEnd( /*[in]*/ const SuCharType*  srcStr,
                      /*[in]*/ const int32 startIndex = -1,
                      /*[in]*/ bool bCaseSensitive = true) const;

   /// Search methods. Return the index of the character found
   /// in the string and -1 if the input string or character
   /// are not found in the queried string.
   ///
   /// 'startIndex' specifies the index where to begin search 
   /// from, if that value is -1, then the search starts from the 
   /// default location.
   /// Default search starts at 0th character, except for 
   /// 'FindFromEnd' method.   
   /// For FindFromEnd the default search location is at the end 
   /// of string                
   ///
   /// \param src Source character
   /// \param startIndex Location of comparison starts
   /// \param bCaseSensitive If true, comparison is case sensitive
   /// \return Location of search result.  -1 if not found
   int32 FindFromEnd( /*[in]*/ const SuCharType Char,
                      /*[in]*/ const int32 startIndex = -1,
                      /*[in]*/ bool bCaseSensitive = true) const;

   /// Search for one of the characters found in the string 'src' 
   /// in this string instance.
   /// Returns the index of the first found occurence of the input
   /// string or -1 if none were foun.
   /// 
   /// - Eg. If src string is "abcd", then it returns the found location
   ///       as soon as one of 'a' or 'b' or 'c' or 'd' is found.
   int32 FindOneOfFromEnd( /*[in]*/ const SuStringT<SuCharType> &src,
                           /*[in]*/ const int32 startIndex = -1,
                           /*[in]*/ bool bCaseSensitive = true) const;

   /// Search for one of the characters found in the string 'src' 
   /// in this string instance.
   /// Returns the index of the first found occurence of the input
   /// string or -1 if none were foun.
   /// 
   /// - Eg. If src string is "abcd", then it returns the found location
   ///       as soon as one of 'a' or 'b' or 'c' or 'd' is found.
   int32 FindOneOfFromEnd( /*[in]*/ const SuCharType* srcStr,
                           /*[in]*/ const int32 startIndex = -1,
                           /*[in]*/ bool bCaseSensitive = true) const;

   /// Search for whole word in this string. Returns -1 if the
   ///   word is not found. If the word is found, returns the
   ///   index of first found character of that word.
   /// 
   /// - Eg. If srcSr is "abc", then it will search for 
   ///       a word "abc" ( separated by space or special characters )
   ///       So "abcd" or "zabc" will not be found by this function.
   int32 FindWholeWord( /*[in]*/ const SuCharType* srcStr,
                        /*[in]*/ const int32 startIndex = 0,
                        /*[in]*/ bool bCaseSensitive = true) const;

   /// Split string into substrings that are separated by a separator string
   /// 
   /// \param rSep The separator to use for splitting
   /// \param bCaseSensitive Whether to use case when matching separator or not.
   /// \return A list of strings that are between the separator. If the separator
   ///         doesn't appear in the string, the list's only entry will be the
   ///         complete source string.
   SuList< SuStringT<SuCharType> > Split ( const SuStringT<SuCharType>& rSep,
                                           bool bCaseSensitive = true ) const;

   /// RemoveBefore will remove all characters before nIndex
   void RemoveBefore( /*[in]*/ const uint32 nIndex );

   /// RemoveAfter will remove all characters after nIndex
   void RemoveAfter( /*[in]*/  const uint32 nIndex );

   /// Insertion of String to arbitary location, specified by 
   ///   'destIndex'
   ///
   /// - srcStartIndex and numChars specifies sub string inside src 
   ///   for insertion. destIndex specifies where to insert in the target 
   ///   string. If numChars = 0, then srcStrIndex to the end of string
   ///   will be used for insertion.
   ///
   ///   Eg - If destination string is "blahblah" and src is "hello world" 
   ///        and destIndex = 1, srcStartIndex = 2 and numChars = 3, then 
   ///        "llo" is inserted into "b lahblah"
   ///                                 ^
   ///                                 |------ here
   void Insert( /*[in]*/ const int32 destIndex,
                /*[in]*/ const SuStringT<SuCharType> &src,
                /*[in]*/ const uint32 srcStartIndex = 0,
                /*[in]*/ const uint32 numChars = 0 );

   /// Insertion of String to arbitary location, specified by 
   ///   'destIndex'
   ///
   /// - srcStartIndex and numChars specifies sub string inside src 
   ///   for insertion. destIndex specifies where to insert in the target 
   ///   string. If numChars = 0, then srcStrIndex to the end of string
   ///   will be used for insertion.
   ///
   ///   Eg - If destination string is "blahblah" and src is "hello world" 
   ///        and destIndex = 1, srcStartIndex = 2 and numChars = 3, then 
   ///        "llo" is inserted into "b lahblah"
   ///                                 ^
   ///                                 |------ here
   void Insert( /*[in]*/ const int32 destIndex,
                /*[in]*/ const SuCharType* srcStr,
                /*[in]*/ const uint32 srcStartIndex = 0,
                /*[in]*/ const uint32 numChars = 0 );

   /// Insertion of String to arbitary location, specified by 
   ///   'destIndex'
   //
   /// - srcStartIndex and numChars specifies sub string inside src 
   ///   for insertion. destIndex specifies where to insert in the target 
   ///   string. If numChars = 0, then srcStrIndex to the end of string
   ///   will be used for insertion.
   //
   ///   Eg - If destination string is "blahblah" and src is "hello world" 
   ///        and destIndex = 1, srcStartIndex = 2 and numChars = 3, then 
   ///        "llo" is inserted into "b lahblah"
   ///                                 ^
   ///                                 |------ here
   void Insert( /*[in]*/ const int32 destIndex,
                /*[in]*/ const SuCharType Char );
                
   /// Copy from source string
   ///
   /// - srcStrtIndex and numChars specifies substring from src string
   void Copy( /*[in]*/ const SuStringT<SuCharType> &src,
              /*[in]*/ const uint32 srcStartIndex = 0,
              /*[in]*/ const uint32 numChars = 0 );

   /// Copy from source string
   ///
   /// - srcStrtIndex and numChars specifies substring from src string
   void Copy( /*[in]*/ const SuCharType* srcStr,
              /*[in]*/ const uint32 srcStartIndex = 0,
              /*[in]*/ const uint32 numChars = 0 );

   /// Append String
   void Append( /*[in]*/ const SuStringT<SuCharType> &src,
                /*[in]*/ const uint32 srcStartIndex = 0,
                /*[in]*/ const uint32 numChars = 0 );   

   /// Append String
   void Append( /*[in]*/ const SuCharType* srcStr,
                /*[in]*/ const uint32 srcStartIndex = 0,
                /*[in]*/ const uint32 numChars = 0 );   

   /// Append String
   void Append( /*[in]*/ const SuCharType srcChar );

   /// Prepend String
   void Prepend( /*[in]*/ const SuStringT<SuCharType> &src,
                 /*[in]*/ const uint32 srcStartIndex = 0,
                 /*[in]*/ const uint32 numChars = 0 );   

   /// Prepend String
   void Prepend( /*[in]*/ const SuCharType* srcStr,
                 /*[in]*/ const uint32 srcStartIndex = 0,
                 /*[in]*/ const uint32 numChars = 0 );   

   /// Prepend String
   void Prepend( /*[in]*/ const SuCharType srcChar );

   /// Replace
   void Replace( /*[in]*/ const SuCharType *origStr,
                 /*[in]*/ const SuCharType *newStr,
                 /*[in]*/ const int32 beginScanIndex = 0,
                 /*[in]*/ const int32 endScanIndex = 0,
                 /*[in]*/ bool bCaseSensitive = true,
                 /*[in]*/ bool bWholeWord = false );

   /// Convert string cases to upper
   void ToUpper();

   /// Convert string cases to upper
   void ToLower();

   /// Performs numerical conversion to int32.  Fails if string is padded with white spaces.
   /// \param pInt Optional int32 pointer that gets set to the converted integer value.
   /// \param iBase Optional number base to use (defaults to 10)
   /// \return True if conversion succeeds, false if it fails.
   bool AsInt ( int32 *pInt = NULL, int32 iBase = 10 ) const;

   /// Performs numerical conversion to int32.  Fails if string is padded with white spaces.
   /// \sa AsInt
   bool AsUInt( uint32 *pInt = NULL, int32 iBase = 10 ) const;

   /// Perform numerical conversion to float32.  Fails if string is padded with white spaces.
   /// \param pFloat Optional float32 pointer that gets set to the converted floating point value.
   /// \return True if conversion succeeds, false if it fails.
   bool AsFloat ( float32 *pFloat = NULL ) const;

   /// C Style Formatting
   ///
   /// - This is similar to C function sprintf.
   ///   
   ///   Eg. myStr.Format("my number %d", 10) will produce
   ///       "my number 10".
   ///
   ///   Eg. If mystr is "hi "
   ///       and myStr.AppendFormat("my number %d", 10) will produce
   ///       "hi my number is 10".
   void Format( /*[in]*/ const SuCharType* szFormat, /*[in]*/ ... );

   /// C Style Formatting
   ///
   /// - This is similar to C function sprintf.
   ///   
   ///   Eg. myStr.Format("my number %d", 10) will produce
   ///       "my number 10".
   ///
   ///   Eg. If mystr is "hi "
   ///       and myStr.AppendFormat("my number %d", 10) will produce
   ///       "hi my number is 10".
   void AppendFormat( /*[in]*/ const SuCharType* szFormat, /*[in]*/ ... );

   //==========================================================================
   /// \name Operators
   //@{
   SuCharType& operator[] ( /*[in]*/ const int32 nIndex );
   SuCharType& operator[] ( /*[in]*/ const uint32 nIndex );

   const SuCharType& operator[] ( /*[in]*/ const int32 nIndex ) const;
   const SuCharType& operator[] ( /*[in]*/ const uint32 nIndex ) const;

   /// Assignment operators
   void operator = ( /*[in]*/ const SuStringT<SuCharType> &src );
   void operator = ( /*[in]*/ const SuCharType* srcStr );

   /// Comparison operators
   bool operator == ( /*[in]*/ const SuStringT<SuCharType> &src ) const;
   bool operator == ( /*[in]*/ const SuCharType* srcStr ) const;

   bool operator != ( /*[in]*/ const SuStringT<SuCharType> &src ) const;
   bool operator != ( /*[in]*/ const SuCharType* srcStr ) const;

   bool operator < ( /*[in]*/ const SuStringT<SuCharType> &src ) const;
   bool operator < ( /*[in]*/ const SuCharType* srcStr ) const;

   bool operator <= ( /*[in]*/ const SuStringT<SuCharType> &src ) const;
   bool operator <= ( /*[in]*/ const SuCharType* srcStr ) const;

   bool operator > ( /*[in]*/ const SuStringT<SuCharType> &src ) const;
   bool operator > ( /*[in]*/ const SuCharType* srcStr ) const;

   bool operator >= ( /*[in]*/ const SuStringT<SuCharType> &src ) const;
   bool operator >= ( /*[in]*/ const SuCharType* srcStr ) const;

   /// Quick test for empty string
   inline bool IsEmpty() const { return m_pBuffer[0] == '\0'; };

   /// Additive operator
   /// Add srcStr to this string and returns result
   SuStringT<SuCharType> operator + ( /*[in]*/ const SuStringT<SuCharType> &src ) const;
   SuStringT<SuCharType> operator + ( /*[in]*/ const SuCharType* srcStr ) const;
   SuStringT<SuCharType> operator + ( /*[in]*/ const SuCharType srcChar ) const;

   /// Appending operator
   /// Add srcStr to this string
   void operator += ( /*[in]*/ const SuStringT<SuCharType> &src );
   void operator += ( /*[in]*/ const SuCharType* srcStr );
   void operator += ( /*[in]*/ const SuCharType srcChar );

   /// Implicit Cast to const RM_TCHAR*
   ///
   /// - So this class can be passed to any function that accepts,
   ///   const char/wchar *
   operator const SuCharType*() const
   {
       return m_pBuffer;
   }; /// End of operator SuCharType*
   //@{
   //==========================================================================

private :
    SuCharType *m_pBuffer; //< Buffer

    /// Allocate Buffer
    void AllocBuffer( /*[in]*/ const int32 iNewSize, 
                      /*[in]*/ const bool bPreserve );

    /// Deallocate Buffer
    void DeallocBuffer();

    /// Copy Buffer
    void CopyBuffer( /*[in]*/ const SuCharType* srcStr,
                     /*[in]*/ const uint32 iSize );

    /// Append Buffer
    void AppendBuffer( /*[in]*/ const SuCharType* srcStr,
                       /*[in]*/ const int32 iSize );

    /// Prepend Buffer
    void PrependBuffer( /*[in]*/ const SuCharType* srcStr,
                        /*[in]*/ const int32 iSize );

    /// Insert Buffer
    void InsertBuffer( /*[in]*/ const int32 destIndex,
                       /*[in]*/ const SuCharType* srcStr,
                       /*[in]*/ const int32 iSize );

    /// Move Buffer
    void MoveBuffer( /*[in]*/ const uint32 srcIndex,
                     /*[in]*/ const uint32 destIndex,
                     /*[in]*/ const uint32 iSize );                     

    /// Get string size ( serahc for NULL character )
    int32 GetStrSize( /*[in]*/ const SuCharType* srcStr ) const;

    /// Returns true, if given ch is not a valid character for a string
    bool IsDelimeter( /*[in]*/ SuCharType ch ) const;
}; // End of SuStringT

/// ASCII string type
typedef SuStringT<char8>  SuStringA;
typedef SuStringT<char16>  SuStringW;

/// Sushi string to Lua support
#define tolua_pushsushistring(x,y) tolua_pushstring(x, (const char*)(y))
#define tolua_issushistring	tolua_isstring
#define tolua_issushistringarray tolua_isstringarray
#define tolua_pushfieldsushistring(L,lo,idx,s) tolua_pushfieldstring(L, lo, idx, (const char*)(s))

#define tolua_tosushistring tolua_tostring
#define tolua_tofieldsushistring tolua_tofieldstring

//===========================================================================
// External Operators
//===========================================================================
template <class SuCharType> 
SuStringT<SuCharType> operator + ( /*[in]*/ const SuCharType* srcStr1,
                                  /*[in]*/ const SuStringT<SuCharType>& srcStr2 );

template< class SuCharType>
bool operator == ( const SuCharType* srcStr1, const SuStringT<SuCharType>& srcStr2 );

template< class SuCharType>
bool operator != ( const SuCharType* srcStr1, const SuStringT<SuCharType>& srcStr2 );

//===========================================================================
// Inlined implementation
// 
// - Actual Implemention of SuStringT is defined in SuStringT.inl ( inline )
//===========================================================================
#include "SuString.inl"

//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//
// Helper Functions
//
// Note : For all function, if numChar == -1, then operation continues until either src or dest terminates
//        string with '\0' character.
//
// <String Function Name>A - String function for ASCII string
// <String Function Name>A - String function for Unicode string
//
//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//

//=============================================================
// Conversion Functions
// 
// - pDest must have enough memory allocated. 
//   ( at least same length of pSrc )
//=============================================================
void SuStrWtoA( char8  *pDest, const char16 *pSrc, int numChar = -1 );
void SuStrAtoW( char16 *pDest, const char8  *pSrc, int numChar = -1 );
void SuStrAtoA( char8  *pDest, const char8  *pSrc, int numChar = -1 );
void SuStrWtoW( char16 *pDest, const char16 *pSrc, int numChar = -1 );

//=============================================================
// Conversion with allocation
//
// - Allocate string and copy the contents from src then return 
//   that string.  
//   User of this function is responsible for calling SuStrFree
//   after done using returned string
//=============================================================
char8*  SuStrAllocWtoA( const char16 *pSrc );
char8*  SuStrAllocAtoA( const char8 *pSrc );

char16* SuStrAllocAtoW( const char8 *pSrc );
char16* SuStrAllocWtoW( const char16 *pSrc );


//=============================================================
// Free string
//
// - Free pointer to characters that is returned by SuStrAllocXXXX
//   functions
//=============================================================
void SuStrFreeA( char8 *pStr );
void SuStrFreeW( char16 *pStr );

//=============================================================
// Duplicate
//
// - Same as strdup or wcsdup ( for unicode ).  User of this function
//   must call free to destroy the returned pointer.
//=============================================================
char8*  SuStrDupA( const char8 *pStr );
char16* SuStrDupW( const char16 *pStr );

//=============================================================
// String Length
//=============================================================
int SuStrLenA( const char8 *pSrc );
int SuStrLenW( const char16 *pSrc );

//=============================================================
// Find sub string ( or character )inside source string
// 
// - Returns NULL, if sub string was not found
//=============================================================
char8*  SuStrStrA( const char8 *pSrc, const char8 *pFindStr );
char16* SuStrStrW( const char16 *pSrc, const char16 *pFindStr );
char8*  SuStrChrA( const char8 *pSrc, char8 ch );
char16* SuStrChrW( const char16 *pSrc, char16 ch );
char8*  SuStrRChrA( const char8 *pSrc, char8 ch );   // Find from end
char16* SuStrRChrW( const char16 *pSrc, char16 ch ); // Find from end

//=============================================================
// String Copy
//=============================================================
void SuStrCopyA( char8 *pDest, const char8 *pSrc, int numChar = -1 );
void SuStrCopyW( char16 *pDest, const char16 *pSrc, int numChar = -1 );

//=============================================================
// Concatenation
// 
// - Appends src string to the end of dest string
//=============================================================
void SuStrCatA( char8 *pDest, const char8 *pSrc, int numChar = -1 );
void SuStrCatW( char16 *pDest, const char16 *pSrc, int numChar = -1 );

//=============================================================
// Compare
//
// - Returns 1, if Str1 is greater than Str2
//           0, if they are equal
//          -1, if Str2 is greater than Str1
//=============================================================
int SuStrCmpA( const char8 *pStr1, const char8 *pStr2, int numChar = -1  );
int SuStrCmpW( const char16 *pStr1, const char16 *pStr2, int numChar = -1  );

// Non-case sensitive compare
int SuStrICmpA( const char8 *pStr1, const char8 *pStr2, int numChar = -1  );
int SuStrICmpW( const char16 *pStr1, const char16 *pStr2, int numChar = -1  );

//=============================================================
// Type Conversion
//
// - String to float, int, etc 
//=============================================================
int32   SuStrToLongA( const char8 *pSrc, char8 **pEndPtr, int base );
uint32  SuStrToULongA( const char8 *pSrc, char8 **pEndPtr, int base );
float64 SuStrToDoubleA( const char8 *pSrc, char8 **pEndPtr );

int32   SuStrToLongW( const char16 *pSrc, char16 **pEndPtr, int base );
uint32  SuStrToULongW( const char16 *pSrc, char16 **pEndPtr, int base );
float64 SuStrToDoubleW( const char16 *pSrc, char16 **pEndPtr );

//=============================================================
// Macros to map string function based on flag ( unicode or not )
//
// - In you application use <String Function Name>T whereever applicable.
//   For example, use SuStrCmpT instead of strcmp.   Your source is
//   compilable for both Unicode an ASCII build   
//=============================================================
#ifdef _UNICODE
   typedef SuStringW SuString;
   typedef char16 SuChar;

   //#define SuString       SuStringW

   #define SuStrAtoT      SuStrAtoW
   #define SuStrWtoT      SuStrWtoW

   #define SuStrTtoA      SuStrWtoA
   #define SuStrTtoW      SuStrWtoW

   #define SuStrAllocAtoT SuStrAllocAtoW
   #define SuStrAllocWtoT SuStrAllocWtoW

   #define SuStrAllocTtoA SuStrAllocWtoA
   #define SuStrAllocTtoW SuStrAllocWtoW

   #define SuStrCopyT     SuStrCopyW
   #define SuStrCmpT      SuStrCmpW

   #define SuStrCatT      SuStrCatW

   #define SuStrStrT      SuStrStrW
   #define SuStrChrT      SuStrChrW
   #define SuStrRChrT     SuStrRChrW

   #define SuStrICmpT     SuStrICmpW

   #define SuStrToLongT   SuStrToLongW
   #define SuStrToULongT  SuStrToULongW
   #define SuStrToDoubleT SuStrToDoubleW

   #define SuStrDupT      SuStrDupW

   #define SuStrFreeT     SuStrFreeW

   #define SuStrLenT      SuStrLenW

   #define SuStrPrintf    swprintf
   #define SuStrNPrintf   _snwprintf

   #define SuStrScanf     swscanf

   #define SuStrTokT      wcstok 

   #define SuStrAtoF      _wtof
   #define SuStrAtoI      _wtoi
   #define SuStrAtoL      _wtol

#else // For non unicode build
   typedef SuStringA SuString;
   typedef char8 SuChar;

   //#define SuString       SuStringA

   #define SuStrAtoT      SuStrAtoA
   #define SuStrWtoT      SuStrWtoA

   #define SuStrTtoA      SuStrAtoA
   #define SuStrTtoW      SuStrAtoW

   #define SuStrAllocAtoT SuStrAllocAtoA
   #define SuStrAllocWtoT SuStrAllocWtoA

   #define SuStrAllocTtoA SuStrAllocAtoA
   #define SuStrAllocTtoW SuStrAllocAtoW

   #define SuStrCopyT     strcpy
   #define SuStrCmpT      strcmp

   #define SuStrCatT      strcat

   #define SuStrStrT      SuStrStrA
   #define SuStrChrT      SuStrChrA
   #define SuStrRChrT     SuStrRChrA

   #define SuStrICmpT     SuStrICmpA

   #define SuStrToLongT   SuStrToLongA
   #define SuStrToULongT  SuStrToULongA
   #define SuStrToDoubleT SuStrToDoubleA

   #define SuStrFreeT     SuStrFreeA

   #define SuStrLenT      strlen

   #define SuStrDupT      SuStrDupA

   #define SuStrPrintf    sprintf
#ifdef __GNUC__
   #define SuStrNPrintf   snprintf
#else
   #define SuStrNPrintf   _snprintf
#endif

   #define SuStrScanf     sscanf

   #define SuStrTokT      strtok 

   #define SuStrAtoF      atof
   #define SuStrAtoI      atoi
   #define SuStrAtoL      atol

#endif

// define string list
typedef SuList<SuString> SuStringList;
typedef SuArray<SuString> SuStringArray;


#endif
