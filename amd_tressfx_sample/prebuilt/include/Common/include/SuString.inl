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

#if (_MSC_VER >= 1300) || defined ( MACOSX )
#include <string> // For _vscprintf/vasprintf
#endif

#if defined (__SYMBIAN32__) || defined (_LINUX)
#include <stdio.h> // For vsprintf/BUFSIZ
#endif

//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//
// Templated String Class ( implementation )
//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//

//=============================================================
// Constructor/Destructor
//=============================================================
template<class SuCharType>
SuStringT<SuCharType>::
SuStringT() : 
    m_pBuffer(NULL)
{
    AllocBuffer(0,false);
} // End of Constructor


//-------------------------------------------------------------
template<class SuCharType>
SuStringT<SuCharType>::
SuStringT( /*[in]*/ const SuStringT<SuCharType> &src ) :
    m_pBuffer(NULL)
{
    Reserve(src.GetLength()+1);
    CopyBuffer(src.GetBuffer(),src.GetLength());
} // End of Constructor


//-------------------------------------------------------------
template<class SuCharType>
SuStringT<SuCharType>::
SuStringT( /*[in]*/ const SuCharType* srcStr ) :
    m_pBuffer(NULL)
{
    if (srcStr==NULL)
    {
        AllocBuffer(0,false);
        return;
    } // End if

    int32 strSize = GetStrSize(srcStr);
    Reserve(strSize+1);
    CopyBuffer(srcStr,strSize);
} // End of Constructor


//-------------------------------------------------------------
template<class SuCharType>
SuStringT<SuCharType>::
SuStringT( /*[in]*/ const int32 ReserveSize ) : 
    m_pBuffer(NULL)
{
    if (ReserveSize>0)
    {
        Reserve(ReserveSize);
        m_pBuffer[0] = (SuCharType)NULL;
    } // End if
    else
    {
        AllocBuffer(0,false);
    } // End else
} // End of Constructor


//-------------------------------------------------------------
template<class SuCharType>
SuStringT<SuCharType>::
~SuStringT()
{
   DeallocBuffer();
} // End of Destructor


//=============================================================
// GetData to return the pointer to private data structure.
//=============================================================
template<class SuCharType>
typename SuStringT<SuCharType>::StringData* 
SuStringT<SuCharType>::GetData()
{
   SU_ASSERT(m_pBuffer!=NULL);
   char8 *dataAddr = (char8 *)m_pBuffer - sizeof(StringData);
   StringData* pData = (StringData*)dataAddr;
   return pData;
} // End of GetData


//-------------------------------------------------------------
template<class SuCharType>
const typename SuStringT<SuCharType>::StringData* 
SuStringT<SuCharType>::GetData() const
{
   SU_ASSERT(m_pBuffer!=NULL);
   char8 *dataAddr = (char8 *)m_pBuffer - sizeof(StringData);
   const StringData* pData = (const StringData*)dataAddr;
   return pData;
} // End of GetData


//=============================================================
// Memory operation ( reserving, clearing )
//=============================================================
template<class SuCharType>
void 
SuStringT<SuCharType>::
Reserve( /*[in]*/ const int32 ReserveSize )
{
    AllocBuffer(ReserveSize,true);
} // End of Reserve


//-------------------------------------------------------------
template<class SuCharType>
void 
SuStringT<SuCharType>::
Clear()
{
    DeallocBuffer();
    AllocBuffer(0,false);
} // End of Clear


//=============================================================
// Comapare functions
//=============================================================
template<class SuCharType>
int32 
SuStringT<SuCharType>::
Compare( /*[in]*/ const SuStringT<SuCharType> &src,
         /*[in]*/ bool bCaseSensitive ) const
{
    return Compare(src.GetBuffer(),bCaseSensitive);
} // End of Compare


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
Compare( /*[in]*/ const SuCharType* srcStr,
         /*[in]*/ bool bCaseSensitive ) const
{
    SuCharType Ch1,Ch2;
    int32 nIndex = 0;

    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (srcStr==NULL)
    {
        if(GetData()->iSize==0)
            return 1;
    } // End if

    //===================================================
    // Start comparing character by character
    //===================================================
    while (1)
    {
        Ch1 = srcStr[nIndex];
        Ch2 = m_pBuffer[nIndex];
        if ( (Ch1==(SuCharType)0) &&
             (Ch2==(SuCharType)0) )
            return 0;

        if (Ch1==(SuCharType)0) 
            return -1;

        if (Ch2==(SuCharType)0) 
            return 1;

        if (bCaseSensitive==false)
        {
            Ch1 = toupper(Ch1);
            Ch2 = toupper(Ch2);
        } // End if

        if (Ch1<Ch2)
            return -1;

        if (Ch1>Ch2)
            return 1;

        nIndex++;
    } // End while

    return 0;
} // End of Compare


