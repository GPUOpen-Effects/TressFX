// 
// Copyright Emin Martinian, emin@allegro.mit.edu, 1998
// Permission granted to copy and distribute provided this
// comment is retained at the top.
//

//=================================================================================================================================
//                                                                                                 
// Author: Dan Ginsburg (adapted from Emin's my_hash_map: http://www.csua.berkeley.edu/~emin/source_code/hash_map/my_hash_map.H)
// 
// Copyright (c) 2017 Advanced Microsoft Devices, Inc. to the modifications for these files.  
//
// Implementation of templated hash map
//                 
//=================================================================================================================================

#ifndef _SUHASHMAP_H
#define _SUHASHMAP_H

#include "SuTypes.h"
#include "SuAssert.h"
#include "SuCommon.h"

// Cell enums
enum SuCellStatus
{
   EMPTY_CELL,
   VALID_CELL,
   DELETED_CELL
};


//-----------------------------------------------------------------------------
/// \class SuPair
/// \brief Helper struct for SuHashMap that stores key-value pair
/// \ingroup Common
/// 
//-----------------------------------------------------------------------------
template <class TypeFirst, class TypeSecond> 
class SuPair
{
public:

   //---------------------------------------------------------------------------------------------------------------------------  
   /// Default constructor.  We'll let the compiler invoke the default constructors of the component types if there are any
   //---------------------------------------------------------------------------------------------------------------------------  
   SuPair() : first(), second()
   {
   }

   //--------------------------------------------------------------------------      
   /// Constructor
   //--------------------------------------------------------------------------      
   SuPair(TypeFirst const & rFirst, TypeSecond const & rSecond) :
      first(rFirst), 
      second(rSecond)
   {
   }

   TypeFirst first;
   TypeSecond second;
};

//-----------------------------------------------------------------------------
/// \class SuStringHasher
/// \brief Use this hasher to hash SuStrings
/// \ingroup Common
//-----------------------------------------------------------------------------
class SuStringHasher
{
public:

   //-----------------------------------------------------------------------------
   /// This hash function was taken from SGI's STL hash function
   /// implementation used for their hash<const char*>
   //-----------------------------------------------------------------------------
   int32 operator()( const SuString& rKey ) const
   {
      int32 nHash = 0;

      for (int32 i = 0; i < (int32)rKey.GetLength(); i++)
      {
         nHash = 5 * nHash + rKey[i];
      }      
      return nHash;
   }   

   //-----------------------------------------------------------------------------
   /// The SecondHashValue member function is another hash function which
   /// takes the key as input, but it MUST return an ODD integer as
   /// the output.  The reason SecondHashValue must return and ODD
   /// integer is described in the Design Overview section.
   //-----------------------------------------------------------------------------   
   int32 SecondHashValue( const SuString& rKey ) const 
   {
      return (operator()(rKey) << 1) + 1;
   }
};


//-----------------------------------------------------------------------------
/// \class SuRawStringHasher
/// \brief Use this hasher to hash raw C strings or unicode strings.
/// \ingroup Common
//-----------------------------------------------------------------------------
template<typename T> class SuRawStringHasher
{
public:
   //-----------------------------------------------------------------------------
   /// This hash function was taken from SGI's STL hash function
   /// implementation used for their hash<const char*>
   //-----------------------------------------------------------------------------
   int32 operator()( const T* pKey ) const
   {
      int32 nHash = 0;

      uint32 i=0;
      while( pKey[i] != '\0' )
      {
         nHash = 5 * nHash + pKey[i];
         i++;
      }      

      return nHash;
   }   

   //-----------------------------------------------------------------------------
   /// The SecondHashValue member function is another hash function which
   /// takes the key as input, but it MUST return an ODD integer as
   /// the output.  The reason SecondHashValue must return and ODD
   /// integer is described in the Design Overview section.
   //-----------------------------------------------------------------------------   
   int32 SecondHashValue( const T* pKey ) const 
   {
      return (operator()(pKey) << 1) + 1;
   }
};


//-----------------------------------------------------------------------------
/// \brief Use this hasher to hash SuStrings
/// \ingroup Common
//-----------------------------------------------------------------------------
template<typename T> class SuPointerHasher
{
public:

   //-----------------------------------------------------------------------------
   /// This hash function was taken from SGI's STL hash function
   /// implementation used for their hash<const char*>
   //-----------------------------------------------------------------------------
   int32 operator()( const T* pKey ) const
   {
      // This clearly isn't great, but it should work
      //return reinterpret_cast<int32>( pKey );
      return static_cast<int32>( reinterpret_cast<size_t>(pKey) );
   }   

   //-----------------------------------------------------------------------------
   /// The SecondHashValue member function is another hash function which
   /// takes the key as input, but it MUST return an ODD integer as
   /// the output.  The reason SecondHashValue must return and ODD
   /// integer is described in the Design Overview section.
   //-----------------------------------------------------------------------------   
   int32 SecondHashValue( const T* pKey ) const 
   {
      return (operator()( pKey ) << 1) + 1;
   }

};


