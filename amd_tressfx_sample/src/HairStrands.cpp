//---------------------------------------------------------------------------------------
// Example code that encapsulates three related objects.
// 1.  The TressFXHairObject
// 2.  An interface to get the current set of bones in world space that drive the hair object.
// 3.  An interface to set up for drawing the strands, such as setting lighting parmeters, etc.
//
// Normally, you'd probably contain the TressFXObject in the engine wrapper, but we've arranged it this 
// way to focus on the important aspects of integration.
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

#include "HairStrands.h"
#include "EngineTressFXObject.h"

#include "SuAnimatedModel.h"
#include "SkeletonInterface.h"

#include "TressFXSimulation.h"

#include "SushiGPUInterface.h"



HairStrands* HairStrands::Load(
    const SuString&   sushiModelName,
    const SuString&   tfxFilePath,
    const SuString&   tfxboneFilePath,
    const SuString&   hairObjectName,
    int               numFollowHairsPerGuideHair,
    float             tipSeparationFactor,
    SuGPUSamplingResourceViewPtr colorTextureView)
{
    HairStrands* pInstance = new HairStrands;
    if (pInstance)
    {
        pInstance->Initialize(sushiModelName, tfxFilePath, tfxboneFilePath, hairObjectName, numFollowHairsPerGuideHair, tipSeparationFactor, colorTextureView);
    }
    return pInstance;
}

    void HairStrands::Destroy(HairStrands* pInstance, EI_Device* pDevice)
    {
        SU_ASSERT(pInstance);

        pInstance->Destroy(pDevice);
        delete pInstance;
    }


    void HairStrands::TransitionSimToRendering(EI_CommandContextRef context)
    {
        m_pStrands->GetPosTanCollection().TransitionSimToRendering(context);
    }

    void HairStrands::TransitionRenderingToSim(EI_CommandContextRef context)
    {
        m_pStrands->GetPosTanCollection().TransitionRenderingToSim(context);
    }

    void HairStrands::UpdateBones(EI_CommandContextRef context)
    {
        const SuArray<SuMatrix4>& boneMatrices = m_pSkeleton->GetSkinningMatrices();
        const float*              pBoneMatricesInWS = (const float32*)boneMatrices[0];

        // update bone matrices for bone skinning of first two vertices of hair strands
        m_pStrands->UpdateBoneMatrices(context, pBoneMatricesInWS, 16 * sizeof(float) * boneMatrices.size());


    }

    void HairStrands::Simulate(EI_CommandContextRef context, TressFXSimulation* pSimulation)
    {
        pSimulation->Simulate(context, *m_pStrands);
    }




    HairStrands::HairStrands()
    {
        // m_pStrands = new TressFXHairObject;
    }
    HairStrands::~HairStrands()
    {
        //  delete m_pStrands;
        //  delete m_pEngineObjectWrapper;
    }

    void HairStrands::Initialize(
        const SuString&   sushiModelName,
        const SuString&   tfxFilePath,
        const SuString&   tfxboneFilePath,
        const SuString&   hairObjectName,
        int               numFollowHairsPerGuideHair,
        float             tipSeparationFactor,
        SuGPUSamplingResourceViewPtr colorTextureView)
    {
        EI_Device* pDevice = GetDevice();
        EI_CommandContextRef uploadCommandContext = GetContext();

        SkeletonInterface skeletonData;
        skeletonData.SetModel(sushiModelName);

        TressFXHairObject* hairObject = new TressFXHairObject;
        AMD::TressFXAsset* asset = new AMD::TressFXAsset();
        size_t memOffset = 0;

        // Load *.tfx 
        FILE * fp = fopen(tfxFilePath, "rb");
        asset->LoadHairData(fp);
        fclose(fp);

        asset->GenerateFollowHairs(numFollowHairsPerGuideHair, tipSeparationFactor, 1.2f);
        asset->ProcessAsset();

        // Load *.tfxbone
        fp = fopen(tfxboneFilePath, "rb");
        asset->LoadBoneData(skeletonData, fp);
        fclose(fp);


        // The color texture is only used as an srv.
        // But, TressFX only understands EI_Resource*, so we just wrap it in that.
        // As user of TressFX, we have defined EI_Resource to work with this.
        EI_Resource colorTextureWrapper;
        colorTextureWrapper.srv = colorTextureView;

        hairObject->Create(asset, pDevice, uploadCommandContext, hairObjectName, &colorTextureWrapper);

        m_pStrands = hairObject;
        m_pEngineObjectWrapper = new EngineTressFXObject(hairObjectName, hairObject);
        m_pSkeleton = dynamic_cast<SuAnimatedModel*>(SuObjectManager::GetRef().GetObjectByName(sushiModelName));

        delete asset;
    }

    void HairStrands::Destroy(EI_Device* pDevice)
    {
        SU_ASSERT(m_pStrands);
        m_pStrands->Destroy(pDevice);
        delete m_pStrands;
        m_pStrands = nullptr;

        // Object deleted by object manager, not here.
        m_pEngineObjectWrapper = nullptr;

        // m_pSkeketon is not owned by this object.
    }








