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

#ifndef _SULIST_H
#define _SULIST_H

#include "SuTypes.h"

#ifndef SU_USE_NATIVE_STL

#include "SuAssert.h"

//-----------------------------------------------------------------------------
/// \class SuList
/// \brief Templated linked list data structure
/// \ingroup Common
///
/// SuList provides a templated linked list class, eliminating the need for
/// an external STL implementation.  All standard linked list access, iterator,
/// and modifier functions are available through this implementation.
///
/// To define the class:
///
///    SuList< TYPE > myLinkedList
///
/// To define an iterator to use with the class:
///
///    SuList< TYPE >::iterator myLinkedListIterator;
///
//-----------------------------------------------------------------------------

template <class Type>
class SuList
{
public : 
   //--------------------------------------------------------------------------
   /// \class ItemNode
   /// \brief Internal data structure to hold item and next/previous item's link
   ///
   /// Internal Data to hold data and next, prev pointers
   ///
   //--------------------------------------------------------------------------
   class ItemNode
   {
   public :
      //-----------------------------------------------------------------------
      /// \brief Constructor
      ///
      /// Construction
      ///
      /// \param pParent Parent list that owns this data
      ///
      //-----------------------------------------------------------------------
      ItemNode( SuList<Type> *pParent ) :  
         m_pParent(pParent),
         m_pPrev(NULL),
         m_pNext(NULL)
      {
      }; // End of Constructor

      //-----------------------------------------------------------------------
      /// \brief Constructor
      ///
      /// Construction
      ///
      /// \param pParent Parent list that owns this data
      /// \param data Initial data
      ///
      //-----------------------------------------------------------------------
      ItemNode( SuList<Type> *pParent, Type data ) : 
         m_pParent(pParent),
         m_data(data),
         m_pPrev(NULL),
         m_pNext(NULL)
      {
      }; // End of Constructor

      //-----------------------------------------------------------------------
      /// \brief Destructor
      ///
      /// Destructor
      ///
      //-----------------------------------------------------------------------
      ~ItemNode() 
      {
      }; // End of Destructor

      //-----------------------------------------------------------------------
      /// \brief Get parent list
      ///
      /// Reyurn parent list class
      ///
      /// \return Parent list that owns this data
      //-----------------------------------------------------------------------
      SuList<Type>*       GetParent() { return m_pParent; };

      //-----------------------------------------------------------------------
      /// \brief Get parent list
      ///
      /// Reyurn parent list class
      ///
      /// \return Parent list that owns this data
      //-----------------------------------------------------------------------
      const SuList<Type>* GetParent() const { return m_pParent; };

      //-----------------------------------------------------------------------
      /// \brief Data accessor
      ///
      /// Data Accessor
      ///
      /// Reference to data that this node is holding
      //-----------------------------------------------------------------------
      Type&       GetData() { return m_data; };

      //-----------------------------------------------------------------------
      /// \brief Data accessor
      ///
      /// Data Accessor
      ///
      /// Reference to data that this node is holding
      //-----------------------------------------------------------------------
      const Type& GetData() const { return m_data; };

      //-----------------------------------------------------------------------
      // Neighbor Accessors
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      /// \brief Set next pointer
      ///
      /// Sets the next item in a linked list
      /// 
      /// \param pNext Pointer to next node
      //-----------------------------------------------------------------------
      void SetNext( ItemNode *pNext ) { m_pNext = pNext; };

      //-----------------------------------------------------------------------
      /// \brief Set previous pointer
      ///
      /// Sets the previous item in a linked list
      /// 
      /// \param pPrev Pointer to previous node
      //-----------------------------------------------------------------------
      void SetPrev( ItemNode *pPrev ) { m_pPrev = pPrev; };

      //-----------------------------------------------------------------------
      /// \brief Get next node
      ///
      /// Gets the next item in a linked list
      ///
      /// \return Next node, NULL if this is the last item
      //-----------------------------------------------------------------------
      ItemNode*       GetNext() { return m_pNext; };