//=============================================================
// Searching function ( sub string, character )
//=============================================================
template<class SuCharType>
int32 
SuStringT<SuCharType>::
Find( /*[in]*/ const SuStringT<SuCharType> &src,
      /*[in]*/ const int32 startIndex,
      /*[in]*/ bool bCaseSensitive ) const
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (GetLength()==0)
        return -1;

    if (src.GetLength()==0)
        return -1;

    //===================================================
    // If case senistive == false, 
    // we make it upper case before comparing
    //===================================================
    SuCharType startChar = src[0];
    if (bCaseSensitive==false)
        startChar = toupper(startChar);

    //===================================================
    // Start comparing character by character
    //===================================================
    uint32 i = (uint32)startIndex;

    while (i<GetData()->iSize)
    {
        SuCharType thisChar = m_pBuffer[i];
        if (bCaseSensitive==false)
            thisChar = toupper(thisChar);

        if (thisChar==startChar)
        {
            int32 j=1;
            SuCharType srcChar = src[j];
            if (bCaseSensitive==false)
                srcChar = toupper(srcChar);

            while (srcChar!=(SuCharType)NULL)
            {
                thisChar = m_pBuffer[i+j];
                if (bCaseSensitive==false)
                    thisChar = toupper(thisChar);

                if (thisChar!=srcChar)
                    break;

                j++;

                srcChar = src[j];
                if (bCaseSensitive==false)
                    srcChar = toupper(srcChar);
            } // End while

            if (srcChar==(SuCharType)NULL)
                return (int32)i;
        } // End if

        i++;
    } // End while

    return -1;
} // End of Find


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
Find( /*[in]*/ const SuCharType* srcStr,
      /*[in]*/ const int32 startIndex,
      /*[in]*/ bool bCaseSensitive ) const
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (GetLength()==0)
        return -1;

    if (GetStrSize(srcStr)==0)
        return -1;

    //===================================================
    // If case senistive == false, 
    // we make it upper case before comparing
    //===================================================
    uint32 i = startIndex;

    SuCharType startChar = srcStr[0];
    if (bCaseSensitive==false)
        startChar = toupper(startChar);

    //===================================================
    // Start comparing character by character
    //===================================================
    while (i<GetData()->iSize)
    {
        SuCharType thisChar = m_pBuffer[i];
        if (bCaseSensitive==false)
            thisChar = toupper(thisChar);

        if (thisChar==startChar)
        {
            int32 j=1;
            SuCharType srcChar = srcStr[j];
            if (bCaseSensitive==false)
                srcChar = toupper(srcChar);

            while (srcChar!=(SuCharType)NULL)
            {
                thisChar = m_pBuffer[i+j];
                if (bCaseSensitive==false)
                    thisChar = toupper(thisChar);

                if (thisChar!=srcChar)
                    break;

                j++;

                srcChar = srcStr[j];
                if (bCaseSensitive==false)
                    srcChar = toupper(srcChar);
            } // End while

            if (srcChar==(SuCharType)NULL)
                return i;
        } // End if

        i++;
    } // End while

    return -1;
} // End of Find


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
Find( /*[in]*/ const SuCharType Char,
      /*[in]*/ const int32 startIndex,
      /*[in]*/ bool bCaseSensitive ) const
{
    SuCharType str[2] = { Char, (SuCharType)NULL };
    return Find(str,startIndex,bCaseSensitive);
} // End of Find


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
FindOneOf( /*[in]*/ const SuStringT<SuCharType> &src,
           /*[in]*/ const int32 startIndex,
           /*[in]*/ bool bCaseSensitive ) const
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    int32 i = startIndex;
    if (GetLength()==0)
        return -1;

    int32 srcSize = src.GetLength();
    if (srcSize==0)
        return -1;

    //===================================================
    // Start comparing character by character
    //===================================================
    while (i<GetData()->iSize)
    {
        SuCharType thisChar = m_pBuffer[i];
        if (bCaseSensitive==false)
            thisChar = toupper(thisChar);

        int32 j;
        for (j=0;j<srcSize;j++)
        {
            SuCharType srcChar = src[j];
            if (bCaseSensitive==false)
                srcChar = toupper(srcChar);

            if (thisChar==srcChar)
                return i;
        } // End for

        i++;
    } // End while

    return -1;
} // End of FindOneOf


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
FindOneOf( /*[in]*/ const SuCharType* srcStr,
           /*[in]*/ const int32 startIndex,
           /*[in]*/ bool bCaseSensitive ) const
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    uint32 i = startIndex;
    if (GetLength()==0)
        return -1;

    uint32 srcSize = GetStrSize(srcStr);
    if (srcSize==0)
        return -1;

    //===================================================
    // Start comparing character by character
    //===================================================
    while (i<GetData()->iSize)
    {
        SuCharType thisChar = m_pBuffer[i];
        if (bCaseSensitive==false)
            thisChar = toupper(thisChar);

        uint32 j;
        for (j=0;j<srcSize;j++)
        {
            SuCharType srcChar = srcStr[j];
            if (bCaseSensitive==false)
                srcChar = toupper(srcChar);

            if (thisChar==srcChar)
                return i;
        } // End for

        i++;
    } // End while

    return -1;
} // End of FindOneOf


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
FindFromEnd( /*[in]*/ const SuStringT<SuCharType> &src,
             /*[in]*/ const int32 startIndex,
             /*[in]*/ bool bCaseSensitive ) const
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (GetLength()==0)
        return -1;

    if (src.GetLength()==0)
        return -1;

    //===================================================
    // Default to last nIndex if startIndex == -1
    //===================================================
    int32 i = startIndex;
    if (i==-1)
        i = GetLength() - 1;

    //===================================================
    // If case senistive == false, 
    // we make it upper case before comparing
    //===================================================
    SuCharType startChar = src[0];
    if (bCaseSensitive==false)
        startChar = toupper(startChar);

    //===================================================
    // Start comparing character by character
    //===================================================
    while (i>=0)
    {
        SuCharType thisChar = m_pBuffer[i];
        if (bCaseSensitive==false)
            thisChar = toupper(thisChar);

        if (thisChar==startChar)
        {
            int32 j=1;
            SuCharType srcChar = src[j];
            if (bCaseSensitive==false)
                srcChar = toupper(srcChar);

            while (srcChar!=(SuCharType)NULL)
            {
                thisChar = m_pBuffer[i+j];
                if (bCaseSensitive==false)
                    thisChar = toupper(thisChar);

                if (thisChar!=srcChar)
                    break;

                j++;

                srcChar = src[j];
                if (bCaseSensitive==false)
                    srcChar = toupper(srcChar);
            } // End while

            if (srcChar==(SuCharType)NULL)
                return i;
        } // End if

        i--;
    } // End while

    return -1;
} // End of FindFromEnd


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
FindFromEnd( /*[in]*/ const SuCharType* srcStr,
             /*[in]*/ const int32 startIndex,
             /*[in]*/ bool bCaseSensitive ) const
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (GetLength()==0)
        return -1;

    if (GetStrSize(srcStr)==0)
        return -1;

    //===================================================
    // Default to last nIndex if startIndex == -1
    //===================================================
    int32 i = startIndex;
    if (i==-1)
        i = GetLength() - 1;

    //===================================================
    // If case senistive == false, 
    // we make it upper case before comparing
    //===================================================
    SuCharType startChar = srcStr[0];
    if (bCaseSensitive==false)
        startChar = toupper(startChar);

    //===================================================
    // Start comparing character by character
    //===================================================
    while (i>=0)
    {
        SuCharType thisChar = m_pBuffer[i];
        if (bCaseSensitive==false)
            thisChar = toupper(thisChar);

        if (thisChar==startChar)
        {
            int32 j=1;
            SuCharType srcChar = srcStr[j];
            if (bCaseSensitive==false)
                srcChar = toupper(srcChar);

            while (srcChar!=(SuCharType)NULL)
            {
                thisChar = m_pBuffer[i+j];
                if (bCaseSensitive==false)
                    thisChar = toupper(thisChar);

                if (thisChar!=srcChar)
                    break;

                j++;

                srcChar = srcStr[j];
                if (bCaseSensitive==false)
                    srcChar = toupper(srcChar);
            } // End while

            if (srcChar==(SuCharType)NULL)
                return i;
        } // End if

        i--;
    } // End while

    return -1;
} // End of FindFromEnd


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
FindFromEnd( /*[in]*/ const SuCharType Char,
             /*[in]*/ const int32 startIndex,
             /*[in]*/ bool bCaseSensitive ) const
{
    SuCharType str[2] = { Char, (SuCharType)NULL };
    return FindFromEnd(str,startIndex,bCaseSensitive);
} // End of FindFromEnd


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
FindOneOfFromEnd( /*[in]*/ const SuStringT<SuCharType> &src,
                  /*[in]*/ const int32 startIndex,
                  /*[in]*/ bool bCaseSensitive) const
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (GetLength()==0)
        return -1;

    int32 srcSize = src.GetLength();
    if (srcSize==0)
        return -1;

    //===================================================
    // Default to last nIndex if startIndex == -1
    //===================================================
    int32 i = startIndex;
    if (i==-1)
        i = GetLength() - 1;

    //===================================================
    // Start comparing character by character
    //===================================================
    while (i>=0)
    {
        SuCharType thisChar = m_pBuffer[i];
        if (bCaseSensitive==false)
            thisChar = toupper(thisChar);

        int32 j;
        for (j=0;j<srcSize;j++)
        {
            SuCharType srcChar = src[j];
            if (bCaseSensitive==false)
                srcChar = toupper(srcChar);

            if (thisChar==srcChar)
                return i;
        } // End for

        i--;
    } // End while

    return -1;
} // End of FindOneOfFromEnd


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
FindOneOfFromEnd( /*[in]*/ const SuCharType* srcStr,
                  /*[in]*/ const int32 startIndex,
                  /*[in]*/ bool bCaseSensitive) const
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (GetLength()==0)
        return -1;

    int32 srcSize = GetStrSize(srcStr);
    if (srcSize==0)
        return -1;

    //===================================================
    // Default to last nIndex if startIndex == -1
    //===================================================
    int32 i = startIndex;
    if (i==-1)
        i = GetLength() - 1;

    //===================================================
    // Start comparing character by character
    //===================================================
    while (i>=0)
    {
        SuCharType thisChar = m_pBuffer[i];
        if (bCaseSensitive==false)
            thisChar = toupper(thisChar);

        int32 j;
        for (j=0;j<srcSize;j++)
        {
            SuCharType srcChar = srcStr[j];
            if (bCaseSensitive==false)
                srcChar = toupper(srcChar);

            if (thisChar==srcChar)
                return i;
        } // End for

        i--;
    } // End while

    return -1;
} // End of FindOneOfFromEnd


