//---------------------------------------------------------------------------------------
// Example code for managing simulation
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

#pragma once

#include "TressFXSDFCollision.h"
#include "TressFXSimulation.h"
#include <vector>

#include "SuEffect.h"

#include "SushiUtils.h"

class HairStrands;
class CollisionMesh;


class Simulation
{
public:
    Simulation();
    ~Simulation();

    //void Initialize();
    //void Shutdown();

    void StartSimulation(
        double fTime,
        std::vector<HairStrands*>& hairStrands,
        std::vector<CollisionMesh*>& collisionMeshes,
        bool bUpdateCollMesh, bool bSDFCollisionResponse
    );

    void WaitOnSimulation();

private:

    void UpdateCollisionMesh(EI_CommandContextRef simContext, std::vector<CollisionMesh*>& collisionMeshes);
    void RunCollision(EI_CommandContextRef simContext, std::vector<HairStrands*>& hairStrands, std::vector<CollisionMesh*>& collisionMeshes);
    void RunSimulation(EI_CommandContextRef simContext, std::vector<HairStrands*>& hairStrands);

    TressFXSimulation        mSimulation;
    TressFXSDFCollisionSystem mSDFCollisionSystem;

    SuEffectPtr m_pTressFXSimEffect;
    SuEffectPtr m_pTressFXSDFCollisionEffect;
    AsyncControl m_ComputeQueue;

};

