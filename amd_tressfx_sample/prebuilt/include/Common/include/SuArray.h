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

#ifndef _SUARRAY_H
#define _SUARRAY_H

#include "SuTypes.h"

#ifndef SU_USE_NATIVE_STL

#include "SuAssert.h"

//-----------------------------------------------------------------------------
/// \class SuArray
/// \brief Templated dynamically resizable array
/// \ingroup Common
/// 
/// SuArray provides a templated array class, eliminating the need for
/// an external STL implementation.  All standard array access, iterator,
/// and modifier functions are available through this implementation.
///
/// To define the class:
///
///    SuArray< TYPE > myArray
///
/// To define an iterator to use with the class:
///
///    SuArray< TYPE >::iterator myArrayIterator;
///
//-----------------------------------------------------------------------------
template <class Type>
class SuArray
{
public : 

typedef Type* iterator;
typedef const Type* const_iterator;

public :

   //--------------------------------------------------------------------------
   /// \brief Constructor
   ///
   /// Constructor
   ///
   //--------------------------------------------------------------------------
   SuArray() : 
      m_nNumItems(0),
      m_nNumReserved(0),
      m_pTable(NULL)
   {
   }; // End of Constructor

   //--------------------------------------------------------------------------
   /// \brief Constructor
   ///
   /// Copy Constructor
   ///
   /// \param src Source array to copy from
   //--------------------------------------------------------------------------
   SuArray( const SuArray<Type>& rSrc ) : 
      m_nNumItems( 0 ),
      m_nNumReserved( 0 ),
      m_pTable( NULL )
   {
      if ( rSrc.capacity() > 0 )
      {
         AllocTable( rSrc.capacity(), false );

         for ( size_t i = 0; i < m_nNumReserved; i++ )
         {
            m_pTable[i] = rSrc.m_pTable[i];
         }               

         m_nNumItems = (uint32) rSrc.size();

      } // End if

   }; // End of Constructor

   //--------------------------------------------------------------------------
   /// \brief Desstructor
   ///
   /// Destructor
   ///
   //--------------------------------------------------------------------------
   ~SuArray()
   {
      DeallocTable();
   }; // End of Destructor

   //--------------------------------------------------------------------------
   // Information Accessor
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   /// \brief Random access
   /// Returns a reference to a specific array element
   //--------------------------------------------------------------------------
   const Type& at( size_t nIndex ) const 
   { 
      SU_ASSERT( nIndex < size() );
      return m_pTable[nIndex]; 
   }

   //--------------------------------------------------------------------------
   /// \brief Random access
   /// Returns a reference to a specific array element
   //--------------------------------------------------------------------------
   Type& at( size_t nIndex ) 
   {
      SU_ASSERT( nIndex < size() );
      return m_pTable[nIndex]; 
   }

   //--------------------------------------------------------------------------
   /// \brief Size of array
   ///
   /// Returns the number of elements in the array
   ///
   /// \returh None
   //--------------------------------------------------------------------------
   size_t  size() const { return m_nNumItems; };

   //--------------------------------------------------------------------------
   ///\ breif Empty check
   ///
   /// Returns true of the array has 0 elements, false otherwise
   ///
   /// \return true if array has no items, false otherwise
   //--------------------------------------------------------------------------
   bool empty() const { return ( m_nNumItems == 0 ); };

   //--------------------------------------------------------------------------
   ///\ breif Get capacity
   ///
   /// Returns array's maximum reserved data
   ///
   /// \return Size of reserved ( allocated data );
   //--------------------------------------------------------------------------
   size_t  capacity() const { return m_nNumReserved; };

   //--------------------------------------------------------------------------
   /// \brief Reserve memory
   ///
   /// Reserve memory enough to store reserveSize items
   /// so that each push_back will not have to reallocate memory until
   /// reserveSize items is reached.
   ///
   /// \param \reserveSize Number of items to reserve memory for
   /// \return None
   //--------------------------------------------------------------------------
   bool reserve( size_t nReserveSize )
   {
      if ( AllocTable( nReserveSize, true ) == false )
      {
         return false;
      }

      if ( m_nNumReserved < m_nNumItems )
      {
         m_nNumItems = nReserveSize;
      }

      return true;

   }; // End of reserve

   //--------------------------------------------------------------------------
   /// \brief Resize size of array
   ///
   /// Adjusts the size of the array
   ///
   /// \param newSize Number of items
   /// \return None
   //--------------------------------------------------------------------------
   bool resize( size_t nNewSize )
   {
      if ( nNewSize >= m_nNumReserved )
      {
         if ( AllocTable( nNewSize, true ) == false )
         {
            return false;
         }
      } // End if

      m_nNumItems = nNewSize;

      return true;

   }; // End of resize