//-----------------------------------------------------------------------------
/// \brief A hasher which hashes integers by directly casting them to int32
/// \ingroup Common
//-----------------------------------------------------------------------------
template<typename T> class SuIntegerHasher
{
public:
   int32 operator()( T nKey ) const
   {
      return static_cast<int32>( nKey );
   };

   int32 SecondHashValue( T nKey ) const
   {
      int32 nInt = operator()( nKey );
      return ( (nInt & 0xfffffffe) + 1 ); // force result to be odd
   };
};


//-----------------------------------------------------------------------------
/// \class SuHashComparer
/// \brief Templatized equality comparison class.  This requires that whatever
///        type you use has implemented the "==" operator.
/// \ingroup Common
//-----------------------------------------------------------------------------
template <class Type>
class SuHashComparer 
{
public:
  bool operator()(const Type &x, const Type &y) const {return x == y;}
};


//-----------------------------------------------------------------------------
/// \class SuHashComparer
/// \brief Equality comparison class to compare raw C strings using RmStrCmpT()
/// \ingroup Common
//-----------------------------------------------------------------------------
template <class T>
class SuRawStringComparer
{
public:
   bool operator()( const T* x, const T* y ) const { return SuStrCmpT( x, y ) == 0; };
};


#ifndef SU_USE_NATIVE_STL

// Forward declaration
template<class HashType, class HashValue, class Hasher, class EqualityComparer> class SuHashMap;

//-----------------------------------------------------------------------------
/// \class HashTableItearator
/// \brief Iterator for hash map
/// \ingroup Common
//-----------------------------------------------------------------------------
template<class HashType, class HashValue, class Hasher, class EqualityComparer>   
class HashTableIterator 
{
public:

   //--------------------------------------------------------------------------      
   /// Constructor
   //--------------------------------------------------------------------------         
   HashTableIterator(SuHashMap<HashType,HashValue,Hasher,EqualityComparer> const& rHashTable):
      m_table(&rHashTable), 
      m_currentIndex(0)
   {
   }

   //--------------------------------------------------------------------------      
   /// Constructor
   //--------------------------------------------------------------------------            
   HashTableIterator(SuHashMap<HashType,HashValue,Hasher,EqualityComparer> const &rHashTable, 
                     const int32 nStartingIndex):
      m_table(&rHashTable), 
      m_currentIndex(nStartingIndex)
   {
   }

   //--------------------------------------------------------------------------      
   /// Default Constructor
   //--------------------------------------------------------------------------            
   HashTableIterator():
      m_table(NULL), 
      m_currentIndex(0)
   {
   }

   //--------------------------------------------------------------------------      
   /// \return Pair that the current iterator points to
   //--------------------------------------------------------------------------      
   SuPair<const HashType, HashValue>* GetCurrentItem() 
   {
      while(m_currentIndex < m_table->m_nMaxLength)
      {
         if (VALID_CELL == m_table->m_tableStatus[m_currentIndex])
            return m_table->m_tableData[m_currentIndex];
         else
            m_currentIndex++;
      }
      return NULL;
   }

   //--------------------------------------------------------------------------      
   /// Postfix increment for iterator
   //--------------------------------------------------------------------------         
   SuPair<const HashType, HashValue>* operator++(int32)     
   {
      SuPair<const HashType, HashValue>* result = NULL;
      
      for(; m_currentIndex < m_table->m_nMaxLength; m_currentIndex++)
      {
         if (VALID_CELL == m_table->m_tableStatus[m_currentIndex]) 
         {
            result = m_table->m_tableData[m_currentIndex++];
            break;
         }
      }
      
      for(; m_currentIndex < m_table->m_nMaxLength; m_currentIndex++)
      {
         if (VALID_CELL == m_table->m_tableStatus[m_currentIndex]) 
            break;
      }
      
      return result;
   }
   
   //--------------------------------------------------------------------------      
   /// Dereference operator for iterator
   //--------------------------------------------------------------------------            
   SuPair<const HashType, HashValue>& operator*() 
   {
      return *(GetCurrentItem());
   }
   
   //--------------------------------------------------------------------------      
   /// Dereference operator for iterator
   //--------------------------------------------------------------------------               
   SuPair<const HashType, HashValue>* operator->() 
   {
      return & (operator*());
   }

   //--------------------------------------------------------------------------      
   /// Prefix increment for iterator
   //--------------------------------------------------------------------------               
   SuPair<const HashType, HashValue>* operator++()     
   {
      ++m_currentIndex;
      while(m_currentIndex < m_table->m_nMaxLength)
      {
         if (VALID_CELL == m_table->m_tableStatus[m_currentIndex]) 
           return m_table->m_tableData[m_currentIndex];
         else 
            ++m_currentIndex;
      }

      return NULL;
   }
   
