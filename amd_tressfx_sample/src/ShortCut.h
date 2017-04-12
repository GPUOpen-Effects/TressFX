//---------------------------------------------------------------------------------------
// Example code for running the shortcut method.
//-------------------------------------------------------------------------------------
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


class FullscreenPass;
class TressFXPPLL;

#include "AMD_TressFX.h"

#include "SuTypes.h"
#include "SuArray.h"
#include "SuGPUResource.h"


#include <vector>

class HairStrands;
class SuEffect;
class TressFXShortCut;

class ShortCut
{
public:
    ShortCut();
    ~ShortCut();

    void Initialize(int width, int height, SuEffect* pStrandEffect, SuEffect* pQuadEffect);
    void Shutdown(EI_Device* pDevice);

    void Draw(EI_CommandContextRef commandContext, std::vector<HairStrands*>& hairStrands);

private:
    TressFXShortCut*     m_pTressFXShortCut;
    FullscreenPass* m_pFullscreenPass;

    EI_PSO* m_pDepthsAlphaPSO;
    EI_PSO* m_pFillColorsPSO;
    EI_PSO* m_pResolveDepthPSO;
    EI_PSO* m_pResolveColorPSO;

    // color and depth buffer will be used, replaced, restored by shortcut.
    SuGPURenderableResourceViewPtr   m_pBackBufferView;
    SuGPUDepthStencilResourceViewPtr m_pDepthBufferView;
    void GetBackandDepthBufferViews();
};