      //-----------------------------------------------------------------------
      /// \brief Get next node
      ///
      /// Gets the next item in a linked list
      ///
      /// \return Next node, NULL if this is the last item
      //-----------------------------------------------------------------------
      const ItemNode* GetNext() const { return m_pNext; };

      //-----------------------------------------------------------------------
      /// \brief Get previous item
      ///
      /// Gets the previous item in a linked list
      ///
      /// \return Privious node, NULL if this is the first item
      //-----------------------------------------------------------------------
      ItemNode*       GetPrev() { return m_pPrev; };

      //-----------------------------------------------------------------------
      /// \brief Get previous item
      ///
      /// Gets the previous item in a linked list
      ///
      /// \return Privious node, NULL if this is the first item
      //-----------------------------------------------------------------------
      const ItemNode* GetPrev() const { return m_pPrev; };      

   private :
      SuList<Type> *m_pParent; ///< Parent linkedlist

      Type      m_data;              ///< Actual data
      ItemNode *m_pNext;             ///< Pointer to next item
      ItemNode *m_pPrev;             ///< Pointer to previous item
   }; // End of ItemNode

   //--------------------------------------------------------------------------
   /// \class const_iterator
   /// \brief Constant iterator (contents not modifiable )
   /// 
   /// Constant iterator for this array class.  begin() returns the beginning 
   /// iterator and ++ operator of iterator will advance iterator by 1.
   /// Similarly -- operator will move backward by 1.
   //--------------------------------------------------------------------------
   class const_iterator
   {
   public :
      friend class SuList<Type>;

      //-----------------------------------------------------------------------
      /// \brief Constructor
      /// 
      /// Constructor
      ///
      //-----------------------------------------------------------------------
      const_iterator() : 
         m_pParent(NULL),
         m_pItemNode(NULL) 
      {
      }; // End of Constructor

      //-----------------------------------------------------------------------
      /// \brief Constructor
      /// 
      /// Constructor
      ///
      //-----------------------------------------------------------------------
      const_iterator( SuList<Type> *pParent,
                      ItemNode* pItemNode ) : 
         m_pParent(pParent),
         m_pItemNode(pItemNode)
      {
      }; // End of Constructor

      //-----------------------------------------------------------------------
      /// \brief Destructor
      /// 
      /// Destructor
      ///
      //-----------------------------------------------------------------------
      ~const_iterator() {};

      //-----------------------------------------------------------------------
      // Operators
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      /// \brief Prefix++
      ///
      /// Increment operator to advance iterator
      ///
      /// \return Advanced iterator reference
      //-----------------------------------------------------------------------
      const_iterator& operator++ ()
      {
         SU_ASSERT(m_pItemNode!=NULL);

         m_pItemNode = m_pItemNode->GetNext();
         return *this;
      }; // End of operator ++

      //-----------------------------------------------------------------------
      /// \brief Postfix++
      ///
      /// Increment operator to advance iterator
      ///
      /// \return Advanced iterator
      //-----------------------------------------------------------------------
      const_iterator operator++ ( int )
      {
         SU_ASSERT(m_pItemNode!=NULL);

         const_iterator itr(m_pParent,m_pItemNode);
         m_pItemNode = m_pItemNode->GetNext();

         return itr;
      }; // End of operator ++

      //-----------------------------------------------------------------------
      /// \brief  Prefix--
      /// 
      /// Decrement operator to move iterator backward
      /// 
      /// \return Iterator reference moved backward
      //-----------------------------------------------------------------------
      const_iterator& operator-- ()
      {
         // m_pItemNode can be NULL if its end iterator
         // try to grab last element of list
         if (m_pItemNode==NULL)
         {
            m_pItemNode = m_pParent->m_pTail;
         } // End if
         else
         {
            m_pItemNode = m_pItemNode->GetPrev();
         } // End else

         return *this;
      }; // End of operator ++