   //--------------------------------------------------------------------------      
   /// Operator "<" for iterator
   //--------------------------------------------------------------------------                  
   bool operator< (HashTableIterator<HashType,HashValue,Hasher,EqualityComparer> const& other) const 
   {
      return m_currentIndex < other.m_currentIndex;
   }
   
   //--------------------------------------------------------------------------      
   /// Operator "!=" for iterator
   //--------------------------------------------------------------------------                     
   bool operator!= (HashTableIterator<HashType,HashValue,Hasher,EqualityComparer>  const& other) const 
   {
      return m_currentIndex != other.m_currentIndex;
   }
   
   //--------------------------------------------------------------------------      
   /// Operator > for iterator
   ///--------------------------------------------------------------------------                     
   bool operator> (HashTableIterator<HashType,HashValue,Hasher,EqualityComparer>  const& other) const
   {
      return m_currentIndex > other.m_currentIndex;
   }
   
   //--------------------------------------------------------------------------      
   /// Operator == for iterator
   ///--------------------------------------------------------------------------                     
   bool operator== (HashTableIterator<HashType,HashValue,Hasher,EqualityComparer>  const& other) const
   {
      return m_currentIndex == other.m_currentIndex;
   }   

private:

   // allow hash table access to m_currentIndex to implement erase() method
   friend class SuHashMap<HashType,HashValue,Hasher,EqualityComparer>;

   const SuHashMap<HashType,HashValue,Hasher,EqualityComparer>  *m_table;
   uint32 m_currentIndex;
};

