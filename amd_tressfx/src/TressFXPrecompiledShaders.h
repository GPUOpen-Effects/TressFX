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

#ifndef AMD_TRESSFX_PRECOMPILED_SHADERS_H
#define AMD_TRESSFX_PRECOMPILED_SHADERS_H

// TressFXRenderer Vertex Shaders
#include "shaders\Inc\VS_RenderHair.inc"
#include "shaders\Inc\VS_RenderHair_AA.inc"
#include "shaders\Inc\VS_GenerateHairSM.inc"
#include "shaders\Inc\VS_RenderHair_StrandCopies.inc"
#include "shaders\Inc\VS_RenderHair_AA_StrandCopies.inc"
#include "shaders\Inc\VS_ScreenQuad.inc"

// TressFXRenderer Pixel Shaders
#include "shaders\Inc\PS_ABuffer_Hair.inc"
#include "shaders\Inc\PS_KBuffer_Hair.inc"

#include "shaders\Inc\PS_Depth_Hair.inc"
#include "shaders\Inc\PS_FillColors_Hair.inc"
#include "shaders\Inc\PS_ResolveDepth_Hair.inc"
#include "shaders\Inc\PS_ResolveColor_Hair.inc"

// TressFXSimulation Compute Shaders
#include "shaders\Inc\IntegrationAndGlobalShapeConstraints.inc"
#include "shaders\Inc\ApplyHairTransformGlobally.inc"
#include "shaders\Inc\ComputeTangents.inc"
#include "shaders\Inc\LocalShapeConstraints.inc"
#include "shaders\Inc\LocalShapeConstraintsWithIteration.inc"
#include "shaders\Inc\LengthConstraintsWindAndCollision.inc"
#include "shaders\Inc\UpdateFollowHairVertices.inc"
#include "shaders\Inc\PrepareFollowHairBeforeTurningIntoGuide.inc"
#include "shaders\Inc\GenerateTransforms.inc"

#endif