      //-----------------------------------------------------------------------
      /// \brief  Prefix--
      /// 
      /// Decrement operator to move iterator backward
      /// 
      /// \return Iterator moved backward
      //-----------------------------------------------------------------------
      const_iterator operator-- ( int )
      {
         SU_ASSERT(m_pItemNode!=NULL);

         const_iterator itr(m_pParent,m_pItemNode);

         // m_pItemNode can be NULL if its end iterator
         // try to grab last element of list
         if (m_pItemNode==NULL)
         {
            m_pItemNode = m_pParent->m_pTail;
         } // End if
         else
         {
            m_pItemNode = m_pItemNode->GetPrev();
         } // End else

         return itr;
      }; // End of operator ++

      //-----------------------------------------------------------------------
      /// \brief  Accessing actual item
      /// 
      /// Access item by dereferencing iterator
      /// 
      /// \return Type(templated) data
      //-----------------------------------------------------------------------
      const Type& operator * () const
      {
         SU_ASSERT(m_pItemNode!=NULL);
         return m_pItemNode->GetData();
      }; // End of operator *

      //-----------------------------------------------------------------------
      /// \brief  Accessing actual item
      /// 
      /// Access item by dereferencing iterator
      /// 
      /// \return Type(templated) data
      //-----------------------------------------------------------------------
      const Type* operator -> () const
      {
         SU_ASSERT(m_pItemNode!=NULL);
         return &m_pItemNode->GetData();
      }; // End of operator ->

      //-----------------------------------------------------------------------
      /// \brief opreator ==
      ///
      /// Equality operator
      ///
      /// \param src Source iterator to compare to
      /// \return true if two iterators are equal, false otherwise
      //-----------------------------------------------------------------------
      bool operator== ( const const_iterator &src ) const
      {
         if (m_pItemNode==src.m_pItemNode)
            return true;

         return false;
      }; // End of operator ==

      //-----------------------------------------------------------------------
      /// \brief opreator !=
      ///
      /// Equality operator
      ///
      /// \param src Source iterator to compare to
      /// \return true if two iterators are NOT equal, false otherwise
      //-----------------------------------------------------------------------
      bool operator!= ( const const_iterator &src ) const
      {
         if (m_pItemNode!=src.m_pItemNode)
            return true;

         return false;
      }; // End of operator ==

   protected :
      SuList<Type>       *m_pParent;   ///< Linkedlist that owns this iterator
      ItemNode                   *m_pItemNode; ///< ItemNode \sa ItemNode
   }; // End of const_iterator

   //--------------------------------------------------------------------------
   /// \brief iterator
   ///
   /// Iterator Class ( non - const )
   ///
   //--------------------------------------------------------------------------
   class iterator : public const_iterator
   {
   public :
      friend class SuList<Type>;
      
#if !defined (__SYMBIAN32__)
      // The following statements were put in to fix compile errors on the Mac.  However, the
      // Symbian CW compiler barfs on these statements.
      using const_iterator::m_pItemNode;
      using const_iterator::m_pParent;
#endif
      
      //-----------------------------------------------------------------------
      /// \brief Constructor
      ///
      /// Constructor
      ///
      //-----------------------------------------------------------------------
      iterator() : const_iterator()
      {
      }; // End of Constructor

      //-----------------------------------------------------------------------
      /// \brief Constructor
      ///
      /// Constructor
      ///
      //-----------------------------------------------------------------------
      iterator( SuList<Type> *pParent,
                ItemNode* pItemNode ) : const_iterator(pParent,pItemNode)
      {
      }; // End of Constructor

      //-----------------------------------------------------------------------
      /// \brief Destructor
      ///
      /// Destructor
      ///
      //-----------------------------------------------------------------------
      ~iterator() {};

      //-----------------------------------------------------------------------
      // Operators
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      /// \brief Prefix++
      ///
      /// Increment operator to advance iterator
      ///
      /// \return Advanced iterator reference
      //-----------------------------------------------------------------------
      iterator& operator++ ()
      {
         SU_ASSERT(m_pItemNode!=NULL);

         m_pItemNode = m_pItemNode->GetNext();
         return *this;
      }; // End of operator ++