   //--------------------------------------------------------------------------
   /// \brief Destroy allocated memory
   ///
   /// Clears the entire array, and frees any allocated memory
   ///
   /// \return None
   //--------------------------------------------------------------------------
   bool clear()
   {
      DeallocTable();
      return true;

   }; // End of reserve

   //---------------------------------------------------------------------------------------
   /// \brief Clears the size counter on the array, while retaining the allocated memory
   ///
   ///  This is useful in cases where it is desirable to cache the allocated memory
   ///  to avoid the performance cost of repeated re-allocation
   ///
   //---------------------------------------------------------------------------------------
   void clearAndKeep()
   {
      m_nNumItems = 0;
   };

   //--------------------------------------------------------------------------
   /// \brief Add new item to end of array
   ///
   /// Add an item to the back of the array
   ///
   /// \param data Data to add
   /// \return true on success
   //--------------------------------------------------------------------------
   bool push_back( Type data )
   {
      if ( m_nNumItems >= m_nNumReserved )
      {
         if ( DoubleTableSize( true ) == false )
         {
            return false;
         }
      } // End if

      m_pTable[m_nNumItems] = data;

      m_nNumItems++;

      return true;

   }; // End of push_back

   //--------------------------------------------------------------------------
   /// \brief Remove the last element in the array
   //--------------------------------------------------------------------------
   void pop_back()
   {
      if( m_nNumItems > 0 )
      {
         ReduceNumItems();
      }
   }; // End of push_back

   //--------------------------------------------------------------------------
   /// \brief Insert n copies of a particular element before location
   //--------------------------------------------------------------------------
   void insert( iterator location, size_t nCopies, const Type& data )
   {
      if( nCopies == 0 )
         return;

      size_t nIndex = location - begin();

      // grow array as needed
      if ( (m_nNumItems + nCopies) >= m_nNumReserved )
      {
         AllocTable( m_nNumItems + nCopies, true );
      } 

      // shift elements, as needed
      if ( nIndex < m_nNumItems ) 
      {
         ShiftData( nIndex, (uint32) nCopies );
      } 
      
      // insert new elements
      size_t nLastInsertedIndex = (nIndex+nCopies);
      while( nIndex < nLastInsertedIndex )
      {
         m_pTable[nIndex] = data;
         nIndex++;
      }

      m_nNumItems += nCopies;

   }; // End of insert( location, nCopies, data )

   //--------------------------------------------------------------------------
   /// \brief Insert item
   ///
   /// insert item before an iterator
   ///
   /// \param location  Iterator to the location to insert
   /// \param data      Item to insert
   ///
   /// \return An iterator to the inserted element
   //--------------------------------------------------------------------------
   iterator insert( iterator location, const Type& data )
   {
      size_t nIndex = location - begin();
      if ( m_nNumItems >= m_nNumReserved )
      {
         if ( DoubleTableSize( true ) == false )
         {
            SU_ASSERT(false);
         }
      } 

      if ( nIndex >= m_nNumItems ) 
      {
         push_back( data );
      } 
      else
      {
         ShiftData( nIndex, +1 );
         m_pTable[nIndex] = data;
         m_nNumItems++;
      } 
      
      return ( begin() + nIndex );
   }; // End of insert


   //--------------------------------------------------------------------------
   /// \brief Remove item by iterator
   ///
   /// Removes the item pointed to by the iterator itr from the array
   ///
   /// \param itr Iterator to specify location of item to remove
   /// \return Next iterator, End iterator if last item is removed
   //--------------------------------------------------------------------------
   iterator erase( iterator &itr )
   {
      size_t nIndex = itr - begin();
      SU_ASSERT( nIndex < m_nNumItems );
      ShiftData( nIndex + 1, -1 );

      ReduceNumItems( );

      return ( begin() + nIndex );

   }; // End of erase

   //--------------------------------------------------------------------------
   // Iterator Functions
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   /// \brief Begin iterator
   ///
   /// Returns an iterator initializes to the beginning of the array
   ///
   /// \return First item iterator
   //--------------------------------------------------------------------------
   iterator begin()
   {
      return m_pTable;
   }; // End of begin

   //--------------------------------------------------------------------------
   /// \brief Begin iterator
   ///
   /// Returns an iterator initializes to the beginning of the array
   ///
   /// \return First item iterator
   //--------------------------------------------------------------------------
   const_iterator begin() const
   {
      return m_pTable;
   }; // End of begin