//-----------------------------------------------------------------------------
/// \class SuHashMap
/// \brief Templated hash map
/// \ingroup Common
/// 
/// SuHashMap provides a templated hash map implementation similar to the
/// SGI STL hash_map.  The int32erfaces have been mostly kept compatible with STL.
/// This implementation was based on Emin Martinian my_hash_map at:
///
///   http://www.csua.berkeley.edu/~emin/source_code/hash_map/my_hash_map.H)
///
///
/// Introduction:
///
/// This file is desigend to implement a generic hash table class
/// called "SuHashMap".  The SuHashMap class is based upon the
/// hash_map class in the UNIX implementation of the
/// Standard Template Library.  For some incomprehensible reason
/// the hash_map class is not part of the STL standard so some 
/// compilers (such as the Microsoft compiler) don't implement
/// hash_map.  Basically, the SuHashMap class was designed to 
/// have the same interface as the hash_map class from STL, but
/// there are currently a few slight differences which I will
/// eventually fix.
///
/// How To Use: Setting things up
///
/// Using the SuHashMap class is pretty simple.  First decide 
/// the key type and the value type.  For example, assume you want
/// a hash table indexed with integer keys with characters as the
/// value.  The first thing you need to do is create a hashing
/// object.  The hashing object will implement the hash function
/// for your keys.  The hashing object has to be an object which 
/// implements hash functions.  For most efficient preformance
/// your functor should implement operator() and SecondHashValue,
/// but if you want you can just implement operator().
///
/// The operator() member function should take the key as input and
/// return an integer as output.  Ideally the hash function will
/// have the property that different keys are mapped to different
/// hash results, but this is not absolutely essential.  The
/// SecondHashValue member function is another hash function which
/// takes the key as input, but it MUST return an ODD integer as
/// the output.  The reason SecondHashValue must return and ODD
/// integer is described in the Design Overview sectoion.
///
/// The GENERIC_HASH function is a generic hash function which
/// hashes on integers.  You can use this function if your
/// keys are integers, or you can use it to build more
/// complicated hash functions.
///
/// How To Use: constructor
///
/// Assume we have defined a hashing object called int32Hasher, then
/// we can create a hash table as follows:
///
///		int32Hasher hashObject;
///		SuHashMap<int32, char, int32Hasher> myTable(10,hashObject);
///
/// The first argument in the template specification is "int32" 
/// which means that the keys will be integers.  The second
/// argument in the template specifcation is "char" meaning that
/// the values will be characters.  Finally, the third argument
/// is "int32Hasher" meaning an object of the hashObject class will
/// provide the necessary hashing.  
///
/// The first argument of the constructor is the initial size of the
/// hash table.  The second argument to the constructor is an instance
/// of the int32Hasher class which will provide the necessary hashing
/// services.  The initial size will be rounded up to the nearest power
/// of 2.  Therefore we could have equivalently put 11, 13, or 16 and
/// the size would still be 16.  You don't really have to worry about
/// the size because the SuHashMap classes will automatically grow
/// itself when necessary.  Thus even if you start with size 16, and
/// then put in 50 items everything will be fine.
//
/// You can control the resize behavior by providing a third argument
/// corresponding to the resize ratio.  Whenver the ratio of items in
/// the table to the table size exceeds the resize ratio, the hash
/// table expands.  The default resize ratio is .5 which means that the
/// table expands once it becomes over half full.  You probably
/// shouldn't mess with the resize ratio unless you understand how this
/// affects hash table and search times.  See the
/// Introduction_To_Algorithms_ book by Cormen, Leisserson, and Rivest
/// for such a discussion.
///
/// How To Use: inserting
///
/// To insert a key and value you can either use the insert
/// member function or the [] operator.  For example, 
/// the following statements would insert a value of 'c' or 'd' 
/// into the table for a key of 3:
//
///		myTable.insert(3,'c');
///		myTable[3] = 'd';
///
/// Note that the hash table will only store 1 value for each key.
/// Thus the second statement above will replace the value of 'c'
/// with 'd' for the key 3.
//
/// How To Use: searching
///
/// The find(...) function can be used to look for a key and value
/// pair in the hash table.  Calling myTable.find(someKey) returns
/// a HashTableIterator.  Iterator are basically like generalized
/// point32ers.  The iterator idea is from the Standard Template
/// Library.  If someKey exists in myTable, then the iterator 
/// returned will point32 to the corresponding key-value pair.  If
/// someKey is not in myTable the the iterator returned will match
/// the iterator returned by myTable.end().  The following code
/// fragment gives an example of how search for an item using
/// iterators:
///
/// HashTableIterator<int32,char,int32Hasher> i = myTable.find(3);
/// if (i < myTable.end()) {
///		printf("found item with key = %i ",i->first);
///		printf("and value %c\n",i->second);
/// } else {
///		printf("no match in table\n");
/// }
///
//// You can also say SuHashMap<int32,char,int32Hasher>::iterator
/// instead of HashTableIterator<int32,char,int32Hasher>.  The first
/// syntax corresponds to the STL convention.  Alternativly
/// you can use a typedef statement.
///
/// How To Use: Deleting
///
/// To delete an item from the table you can use the Delete
/// member function.  Calling myTable.Delete(someKey) will attempt 
/// to delete the pair with key someKey.  If someKey is found in
/// the table then it is deleted and true is returned, otherwise
/// false is returned and the table is not modified.
///
/// How To Use: for_each
///
/// One of the main uses of iterators is that you can do an
/// operation for each item in the hash table.  If you are
/// using the Standard Template Library you can simply use
/// the for_each function.  Otherwise you can use a code
/// fragment like the one below:
///
/// for(HashTableIterator<int32,char,int32Hasher> i = myTable.begin();
///      i < myTable.end(); i++) {
///		print32f("key: %i, value %c\n",i->first,i->second);
/// }
///
/// Design Overview:
///
///
/// The SuHashMap is implemented as a quadratic probing hash
/// table.  In a quadratic probing table we use two hash functions
/// for each key.  The first hash function tells us a starting
/// point32 in the table to insert the new record.  If there is
/// already another record in that spot (e.g. because another
/// key hashed to the same spot), then we use the second hash
/// function to check other spots in the table.  To make sure
/// that the entire hash table is searched when necessary the
/// value returned by the second hash function must be 
/// relatively prime to the size of the hash table.  This is
/// accomplished by making the hash table size a power of 2 and
/// requiring the second hash function return an odd value.
///
/// 
///
/// Efficiency Comments: If you do a lot of insertion and deletion
/// without resizing the table, then it is possible for the hash table
/// to get into an inefficient state.  This is because when stuff is
/// deleted from the hash table we mark the location as deleted instead
/// of empty due to the requirements of the quadratic probing
/// algorithm.  Consequently if lots of cells get marked as deleted
/// instead of empty then inserting and searching will become slow (as
/// slow as O(n)).  Therefore if you are using the hash table in an
/// application where you do lots of inserting and deleting it might be
/// good to periodically resize or otherwise clear the table.  When I
/// get some time I will put in stuff to automatically keep track of
/// the fraction of deleted cells and automatically clean up the table
/// when there are too many deleted cells.
///
//--------------------------------------------------------------
template<class HashType, class HashValue, class Hasher, class EqualityComparer>  
class SuHashMap 
{
public:
   typedef SuPair<const HashType, HashValue> pairType;
   typedef HashTableIterator<HashType,HashValue,Hasher,EqualityComparer>  iterator;
   typedef HashTableIterator<HashType,HashValue,Hasher,EqualityComparer>  const_iterator;

   friend class HashTableIterator<HashType,HashValue,Hasher,EqualityComparer> ;

