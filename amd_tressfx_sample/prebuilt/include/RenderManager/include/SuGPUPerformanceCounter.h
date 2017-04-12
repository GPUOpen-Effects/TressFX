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

#ifndef _SU_GPUPERFORMANCECOUNTER_H_
#define _SU_GPUPERFORMANCECOUNTER_H_

/// Enum describing the supported types of GPU performance counters
// tolua_begin
enum SuGPUCounterType
{
   SU_GPU_IDLE_COUNTER,       ///< Fraction of GPU Idle time
   SU_GPU_VS_COUNTER,         ///< Fraction of time VS was busy
   SU_GPU_GS_COUNTER,         ///< Fraction of time GS was busy
   SU_GPU_PS_COUNTER,         ///< Fraction of time PS was busy
   SU_GPU_VS_ALU_COUNTER,     ///< Fraction of time that VS was executing ALU ops.  TODO:  Ask Zack about these, that might not be correct
   SU_GPU_GS_ALU_COUNTER,     ///< Fraction of time that GS was executing ALU ops
   SU_GPU_PS_ALU_COUNTER,     ///< Fraction of time that PS was executing ALU ops
   SU_GPU_VCACHE_HIT_COUNTER, ///< Post transform cache hit rate
   SU_GPU_TCACHE_HIT_COUNTER, ///< Texture cache hit rate
   SU_GPU_BUS_BW_COUNTER,     ///< Fraction of peak bus bandwidth being used
   SU_GPU_VRAM_BW_COUNTER,    ///< Fraction of GPU memory bandwidth being used
   SU_GPU_VERTEX_THROUGHPUT_COUNTER,   ///< Fraction of theoretical vertex throughput used.  I do not know what this is supposed to mean (vfetch bw?)
   SU_GPU_SETUP_THROUGHPUT_COUNTER,    ///< Fraction of threoretical tri setup throughput used
   SU_GPU_FILLRATE_THROUGHPUT_COUNTER  ///< Fraction of theroetical fillrate used.  (framebuffer bw?)
   
   // please do not change the order of these enum elements!!!
};
// tolua_end

//=================================================================================================================================
/// \ingroup RenderManager
/// \brief A counter which gathers GPU related performance data.  
///
///  This counter subclass is an API-agnostic wrapper for the D3D10 performance counting API.  D3D9 or OGL support could, in theory,
///  be added, possibly by adding engine support for UniPerf.
/// 
//=================================================================================================================================
// tolua_begin
class SuGPUPerformanceCounter
{
public:

   /// Default constructor for GPU performance counters
   inline SuGPUPerformanceCounter( SuGPUCounterType eType ) : m_eType( eType ) {};

   /// Destructor
   virtual SU_DLL ~SuGPUPerformanceCounter() {};

   /// Returns the type of this performance counter
   inline SuGPUCounterType GetType() const { return m_eType; };

   /// \brief Allocates GPU resources for this counter
   /// This must be called after construction to initialize the counter before use
   /// \return True if counter initialization succeeded.  False otherwise.
   virtual bool Create() = 0;

   /// Begins counting
   ///
   /// Performance data is collected by the counter during the period between a Begin() call and a subsequent End() call
   virtual bool Begin() = 0;

   /// Ends counting
   ///
   /// Performance data is collected by the counter during the period between a Begin() call and a subsequent End() call
   virtual bool End() = 0;

   /// \brief Retrieves query results, blocking until results are available. 
   /// \return A counter specific float32 value.  Usually the fraction of time spent doing something
   virtual float32 GetResultSynchronized() = 0;


   /// \brief Polls the for the availability of counter results.
   ///
   /// This method checks to see if query results are available, and retrieves them if they are.  If results are not available
   /// then this method returns immediately
   ///
   /// \param  pResult  Pointer to a variable which will receive the result, if available.  The variable is not modified 
   ///                  if results are not available.  If NULL, then no result is returned.
   /// \return True if counter results were available.  False otherwise
   virtual bool CheckResult( float32* pResult ) = 0;

// tolua_end
private:
   SuGPUCounterType m_eType;  ///< The type of performance counter

// tolua_begin
};
// tolua_end


#endif // _SU_GPUPERFORMANCECOUNTER_H_