   //--------------------------------------------------------------------------
   /// \brief End iterator
   ///
   /// Returns an iterator initializes to the beginning of the array
   ///
   /// \return Last item iterator
   //--------------------------------------------------------------------------
   iterator end()
   {
      return m_pTable + m_nNumItems;
   }; // End of end

   //--------------------------------------------------------------------------
   /// \brief End iterator
   ///
   /// Returns a const iterator initializes to the end of the array
   ///
   /// \return Last item iterator
   //--------------------------------------------------------------------------
   const_iterator end() const
   {  
      return m_pTable + m_nNumItems;
   }; // End of end

   //--------------------------------------------------------------------------
   /// \brief Back reference
   ///
   /// Returns an reference to the last element in the array
   ///
   /// \return Reference to the last element in the array
   //--------------------------------------------------------------------------
   Type& back() const
   {
      SU_ASSERT (m_pTable != NULL);
      SU_ASSERT (m_nNumItems > 0);
      return m_pTable[m_nNumItems-1];
   }

   //--------------------------------------------------------------------------
   /// \brief Front reference
   ///
   /// Returns an reference to the first element in the array
   ///
   /// \return Reference to the first element in the array
   //--------------------------------------------------------------------------
   Type& front() const
   {
      SU_ASSERT( m_pTable != NULL );
      SU_ASSERT( m_nNumItems > 0 );

      return m_pTable[0];

   }  // End of front() 

   //--------------------------------------------------------------------------
   // Indexer functions
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   /// \brief Access item by index
   ///
   /// Returns the array item at index
   ///
   /// \param nIndex Index to retriev item for
   /// \return Item at index
   //--------------------------------------------------------------------------
   Type& operator[] ( size_t nIndex )
   {
      SU_ASSERT( nIndex >= 0 );
      SU_ASSERT( nIndex < m_nNumItems || nIndex == 0 );      

      return m_pTable[ nIndex ];

   }; // End of operator[]

   //--------------------------------------------------------------------------
   /// \brief Access item by index
   ///
   /// Returns the array item at index
   ///
   /// \param nIndex Index to retriev item for
   /// \return Item at index
   //--------------------------------------------------------------------------
   const Type& operator[] ( size_t nIndex ) const
   {
      SU_ASSERT( nIndex >= 0 );
      SU_ASSERT( nIndex < m_nNumItems || nIndex == 0 );

      return m_pTable[ nIndex ];

   }; // End of operator[]

   //--------------------------------------------------------------------------
   /// \brief Copy array
   ///
   /// Copy Operator
   ///
   /// \param src Source array to copy from
   /// \return None
   //--------------------------------------------------------------------------
   void operator = ( const SuArray<Type>& rSrc )
   {
      if ( rSrc.capacity() > 0 )
      {
         AllocTable( rSrc.capacity(), false );
         for ( size_t i = 0; i < m_nNumReserved; i++ )
         {
            m_pTable[i] = rSrc.m_pTable[i];
         }
         
         m_nNumItems = (uint32) rSrc.size();

      } // End if
   }; // End of operator =


   //--------------------------------------------------------------------------
   /// \brief Equality Comparison
   ///
   /// \param rSrc Source array to compare to
   /// \return True if the arrays are identical.  False otherwise
   //--------------------------------------------------------------------------
   bool operator == ( const SuArray<Type>& rSrc ) const
   {
      if( m_nNumItems != rSrc.m_nNumItems )
      {
         return false;
      }

      for( size_t i = 0; i<m_nNumItems; i++ )
      {
         if( !(m_pTable[i] == rSrc.m_pTable[i]) )
         {
            return false;
         }
      }
      return true;
   };

   //--------------------------------------------------------------------------
   /// \brief Inequality Comparison
   ///
   /// \param rSrc Source array to compare to
   /// \return True if the arrays are dis-similar, false otherwise
   //--------------------------------------------------------------------------
   bool operator!= ( const SuArray<Type>& rSrc ) const
   {
      return !( *this == rSrc );
   };


private :
   size_t     m_nNumItems;     ///< Number of items
   size_t     m_nNumReserved;  ///< Reserved size
   Type *     m_pTable;        ///< Array of items

   //--------------------------------------------------------------------------
   // Allocation/Deallocation of array
   //--------------------------------------------------------------------------

