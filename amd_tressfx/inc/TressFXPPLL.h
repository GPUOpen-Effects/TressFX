// ----------------------------------------------------------------------------
// Interface for TressFX OIT using per-pixel linked lists.
// ----------------------------------------------------------------------------
//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef AMD_TRESSFXPPLL_H
#define AMD_TRESSFXPPLL_H


#include "TressFXGPUInterface.h"

// By default, TressFX node sizes are 16 bytes.  4 bytes for each of:  tangentcoverage, depth,
// baseColor, next pointer
#define TRESSFX_DEFAULT_NODE_SIZE 16

// The special value meaning "end of list" in the PPLL.
// So the UAV of head pointers gets cleared to this.
#define TRESSFX_PPLL_NULL_PTR 0xffffffff

// This is the interface the engine uses to call TressFX at a high level.
// There are basically two steps each frame.
// In the first, hair strands are rendered, accumulating the fragments in
// the PPLL:
//    BindForBuild()
//    [Draws hair]
//    DoneBuilding()
//
// In the second, the fragments are gathered and blended into the backbuffer.
//    BindForRead()
//    [Draw screen pass]
//    DoneReading()
//
class TressFXPPLL
{
public:
    TressFXPPLL();
    ~TressFXPPLL();

    // node size should include room for UINT next pointer.
    bool Create(EI_Device* pDevice,
                int              width,
                int              height,
                int              nNodes,
                int              nodeSize = TRESSFX_DEFAULT_NODE_SIZE);
    void Destroy(EI_Device* context);

    void Clear(EI_CommandContextRef context);


    // These might get removed.
    // Do we want to gather the bindings to be sent for each
    // draw, or just bind once?
    void BindForBuild(EI_CommandContextRef commandContext);
    void BindForRead(EI_CommandContextRef context);

    // The "dones" should not require a layout if it's just a transition,
    // but this is an opportunity for unbinding.
    void DoneBuilding(EI_CommandContextRef context);
    void DoneReading(EI_CommandContextRef context);

private:
    int m_nScreenWidth;
    int m_nScreenHeight;
    int m_nNodes;
    int m_nNodeSize;

    // We track whether the current PPLL is valid
    // so the engine implementation doesn't have to.
    bool m_bCreated;

    EI_RWTexture2D*      m_pHeads;
    EI_StructuredBuffer* m_pNodes;

    void CreateBuildBindSet(EI_Device* pDevice);
    void CreateReadBindSet(EI_Device* pDevice);


    EI_BindSet* m_pPPLLBuildBindSet;
    EI_BindSet* m_pPPLLReadBindSet;
};


#endif