//=============================================================
// FindWholeWord
//=============================================================
template<class SuCharType>
int32 
SuStringT<SuCharType>::
FindWholeWord( /*[in]*/ const SuCharType* srcStr,
               /*[in]*/ const int32 startIndex,
               /*[in]*/ bool bCaseSensitive) const
{
   int32 nIndex = startIndex;
   int32 srcLen = GetStrSize(srcStr);

   while (1)
   {
      int32 offset    = Find(srcStr,nIndex,bCaseSensitive);
      int32 endoffset = offset + srcLen;

      bool bWholeWord = true;
      if (offset>0)
      {
         if (IsDelimeter(m_pBuffer[offset-1])==false)
            bWholeWord = false;
      } // End if

      if ( ((uint32)endoffset<GetData()->iSize-1) &&
           (bWholeWord) )
      {
         if (IsDelimeter(m_pBuffer[endoffset])==false)
            bWholeWord = false;
      } // End if

      // If ok
      if (bWholeWord)
         return offset;

      nIndex += srcLen;
      if ((uint32)nIndex>=(GetData()->iSize-1))
         break;
   } // End while

   return -1;
} // End of FindWholeWord


//=============================================================
// Returns true, if given ch is not a valid character for a string
//=============================================================
template<class SuCharType>
bool
SuStringT<SuCharType>::
IsDelimeter( /*[in]*/ SuCharType ch ) const
{
   if (ch<256)
   {
      if (ch==(SuCharType)NULL)
         return true;

      // Is it one of ascii char?
      if ( (ch>=_T('A')) &&
           (ch<=_T('Z')) )
        return false;

      if ( (ch>=_T('a')) &&
           (ch<=_T('z')) )
        return false;

      // Underscore can be one of string character
      if (ch>=_T('_'))
        return false;

      return true;
   } // End if

   // treat all unicode character above 255, as valid character, so it's not a delimeter
   return false;
} // End of IsDelimeter


//=============================================================
// String truncation
//=============================================================
template<class SuCharType>
void
SuStringT<SuCharType>::
RemoveBefore( /*[in]*/ const uint32 nIndex )
{
    SU_ASSERT( nIndex <= GetData()->iSize );

    //===================================================
    // Shift characters to forward
    //===================================================
    uint32 i = nIndex;
    while ( i < GetData()->iSize )
    {
        m_pBuffer[ i - nIndex ] = m_pBuffer[ i ];
        i++;
        
    } // end while ( i < GetData()->iSize )

    m_pBuffer[ GetData()->iSize - nIndex ] = ( SuCharType )NULL;
    GetData()->iSize = GetData()->iSize - nIndex;
    
} // End of RemoveBefore


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
RemoveAfter( /*[in]*/ const uint32 nIndex )
{
    if( ( nIndex + 1 ) > GetData()->iSize )
    {
       // nothing to do, the string already is shorter than nIndex
       return;
    }

    //===================================================
    // Chop off string by putting null terminator at nIndex
    //===================================================
    GetData()->iSize = nIndex + 1;
    m_pBuffer[ GetData()->iSize ] = ( SuCharType )0;
    
} // End of RemoveAfter


//=============================================================
// String insertion
//=============================================================
template<class SuCharType>
void
SuStringT<SuCharType>::
Insert( /*[in]*/ const int32 destIndex,
        /*[in]*/ const SuStringT<SuCharType> &src,
        /*[in]*/ const uint32 srcStartIndex,
        /*[in]*/ const uint32 numChars )
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (src.GetLength()==0)
        return;

    uint32 num = numChars;

    SU_ASSERT(src.GetLength()>srcStartIndex);

    if (num==0)
        num = src.GetLength() - srcStartIndex;

    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+num)
    {
        AllocBuffer(GetData()->iSize+num+1,true);
    } // End if

    //===================================================
    // Insert src string
    //===================================================
    InsertBuffer(destIndex,&(src.GetBuffer()[srcStartIndex]),num);
} // End of Insert


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
Insert( /*[in]*/ const int32 destIndex,
        /*[in]*/ const SuCharType* srcStr,
        /*[in]*/ const uint32 srcStartIndex,
        /*[in]*/ const uint32 numChars )
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if ((srcStr==NULL)||(srcStr[0]==(SuCharType)0))
        return;

    int32 num = numChars;
    if (num==0)
        num = GetStrSize(srcStr) - srcStartIndex;

    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+num)
    {
        AllocBuffer(GetData()->iSize+num+1,true);
    } // End if

    //===================================================
    // Insert src string
    //===================================================
    InsertBuffer(destIndex,&srcStr[srcStartIndex],num);
} // End of Insert


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
Insert( /*[in]*/ const int32 destIndex,
        /*[in]*/ const SuCharType Char )
{
    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+1)
    {
        AllocBuffer(GetData()->iSize+1+1,true);
    } // End if

    //===================================================
    // Insert src string
    //===================================================
    InsertBuffer(destIndex,&Char,1);
} // End of Insert


//=============================================================
// String Copy
//=============================================================
template<class SuCharType>
void
SuStringT<SuCharType>::
Copy( /*[in]*/ const SuStringT<SuCharType> &src,
      /*[in]*/ const uint32 srcStartIndex,
      /*[in]*/ const uint32 numChars )
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (src.GetLength()==0)
    {
        if (m_pBuffer!=NULL)
            m_pBuffer[0] = (SuCharType)NULL;
        GetData()->iSize = 0;
        return;
    } // End if

    uint32 num = numChars;
    SU_ASSERT(src.GetLength()>srcStartIndex);

    if (num==0)
        num = src.GetLength() - srcStartIndex;

    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=num)
        AllocBuffer(num+1,false);

    //===================================================
    // Copy string
    //===================================================
    CopyBuffer(&(src.GetBuffer()[srcStartIndex]),num);
} // End of Copy


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
Copy( /*[in]*/ const SuCharType* srcStr,
      /*[in]*/ const uint32 srcStartIndex,
      /*[in]*/ const uint32 numChars )
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if ((srcStr==NULL)||(srcStr[0]==(SuCharType)0))
    {
        if (m_pBuffer!=NULL)
            m_pBuffer[0] = (SuCharType)NULL;
        GetData()->iSize = 0;
        return;
    } // End if

    uint32 num = numChars;
    if (num==0)
        num = GetStrSize(srcStr) - srcStartIndex;

    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=num)
        AllocBuffer(num+1,false);

    CopyBuffer(&srcStr[srcStartIndex],num);
} // End of Copy


