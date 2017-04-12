//---------------------------------------------------------------------------------------
// Constant buffer layouts.
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

// Contains data typically passed through constant buffers.


struct TressFXSimulationConstantBuffer
{
    AMD::TRESSFX::float4 m_Wind;
    AMD::TRESSFX::float4 m_Wind1;
    AMD::TRESSFX::float4 m_Wind2;
    AMD::TRESSFX::float4 m_Wind3;

    AMD::TRESSFX::float4 m_Shape;  // damping, local stiffness, global stiffness, global range.
    // float m_Damping;
    // float m_StiffnessForLocalShapeMatching;
    // float m_StiffnessForGlobalShapeMatching;
    // float m_GlobalShapeMatchingEffectiveRange;

    AMD::TRESSFX::float4 m_GravTimeTip;  // gravity, time step size,
    // float m_GravityMagnitude;
    // float m_TimeStep;
    // float m_TipSeparationFactor;
    // float m_velocityShockPropogation;

    AMD::TRESSFX::sint4 m_SimInts;  // 4th component unused.
    // int m_NumLengthConstraintIterations;
    // int m_NumLocalShapeMatchingIterations;
    // int m_bCollision;
    // int m_CPULocalIterations;

    AMD::TRESSFX::sint4 m_Counts;
    // int m_NumOfStrandsPerThreadGroup;
    // int m_NumFollowHairsPerGuideHair;
    // int m_NumVerticesPerStrand; // should be 2^n (n is integer and greater than 2) and less than
    // or equal to TRESSFX_SIM_THREAD_GROUP_SIZE. i.e. 8, 16, 32 or 64

    AMD::TRESSFX::float4 m_VSP;

    AMD::TRESSFX::float4x4 m_BoneSkinningMatrix[AMD_TRESSFX_MAX_NUM_BONES];

#if TRESSFX_COLLISION_CAPSULES
    AMD::TRESSFX::float4 m_centerAndRadius0[TRESSFX_MAX_NUM_COLLISION_CAPSULES];
    AMD::TRESSFX::float4 m_centerAndRadius1[TRESSFX_MAX_NUM_COLLISION_CAPSULES];
    AMD::TRESSFX::sint4 m_numCollisionCapsules;
#endif


    void SetDamping(float d) { m_Shape.x = d; }
    void SetLocalStiffness(float s) { m_Shape.y = s; }
    void SetGlobalStiffness(float s) { m_Shape.z = s; }
    void SetGlobalRange(float r) { m_Shape.w = r; }

    void SetGravity(float g) { m_GravTimeTip.x = g; }
    void SetTimeStep(float dt) { m_GravTimeTip.y = dt; }
    void SetTipSeperation(float ts) { m_GravTimeTip.z = ts; }

    void SetVelocityShockPropogation(float vsp) { m_VSP.x = vsp; }
    void SetVSPAccelThreshold(float vspAccelThreshold) { m_VSP.y = vspAccelThreshold; }

    void SetLengthIterations(int i) { m_SimInts.x = i; }
    void SetLocalIterations(int i) { m_SimInts.y = i; }
    void SetCollision(bool on) { m_SimInts.z = on ? 1 : 0; }

    void SetVerticesPerStrand(int n)
    {
        m_Counts.x = TRESSFX_SIM_THREAD_GROUP_SIZE / n;
        m_Counts.z = n;
    }
    void SetFollowHairsPerGuidHair(int n) { m_Counts.y = n; }
};

struct TressFXCapsuleCollisionConstantBuffer
{
    AMD::TRESSFX::float4 m_centerAndRadius[TRESSFX_MAX_NUM_COLLISION_CAPSULES];
    AMD::TRESSFX::float4 m_centerAndRadiusSquared[TRESSFX_MAX_NUM_COLLISION_CAPSULES];
    int m_numCollisionCapsules;
};

struct TressFXSDFCollisionConstantBuffer
{
    AMD::TRESSFX::float4 m_Origin;
    float                m_CellSize;
    int                  m_NumCellsX;
    int                  m_NumCellsY;
    int                  m_NumCellsZ;
    int                  m_MaxMarchingCubesVertices;
    float                m_MarchingCubesIsolevel;
    float                m_CollisionMargin;
    int                  m_NumHairVerticesPerStrand;
    int                  m_NumTotalHairVertices;
    float                pad1;
    float                pad2;
    float                pad3;
};

struct TressFXRenderingConstantBuffer
{
    AMD::int32 m_NumVerticesPerStrand;
    AMD::int32 pad[3];
};
