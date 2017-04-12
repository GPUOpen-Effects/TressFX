
///---------------------------------------------------------------------------------------
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

#include "SDF.h"

#include "SkeletonInterface.h"
#include "SuTressFXBoneSkinning.h"
#include "SuTressFXSDFMarchingCubes.h"
#include "TressFXSDFCollision.h"





    void CollisionMesh::Initialize(
        const SuString&   name,
        const SuString&   tfxmeshFilePath,
        int               numCellsInXAxis,
        float             SDFCollMargin)
    {
        EI_Device* pDevice = GetDevice();
        EI_CommandContextRef uploadCommandContext = GetContext();

        SkeletonInterface skeletonData;
        skeletonData.SetModel(name);

        m_pBoneSkinning->SetModel(name);
        m_pBoneSkinning->LoadTressFXCollisionMeshData(&skeletonData, tfxmeshFilePath);
        m_pBoneSkinning->Initialize(pDevice, uploadCommandContext, name);

        m_pCollisionMesh->SetNumCellsInXAxis(numCellsInXAxis);
        m_pCollisionMesh->SetSDFCollisionMargin(SDFCollMargin);

        m_pCollisionMesh->Initialize(pDevice, m_pBoneSkinning, name);

#if ENABLE_MARCHING_CUBES
        m_pSDFMachingCubes->SetSDF(m_pCollisionMesh);
        m_pSDFMachingCubes->SetSDFIsoLevel(m_pCollisionMesh->GetSDFCollisionMargin());
        m_pSDFMachingCubes->Initialize(name);
#endif
    }

   CollisionMesh* CollisionMesh::Load(
        const SuString&   name,
        const SuString&   tfxmeshFilePath,
        int               numCellsInXAxis,
        float             SDFCollMargin)
    {
        CollisionMesh* pInstance = new CollisionMesh;
        if (pInstance)
        {
            pInstance->Initialize(name, tfxmeshFilePath, numCellsInXAxis, SDFCollMargin);
        }
        return pInstance;
    }


    void CollisionMesh::Destroy(CollisionMesh* pInstance, EI_Device* pDevice)
    {
        pInstance->Destroy(pDevice);
        delete pInstance;
    }

    void CollisionMesh::Destroy(EI_Device* pDevice)
    {
        m_pBoneSkinning->Destroy(pDevice);
#if ENABLE_MARCHING_CUBES
        m_pSDFMachingCubes->Destroy();
#endif
        m_pCollisionMesh->Destroy(pDevice);

    }

    void CollisionMesh::SkinTheMesh(EI_CommandContextRef context, double fTime)
    {
        m_pBoneSkinning->Update(context, fTime);
    }

    void CollisionMesh::AccumulateSDF(EI_CommandContextRef context, TressFXSDFCollisionSystem& sdfCollisionSystem)
    {
        m_pCollisionMesh->Update(context, sdfCollisionSystem);
    }

    void CollisionMesh::ApplySDF(EI_CommandContextRef context, TressFXSDFCollisionSystem& sdfCollisionSystem, TressFXHairHandle strands)
    {
        m_pCollisionMesh->CollideWithHair(context, sdfCollisionSystem, *strands);
    }

    void CollisionMesh::GenerateIsoSurface(EI_CommandContextRef context)
    {
        m_pSDFMachingCubes->Update(context);
    }
    void CollisionMesh::DrawIsoSurface(EI_CommandContextRef context)
    {
        (void)context;
        m_pSDFMachingCubes->Draw();
    }

    void CollisionMesh::DrawMesh(EI_CommandContextRef context)
    {
        m_pBoneSkinning->DrawMesh(context);
    }

    CollisionMesh::CollisionMesh()
    {
        m_pBoneSkinning = new SuTressFXBoneSkinning;
        m_pCollisionMesh = new TressFXSDFCollision;
        m_pSDFMachingCubes = new SuTressFXSDFMarchingCubes;
    }
    CollisionMesh::~CollisionMesh()
    {
        delete m_pCollisionMesh;
        delete m_pBoneSkinning;
#if ENABLE_MARCHING_CUBES
        delete m_pSDFMachingCubes;
#endif
    }