//=============================================================
// String appending, prepending
//=============================================================
template<class SuCharType>
void
SuStringT<SuCharType>::
Append( /*[in]*/ const SuStringT<SuCharType> &src,
        /*[in]*/ const uint32 srcStartIndex,
        /*[in]*/ const uint32 numChars )   
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (src.GetLength()==0)
        return;

    int32 num = numChars;

    SU_ASSERT(srcStartIndex<src.GetLength());

    if (num==0)
        num = src.GetLength() - srcStartIndex;

    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+num)
    {
        AllocBuffer(GetData()->iSize+num+1,true);
    } // End if

    //===================================================
    // Do appinding
    //===================================================
    AppendBuffer(&(src.GetBuffer()[srcStartIndex]),num);
} // End of Append


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
Append( /*[in]*/ const SuCharType* srcStr,
        /*[in]*/ const uint32 srcStartIndex,
        /*[in]*/ const uint32 numChars )   
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if ((srcStr==NULL)||(srcStr[0]==(SuCharType)0))
        return;

    int32 num = numChars;
    if (num==0)
        num = GetStrSize(srcStr) - srcStartIndex;

    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+num)
    {
        AllocBuffer(GetData()->iSize+num+1,true);
    } // End if

    //===================================================
    // Do appinding
    //===================================================
    AppendBuffer(&srcStr[srcStartIndex],num);
} // End of Append


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
Append( /*[in]*/ const SuCharType srcChar )
{
    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+1)
    {
        // if the buffer size is non-zero, then double the size when appending
        // this makes the append asymptotically more efficient if appending lots of characters, one at a time
        // as is done during effect parsing.  If the buffer size is zero, then only allocate space for the one character
        uint32 nNewBufferSize = ( GetData()->iBufferSize > 0 ) ? GetData()->iBufferSize*2 : 2;
        AllocBuffer( nNewBufferSize ,true);
    } // End if

    //===================================================
    // Append character
    //===================================================
    m_pBuffer[GetData()->iSize]   = srcChar;
    m_pBuffer[GetData()->iSize+1] = (SuCharType)NULL;
    GetData()->iSize++;
} // End of Append


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
Prepend( /*[in]*/ const SuStringT<SuCharType> &src,
         /*[in]*/ const uint32 srcStartIndex,
         /*[in]*/ const uint32 numChars )   
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (src.GetLength()==0)
        return;

    SU_ASSERT(srcStartIndex<src.GetLength());

    int32 num = numChars;
    if (num==0)
        num = src.GetLength() - srcStartIndex;

    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+num)
    {
        AllocBuffer(GetData()->iSize+num+1,true);
    } // End if

    //===================================================
    // Do prepending
    //===================================================
    PrependBuffer(&(src.GetBuffer()[srcStartIndex]),num);
} // End of Prepend


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
Prepend( /*[in]*/ const SuCharType* srcStr,
         /*[in]*/ const uint32 srcStartIndex,
         /*[in]*/ const uint32 numChars )   
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if ((srcStr==NULL)||(srcStr[0]==(SuCharType)0))
        return;

    int32 num = numChars;
    if (num==0)
        num = GetStrSize(srcStr) - srcStartIndex;

    //===================================================
    // Allocate if more memory is needed
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+num)
    {
        AllocBuffer(GetData()->iSize+num+1,true);
    } // End if

    //===================================================
    // Do prepending
    //===================================================
    PrependBuffer(&srcStr[srcStartIndex],num);
} // End of Prepend


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
Prepend( /*[in]*/ const SuCharType srcChar )
{
    //===================================================
    // Do basic checking for NULL strings
    //===================================================
    if (GetData()->iBufferSize<=GetData()->iSize+1)
    {
        AllocBuffer(GetData()->iSize+1+1,true);
    } // End if

    //===================================================
    // Do prepending
    //===================================================
    PrependBuffer(&srcChar,1);
} // End of Prepend


//=============================================================
// Replacing sub string or character
//=============================================================
template<class SuCharType>
void
SuStringT<SuCharType>::
Replace( /*[in]*/ const SuCharType *origStr,
         /*[in]*/ const SuCharType *newStr,
         /*[in]*/ const int32 beginScanIndex,
         /*[in]*/ const int32 endScanIndex,
         /*[in]*/ bool bCaseSensitive,
         /*[in]*/ bool bWholeWord )
{
    //===================================================
    // Steps to do replace
    //
    // 1. Find all occurrence of string that you are searching for
    // 2. Keep all the nIndex of found location to list
    // 3. Resize the memory buffer ( if necessary ) so that
    //    we have enough to hold new string
    // 4. Use the list in step 2.
    //    For each replace location, shift old string to correct location
    //    and insert new string
    //===================================================
    int32 startScan = beginScanIndex;
    int32 endScan   = endScanIndex;
    if (endScan==0)
        endScan = GetData()->iSize;

    SuList<int32> m_matchLocationList;

    //===================================================
    // Remember all occurrence of sub string
    //===================================================
    int32 matchSize = GetStrSize(origStr);
    int32 matchIndex;
    
    if (bWholeWord)
       matchIndex = FindWholeWord(origStr,startScan,bCaseSensitive);
    else
       matchIndex = Find(origStr,startScan,bCaseSensitive);

    while ((matchIndex!=-1)&&(matchIndex<=endScan))
    {
        m_matchLocationList.push_back(matchIndex);

        if (bWholeWord)
           matchIndex = FindWholeWord(origStr,matchIndex+matchSize,bCaseSensitive);
        else
           matchIndex = Find(origStr,matchIndex+matchSize,bCaseSensitive);
    } // End while

    int32 numOfOccurence = (int32)m_matchLocationList.size();
    if (numOfOccurence==0) // Nothing to replace, so quit
        return;

    //===================================================
    // Resize memory if we have to 
    //===================================================
    int32 origSize = GetStrSize(origStr);
    int32 newSize  = GetStrSize(newStr);

    int32 strSizeBefore    = GetData()->iSize;
    uint32 bufferSizeAfter  = strSizeBefore 
                                  + newSize*numOfOccurence
                                  - origSize*numOfOccurence;
    if (bufferSizeAfter>=GetBufferSize())
        Reserve(bufferSizeAfter+1);

    //===================================================
    // Start replacing text
    //===================================================
    if (origSize<=newSize)
    {
        // Move all strings from back
        int32 destOffset = (newSize-origSize)*(numOfOccurence-1);
        int32 CopyEndIndex = GetData()->iSize;

        SuList<int32>::iterator itr = m_matchLocationList.end();
        --itr;
        while (1)
        {
            int32 loc = (*itr);

            //===================================================
            // Shift old code to make room for new string to be 
            // inserted
            //===================================================
            MoveBuffer(loc+origSize,loc+newSize+destOffset,CopyEndIndex-(loc+origSize));

            //===================================================
            // Insert new text
            //===================================================
            memcpy(&(m_pBuffer[loc+destOffset]),newStr,sizeof(SuCharType)*newSize);

            //===================================================
            // If we just replaced very last character, 
            // put terminating zero
            //===================================================
            if (loc+origSize>=strSizeBefore)
               m_pBuffer[loc+newSize+destOffset] = (SuCharType)0;

            destOffset -= (newSize-origSize);

            CopyEndIndex = loc;
            if (itr==m_matchLocationList.begin())
               break;

            --itr;
        } // End while
                
        GetData()->iSize = GetStrSize(m_pBuffer);
    } // End if
    else
    {
        // Move all strings from front
        int32 shiftOffset = 0;
        int32 CopyEndIndex = GetData()->iSize;

        SuList<int32>::iterator itr = m_matchLocationList.begin();
        while (itr!=m_matchLocationList.end())
        {
            int32 loc = (*itr);

            //===================================================
            // Shift old code to make room for new string to be 
            // inserted
            //===================================================
            MoveBuffer(loc+origSize+shiftOffset,loc+newSize+shiftOffset,CopyEndIndex-(loc+origSize+shiftOffset));

            if (CopyEndIndex-(loc+origSize+shiftOffset)==0)
            {
                m_pBuffer[loc+newSize+shiftOffset] = (SuCharType)NULL;
                GetData()->iSize = loc+newSize+shiftOffset;
            } // End if

            //===================================================
            // Insert new text
            //===================================================
            memcpy(&(m_pBuffer[loc+shiftOffset]),newStr,sizeof(SuCharType)*newSize);

            //===================================================
            // If we just replaced very last character, 
            // put terminating zero
            //===================================================
            if (loc+origSize>=strSizeBefore)
               m_pBuffer[loc+newSize+shiftOffset] = (SuCharType)0;

            shiftOffset += (newSize-origSize);

            CopyEndIndex = GetData()->iSize;
            ++itr;
        } // End while
    } // End else    
} // End of Replace