      //-----------------------------------------------------------------------
      /// \brief Postfix++
      ///
      /// Increment operator to advance iterator
      ///
      /// \return Advanced iterator
      //-----------------------------------------------------------------------
      iterator operator++ ( int )
      {
         SU_ASSERT(m_pItemNode!=NULL);

         iterator itr(m_pParent,m_pItemNode);
         m_pItemNode = m_pItemNode->GetNext();

         return itr;
      }; // End of operator ++

      //-----------------------------------------------------------------------
      /// \brief  Prefix--
      /// 
      /// Decrement operator to move iterator backward
      /// 
      /// \return Iterator reference moved backward
      //-----------------------------------------------------------------------
      iterator& operator-- ()
      {
         // m_pItemNode can be NULL if its end iterator
         // try to grab last element of list
         if (m_pItemNode==NULL)
         {
            m_pItemNode = m_pParent->m_pTail;
         } // End if
         else
         {
            m_pItemNode = m_pItemNode->GetPrev();
         } // End else

         return *this;
      }; // End of operator ++

      //-----------------------------------------------------------------------
      /// \brief  Postfix--
      /// 
      /// Decrement operator to move iterator backward
      /// 
      /// \return Iterator reference moved
      //-----------------------------------------------------------------------
      iterator operator-- ( int )
      {
         SU_ASSERT(m_pItemNode!=NULL);

         iterator itr(m_pParent,m_pItemNode);

         // m_pItemNode can be NULL if its end iterator
         // try to grab last element of list
         if (m_pItemNode==NULL)
         {
            m_pItemNode = m_pParent->m_pTail;
         } // End if
         else
         {
            m_pItemNode = m_pItemNode->GetPrev();
         } // End else


         return itr;
      }; // End of operator ++

      //-----------------------------------------------------------------------
      /// \brief  Accessing actual item
      /// 
      /// Access item by dereferencing iterator
      /// 
      /// \return Type(templated) data
      //-----------------------------------------------------------------------
      Type& operator * () const
      {
         SU_ASSERT(m_pItemNode!=NULL);
         return m_pItemNode->GetData();
      }; // End of operator *


      //-----------------------------------------------------------------------
      /// \brief  Accessing actual item
      /// 
      /// Access item by dereferencing iterator
      /// 
      /// \return Type(templated) data
      //-----------------------------------------------------------------------
      Type* operator -> () const
      {
         SU_ASSERT(m_pItemNode!=NULL);
         return &m_pItemNode->GetData();
      }; // End of operator ->

      //-----------------------------------------------------------------------
      /// \brief opreator ==
      ///
      /// Equality operator
      ///
      /// \param src Source iterator to compare to
      /// \return true if two iterators are equal, false otherwise
      //-----------------------------------------------------------------------
      bool operator == ( const iterator &src ) const
      {
         if (m_pItemNode==src.m_pItemNode)
            return true;

         return false;
      }; // End of operator ==

      //-----------------------------------------------------------------------
      /// \brief opreator !=
      ///
      /// Equality operator
      ///
      /// \param src Source iterator to compare to
      /// \return true if two iterators are NOT equal, false otherwise
      //-----------------------------------------------------------------------
      bool operator!= ( const iterator &src ) const
      {
         if (m_pItemNode!=src.m_pItemNode)
            return true;

         return false;
      }; // End of operator ==      

#if defined ( __GCCE__ )
   protected:
      // The ARM g++ (3.4.3) requires these declarations otherwise there are
      // compiler errors on each use.  Why?  I don't know, I think it's a compiler
      // bug.  The CodeWarrior compiler will not even allow these declarations.
      const_iterator::m_pParent;
      const_iterator::m_pItemNode; 
#endif

   }; // End of iterator

public :


   //--------------------------------------------------------------------------
   /// \brief Constructor
   ///
   /// Constructor
   ///
   //--------------------------------------------------------------------------
   SuList() : 
      m_nNumItems(0),
      m_pHead(NULL),
      m_pTail(NULL)
   {
   }; // End of Constructor

