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

#ifndef AMD_TRESSFX_OPAQUE_H
#define AMD_TRESSFX_OPAQUE_H

#include "TressFXAsset.h"
#include "TressFXMesh.h"
#include "TressFXRenderer.h"
#include "TressFXSimulation.h"

namespace AMD
{

struct TressFX_OpaqueDesc
{
public:
    TressFX_OpaqueDesc() : initialized(false), refCount(0) {}
    void Initialize(TressFX_Desc & desc);
    void Release();

    bool LoadAppendAsset(TressFX_HairBlob *pRawHairBlob, const TressFX_GuideFollowParams& guideFollowParams, int groupId);
    bool CreateProcessedAsset(TressFX_Desc &desc, TressFX_HairBlob **ppHairBlob, TressFX_SceneMesh *sceneMesh, ID3D11ShaderResourceView *hairTexture);

    bool Begin(TressFX_Desc & desc);
    bool End(TressFX_Desc & desc);
    bool RenderShadowMap(TressFX_Desc & desc);
    bool RenderHair(TressFX_Desc & desc);
    bool Simulate(TressFX_Desc & desc, float elapsedTime);
    bool GenerateTransforms(TressFX_Desc & desc, TressFX_SceneMesh &sceneMesh);
    bool ApplyRigidTransforms(TressFX_Desc & desc);
    bool Resize(TressFX_Desc & desc);

private:
    bool                initialized;            // only initialize this structure once
    TressFXRenderer     tressFXRenderer;        // Hair rendering class
    TressFXSimulation   tressFXSimulation;      // Hair simulation class
    TressFXAssetLoader  tressFXAssetLoader;     // Hair asset loading class
    int                 refCount;               // reference count - delete allocations when 0
};

} // namespace AMD

#endif