//=============================================================
// Split string into substrings based on separator
//=============================================================
template<class SuCharType>
SuList< SuStringT<SuCharType> >
SuStringT<SuCharType>::Split ( const SuStringT<SuCharType>& rSep, bool bCaseSensitive ) const
{
   SuList< SuStringT<SuCharType> > pieces;

   uint32 nSepLen = rSep.GetLength ();
   int32 nPos = 0;
   int32 nPrevPos = 0;
   bool bDone = false;
   while ( !bDone )
   {
      nPrevPos = nPos;
      nPos = Find ( rSep, nPos, bCaseSensitive );

      if ( nPos == -1 )
      {
         // no more occurences of separator found, append last substring and return
         nPos = GetLength ();
         bDone = true;
      }
      // append substring to list
      SuStringT<SuCharType> subStr;
      if ( nPos > nPrevPos )
      {
        subStr.Copy ( *this, nPrevPos, nPos - nPrevPos );
      }
      pieces.push_back ( subStr );
      nPos += nSepLen;          // skip past the separator string
   }

   return pieces;
}

//=============================================================
// Case conversion
//=============================================================
template<class SuCharType>
void
SuStringT<SuCharType>::
ToUpper()
{
    uint32 i;
    for (i=0;i<GetData()->iSize;i++)
    {
        m_pBuffer[i] = toupper(m_pBuffer[i]);
    } // End for
} // End of ToUpper


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
ToLower()
{
    uint32 i;
    for (i=0;i<GetData()->iSize;i++)
    {
        m_pBuffer[i] = tolower(m_pBuffer[i]);
    } // End for
} // End of ToUpper

//=============================================================
// Numerical conversion
//=============================================================
template<class SuCharType>
bool
SuStringT<SuCharType>::
AsInt( int32 *pInt, int32 iBase ) const
{
   // This won't work for wide character strings
   SU_ASSERT( 1 == sizeof(SuCharType) );

   // Empty strings can't be converted
   if ( 0 >= GetLength() )
   {
      return false;
   }

   // Convert the string
   SuCharType* pEnd;
   int32 iConverted = strtol( GetBuffer(), &pEnd, iBase );

   // Conversion scan length
   int32 iScanLength = (int32) ( pEnd - GetBuffer() );

   // Detect if the conversion happened.  
   // NOTE: Failure will be detected if the string was padded with white spaces!
   bool bStatus = false;
   if ( iScanLength == GetLength() )
   {
      //If the full string was scanned, we're good
      bStatus = true;
   }
   else if ( (iScanLength == GetLength()-1) && ( 'l' == m_pBuffer[iScanLength] || 'L' == m_pBuffer[iScanLength]) )
   {
      // The full string wasn't scanned but we're still ok because the scan stopped on an 'f' at the end
      bStatus = true;
   }

   // If the conversion succeeded, set the float pointer (if it's requested)
   if ( ( bStatus ) && ( NULL != pInt ) )
   {
      *pInt = iConverted;
   }

   // Return conversion status
   return bStatus;
      
} // End of AsInt

//-------------------------------------------------------------
template<class SuCharType>
bool
SuStringT<SuCharType>::
AsUInt( uint32 *pInt, int32 iBase ) const
{
   // This won't work for wide character strings
   SU_ASSERT( 1 == sizeof(SuCharType) );

   // Empty strings can't be converted
   if ( 0 >= GetLength() )
   {
      return false;
   }

   // Convert the string
   SuCharType* pEnd;
   uint32 iConverted = strtoul( GetBuffer(), &pEnd, iBase );

   // Conversion scan length
   int32 iScanLength = (int32) ( pEnd - GetBuffer() );

   // Detect if the conversion happened.  
   // NOTE: Failure will be detected if the string was padded with white spaces!
   bool bStatus = false;
   if ( iScanLength == GetLength() )
   {
      //If the full string was scanned, we're good
      bStatus = true;
   }
   else if ( (iScanLength == GetLength()-1) && ( 'l' == m_pBuffer[iScanLength] || 'L' == m_pBuffer[iScanLength]) )
   {
      // The full string wasn't scanned but we're still ok because the scan stopped on an 'f' at the end
      bStatus = true;
   }

   // If the conversion succeeded, set the float pointer (if it's requested)
   if ( ( bStatus ) && ( NULL != pInt ) )
   {
      *pInt = iConverted;
   }

   // Return conversion status
   return bStatus;
      
} // End of AsUInt

//-------------------------------------------------------------
template<class SuCharType>
bool
SuStringT<SuCharType>::
AsFloat( float32 *pFloat ) const
{
   // This won't work for wide character strings
   SU_ASSERT( 1 == sizeof(SuCharType) );

   // Empty strings can't be converted
   if ( 0 >= GetLength() )
   {
      return false;
   }

   // Convert the string
   SuCharType* pEnd;
   float32 fConverted = (float32) strtod( GetBuffer(), &pEnd );

   // Conversion scan length
   int32 iScanLength = (uint32) ( pEnd - GetBuffer() );

   // Detect if the conversion happened.  
   // NOTE: Failure will be detected if the string was padded with white spaces!
   bool bStatus = false;
   if ( iScanLength == GetLength() )
   {
      //If the full string was scanned, we're good
      bStatus = true;
   }
   else if ( (iScanLength == GetLength()-1) && ( 'f' == m_pBuffer[iScanLength] || 'F' == m_pBuffer[iScanLength]) )
   {
      // The full string wasn't scanned but we're still ok because the scan stopped on an 'f' at the end
      bStatus = true;
   }

   // If the conversion succeeded, set the float pointer (if it's requested)
   if ( ( bStatus ) && ( NULL != pFloat ) )
   {
      *pFloat = fConverted;
   }

   // Return conversion status
   return bStatus;
      
} // End of AsFloat