   //--------------------------------------------------------------------------
   /// \brief Copy Constructor
   ///
   /// Copy Constructor
   ///
   /// \param src Source linked list to copy from
   ///
   //--------------------------------------------------------------------------
   SuList( const SuList<Type> &src ) : 
      m_nNumItems(0),
      m_pHead(NULL),
      m_pTail(NULL)
   {
      ItemNode *pNode = src.m_pHead;
      while (pNode!=NULL)
      {
         push_back(pNode->GetData());
         pNode = pNode->GetNext();
      } // End while
   }; // End of Constructor

   //--------------------------------------------------------------------------
   /// Destructor
   ///
   /// Destructor
   ///
   //--------------------------------------------------------------------------
   ~SuList()
   {
      clear();
   }; // End of Destructor

   //--------------------------------------------------------------------------
   // Information Accessors
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   /// \brief Get number of items
   ///
   /// Returns the number if items in the list
   ///
   /// \return Number of items in list
   //--------------------------------------------------------------------------
   size_t size() const { return m_nNumItems; };

   //--------------------------------------------------------------------------
   /// \brief Check for empty
   ///
   /// Returns true if the list is empty, false otherwise
   /// 
   /// \return true if list is empty, false otherwise
   //--------------------------------------------------------------------------
   bool empty() const { return (m_nNumItems==0); };


   //--------------------------------------------------------------------------
   // Operations
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   /// \brief Clear everything
   ///
   /// Clears out all items in the list
   /// 
   /// \return true on success
   //--------------------------------------------------------------------------
   bool clear()
   {
      while (m_pHead!=NULL)
      {
         removeItemNode(m_pHead);
      } // End while

      return true;
   }; // End of clear


   //--------------------------------------------------------------------------
   /// \brief Get first item
   ///
   /// Returns the first item in the list
   ///
   /// \return First item in list
   //--------------------------------------------------------------------------
   Type& front() 
   {
      SU_ASSERT(m_pHead!=NULL);
      return m_pHead->GetData();
   }; // End of front

   //--------------------------------------------------------------------------
   /// \brief Get first item
   ///
   /// Returns the first item in the list
   ///
   /// \return First item in list
   //--------------------------------------------------------------------------
   const Type& front() const 
   {
      SU_ASSERT(m_pHead!=NULL);
      return m_pHead->GetData();
   }; // End of front

   //--------------------------------------------------------------------------
   /// \brief Get last item in list
   ///
   /// Returns the last item in the list
   /// 
   /// \return Last item in list
   //--------------------------------------------------------------------------
   Type& back() 
   {
      SU_ASSERT(m_pTail!=NULL);
      return m_pTail->GetData();
   }; // End of front


   //--------------------------------------------------------------------------
   /// \brief Get last item in list
   ///
   /// Returns the last item in the list
   /// 
   /// \return Last item in list
   //--------------------------------------------------------------------------
   const Type& back() const 
   {
      SU_ASSERT(m_pTail!=NULL);
      return m_pTail->GetData();
   }; // End of front


   //--------------------------------------------------------------------------
   /// \brief Add new item to end
   ///
   /// Pushes a new item to the back of the list
   ///
   /// \param data New data to add
   /// \return true on success
   //--------------------------------------------------------------------------
   bool push_back( Type data )
   {
      ItemNode *pItemNode = new ItemNode(this,data);
      if (pItemNode==NULL)
         return false;

      if (m_pHead==NULL)
      {
         m_pHead = pItemNode;
         m_pTail = m_pHead;
      } // End if
      else
      {
         SU_ASSERT(m_pTail!=NULL);

         m_pTail->SetNext(pItemNode);
         pItemNode->SetPrev(m_pTail);
         m_pTail = pItemNode;
      } // End if

      m_nNumItems++;
      return true;
   }; // End of push_back