   //--------------------------------------------------------------------------      
   /// Constructor
   //--------------------------------------------------------------------------      
   SuHashMap(uint32 tableSize = 8, float32 newResizeRatio = .5) :
      m_fResizeRatio(newResizeRatio),
      m_nMaxLength(RoundUpToPowerOfTwo(tableSize)), 
      m_nCurrentLength(0) , m_nNumDeletedCells(0),
      m_nMaxLengthTimesResizeRatio((int32)(m_nMaxLength*m_fResizeRatio)),
      m_tableData( new pairType*[m_nMaxLength]),
      m_tableStatus( new uint8[m_nMaxLength])      
   {
      m_begin = iterator(*this,0);
      m_end = iterator(*this,m_nMaxLength);
      for (uint32 i = 0; i < m_nMaxLength; i++) 
         m_tableStatus[i] = EMPTY_CELL;
   }

   //--------------------------------------------------------------------------      
   /// Copy Constructor
   //--------------------------------------------------------------------------      
   SuHashMap ( const SuHashMap<HashType,HashValue,Hasher,EqualityComparer> &rSrc )      
   : m_tableData( NULL ),  // initialize only these pointers. Other members get initialized in CopyHashMap()
     m_tableStatus( NULL )      
   {
      CopyHashMap ( rSrc );
   }

   //--------------------------------------------------------------------------      
   /// Destructor
   //--------------------------------------------------------------------------      
   ~SuHashMap()
   {      
      for (uint32 i = 0; i < m_nMaxLength; i++)
         if (m_tableStatus[i] == VALID_CELL)
            delete m_tableData[i];
    
      delete [] m_tableData;
      delete [] m_tableStatus;

      m_tableData = NULL;
      m_tableStatus = NULL;      
   }

   
   //--------------------------------------------------------------------------   
   /// Access item by key
   ///
   /// \param rKey Key value to lookup
   /// \return Reference to second hash value
   //--------------------------------------------------------------------------      
   HashValue& operator[] ( HashType const &rKey )
   {      
      int32 didInsert;
      iterator i = InsertWithoutDuplication (rKey, HashValue(), &didInsert);
      return i->second;
   }

   //--------------------------------------------------------------------------   
   /// operator= to clone a hash map
   ///
   /// \param rSrc Hash map to clone   
   //--------------------------------------------------------------------------      
   void operator= ( const SuHashMap<HashType,HashValue,Hasher,EqualityComparer> &rSrc )
   {  
      if( this == &rSrc )
      {
         return; // no self-assignment
      }

      clear();
      CopyHashMap ( rSrc );      
   }
   
   //--------------------------------------------------------------------------      
   /// Insert new key value pair
   ///
   /// \param rKey New key to insert to
   /// \param rValue New value to insert 
   /// \return Point32er to new pair
   //--------------------------------------------------------------------------         
   pairType* insert ( HashType const &rKey, HashValue const &rValue )
   {
      int32 didInsert;
      iterator i = InsertWithoutDuplication (rKey, rValue, &didInsert);
      
      if (didInsert) 
      {
         return i.GetCurrentItem();
      } 
      else 
      {
         delete m_tableData[i.m_currentIndex];
         pairType* const newPair = new pairType (rKey, rValue);
         m_tableData[i.m_currentIndex] = newPair;
         return newPair;
      }
   }
   
   iterator erase( const iterator& rIterator )
   {
      uint32 hashLocation = rIterator.m_currentIndex;
      SU_ASSERT( m_tableStatus[hashLocation] == VALID_CELL );

      m_tableStatus[hashLocation] = DELETED_CELL;
      delete m_tableData[hashLocation];
      m_nCurrentLength--;
      Incrementm_nNumDeletedCells();

      iterator itr = rIterator;
      itr++;
      return itr;
   }

   //--------------------------------------------------------------------------      
   /// Removes deleteInfo from the hash table if it exists and does
   /// nothing if the item is not in the hash table.  Returns true if the
   /// item to be deleted was found in the table.
   /// 
   /// \param rKey Key to delete
   /// \return true on success, false on failure
   //--------------------------------------------------------------------------      
   bool Delete(HashType const &rKey)
   {
      int32 hashIncrement;
      int32 hashLocation = m_hashFunctor.operator()(rKey)%m_nMaxLength;
#ifndef NDEBUG
      uint32 timesInLoop = 0; // used for checking assertions
#endif
   
      switch(m_tableStatus[hashLocation]) 
      {
      case EMPTY_CELL:
         {
            return false;
         }
         break;
      case VALID_CELL:
         {
            if (m_equalityFunctor ( m_tableData[hashLocation]->first, rKey) )
            {
               m_tableStatus[hashLocation] = DELETED_CELL;
               delete m_tableData[hashLocation];
               m_nCurrentLength--;
               Incrementm_nNumDeletedCells();
               return true;
            }
         }
         break;
      }

      hashIncrement = m_hashFunctor.SecondHashValue (rKey);
      SU_ASSERT ( (hashIncrement > 0) );
      SU_ASSERT( (hashIncrement % 2) != 0 );
      
      for(;;) 
      {
         hashLocation = ( hashLocation + hashIncrement ) % m_nMaxLength;
         switch(m_tableStatus[hashLocation]) 
         {
         case EMPTY_CELL :
            {
               return false;
            }
            break;
         
         case VALID_CELL :
            {
               if (m_equalityFunctor(m_tableData[hashLocation]->first, rKey)) 
               {
                  m_tableStatus[hashLocation] = DELETED_CELL;
                  delete m_tableData[hashLocation];
                  m_nCurrentLength--;
                  Incrementm_nNumDeletedCells();
                  return true;
               }
            }
            break;
       
         case DELETED_CELL:
            {
            }
            break;
         
         default :
            {
               SU_ASSERT(!"Wrong type in SuHashMap::Delete");
            }
            break;
         }
         
         SU_ASSERT ((++timesInLoop < m_nMaxLength));
      }
   }
   