//=============================================================
// Formating ( similar to sprintf and same rule applies )
//=============================================================
template<class SuCharType>
void
SuStringT<SuCharType>::
Format( /*[in]*/ const SuCharType* szFormat, /*[in]*/ ... )
{
    va_list params;
    va_start ( params, szFormat );

    if (sizeof( SuCharType ) == 1 )
    {
#if (_MSC_VER >= 1300)
        // Allocate a temporary buffer that will hold the formatted string
        int nStrSize = -1;

        nStrSize = _vscprintf( ( char8* )szFormat, params );
        
        SU_ASSERT( nStrSize >= 0 );
        nStrSize = (nStrSize > 0) ? nStrSize : 0;

        AllocBuffer(nStrSize+1,false);

        SU_ASSERT(m_pBuffer!=NULL);
               
        vsprintf_s ( ( char8* )m_pBuffer, nStrSize+1, ( char8* )szFormat, params );

#elif defined ( MACOSX ) || defined (_LINUX)
         SuCharType *pFormattedStr = NULL;
         int i = vasprintf (&pFormattedStr, (char8*)szFormat, params);
         SU_ASSERT ( i >= 0 );
         SU_ASSERT ( pFormattedStr != NULL );
         if ( pFormattedStr != NULL && i >= 0 )
         {
            Clear ();
            Append ( pFormattedStr );
            free ( pFormattedStr );
         }
#elif defined ( __SYMBIAN32__ )
         SuCharType pFormattedStr[BUFSIZ];
         
         vsprintf(pFormattedStr, ( char8* )szFormat, params);
         va_end(params);

         Clear ();
         Append (pFormattedStr);
#else
#error Unknown platform.
#endif // #if platform
        
    }    
    else if (sizeof( SuCharType ) == 2 )
    {
#if (_MSC_VER >= 1300)
        // Allocate a temporary buffer that will hold the formatted string
        int nStrSize = -1;

        nStrSize = _vscwprintf( ( char16* )szFormat, params );
                   
        SU_ASSERT( nStrSize >= 0 );
        nStrSize = (nStrSize > 0) ? nStrSize : 0;

        AllocBuffer(nStrSize+1,false);

        SU_ASSERT(m_pBuffer!=NULL);
        
        vswprintf_s ( ( char16* )m_pBuffer, nStrSize+1, ( char16* )szFormat, params );
#else
         SU_ASSERT (!"SuString::Format not implemented for Unicode strings on this platform");
#endif // #if platform
        
    }
    else
    {
       SU_ASSERT (!"Invalid character size in SuString");
    } // End else if (sizeof( SuCharType ) == ... )

    // Assign new size
    GetData()->iSize = GetStrSize(m_pBuffer);
} // End of Format


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
AppendFormat( /*[in]*/ const SuCharType* szFormat, /*[in]*/ ... )
{
    va_list params;
    va_start ( params, szFormat );

    SU_ASSERT(GetData()!=NULL);
    
    if ( sizeof( SuCharType ) == 1 )
    {
#if (_MSC_VER >= 1300)
        // Allocate a temporary buffer that will hold the formatted string
        int nStrSize = -1;

        nStrSize = _vscprintf( ( char8* )szFormat, params );

        SU_ASSERT( nStrSize >= 0 );
        nStrSize = (nStrSize > 0) ? nStrSize : 0;

        int nOrigSize = GetData()->iSize;
        AllocBuffer(nOrigSize + nStrSize + 1,true);

        SU_ASSERT(m_pBuffer!=NULL);
        
        vsprintf_s ( ( char8* )&( m_pBuffer[nOrigSize] ), nStrSize + 1, ( char8* )szFormat, params );

#elif defined ( MACOSX ) || defined (_LINUX)
         SuCharType *pFormattedStr = NULL;
         int i = vasprintf (&pFormattedStr, (char8*)szFormat, params);
         SU_ASSERT ( i >= 0 );
         SU_ASSERT ( pFormattedStr != NULL );
         if ( pFormattedStr != NULL && i >= 0 )
         {
            Append (pFormattedStr);
            free ( pFormattedStr);
         }
#elif defined ( __SYMBIAN32__ )
         SuCharType pFormattedStr[BUFSIZ];
         
         vsprintf(pFormattedStr, ( char8* )szFormat, params);
         va_end(params);
         
         Append (pFormattedStr);
#else
#error Unknown platform.
#endif // #if platform
        
    } // End if ( sizeof( SuCharType ) == 1 )
    
    else if (sizeof( SuCharType ) == 2 )
    {
#if (_MSC_VER >= 1300)
        // Allocate a temporary buffer that will hold the formatted string
        int nStrSize = -1;

        nStrSize = _vscwprintf( ( char16* )szFormat, params );

        SU_ASSERT( nStrSize >= 0 );
        nStrSize = (nStrSize > 0) ? nStrSize : 0;

        int nOrigSize = GetData()->iSize;
        AllocBuffer(nOrigSize + nStrSize + 1,true);
        
        vswprintf_s ( ( char16* )&( m_pBuffer[nOrigSize] ), nStrSize + 1, ( char16* )szFormat, params );

#else
         SU_ASSERT (!"SuString::AppendFormat not implemented for Unicode strings on this platform");
#endif // #if platform
        
    } // End else if (sizeof( SuCharType ) == 2 )

    // Assign new size
    GetData()->iSize = GetStrSize(m_pBuffer);
} // End of AppendFormat

//=============================================================
// Common Operators ( +, +=, == etc )
//=============================================================
template<class SuCharType>
SuCharType& 
SuStringT<SuCharType>::
operator[] ( /*[in]*/ const int32 nIndex )
{
    SU_ASSERT(nIndex<(int32)GetData()->iSize);
    return m_pBuffer[nIndex];
} // End of operator[]

//-------------------------------------------------------------
template<class SuCharType>
SuCharType& 
SuStringT<SuCharType>::
operator[] ( /*[in]*/ const uint32 nIndex )
{
    SU_ASSERT(nIndex<(int32)GetData()->iSize);
    return m_pBuffer[nIndex];
} // End of operator[]


//-------------------------------------------------------------
template<class SuCharType>
const SuCharType& 
SuStringT<SuCharType>::
operator[] ( /*[in]*/ const int32 nIndex ) const
{
    // If reading, we can read UP to NULL terminating character
    SU_ASSERT((uint32)nIndex<=GetData()->iSize);
    return m_pBuffer[nIndex];
} // End of operator[]

//-------------------------------------------------------------
template<class SuCharType>
const SuCharType& 
SuStringT<SuCharType>::
operator[] ( /*[in]*/ const uint32 nIndex ) const
{
    // If reading, we can read UP to NULL terminating character
    SU_ASSERT(nIndex<=GetData()->iSize);
    return m_pBuffer[nIndex];
} // End of operator[]