   //--------------------------------------------------------------------------
   /// \brief Add new item to front of list
   ///
   /// Pushes a new item to the front of the list
   /// 
   /// \param data New data to add
   /// \return true on success
   //--------------------------------------------------------------------------
   bool push_front( Type data )
   {
      ItemNode *pItemNode = new ItemNode(this,data);
      if (pItemNode==NULL)
         return false;

      if (m_pHead==NULL)
      {
         m_pHead = pItemNode;
         m_pTail = m_pHead;
      } // End if
      else
      {
         SU_ASSERT(m_pHead!=NULL);

         m_pHead->SetPrev(pItemNode);
         pItemNode->SetNext(m_pHead);
         m_pHead = pItemNode;
      } // End if

      m_nNumItems++;

      return true;
   }; // End of push_front

   //--------------------------------------------------------------------------
   /// \brief Pop front item
   ///
   /// Pops the first item and remove it from list
   ///
   /// \return true on success
   //--------------------------------------------------------------------------
   bool pop_front()
   {
      SU_ASSERT(m_nNumItems>0);
      SU_ASSERT(m_pHead!=NULL);

      removeItemNode(m_pHead);
      return true;
   }; // End pof pop_front

   //--------------------------------------------------------------------------
   /// \brief Pop last item
   ///
   /// Pops the last item and remove it from list
   ///
   /// \return true on success
   //--------------------------------------------------------------------------
   bool pop_back()
   {
      SU_ASSERT(m_nNumItems>0);
      SU_ASSERT(m_pTail!=NULL);

      removeItemNode(m_pTail);
      return true;
   }; // End pof pop_back

   //--------------------------------------------------------------------------
   /// \brief Remove item
   /// 
   /// Removes a specific item
   /// 
   /// \param data Data to remove
   /// \return true on success
   //--------------------------------------------------------------------------
   bool remove( Type data )
   {
      ItemNode  *pItemNode = m_pHead;
      while (pItemNode!=NULL)
      {
         if (pItemNode->GetData()==data)
         {
            pItemNode = removeItemNode(pItemNode);
         } // End if
         else
         {
            pItemNode = pItemNode->GetNext();
         } // End else
      } // End while

      return true;
   }; // End of remove

   //--------------------------------------------------------------------------
   /// \brief Insert item
   ///
   /// Inserts an item at a specific location
   ///
   /// \param where Iterator to specify location to insert.  
   ///        This function will insert item before where iterator
   /// \param data Data to insert
   /// \return iterator to the inserted item
   //--------------------------------------------------------------------------
   iterator insert( iterator where, const Type& data )
   {
      // make a new node
      ItemNode* pNewNode = new ItemNode(this,data);
      SU_ASSERT ( pNewNode );

      // figure out what node will go before the new one
      ItemNode* pWhereNode = where.m_pItemNode;
      ItemNode* pPrev;
      if( pWhereNode != NULL )
      {
         pPrev = pWhereNode->GetPrev();
      }
      else
      {
         // inserting after end of list -- change tail pointer
         pPrev = m_pTail;
         m_pTail = pNewNode;
      }


      // make that node point at the new node
      if( pPrev != NULL )
      {
         // inserting in middle of list
         pPrev->SetNext(pNewNode);
      }
      else
      {
         // inserting before head of list -- change head pointer
         m_pHead = pNewNode;
      }

      // make the node after the new node point at the new node
      if( pWhereNode != NULL )
      {
         pWhereNode->SetPrev(pNewNode);
      }

      // set the new node's pointers
      pNewNode->SetPrev(pPrev);
      pNewNode->SetNext(pWhereNode);

      // thats it, it's in there
      m_nNumItems++;
      iterator newNodeItr( this, pNewNode);

      return newNodeItr;
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
      ItemNode *pNextNode = removeItemNode(itr.m_pItemNode);
      iterator nextItr(this,pNextNode);
      return nextItr;
   }; // End of erase