   //--------------------------------------------------------------------------         
   /// Searches for searchInfo in the table and returns an iterator
   /// pointing to the match if possible and end() otherwise.
   ///
   /// \param rKey Key to search for
   /// \return Iterator to result of search 
   //--------------------------------------------------------------------------         
   iterator Search(HashType const &rKey) const
   {
      int32 hashIncrement;
      int32 hashLocation = m_hashFunctor.operator() (rKey) % m_nMaxLength;
#ifdef _DEBUG
      uint32 timesInLoop = 0; // used for checking assertions
#endif
   
      switch(m_tableStatus[hashLocation]) 
      {
      case EMPTY_CELL:
         {
            return end();
         }
         break;
      
      case VALID_CELL:
         {
            if (m_equalityFunctor ( m_tableData[hashLocation]->first, rKey ))
               return iterator(*this,hashLocation);
         }
         break;
      }
      
      hashIncrement = m_hashFunctor.SecondHashValue (rKey);
      SU_ASSERT( (hashIncrement % 2) != 0 );
      for(;;) 
      {
         hashLocation = ( hashLocation + hashIncrement ) % m_nMaxLength;
         switch(m_tableStatus[hashLocation]) 
         {
         case EMPTY_CELL:
            {
               return end();
            }
            break;
         
         case VALID_CELL:
            {
               if (m_equalityFunctor(m_tableData[hashLocation]->first,rKey))
                  return iterator(*this,hashLocation);
            }
            break;
         }
         SU_ASSERT((++timesInLoop < m_nMaxLength));
      }
   }

   //--------------------------------------------------------------------------         
   /// Searches for searchInfo in the table and returns an iterator
   /// point32ing to the match if possible and end() otherwise.
   ///
   /// \param rKey Key to search for
   /// \return Iterator to result of search 
   //--------------------------------------------------------------------------         
   iterator find(HashType const &rKey) const { return Search(rKey); }

   //-------------------------------------------------------------------------- 
   /// If an item matching key is already in the table, then an iterator
   /// point32 to that item is returned and the table is not modified and
   /// *didInsert is set to 0.  Otherwise an iterator point32ing to the
   /// newly inserted item is returned and *didInsert is set to 1.
   ///
   //-------------------------------------------------------------------------- 
   iterator InsertWithoutDuplication(HashType const &rKey, 
                                     HashValue const &rValue, 
                                     int32 *didInsert = NULL)
   {
      int32 firstDeletedLocation = -1;
      if ( NULL != didInsert ) *didInsert = 0;
      if (m_nCurrentLength >= m_nMaxLengthTimesResizeRatio)
         Resize((int32)(m_nMaxLength/m_fResizeRatio));
     
      int32 hashIncrement;
      int32 hashLocation = m_hashFunctor.operator() (rKey) % m_nMaxLength;
      uint32 timesInLoop = 0; 
     
      SU_ASSERT( (hashLocation >= 0) );
      switch (m_tableStatus[hashLocation]) 
      {
      case EMPTY_CELL:
         {
            AssignToCell(hashLocation,new pairType ( rKey , rValue ) );
            if ( NULL != didInsert ) *didInsert = 1;
            return iterator(*this,hashLocation);
         }
         break;
      case DELETED_CELL:
         {
            firstDeletedLocation = hashLocation;
         }
         break;
      case VALID_CELL:
         {
            if (m_equalityFunctor(m_tableData[hashLocation]->first, rKey ) )
               return iterator(*this,hashLocation);
         }
         break;
      default:
         {
            SU_ASSERT(!"Wrong Status in SuHashMap::insert(...)");
         }
         break;
      }

      hashIncrement = m_hashFunctor.SecondHashValue (rKey) ;
      SU_ASSERT( ( hashIncrement % 2) != 0 );

      for(;;) 
      {
         hashLocation = (hashLocation + hashIncrement)%m_nMaxLength;
      
         switch(m_tableStatus[hashLocation]) 
         {
         case EMPTY_CELL:
            {
               if (firstDeletedLocation != -1)  
                  hashLocation = firstDeletedLocation;
               
               AssignToCell(hashLocation,new pairType (rKey, rValue));
               if ( NULL != didInsert ) *didInsert = 1;
               return iterator(*this,hashLocation);
            }
            break;
         
         case DELETED_CELL:
            {
               if (firstDeletedLocation == -1) 
                  firstDeletedLocation = hashLocation;
            }
            break;
         
         case VALID_CELL:
            {
               if (m_equalityFunctor(m_tableData[hashLocation]->first, rKey))
                  return iterator(*this,hashLocation);
            }
            break;
         
         default:
            {
               SU_ASSERT(!"Wrong Status in SuHashMap::Insert");
            }
            break;
         }
         
         if (++timesInLoop > m_nMaxLength) 
         {
            // We searched the entire table and didn't find a matching key or
            // an empty cell.  This means that we are indeed inserting without
            // duplication.  It just happened that lots of stuff was already
            // in the hash table but got deleted.
            SU_ASSERT( (firstDeletedLocation != -1) );
            
            Decrementm_nNumDeletedCells();
            AssignToCell(hashLocation,new pairType (rKey, rValue));
            if ( NULL != didInsert ) *didInsert = 1;
            return iterator(*this,hashLocation);
         }
      }
   }