//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
operator = ( /*[in]*/ const SuStringT<SuCharType> &src )
{
    if (src.GetLength()==0)
    {
        if (m_pBuffer!=NULL)
            m_pBuffer[0] = (SuCharType)NULL;
        GetData()->iSize = 0;
    } // End if

    if (GetBufferSize()<src.GetLength()+1)
        Reserve(src.GetLength()+1);
    Copy(src);
} // End of operator = 


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
operator = ( /*[in]*/ const SuCharType *srcStr )
{
    if ((srcStr==0)||(srcStr[0]==(SuCharType)0))
    {
        if (m_pBuffer!=NULL)
            m_pBuffer[0] = (SuCharType)NULL;
        GetData()->iSize = 0;
    } // End if

    uint32 strSize = GetStrSize(srcStr);
    if (GetBufferSize()<strSize+1)
        Reserve(strSize+1);
    Copy(srcStr);
} // End of operator = 


//-------------------------------------------------------------
template<class SuCharType>
bool
SuStringT<SuCharType>::
operator == ( /*[in]*/ const SuStringT<SuCharType> &src ) const
{
   // early out if string sizes do not match
   uint32 nSizeA = GetLength();
   uint32 nSizeB = src.GetLength();
   if( nSizeA != nSizeB )
   {
      return false;
   }

   const SuCharType* pA = GetBuffer();
   const SuCharType* pB = src.GetBuffer();

   // compare uint32s first, to reduce instruction count, then finish by going char by char
   uint32 nBytesPerChar = sizeof(SuCharType);
   uint32 nStringBytes = nSizeA*nBytesPerChar;
   uint32 nInts =  nStringBytes / sizeof(uint32);

   // compare characters in int-sized blocks
   uint32* pIntA = (uint32*) pA;
   uint32* pIntB = (uint32*) pB;
   for( uint32 i=0; i<nInts; i++ )
   {
      if( pIntA[i] != pIntB[i] )
      {
         return false;
      }
   }

   // now compare the last few chars
   uint32 nExtraChars = nStringBytes - ( nInts*sizeof(uint32) );
   for( uint32 i=nSizeA-nExtraChars; i < nSizeA; i++ )
   {
      if( pA[i] != pB[i] )
      {
         return false;
      }
   }

   return true;
} // End of operator ==


//-------------------------------------------------------------
template<class SuCharType>
bool
SuStringT<SuCharType>::
operator == ( /*[in]*/ const SuCharType *srcStr ) const
{
    if (Compare(srcStr,true)==0)
        return true;
    return false;
} // End of operator ==


//-------------------------------------------------------------
template<class SuCharType>
bool
SuStringT<SuCharType>::
operator != ( /*[in]*/ const SuStringT<SuCharType> &src ) const
{
    return !( *this == src );
} // End of operator !=


//-------------------------------------------------------------
template<class SuCharType>
bool
SuStringT<SuCharType>::
operator != ( /*[in]*/ const SuCharType *srcStr ) const
{
    return !( *this == srcStr );
} // End of operator !=


//-------------------------------------------------------------
template<class SuCharType>
bool SuStringT<SuCharType>::operator < ( /*[in]*/ const SuStringT<SuCharType> &src ) const
{
   return (Compare(src,true) == 1) ? true : false;
}

//-------------------------------------------------------------
template<class SuCharType>
bool SuStringT<SuCharType>::operator < ( /*[in]*/ const SuCharType* srcStr ) const
{
   return (Compare(srcStr,true) == 1) ? true : false;
}

//-------------------------------------------------------------
template<class SuCharType>
bool SuStringT<SuCharType>::operator <= ( /*[in]*/ const SuStringT<SuCharType> &src ) const
{
   return (Compare(src,true) >= 0) ? true : false;
}

//-------------------------------------------------------------
template<class SuCharType>
bool SuStringT<SuCharType>::operator <= ( /*[in]*/ const SuCharType* srcStr ) const
{
   return (Compare(srcStr,true) >= 0) ? true : false;
}

//-------------------------------------------------------------
template<class SuCharType>
bool SuStringT<SuCharType>::operator > ( /*[in]*/ const SuStringT<SuCharType> &src ) const
{
   return (Compare(src,true) == -1) ? true : false;
}

//-------------------------------------------------------------
template<class SuCharType>
bool SuStringT<SuCharType>::operator > ( /*[in]*/ const SuCharType* srcStr ) const
{
   return (Compare(srcStr,true) == -1) ? true : false;
}

//-------------------------------------------------------------
template<class SuCharType>
bool SuStringT<SuCharType>::operator >= ( /*[in]*/ const SuStringT<SuCharType> &src ) const
{
   return (Compare(src,true) <= 0) ? true : false;
}

//-------------------------------------------------------------
template<class SuCharType>
bool SuStringT<SuCharType>::operator >= ( /*[in]*/ const SuCharType* srcStr ) const
{
   return (Compare(srcStr,true) <= 0) ? true : false;
}

//-------------------------------------------------------------
template<class SuCharType>
SuStringT<SuCharType>
SuStringT<SuCharType>::
operator + ( /*[in]*/ const SuStringT<SuCharType> &src ) const
{
    SuStringT<SuCharType> result(*this);
    result.Append(src);
    return result;
} // End of operator +


//-------------------------------------------------------------
template<class SuCharType>
SuStringT<SuCharType>
SuStringT<SuCharType>::
operator + ( /*[in]*/ const SuCharType *srcStr ) const
{
    SuStringT<SuCharType> result(*this);
    result.Append(srcStr);
    return result;
} // End of operator +


//-------------------------------------------------------------
template<class SuCharType>
SuStringT<SuCharType>
SuStringT<SuCharType>::
operator + ( /*[in]*/ const SuCharType srcChar ) const
{
    SuStringT<SuCharType> result(*this);
    result.Append(srcChar);
    return result;
} // End of operator +
            

//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
operator += ( /*[in]*/ const SuStringT<SuCharType> &src )
{
    Append(src);
} // End of operator +=


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
operator += ( /*[in]*/ const SuCharType* srcStr )
{
    Append(srcStr);
} // End of operator +=


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
operator += ( /*[in]*/ const SuCharType srcChar )
{
    Append(srcChar);
} // End of operator +=