   //--------------------------------------------------------------------------
   // Iterators
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
      iterator itr(this,m_pHead);
      return itr;
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
      const_iterator itr(const_cast<SuList*>(this),m_pHead);
      return itr;
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
      iterator itr(this,NULL);
      return itr;
   }; // End of end

   //--------------------------------------------------------------------------
   /// \brief End iterator
   ///
   /// Returns an iterator initializes to the beginning of the array
   ///
   /// \return Last item iterator
   //--------------------------------------------------------------------------
   const_iterator end() const
   {
      const_iterator itr(const_cast<SuList*>(this),NULL);
      return itr;
   }; // End of end

   

   //--------------------------------------------------------------------------
   /// \brief Copy list
   ///
   /// Copy Operator
   ///
   /// \param src Source list to copy from
   /// \return None
   //--------------------------------------------------------------------------
   void operator = ( const SuList<Type> &src )
   {
      ItemNode *pNode = src.m_pHead;
      while (pNode!=NULL)
      {
         push_back(pNode->GetData());
         pNode = pNode->GetNext();
      } // End while
   }; // End of operator =

   //--------------------------------------------------------------------------
   /// \brief Equality Comparison
   ///
   /// \param rSrc Source array to compare to
   /// \return True if the arrays are the same
   //--------------------------------------------------------------------------
   bool operator== ( const SuList<Type>& rSrc ) const
   {
      if( m_nNumItems != rSrc.m_nNumItems )
      {
         return false;
      }

      ItemNode* pNode = m_pHead;
      ItemNode* pOtherNode = rSrc.m_pHead;

      while( pNode )
      {
         SU_ASSERT( pOtherNode );
         if( pNode->GetData() != pOtherNode->GetData() )
         {
            return false;
         }

         pNode = pNode->GetNext();
         pOtherNode = pOtherNode->GetNext();
      }

      return true;
   }

   //--------------------------------------------------------------------------
   /// \brief Inequality Comparison
   ///
   /// \param rSrc Source array to compare to
   /// \return True if the arrays are dis-similar
   //--------------------------------------------------------------------------
   bool operator!= ( const SuList<Type>& rSrc ) const
   {
      return !(*this == rSrc);
   }


private :
   uint32     m_nNumItems; ///< Number of items
   ItemNode  *m_pHead;     ///< First item node
   ItemNode  *m_pTail;     ///< Lats item node

   //--------------------------------------------------------------------------
   /// \brief Remove item node
   ///
   /// Removes ItemNode from list and returns next item ( NULL, if nothing left )
   ///
   /// \param pItemNode Itemnode to remove
   /// \return Next itemnode ( NULL< if removed item was last item )
   //--------------------------------------------------------------------------
   ItemNode* removeItemNode( ItemNode *pItemNode )
   {
      SU_ASSERT(pItemNode!=NULL);
      SU_ASSERT(m_nNumItems>0);

      SU_ASSERT(pItemNode->GetParent()==this);

      ItemNode *pPrev = pItemNode->GetPrev();
      ItemNode *pNext = pItemNode->GetNext();

      // Link prev to next
      if (pPrev!=NULL)
         pPrev->SetNext(pNext);

      if (pNext!=NULL)
         pNext->SetPrev(pPrev);

      // If removing start/end of node
      bool bHead = (m_pHead == pItemNode) ? true : false;
      bool bTail = (m_pTail == pItemNode) ? true : false;

      m_nNumItems--;

      // Now free node
      delete pItemNode;

      if( bHead )
      {
         m_pHead = pNext;

      } // end if ( bHead )

      if( bTail )
      {
         m_pTail = pPrev;

      } // end if ( bTail )

      return pNext;

   } // End of removeItemNode

}; // End of SuList

#else // SU_USE_NATIVE_STL

//--------------------------------------------------------------------------
// STL WRAPPER MODE:
//    Wrap SuList to std:list to use native STL implemenation
//--------------------------------------------------------------------------
#include <list>
template <class T> class SuList : public std::list<T> {};

#endif // SU_USE_NATIVE_STL

#endif // End of _SULIST_H
