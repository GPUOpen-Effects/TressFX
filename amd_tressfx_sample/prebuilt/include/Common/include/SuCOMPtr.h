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

#ifndef _SU_COMPTR_H_
#define _SU_COMPTR_H_

#include "SuCommon.h"

//=================================================================================================================================
/// \ingroup Common
/// \brief A template that provides an automatic reference-counting pointer for COM objects
/// 
/// Using this template provides automatic, scoped control of COM objects, and frees the user from having to remember to manually
///  AddRef and Release them.  To initialize an SuCOMPtr, simply assign it a pointer to a COM object (or pass one to the constructor).
///
/// To initialize a COM smart pointer directly with a call to  COM factory method, the 'Address()' method can be used
///
/// For example, to create a pixel shader in DX, you would do something like the following:
///
///   SuCOMPtr<DXPixelShader> pPixelShader; 
///   pDXDevice->CreatePixelShader( pPixelShader.Address() );
///
/// In general, if your class needs to hold a reference to a COM object, you should do so using an SuCOMPtr instance, so that you
///   do not need to do manual AddRef() and Release() calls.  Scoped reference counting in this way is much less error prone.
///
/// However, it is NOT a good idea to be passing COMPtr instances around by value, because the construction, assignment, and destruction
///    of COM pointers will create a lot of useless AddRef and Release calls.  SuCOMPtrs should be thought of as "storage tanks"
///    for COM objects which need to have references retained
///
//=================================================================================================================================
template< class T >
class SuCOMPtr
{
public:

   /// Default constructor which makes a NULL pointer
   inline SuCOMPtr() : m_p(NULL) {};

   /// Explicit constructor which takes a reference to a COM object
   explicit inline SuCOMPtr( T* p ) : m_p(p) { SU_SAFE_ADDREF(p); };

   /// Copy constructor
   inline SuCOMPtr( const SuCOMPtr<T>& rhs ) : m_p(rhs.m_p) {  SU_SAFE_ADDREF(m_p); };

   /// Assignment operator
   inline const SuCOMPtr<T>& operator=( const SuCOMPtr<T>& rhs )  {
      
      if( m_p == rhs.m_p )
         return *this;

      if( m_p )
         m_p->Release();
      
      m_p = rhs.m_p;
      if( m_p )
         m_p->AddRef();
      
      return *this;
   };

   /// Assignment from raw pointers (automatically adds a reference)
   inline const SuCOMPtr<T>& operator=( T* p ) {
      if( m_p == p )
         return *this;

      if( m_p )
         m_p->Release();

      m_p = p;
      if( p )
         p->AddRef();

      return *this;
   }

   /// Destructor. Releases the reference
   inline ~SuCOMPtr() { SU_SAFE_RELEASE(m_p); };

   /// Releases the referenced object and sets the pointer to NULL
   inline void Release() { SU_SAFE_RELEASE(m_p); }

   /// Implicit conversion to a raw pointer 
   inline operator T*() const { return m_p ; };

   /// Returns the address of the underlying pointer.  This is provided so that the COM pointer to be passed to COM factory methods
   ///  which reply on double-indirection to return the addresses of the objects that they create
   inline T** Address()
   {
      SU_ASSERT( m_p == NULL ); // This means a memory leak is about to occur
      return &m_p;
   };

   /// operator*
   inline T& operator*()  const   {  SU_ASSERT(m_p); return *m_p; }

   /// operator->
   inline T* operator->() const   {  return m_p; }


private:

   T* m_p;
};

/// Comparison operator with SuCOMPtr on the right hand side
template<class T>
inline bool operator==( const T* p, const SuCOMPtr<T>& rhs ) { return (p == rhs); };

/// Comparison operator with SuCOMPtr on the right hand side
template<class T>
inline bool operator!=( const T* p, const SuCOMPtr<T>& rhs ) { return (p != rhs); };


#endif // _SU_COMPTR_H_
