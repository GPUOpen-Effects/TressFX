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

#ifndef _SUCONSTANTBUFFER_H
#define _SUCONSTANTBUFFER_H

#include "SuGPUBuffer.h"

//---------------------------------------------------------------------------------------------------------------------------------
/// \brief System-independent representation of a GPU constant buffer.
///
/// \ingroup GPUResourceManager
/// 
/// SuConstantBuffer provides an abstract interface for system-specific GPU constant buffer resources.  
/// This kind of buffer is currently only supported on D3D10
//---------------------------------------------------------------------------------------------------------------------------------
class SuConstantBuffer : public SuGPUBuffer
{
protected:
   /// Constructor
   SU_DLL SuConstantBuffer( SuGPUResource::Behavior eBehavior, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags, uint32 nBufferSize );

   /// Can this buffer be used as a vertex buffer?
   virtual SU_DLL bool IsVertexBuffer() const { return false; };

   /// Can this buffer be used as an index buffer?
   virtual SU_DLL bool IsIndexBuffer() const { return false; };

public:
   /// Destructor
   virtual SU_DLL ~SuConstantBuffer();   

private:
};

#endif // _SUCONSTANTBUFFER_H
