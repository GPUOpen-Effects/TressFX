// ----------------------------------------------------------------------------
// Invokes simulation compute shaders.
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

#ifndef TRESSFXSIMULATION_H_
#define TRESSFXSIMULATION_H_

// AMD
#include "AMD_Types.h"
#include "TressFXCommon.h"

// TressFX
#include "TressFXGPUInterface.h"

class TressFXHairObject;

class TressFXSimulation : private TressFXNonCopyable
{
public:
    void Initialize(EI_Device* pDevice, EI_LayoutManagerRef simLayoutManager);
    void Shutdown(EI_Device* pDevice);

    void Simulate(EI_CommandContextRef commandContext, TressFXHairObject& hairObject);

private:
    // Maybe these just need to be compute shader references.
    EI_PSO* mVelocityShockPropagationPSO;
    EI_PSO* mIntegrationAndGlobalShapeConstraintsPSO;
    EI_PSO* mLocalShapeConstraintsPSO;
    EI_PSO* mLengthConstriantsWindAndCollisionPSO;
    EI_PSO* mUpdateFollowHairVerticesPSO;
};

#endif  // TRESSFXSIMULATION_H_
