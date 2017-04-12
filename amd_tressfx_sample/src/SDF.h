//---------------------------------------------------------------------------------------
// Example code for managing objects for signed-distance fields (SDFs)
//
// This includes the TressFXSDFCollision objects.  Associated with each is a system
// for skinning the model on the GPU (since that is input to TressFXSDFCollision) and 
// visualizing the SDFs using marching cubes.  The GPU skinning and marching cubes 
// systems could be packaged as library code as well, but are not there yet.
//
// The skinned meshes are loaded through this interface as well.
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



#define ENABLE_MARCHING_CUBES 1


#include "AMD_TressFX.h"

#include "SuString.h"


class TressFXSDFCollisionSystem;
class TressFXSDFCollision;
class SuTressFXBoneSkinning;
class SuTressFXSDFMarchingCubes;


class CollisionMesh
{
public:

    static CollisionMesh* Load(
        const SuString&   name,
        const SuString&   tfxmeshFilePath,
        int               numCellsInXAxis,
        float             SDFCollMargin);


    static void Destroy(CollisionMesh* pInstance, EI_Device* pDevice);


    void SkinTheMesh(EI_CommandContextRef context, double fTime);

    void AccumulateSDF(EI_CommandContextRef context, TressFXSDFCollisionSystem& sdfCollisionSystem);

    void ApplySDF(EI_CommandContextRef context, TressFXSDFCollisionSystem& sdfCollisionSystem, TressFXHairHandle strands);
    void GenerateIsoSurface(EI_CommandContextRef context);
    
    void DrawIsoSurface(EI_CommandContextRef context);

    void DrawMesh(EI_CommandContextRef context);


private:

    CollisionMesh();
    ~CollisionMesh();

    void Initialize(
        const SuString&   name,
        const SuString&   tfxmeshFilePath,
        int               numCellsInXAxis,
        float             SDFCollMargin);

    void Destroy(EI_Device* pDevice);

    TressFXSDFCollision* m_pCollisionMesh;
    SuTressFXBoneSkinning* m_pBoneSkinning;
    SuTressFXSDFMarchingCubes* m_pSDFMachingCubes;
};
