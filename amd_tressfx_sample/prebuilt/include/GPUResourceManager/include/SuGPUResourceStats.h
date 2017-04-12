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

#ifndef _SU_GPURESOURCESTATS_H_
#define _SU_GPURESOURCESTATS_H_


// tolua_begin
enum SuGPUResourceStatField
{
   SU_STAT_CBCOUNT,        ///< Number of constant buffers
   SU_STAT_CBSIZE,         ///< Total size of constant buffers (bytes)

   SU_STAT_VBCOUNT,        ///< Number of vertex buffers
   SU_STAT_VBSIZE,         ///< Total size of vertex buffers (bytes)

   SU_STAT_IBCOUNT,        ///< Number of index buffers
   SU_STAT_IBSIZE,         ///< Total size of index buffers (bytes)
   
   SU_STAT_TEX2DCOUNT,     ///< Number of textures
   SU_STAT_TEX2DSIZE,      ///< Total size of textures (bytes)
   
   SU_STAT_TEXCUBECOUNT,   ///< Number of cube textures
   SU_STAT_TEXCUBESIZE,    ///< Total size of cube textures (bytes)
         
   SU_STAT_TEX3DCOUNT,     ///< Number of 3D textures
   SU_STAT_TEX3DSIZE,      ///< Total size of 3D textures (bytes)

   SU_STAT_DEPTHCOUNT,     ///< Number of depth-stencil resources
   SU_STAT_DEPTHSIZE,      ///< Total size of depth-stencil resources

   SU_STAT_RENDERABLECOUNT, ///< Number of renderable resources
   SU_STAT_RENDERABLESIZE,  ///< Total size of renderable resources (bytes)

   SU_STAT_SBCOUNT,         ///< Number of structured buffer resources
   SU_STAT_SBSIZE,          ///< Total size of structured buffers (bytes)


   SU_RESOURCE_STAT_COUNT
};
// tolua_end


//=================================================================================================================================
/// \ingroup GPUResourceManager
/// \brief An object that is responsible for keeping statistics about the allocated GPU resources
/// 
//=================================================================================================================================
// tolua_begin
class SuGPUResourceStats
{
public:

   SU_DLL SuGPUResourceStats();

   /// Returns the value of a counter
   SU_DLL uint32 GetValue( SuGPUResourceStatField eStat ) const;

   /// Increases the value of a counter
   SU_DLL void IncrementValue( SuGPUResourceStatField eStat, uint32 nAmount=1 );

   /// Decreases the value of a counter
   SU_DLL void DecrementValue( SuGPUResourceStatField eStat, uint32 nAmount=1 );

   /// Helper method which computes total GPU memory usage
   SU_DLL uint32 GetGPUMemoryUsage() const;

// tolua_end
private:

   /// Counter values
   uint32 m_nCounters[SU_RESOURCE_STAT_COUNT];

// tolua_begin
};
// tolua_end


#endif // _SU_GPURESOURCESTATS_H_
