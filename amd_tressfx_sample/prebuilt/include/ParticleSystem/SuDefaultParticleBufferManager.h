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

#ifndef _SU_DEFAULTPARTICLEBUFFERMANAGER_H_
#define _SU_DEFAULTPARTICLEBUFFERMANAGER_H_

#include "SuParticleBufferManager.h"

//=========================================================================================================
/// \ingroup ParticleSystem
/// \brief Default manager for dynamic particle buffers.
/// 
//=========================================================================================================
class SuDefaultParticleBufferManager : public SuParticleBufferManager
{
   /// Allocates a CPU buffer
   virtual SU_DLL void* AllocCPUBuffer( uint32 nSizeInBytes );

   /// Frees a CPU buffer
   virtual SU_DLL void ReleaseCPUBuffer( void* pBuffer );

   /// Allocates a GPU vertex buffer
   virtual SU_DLL SuGPUVertexBufferPtr AllocGPUVertexBuffer( uint32 nVertexSize, uint32 nNumVertices );

   /// Frees a CPU buffer
   virtual SU_DLL void ReleaseGPUVertexBuffer( SuGPUVertexBufferPtr pVB );
};

#endif // _SU_DEFAULTPARTICLEBUFFERMANAGER_H_