   //-------------------------------------------------------------------------- 
   /// Resize the map to a new max size
   ///
   /// \param newMaxSize New maximum size (should be power of 2, will be 
   ///                   rounded if it isn't
   //-------------------------------------------------------------------------- 
   void Resize(uint32 newMaxSize)
   {
      newMaxSize = RoundUpToPowerOfTwo ( newMaxSize );
      pairType** oldm_tableData = m_tableData;
      uint8 * oldm_tableStatus = m_tableStatus;
      int32 oldMaxSize = m_nMaxLength;
            
      SU_ASSERT ((newMaxSize >= m_nCurrentLength));   
      
      m_nMaxLength = newMaxSize;
      m_end.m_currentIndex = m_nMaxLength;
      
      m_tableData = new pairType*[m_nMaxLength];
      m_tableStatus = new uint8[m_nMaxLength];
      
      m_nNumDeletedCells = 0;
      for (uint32 i = 0; i < m_nMaxLength; i++) 
         m_tableStatus[i] = EMPTY_CELL;
      
      m_nCurrentLength = 0;
      for (int32 k = 0; k < oldMaxSize ; k++) 
      {
         if (VALID_CELL == oldm_tableStatus[k]) 
         {
            insert(oldm_tableData[k]->first,oldm_tableData[k]->second);
            delete oldm_tableData[k];
         }
      }
      
      delete [] oldm_tableData;
      delete [] oldm_tableStatus;
      m_nMaxLengthTimesResizeRatio = ((uint32) (m_nMaxLength*m_fResizeRatio));
   }

   
   //-------------------------------------------------------------------------- 
   /// Clear the hash map
   //--------------------------------------------------------------------------    
   void clear()
   {
      for (uint32 i = 0; i < m_nMaxLength; i++) 
      {
         if (m_tableStatus[i] == VALID_CELL)
            delete m_tableData[i];
         m_tableStatus[i] = EMPTY_CELL;
      }
      m_nCurrentLength = 0;
      m_nNumDeletedCells = 0;
   }
   
   //-------------------------------------------------------------------------- 
   /// Get an iterator to the beginning of the map
   /// \return Iterator to start of map
   //--------------------------------------------------------------------------       
   const iterator begin() const 
   { 
      // Note that we can't just return an iterator pointing at m_tableData[0]
      // because m_tableData[0] might not be a VALID_CELL.  So if the table
      // is empty we want begin() == end().
      if (m_nCurrentLength == 0)
         return m_end;
      else
         return m_begin; 
   }

   //-------------------------------------------------------------------------- 
   /// Get an iterator to the end of the map
   /// \return Iterator to end of map
   //--------------------------------------------------------------------------       
   const iterator end() const 
   {
      return m_end; 
   }

   //-------------------------------------------------------------------------- 
   /// Is map empty?
   //--------------------------------------------------------------------------    
   bool empty() const { return m_nCurrentLength == 0; }
   
   //-------------------------------------------------------------------------- 
   /// Get maximum size
   //--------------------------------------------------------------------------       
   uint32 MaxSize() const { return m_nMaxLength; }
   uint32 size() const { return m_nCurrentLength; }
   
   //-------------------------------------------------------------------------- 
   /// Get the current resize ratio
   //--------------------------------------------------------------------------       
   float32 GetResizeRatio() const { return m_fResizeRatio; }
   
   //-------------------------------------------------------------------------- 
   /// Set a new resize ratio
   //--------------------------------------------------------------------------          
   void SetResizeRatio(float32 a) 
   {
      m_fResizeRatio=a; 
      m_nMaxLengthTimesResizeRatio = (int32)(m_nMaxLength*a);
   }

private:

