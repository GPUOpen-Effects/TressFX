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

#ifndef _SU_RENDERINGPREDICATE_H_
#define _SU_RENDERINGPREDICATE_H_


/// Types of supported rendering predicates
// tolua_begin
enum SuRenderingPredicateType
{
   SU_STREAMOUT_OVERFLOW_PREDICATE, ///< A true value indicates that a stream out buffer overflow occurred during the query period
   SU_VISIBILITY_PREDICATE          ///< A true value indicates that at least one sample passed the Z-stencil test during the query period.  False indicates that none passed
};
// tolua_end

//=================================================================================================================================
/// \ingroup RenderManager
/// \brief An API-agnostic interface for a rendering predicate
/// 
//=================================================================================================================================
// tolua_begin
class SuRenderingPredicate
{
public:

   /// Default constructor for rendering predicates
   SU_DLL SuRenderingPredicate( SuRenderingPredicateType eType, bool bIsHint );

   virtual SU_DLL ~SuRenderingPredicate() {};

   //=================================================================================================================================
   /// \brief Called to mark the beginning of the query period
   /// \return True if successful, false on error
   //=================================================================================================================================
   virtual bool Begin() = 0;

   //=================================================================================================================================
   /// \brief Called to mark the end of the query period
   /// \return True if successful, false on error
   //=================================================================================================================================
   virtual bool End() = 0;

   //=================================================================================================================================
   /// Method that should be called after construction to perform API-specific initialization for the predicate
   /// \return True if successful, false on error.  If errors occur, the caller should immediately destroy the predicate
   //=================================================================================================================================
   virtual bool Create() = 0;

   //=================================================================================================================================
   /// \brief Checks if the predicate result is available, and returns it if it is.
   ///    This method is used to poll the GPU for availability of results, without causing a stall.
   /// \param pResultOut  Pointer to a boolean which is set to the result of the predicate.  If results are unavailable, this is unmodified.  
   ///                     May be NULL, in which case this method simply checks for availability of results
   /// \return True if results are available, false otherwise
   //=================================================================================================================================
   virtual bool CheckResult( bool* pResultOut ) = 0;
   
   //=================================================================================================================================
   /// \brief  Retrieves the predicate result, synchronizing with the GPU if necessary
   /// \return The result of the predicate
   //=============================================================================================================================
   virtual bool GetResultSynchronized() = 0;

   //=================================================================================================================================
   /// \brief Tells whether or not the predicate is a hint.  
   /// A hint predicate is an optional predicate, which does not force the GPU to synchronize if it doesn't want to
   /// \return True if the predicate is a hint.  False otherwise
   //=================================================================================================================================
   inline bool IsPredicateHint() const { return m_bIsHint; };

   //=================================================================================================================================
   /// \brief Returns the type of predicate
   //=================================================================================================================================
   inline SuRenderingPredicateType GetPredicateType() const { return m_ePredicateType; };

// tolua_end
private:

   bool m_bIsHint;
   SuRenderingPredicateType m_ePredicateType;

// tolua_begin
};
// tolua_end


#endif // _SU_RENDERINGPREDICATE_H_
