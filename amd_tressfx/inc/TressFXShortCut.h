
// ----------------------------------------------------------------------------
// Interface for the shortcut method.
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

#ifndef AMD_TRESSFXSHORTCUT_H
#define AMD_TRESSFXSHORTCUT_H


#include "TressFXGPUInterface.h"

#define TRESSFX_SHORTCUT_DEPTH_NODE_SIZE 4
#define TRESSFX_SHORTCUT_K 3
#define TRESSFX_SHORTCUT_INITIAL_DEPTH 0x3f800000

// This is the interface the engine uses to call TressFX at a high level.
// There are basically two steps each frame.
// In the first, hair strands are rendered, accumulating the fragments in
// the KBuffer:
//    BindForBuild()
//    [Draws hair]
//    DoneBuilding()
//
// In the second, the fragments are gathered and blended into the backbuffer.
//    BindForRead()
//    [Draw screen pass]
//    DoneReading()
//
class TressFXShortCut
{
public:
    TressFXShortCut();
    ~TressFXShortCut();

    // node size should include room for UINT next pointer.
    bool Create(EI_Device* pDevice, int width, int height);
    void Destroy(EI_Device* context);

    void Clear(EI_CommandContextRef context);


    // These might get removed.
    // Do we want to gather the bindings to be sent for each
    // draw, or just bind once?
    void BindForDepthsAlpha(EI_CommandContextRef context);
    void BindForResolveDepth(EI_CommandContextRef context);
    void BindForFillColors(EI_CommandContextRef context);
    void BindForResolveColor(EI_CommandContextRef context);

    // The "dones" should not require a layout if it's just a transition,
    // but this is an opportunity for unbinding.
    void DoneDepthsAlpha(EI_CommandContextRef context);
    void DoneResolveDepth(EI_CommandContextRef context);
    void DoneFillColors(EI_CommandContextRef context);
    void DoneResolveColor(EI_CommandContextRef context);

    EI_RWTexture2D* GetDepthsTexture() { return m_pDepths; }
    EI_RWTexture2D* GetInvAlphaTexture() { return m_pInvAlpha; }
    EI_RWTexture2D* GetColorsTexture() { return m_pColors; }

private:
    int m_nScreenWidth;
    int m_nScreenHeight;

    // We track whether the current KBuffer is valid
    // so the engine implementation doesn't have to.
    bool m_bCreated;

    EI_RWTexture2D* m_pDepths;
    EI_RWTexture2D* m_pInvAlpha;
    EI_RWTexture2D* m_pColors;

    void CreateDepthsAlphaBindSet(EI_Device* pDevice);
    void CreateResolveDepthBindSet(EI_Device* pDevice);
    void CreateFillColorsBindSet(EI_Device* pDevice);
    void CreateResolveColorBindSet(EI_Device* pDevice);


    EI_BindSet* m_pShortCutDepthsAlphaBindSet;

    // These two are the same. Merge?
    EI_BindSet* m_pShortCutResolveDepthBindSet;
    EI_BindSet* m_pShortCutFillColorsBindSet;

    EI_BindSet* m_pShortCutResolveColorBindSet;
};


#endif
