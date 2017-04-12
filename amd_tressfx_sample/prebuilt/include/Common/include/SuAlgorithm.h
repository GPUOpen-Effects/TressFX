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

#ifndef _SUALGORITHM_H
#define _SUALGORITHM_H

#include "SuAssert.h"
#include "SuTypes.h"
#include "SuFunctional.h"
#include "SuRand.h"        // included because SuSort uses randomized quicksort

#if !defined (SU_USE_NATIVE_STL)

//-----------------------------------------------------------------------
/// \brief Equivalent of "std::find"
///
/// \param First Iterator to beginning of search
/// \param Last Iterator to end of search
/// \param Value Item to find
//-----------------------------------------------------------------------
template<class Iterator, class Type> inline
   Iterator SuFind(Iterator First, Iterator Last, const Type& Value)
{
   for (; First != Last; ++First)
		if (*First == Value)
			break;
	return (First); 
}


/// \brief Equivalent of std::swap
/// \param a First thing to swap
/// \param b Second thing to swap
template<class Type>
inline void SuSwap( Type& a, Type& b )
{
   Type temp = a;
   a = b;
   b = temp;
}

/// Computes the minimum
template<class Type> 
inline Type SuMin( Type a, Type b )
{
   return ( a < b ) ? a : b;
}

/// Computes the maximum
template<class Type> 
inline Type SuMax( Type a, Type b )
{
   return ( a > b ) ? a : b;
}


//=================================================================================================================================
/// \ingroup Common
/// \brief A templatized in-place partition algorithm
///     The template parameter BiDirectionalIterator must be a type that supports operator*, prefix++ and prefix-- 
///     The StrictWeakOrderingParameter must be a binary predicate that compares the results of BiDirectionalIterator::operator*
///
///     The range of elements from first through last-1 are partitioned such that all 'true' elements are in the beginning
///     of the range, and all 'false' elements are at the end of the range.  The partitioning is done in place
///
///     \param first  Iterator to the beginning of the range to partition
///     \param last   Iterator to the end of the range to partition
///     \param pred   A predicate functor that is used to partition the range
///     \return       An iterator to the first element x in the partitioned range for which pred(x) is false
//=================================================================================================================================
template < class BiDirectionalIterator, class Predicate >
BiDirectionalIterator SuPartition( BiDirectionalIterator first, BiDirectionalIterator last,  Predicate pred )
{
   // implementation largely stolen from VC++ algorithm header
   while( true )
   {
      // skip in-place elements at beginning
      while( first != last && pred(*first) )
      { 
         ++first;
      }
      
      if( first == last )  // done
         break;

      // skip in-place elements at end
      do
      {
         --last;
      } while( first != last && !pred(*last) );

      if( first == last )  // done
         break;
    
      SuSwap( *first, *last );
      ++first;
   }

   return first;
}


//=================================================================================================================================
/// \ingroup Common
/// \brief A base class for a functor that can is used for sorting.  This can be subclasses to make defining sort functors easier
//=================================================================================================================================
template< class T >
class SuSortFunctor : public SuBinaryFunction<T,T,bool>
{
};


//=================================================================================================================================
/// \ingroup Common
/// \brief A templatized sorting function for Sushi, which uses a user-specified sorting function.
///     The template parameter RandomAccessIterator must be a type that supports operator*, prefix++, prefix--, '+', and '-'
///     The semantics of these operators on the iterator must match pointer semantics.
///
///     The StrictWeakOrderingParameter must be a binary predicate that compares the results of RandomAccessIterator::operator*
///
///     This SuSort implementation uses randomized selection to avoid quadratic time complexity.  
///     Random pivot locations are selected using genrand_int32()
///
///     A two-argument overload of SuSort, which uses operator <, is currently not provided.
///
///     The range of values from first through last-1 will be sorted in place.
///
///     \param first    Iterator to the beginning of the range to sort. 
///     \param last     Iterator to the end of the range to sort. 
///     \param ordering A 'less-than' function that will be used to sort the elements of the range in ascending order
///
//=================================================================================================================================
template < class RandomAccessIterator, class StrictWeakOrdering >
void SuSort( RandomAccessIterator first, RandomAccessIterator last,  const StrictWeakOrdering& ordering )
{
   uint32 nRangeSize = (uint32) ( last - first );
   if( nRangeSize <= 1 )
   {
      // done
      return;
   }

   // choose a random pivot
   uint32 nRandom = genrand_int32();
   SU_ASSERT( nRangeSize >= 2 );
   uint32 nPivotIndex = nRandom % nRangeSize;

   // partition elements around the pivot.  To do this, we use the predicate  f(x) = { x < *(first+pivot) }
   SuBinderSecond<StrictWeakOrdering> partitionFunc( ordering, *(first+nPivotIndex) );
   RandomAccessIterator pivot = SuPartition( first, last, partitionFunc  );
   SU_ASSERT( !partitionFunc( *pivot ) );

   if( first == pivot )
   {
      // this happens if pivot element happens to be the smallest
      SuSwap( *first, *(first+nPivotIndex) );
      first++;
      SuSort( first, last, ordering );
      return;
   }

   // divide and conquer
   SuSort( first, pivot, ordering );
   SuSort( pivot, last, ordering );
}



#else // !SU_USE_NATIVE_STL

#include <algorithm>

#define SuFind(x,y,z)         std::find(x,y,z)
#define SuSwap(a,b)           std::swap(a,b)
#define SuSort(a,b,func)      std::sort(a,b,func)
#define SuPartition(a,b,pred) std::partition(a,b,pred)

#ifdef WIN32

// VC++ doesn't define std::max and std::min, they define these instead
#define SuMax(a,b) _cpp_max(a,b)
#define SuMin(a,b) _cpp_min(a,b)

#else

#define SuMax(a,b) std::max(a,b)
#define SuMin(a,b) std::min(a,b)

#endif

#endif // SU_USE_NATIVE_STL


#endif // _SUALGORITHM_H
