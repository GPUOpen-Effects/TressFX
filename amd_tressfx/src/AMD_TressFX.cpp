
//---------------------------------------------------------------------------------------
// Hooks interface to TressFX functionality.
//
// AMD_TressFX.h is the interface.
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

#if AMD_TRESSFX_COMPILE_DYNAMIC_LIB
#define AMD_DLL_EXPORTS
#endif

#include "AMD_TressFX.h"

#include "TressFXEngineInterface.h"

#include "TressFXAsset.h"
#include "TressFXHairObject.h"




namespace AMD
{
    TressFX_Callbacks_t g_Callbacks;


    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_GetCallbacks(TressFX_Callbacks** ppCallbacks)
    {
        if (ppCallbacks == nullptr)
        {
            return TRESSFX_RETURN_CODE_INVALID_POINTER;
        }

        *ppCallbacks = &g_Callbacks;

        return TRESSFX_RETURN_CODE_SUCCESS;
    }


    //--------------------------------------------------------------------------------------
    //
    // TressFX_GetVersion
    //
    // Gets the TressFX library version number.
    // This can be called before TressFX_Initialize.
    //
    //
    //--------------------------------------------------------------------------------------
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_GetVersion(uint* major, uint* minor, uint* patch)
    {
        if (major == nullptr || minor == nullptr || patch == nullptr)
        {
            return TRESSFX_RETURN_CODE_INVALID_POINTER;
        }

        *major = AMD_TRESSFX_VERSION_MAJOR;
        *minor = AMD_TRESSFX_VERSION_MINOR;
        *patch = AMD_TRESSFX_VERSION_PATCH;

        return TRESSFX_RETURN_CODE_SUCCESS;
    }



    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_GenerateFollowHairs(TressFXAsset& asset, int numFollowHairsPerGuideHair, float tipSeparationFactor, float maxRadiusAroundGuideHair)
    {
        if (numFollowHairsPerGuideHair < 0 || tipSeparationFactor < 0 || maxRadiusAroundGuideHair < 0)
            return TRESSFX_RETURN_CODE_INVALID_ARGUMENT;

        bool success = asset.GenerateFollowHairs(numFollowHairsPerGuideHair, tipSeparationFactor, maxRadiusAroundGuideHair);
        return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
    }

    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_ProcessAsset(TressFXAsset& asset)
    {
        bool success = asset.ProcessAsset();
        return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
    }

    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_LoadBoneData(TressFXAsset& asset, const TressFXSkeletonInterface& skeletonData, EI_StreamRef pStream)
    {
        bool success = asset.LoadBoneData(skeletonData, pStream);
        return (success ? TRESSFX_RETURN_CODE_SUCCESS : TRESSFX_RETURN_CODE_FAIL);
    }


    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_Create(
        AMD::TressFXAsset* asset,
        EI_Device*   pDevice,
        EI_CommandContextRef  commandContext,
        EI_LayoutManagerRef   layoutManager,
        EI_StringHash      name,
        EI_SRV        hairColorTextureSRV,
        /* output*/ TressFXHairHandle* pHandle)
    {
        // Anything defined by engine can be null (so engine can use NULL if it doesn't care)

        if (asset == nullptr)
        {
            TressFXLogWarning("Asset pointer is null. Can't create an object.");
            return TRESSFX_RETURN_CODE_INVALID_ARGUMENT;
        }

        TressFXHairObject* pObject = (TressFXHairObject*) EI_Malloc(sizeof(TressFXHairObject));
        if (pObject == nullptr)
            return TRESSFX_RETURN_CODE_OUT_OF_MEMORY;

        // TODO Create should return success.
        pObject->Create(asset, pDevice, commandContext, name, hairColorTextureSRV);

        *pHandle = pObject;

        return TRESSFX_RETURN_CODE_SUCCESS;
    }

    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_Destroy(TressFXHairHandle* pHandle, EI_Device* pDevice)
    {
        if (*pHandle == nullptr)
        {
            TressFXLogWarning("Invalide object in call to TressFX_Destroy.");
            return TRESSFX_RETURN_CODE_INVALID_ARGUMENT;
        }
        
        (*pHandle)->Destroy(pDevice);
        EI_Free(*pHandle);
        *pHandle = nullptr;

        return TRESSFX_RETURN_CODE_SUCCESS;
    }

    // pBoneMatricesInWS constains array of column major bone matrices in world space.
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE  TressFX_UpdateBoneMatrices(TressFXHairHandle handle, EI_CommandContextRef commandContext, const float* pBoneMatricesInWS, size_t sizeOfBoneMatrices)
    {
        TressFXHairObject* pHair = handle;
        if (pHair == nullptr)
        {
            TressFXLogWarning("Invalid hair object in call to TressFX_UpdateBoneMatrices.");
            return TRESSFX_RETURN_CODE_INVALID_ARGUMENT;
        }
        if (pBoneMatricesInWS == nullptr || sizeOfBoneMatrices == 0)
        {
            TressFXLogWarning("null ptr for bone matrices, or zero size in call to TressFX_UpdateBoneMatrices.");
            return TRESSFX_RETURN_CODE_INVALID_ARGUMENT;
        }

        pHair->UpdateBoneMatrices(commandContext, pBoneMatricesInWS, sizeOfBoneMatrices);
        return TRESSFX_RETURN_CODE_SUCCESS;
    }

    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE  TressFX_UpdateSimulationParameters(TressFXHairHandle handle, const TressFXSimulationSettings& parameters)
    {
        TressFXHairObject* pHair = handle;
        if (pHair == nullptr)
        {
            TressFXLogWarning("Invalid hair object in call to TressFX_UpdateSimulationParameters.");
            return TRESSFX_RETURN_CODE_INVALID_ARGUMENT;
        }
        pHair->UpdateSimulationParameters(parameters);
        return TRESSFX_RETURN_CODE_SUCCESS;
    }


    // Rendering
    AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE  TressFX_DrawStrands(TressFXHairHandle handle, EI_CommandContextRef commandContext,
        EI_PSO&            pso)
    {
        TressFXHairObject* pHair = handle;
        if (pHair == nullptr)
        {
            TressFXLogWarning("Invalid hair object in call to TressFX_DrawStrands.");
            return TRESSFX_RETURN_CODE_INVALID_ARGUMENT;
        }
        pHair->DrawStrands(commandContext, pso);
        return TRESSFX_RETURN_CODE_SUCCESS;
    }



};

