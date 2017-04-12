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

#ifndef _SU_MD5_HASH_H_
#define _SU_MD5_HASH_H_

#include "SuTypes.h"
#include "SuFile.h"


//=========================================================================================
/// \ingroup Common
/// \brief This class respresents an MD5 hash code.
/// The SuMD5Hash class is an encapsulation of an MD5 hash code.  The class defines methods
/// for comparing hashes, and computing hashes from data blocks. 
//=========================================================================================
class SuMD5Hash
{
public:

   /// The size of an MD5 hash, in bytes.  128 bits equals 16 bytes
   static SU_DLL const int MD5_HASH_LENGTH = 16;

   /// Default Constructor
   SU_DLL SuMD5Hash( );
  
   /// Copy constructor
   SU_DLL SuMD5Hash( const SuMD5Hash& rhs );

   /// Assignment operator
   SU_DLL const SuMD5Hash& operator=( const SuMD5Hash& rhs );

   /// Comparison operator
   bool SU_DLL operator==( const SuMD5Hash& rhs ) const;

   /// Const accessor for the hash data
   inline const uint8* GetHashData() const { return m_hashValue; };

   /// Test if a hash value is all zeros
   SU_DLL bool IsZero ( void ) const;

   /// Method to convert the hash to a string
   SU_DLL SuString ToString( ) const;

   /// Method to compute a new MD5 hash from a data block
   static SU_DLL void ComputeHash( const void* pData, uint32 nBytes, SuMD5Hash* pHashOut );

   /// Method to parse an MD5 hash from a string
   static SU_DLL bool FromString( const SuString& rString, SuMD5Hash* pHashOut );

   /// Method to compute a new MD5 hash for a file (convenience function)
   static SU_DLL bool ComputeFileHash( SuFile* pFile, SuMD5Hash* pHashOut );

   /// Zero all the hash value's bytes
   static SU_DLL void ZeroHash ( SuMD5Hash* pHashOut );


   //=================================================================================================================================
   /// \ingroup Common
   /// \brief A hasher class for SuMD5Hashes, so that MD5 hashes can be used in an SuHashMap
   //=================================================================================================================================
   class SU_DLL Hasher
   {
   public:
      //-----------------------------------------------------------------------------
      /// This hash function was taken from SGI's STL hash function
      /// implementation used for their hash<const char*>
      //-----------------------------------------------------------------------------
      int32 operator()( const SuMD5Hash& rKey ) const
      {
         int32 nHash = 0;

         for (int32 i = 0; i < (int32) SuMD5Hash::MD5_HASH_LENGTH; i++)
         {
            nHash = 5 * nHash + rKey.GetHashData()[i];
         }      
         return nHash;
      }   

      //-----------------------------------------------------------------------------
      /// The SecondHashValue member function is another hash function which
      /// takes the key as input, but it MUST return an ODD integer as
      /// the output.  The reason SecondHashValue must return and ODD
      /// integer is described in the Design Overview section.
      //-----------------------------------------------------------------------------   
      int32 SecondHashValue( const SuMD5Hash& rKey ) const 
      {
         return (operator()( rKey ) << 1) + 1;
      };

   };


private:

   uint8 m_hashValue[MD5_HASH_LENGTH];
};






#endif