   //-------------------------------------------------------------------------- 
   /// Assign a cell location to a pair
   //--------------------------------------------------------------------------             
   void AssignToCell(const int32 hashLocation, pairType* newPair )
   {
      m_tableStatus[hashLocation] = VALID_CELL;
      m_tableData[hashLocation] = newPair;
      m_nCurrentLength++;
   }
   
   //-------------------------------------------------------------------------- 
   /// Increment the number of deleted cells, resize if necessary
   //--------------------------------------------------------------------------                
   void Incrementm_nNumDeletedCells() 
   {
      if (++m_nNumDeletedCells >= m_nMaxLengthTimesResizeRatio) 
         Resize(m_nMaxLength);
   }

   //-------------------------------------------------------------------------- 
   /// Reduce the number of deleted cells
   //--------------------------------------------------------------------------                
   void Decrementm_nNumDeletedCells() 
   {
      m_nNumDeletedCells--;
      SU_ASSERT( (m_nNumDeletedCells >= 0) );
   }

   //-------------------------------------------------------------------------- 
   /// Utility routine to round a value up to the closest power of 2
   //--------------------------------------------------------------------------                   
   uint32 RoundUpToPowerOfTwo(uint32 x) 
   {
      uint32 returnValue = 1;
      for (; x > returnValue; returnValue*=2) 
         ; // ';' is body of for loop
      
      return returnValue;
   }

   //-------------------------------------------------------------------------- 
   /// Clone the src hash map 
   ///
   /// \param rSrc Hash map to clone
   //--------------------------------------------------------------------------    
   void CopyHashMap( const SuHashMap<HashType,HashValue,Hasher,EqualityComparer> &rSrc )
   {
      // First copy all attribute data
      m_fResizeRatio = rSrc.m_fResizeRatio;
      m_nMaxLength = rSrc.m_nMaxLength;
      m_nCurrentLength = rSrc.m_nCurrentLength;
      m_nNumDeletedCells = rSrc.m_nNumDeletedCells;
      m_nMaxLengthTimesResizeRatio = rSrc.m_nMaxLengthTimesResizeRatio;
      m_begin = HashTableIterator<HashType, HashValue, Hasher, EqualityComparer>( *this, 0 );
      m_end = HashTableIterator<HashType, HashValue, Hasher, EqualityComparer>( *this, m_nMaxLength ); // rSrc.m_end;
      m_hashFunctor = rSrc.m_hashFunctor;
      m_equalityFunctor = rSrc.m_equalityFunctor;
      
      // Free old tables if necessary
      SU_SAFE_DELETE_ARRAY( m_tableData );
      SU_SAFE_DELETE_ARRAY( m_tableStatus );

      // Allocate data for new tables
      m_tableData = new pairType* [m_nMaxLength];
      m_tableStatus = new uint8 [m_nMaxLength];

      // Copy data from source
      for ( uint32 i = 0; i < m_nMaxLength; i++)
      {
         m_tableStatus[i] = rSrc.m_tableStatus[i];
         
         if ( m_tableStatus[i] == VALID_CELL )
         {
            m_tableData[i] = new pairType ( rSrc.m_tableData[i]->first,
                                            rSrc.m_tableData[i]->second );
         }         
      }      
   }


   // Private member variables
   float32           m_fResizeRatio;
   uint32            m_nMaxLength;
   uint32            m_nCurrentLength;
   uint32            m_nNumDeletedCells;
   uint32            m_nMaxLengthTimesResizeRatio;
   pairType**        m_tableData;
   uint8*            m_tableStatus;
   iterator          m_begin;
   iterator          m_end;
   Hasher	         m_hashFunctor;
   EqualityComparer  m_equalityFunctor;
};
#else // SU_USE_NATIVE_STL
//--------------------------------------------------------------------------
// STL WRAPPER MODE:
//    Wrap SuHashMap to be a std::map.  These are not actually identical
//    data structures, so I ignore the Hasher and EqualityComparer.  Given
//    no bizarre uses of the Hasher/EqualityComparer, this performs equivalent
//    functionality to the SuHashMap.
//--------------------------------------------------------------------------
#include <map>
template<class HashType, class HashValue, class Hasher, class EqualityComparer>  
class SuHashMap : public std::map<HashType, HashValue> 
{
public:
   typedef typename std::map<HashType, HashValue>::iterator iterator;
public:  
   std::pair<iterator, bool> insert(const HashType key, HashValue ptr) { return std::map<HashType, HashValue>::insert(std::make_pair(key, ptr)); }
   //iterator find(const HashType& __x) { return std::map<HashType, HashValue>::find(__x); }
};

#endif // SU_USE_NATIVE_STL


#endif // End of _SUHASHMAP_H