//=============================================================
// Internal functions ( memory management, copying etc )
//=============================================================
template<class SuCharType>
void
SuStringT<SuCharType>::
AllocBuffer( /*[in]*/ const int32 iNewSize, 
             /*[in]*/ const bool bPreserve )
{
    //=============================================================
    // Even new size is 0, we have to allocate just enough memory
    // to hold StringData structure.
    //=============================================================
    if (iNewSize==0)
    {
        DeallocBuffer();

        // We need to allocate data to hold data
        char8 *pDataBuffer = new char8 [sizeof( StringData ) + sizeof( SuCharType )];
        SU_ASSERT(pDataBuffer!=NULL);

        m_pBuffer = (SuCharType*)(pDataBuffer+sizeof(StringData));
        m_pBuffer[0] = (SuCharType)'\0';

        GetData()->iSize       = 0;
        GetData()->iBufferSize = 1;

        return;
    } // End if

    //=============================================================
    // Determine sizes to save
    //=============================================================
    int32 savedSize = 0;
    if (m_pBuffer!=NULL)
       savedSize = GetData()->iSize;

    SuCharType *pOldBuffer = m_pBuffer;
    int32 oldSize = 0;
    if (m_pBuffer!=NULL)
       oldSize = GetBufferSize();

    //=============================================================
    // We need to allocate data to hold string + StringData
    //=============================================================
    char8  *pDataBuffer = new char8 [sizeof( StringData ) + ( sizeof( SuCharType ) * iNewSize )];
    SU_ASSERT(pDataBuffer!=NULL);

    SuCharType *pNewBuffer = (SuCharType*)(pDataBuffer+sizeof(StringData));

    //=============================================================
    // If we must keep the old string, copy data to new buffer now
    //=============================================================
    if (bPreserve)
    {
        int32 CopySize = oldSize-1;
        if (CopySize>(iNewSize-1))
            CopySize = (iNewSize-1);

        int32 i;
        for (i=0;i<CopySize;i++)
        {
            pNewBuffer[i] = pOldBuffer[i];
        } // End for

        pNewBuffer[i] = (SuCharType)NULL;
    } // End if
    else
    {
        pNewBuffer[0] = (SuCharType)NULL;
    } // End else

    //=============================================================
    // Free old data
    //=============================================================
    DeallocBuffer();

    //=============================================================
    // Assign new buffer and set the Private Data members here
    //=============================================================
    m_pBuffer = pNewBuffer;
    GetData()->iSize       = savedSize;
    GetData()->iBufferSize = iNewSize;
} // End of AllocBuffer


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
DeallocBuffer()
{
    if (m_pBuffer!=NULL)
    {
        //=============================================================
        // m_pBuffer is not the actual begnning of allocated buffer
        // We have to get to the beginning of allocated memory by GetData()
        //=============================================================
        char8 *pDataBuffer = (char8*)GetData();
        if( NULL != pDataBuffer )
        {
           delete [] pDataBuffer;
           m_pBuffer = NULL;
        } // end if ( NULl != pDataBuffer )
    } // End if
} // End of DeallocBuffer


//-------------------------------------------------------------
template<class SuCharType>
int32 
SuStringT<SuCharType>::
GetStrSize( /*[in]*/ const SuCharType* srcStr ) const
{
    int32 i= 0;
    if (srcStr==NULL)
        return 0;

    while (srcStr[i] != 0)
    {
        i++;
    } // End while

    return i;
} // End of GetStrSize


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
CopyBuffer( /*[in]*/ const SuCharType* srcStr,
            /*[in]*/ const uint32 iSize )
{
    SU_ASSERT(iSize<GetData()->iBufferSize);

    //=============================================================
    // Copy memory and put string terminator ( NULL ) at the end
    //=============================================================
    memcpy(m_pBuffer,srcStr,sizeof(SuCharType)*iSize);
    GetData()->iSize = iSize;
    m_pBuffer[GetData()->iSize] = (SuCharType)NULL;
} // End of CopyBuffer


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
AppendBuffer( /*[in]*/ const SuCharType* srcStr,
              /*[in]*/ const int32 iSize )
{
    int32 curSize = GetData()->iSize;

    //=============================================================
    // Copy memory and put string terminator ( NULL ) at the end
    //=============================================================
    SU_ASSERT(iSize+curSize<(int32)GetData()->iBufferSize);
    memcpy(&(m_pBuffer[curSize]),srcStr,sizeof(SuCharType)*iSize);
    GetData()->iSize += iSize;
    m_pBuffer[GetData()->iSize] = (SuCharType)NULL;
} // End of AppendBuffer


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
PrependBuffer( /*[in]*/ const SuCharType* srcStr,
               /*[in]*/ const int32 iSize )
{
    int32 curSize = GetData()->iSize;
    SU_ASSERT(iSize+curSize < (int32)GetData()->iBufferSize);
    int32 i;

    //=============================================================
    // Shift old string to make room for prepending
    //=============================================================
    for (i=curSize-1;i>=0;i--)
    {
        m_pBuffer[i+iSize] = m_pBuffer[i];
    } // End for

    //=============================================================
    // Insert new string at beginning
    //=============================================================
    memcpy(&(m_pBuffer[0]),srcStr,sizeof(SuCharType)*iSize);
    GetData()->iSize += iSize;
    m_pBuffer[GetData()->iSize] = (SuCharType)NULL;
} // End of PrependBuffer


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
InsertBuffer( /*[in]*/ const int32 destIndex,
              /*[in]*/ const SuCharType* srcStr,
              /*[in]*/ const int32 iSize )
{
    int32 curSize = GetData()->iSize;
    SU_ASSERT(iSize+curSize<(int32)GetData()->iBufferSize);

    //=============================================================
    // Shift old string to make room for prepending
    //=============================================================
    int32 i;
    for (i=curSize-1;i>=destIndex;i--)
    {
        m_pBuffer[i+iSize] = m_pBuffer[i];
    } // End for

    //=============================================================
    // Insert new string
    //=============================================================
    for (i=0;i<iSize;i++)
    {
        m_pBuffer[destIndex+i] = srcStr[i];
    } // End for

    m_pBuffer[curSize+iSize] = (SuCharType)NULL;

    GetData()->iSize += iSize;
} // End of InsertBuffer


//-------------------------------------------------------------
template<class SuCharType>
void
SuStringT<SuCharType>::
MoveBuffer( /*[in]*/ const uint32 srcIndex,
            /*[in]*/ const uint32 destIndex,
            /*[in]*/ const uint32 iSize )
{
    SU_ASSERT(srcIndex+iSize<=GetData()->iSize);
    SU_ASSERT(destIndex+iSize<=(int32)GetData()->iBufferSize);

    if (srcIndex==destIndex)
        return;

    if (iSize==0)
        return;

    uint32 i;
    bool bMustResetEndString = false;
    int32 newLastIndex;
    if (srcIndex+iSize>=GetData()->iSize)
    {
        newLastIndex = destIndex+iSize;
        bMustResetEndString = true;
    } // End if

    //=============================================================
    // If (srcIndex<destIndex), we have to shift characters from end
    // If not, we have to shift from beginning
    //=============================================================
    if (srcIndex<destIndex)
    {
        for (i=iSize-1;i>=0;i--)
        {
            m_pBuffer[destIndex+i] = m_pBuffer[srcIndex+i];
            
            if( i == 0 )
               break; // prevent underflow

        } // End for
    } // End if
    else
    {
        for (i=0;i<iSize;i++)
        {
            m_pBuffer[destIndex+i] = m_pBuffer[srcIndex+i];
        } // End for
    } // End if

    //=============================================================
    // Put string terminator
    //=============================================================
    if (bMustResetEndString)
    {
        m_pBuffer[newLastIndex] = (SuCharType)NULL;
        GetData()->iSize = newLastIndex;
    } // End if
} // End of MoveBuffer


//=============================================================
// Public operators ( non member function )
//=============================================================
template<class SuCharType>
SuStringT<SuCharType>
operator + ( /*[in]*/ const SuCharType* srcStr1,
             /*[in]*/ const SuStringT<SuCharType>& srcStr2 )
{
    SuStringT<SuCharType> result(srcStr1);
    result.Append(srcStr2);
    return result;
} // End of operator +

template< class SuCharType>
bool operator==( const SuCharType* srcStr1, const SuStringT<SuCharType>& srcStr2 )
{
   return srcStr2 == srcStr1;
}

template< class SuCharType>
bool operator!=( const SuCharType* srcStr1, const SuStringT<SuCharType>& srcStr2 )
{
   return srcStr2 != srcStr1;
}