   //---------------------------------------------------------------------------------------
   /// \brief DoubleTableSize
   ///
   /// Doubles the size of the table
   ///
   /// \return true on success
   //--------------------------------------------------------------------------------------
   bool DoubleTableSize( bool bPreserve )
   {
      if( m_nNumReserved == 0 )
      {
         return AllocTable( 1, false );
      }
      else
      {
         return AllocTable( m_nNumReserved*2, true );
      }
   }

   //---------------------------------------------------------------------------------------
   /// \brief ReduceNumItems
   ///
   /// Decrements the number of items, and re-sizes the array if it is less than 1/4 full
   ///
   /// \return true on success
   //--------------------------------------------------------------------------------------
   bool ReduceNumItems( )
   {
      m_nNumItems--;
      if( m_nNumItems < (m_nNumReserved / 4) )
      {
         return AllocTable( m_nNumReserved / 2, true );
      }
      return true;
   }

   //--------------------------------------------------------------------------
   /// \brief AllocTable
   ///
   /// Allocates a specified size to hold the array data
   ///
   /// \param nNewSize New size to alocate
   /// \param bPreserve Flag to specify if old data should be preserved at reallocation
   /// \return true on success
   //--------------------------------------------------------------------------
   bool AllocTable( size_t nNewSize, bool bPreserve )
   {
      if ( m_nNumReserved == nNewSize )
      {
         return true;
      }

      if ( nNewSize == 0 )
      {
         DeallocTable();
         return true;

      } // End if

      size_t nCopySize = m_nNumItems;

      if ( nCopySize > nNewSize )
      {
         nCopySize = nNewSize;
      }

      Type* pNewTable = new Type[ nNewSize ];
      if ( pNewTable == NULL )
      {
         return false;
      }

      for ( size_t i = 0; i < nNewSize; i++ )
      {
         pNewTable[i] = Type();

      } // End if

      if ( bPreserve ) // Do the copy, if we have to preserve data
      {
         for (size_t i = 0; i < nCopySize; i++ )
         {
            pNewTable[i] = m_pTable[i];

         } // End for

      } // End if

      DeallocTable();
      m_pTable       = pNewTable;
      m_nNumItems    = nCopySize;
      m_nNumReserved = nNewSize; 

      return true;

   }; // End of AllocTable

   //--------------------------------------------------------------------------
   /// \brief Deallocate memory
   ///
   /// Deallocates the array data
   ///
   /// \return None
   //--------------------------------------------------------------------------
   void DeallocTable()
   {
      if ( m_pTable != NULL )
      {
         delete[] m_pTable;
         m_nNumItems    = 0;
         m_nNumReserved = 0;
         m_pTable       = NULL;

      } // End if

   }; // End of DeallocTable

   //--------------------------------------------------------------------------
   /// \brief Shift array of data
   ///
   /// Data shifting
   ///
   /// - If direction is positive, shift data towards end
   ///   If direction is negative, shift data towards zero
   ///   This function will not update number of items etc
   ///
   /// \param nStartIndex Location to start shifting
   /// \partam nDirection Direction to shift to
   /// \return true on success
   //--------------------------------------------------------------------------
   bool ShiftData( size_t nStartIndex, int nDirection )
   {
      if( m_nNumItems == 0 )
      {
         return false;
      }

      if ( nDirection > 0 )
      {
         SU_ASSERT(( m_nNumItems + nDirection ) <= m_nNumReserved );
         
         for ( size_t i = m_nNumItems - 1; i > nStartIndex; i-- ) 
         {
            m_pTable[ i + nDirection ] = m_pTable[ i ];

         } // End for

         // copy the last element as a special case, in case nStartIndex ==  0
         m_pTable[ nStartIndex + nDirection ] = m_pTable[nStartIndex];

      } // End if
      else
      {
         SU_ASSERT(( m_nNumItems + nDirection ) >= 0 );

         for ( size_t i = nStartIndex; i < m_nNumItems; i++ )
         {
            m_pTable[ i + nDirection ] = m_pTable[ i ];

         } // End for

      } // End else

      return true;

   } // End of ShiftData(..)


}; // End of SuArray

#else // SU_USE_NATIVE_STL

//--------------------------------------------------------------------------
// STL WRAPPER MODE:
//    Wrap SuArray to std:vector to use native STL implemenation
//--------------------------------------------------------------------------
#include <vector>
template <class T> class SuArray : public std::vector<T> 
{
public:
   bool push_back(const T& __x) { push_back(__x); return true; }
};

#endif // SU_USE_NATIVE_STL

#endif // End of _SUARRAY_H
