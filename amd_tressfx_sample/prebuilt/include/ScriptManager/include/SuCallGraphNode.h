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

#ifndef _SU_CALLGRAPHNODE_H_
#define _SU_CALLGRAPHNODE_H_


//=================================================================================================================================
/// \ingroup ScriptManager
/// \brief A class to represent a node in a call graph
/// 
//=================================================================================================================================
class SuCallGraphNode
{
public:

   /// Constructor for call graph nodes
   SU_DLL SuCallGraphNode( const SuString& rFunction ) ;

   /// Returns the name of the function that this node represents
   inline const SuString& GetFunctionName() const { return m_function; };
   
   /// Returns the number of times this function has been called
   inline uint32 GetNumCalls() const { return m_nCalls; };

   /// Returns the time spent in this function itself (may include calls to C)
   inline SuTime GetSelfTime() const { return m_fSelfTime; };

   /// Returns the total time spent in this function or a child
   inline SuTime GetTotalTime() const { return m_fSelfTime + m_fChildTime; };

   /// Increments the number of calls to this node
   inline void IncrementCalls() { m_nCalls++; };

   /// Increments the time spent in this node
   inline void IncrementSelfTime( SuTime fTime ) { m_fSelfTime += fTime; };

   /// Increments the time spent in a child of this node
   inline void IncrementChildTime( SuTime fTime ) { m_fChildTime += fTime; };

private:

   SuString m_function;
   uint32 m_nCalls;
   SuTime m_fSelfTime;
   SuTime m_fChildTime;
};


#endif // _SU_CALLGRAPHNODE_H_
