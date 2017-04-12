//---------------------------------------------------------------------------------------
// Main header file for TressFX
// This will eventually contain the C interface to all functionality.
//
// In the meantime, users need to use the individual headers for the components they 
// require.
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

#ifndef AMD_TRESSFX_H
#define AMD_TRESSFX_H

#define AMD_TRESSFX_VERSION_MAJOR                    4
#define AMD_TRESSFX_VERSION_MINOR                    0
#define AMD_TRESSFX_VERSION_PATCH                    0

// default to static lib
#ifndef AMD_TRESSFX_COMPILE_DYNAMIC_LIB
#define AMD_TRESSFX_COMPILE_DYNAMIC_LIB   0
#endif

#if AMD_TRESSFX_COMPILE_DYNAMIC_LIB
#    ifdef AMD_DLL_EXPORTS
#        define AMD_TRESSFX_DLL_API __declspec(dllexport)
#    else // AMD_DLL_EXPORTS
#        define AMD_TRESSFX_DLL_API __declspec(dllimport)
#    endif // AMD_DLL_EXPORTS
#else // AMD_TRESSFX_COMPILE_DYNAMIC_LIB
#    define AMD_TRESSFX_DLL_API
#endif // AMD_TRESSFX_COMPILE_DYNAMIC_LIB

#include "AMD_Types.h"

#if defined(DEBUG) || defined(_DEBUG)
#define AMD_TRESSFX_DEBUG                       1
#endif

#ifndef TRESSFX_STRING_TYPE
typedef const char* EI_StringHash;
#define TRESSFX_STRING_TYPE 1
#endif


#ifndef EI_STREAM
typedef void EI_Stream;
typedef void* EI_StreamRef;
#define EI_STREAM 1
#endif

#ifndef TRESSFX_ASSERT
#include <assert.h>
#define TRESSFX_ASSERT assert
#endif


// Max number of bones in a skeleton
#ifndef AMD_TRESSFX_MAX_NUM_BONES
#define AMD_TRESSFX_MAX_NUM_BONES 256
#endif 

#include "TressFXBoneSkeletonInterface.h"

static const AMD::uint32 TRESSFX_INDEX_SIZE = 4;

class EI_Resource;
class EI_Device;
class EI_CommandContext;      // ID3D11DeviceContext
class EI_LayoutManager;
struct EI_BindLayout;
class EI_BindSet;
class EI_PSO;
class EI_IndexBuffer;


typedef EI_CommandContext& EI_CommandContextRef;
typedef EI_Resource& EI_StructuredBufferRef;
typedef EI_LayoutManager& EI_LayoutManagerRef;

typedef EI_Resource* EI_UAV;
typedef EI_Resource* EI_SRV;

class TressFXAsset;

class TressFXHairObject;
typedef TressFXHairObject* TressFXHairHandle;

namespace AMD
{

    // The Return codes
    enum TRESSFX_RETURN_CODE
    {
        TRESSFX_RETURN_CODE_SUCCESS,
        TRESSFX_RETURN_CODE_FAIL,
        TRESSFX_RETURN_CODE_INVALID_ARGUMENT,
        TRESSFX_RETURN_CODE_INVALID_POINTER,
        TRESSFX_RETURN_CODE_OUT_OF_MEMORY,

        TRESSFX_RETURN_CODE_COUNT,
    };

    enum EI_ShaderStage
    {
        EI_UNINITIALIZED = 0,  // we will always specify shader stage.  "all" is never used.
        EI_VS,
        EI_PS,
        EI_CS
    };
    enum EI_ResourceState
    {
        EI_STATE_NON_PS_SRV,
        EI_STATE_VS_SRV,
        EI_STATE_CS_SRV,
        EI_STATE_PS_SRV,
        EI_STATE_UAV,
        EI_STATE_COPY_DEST,
        EI_STATE_COPY_SOURCE,
        EI_STATE_RENDER_TARGET
    };

    struct TressFXConstantBufferDesc
    {
        const EI_StringHash     constantBufferName;
        const int               bytes;
        const int               nConstants;
        const EI_StringHash*    parameterNames;
    };

    struct TressFXLayoutDescription
    {
        const int nSRVs;
        const EI_StringHash* srvNames;

        const int nUAVs;
        const EI_StringHash* uavNames;

        TressFXConstantBufferDesc constants;
        EI_ShaderStage stage;
    };

    struct EI_Barrier
    {
        EI_Resource* pResource;
        EI_ResourceState from;
        EI_ResourceState to;
    };

    struct TressFXBindSet
    {
        int     nSRVs;
        EI_SRV* srvs;
        int     nUAVs;
        EI_UAV* uavs;
        void*   values;
        int     nBytes;
    };




    extern "C"
    {
        /////////////////////////////////////////////////////////////////////////////////
        // CPU Memory Allocation Callbacks.
        // 
        typedef void* (*EI_Malloc_t)(size_t size);
        typedef void(*EI_Free_t)(void*);
        typedef void(*EI_Error_t)(EI_StringHash message);

        /////////////////////////////////////////////////////////////////////////////////
        // File IO Callbacks.
        // 
        typedef void(*EI_Read_t)(void* ptr, uint size, EI_Stream* pFile);
        typedef void(*EI_Seek_t)(EI_Stream* pFile, uint offset);

        /////////////////////////////////////////////////////////////////////////////////
        // Layouts
        //
        // Callbacks for the system that defines / assigns slots. 
        // Bind sets, in the next section, the actual resources that are bound to these slots.

        typedef EI_BindLayout* (*EI_CreateLayout_t)(EI_Device* pDevice,
            EI_LayoutManagerRef pLayoutManager,
            const TressFXLayoutDescription& description);

        typedef void(*EI_DestroyLayout_t)(EI_Device* pDevice, EI_BindLayout* pLayout);

        /////////////////////////////////////////////////////////////////////////////////
        // Bindsets
        //
        typedef EI_BindSet* (*EI_CreateBindSet_t)(EI_Device* pDevice, TressFXBindSet& bindSet);
        typedef void(*EI_DestroyBindSet_t)(EI_Device* pDevice, EI_BindSet* bindSet);
        typedef void(*EI_Bind_t)(EI_CommandContextRef commandContext, EI_BindLayout* pLayout, EI_BindSet& bindSet);

        /////////////////////////////////////////////////////////////////////////////////
        // Structured Buffers / Linear buffers
        //
        typedef EI_Resource* (*EI_CreateSB_t)(EI_Device* pDevice, AMD::uint32 structSize, AMD::uint32 structCount, EI_StringHash resourceName, EI_StringHash objectName);
        typedef void(*EI_DestroyResource_t)(EI_Device* pDevice, EI_Resource* pResource);

        // Map gets a pointer to upload heap / mapped memory.
        // Unmap issues the copy.
        // This is always called on linear buffers.
        typedef void* (*EI_Map_t)(EI_CommandContextRef pContext, EI_StructuredBufferRef sb);
        typedef bool (*EI_Unmap_t)(EI_CommandContextRef pContext, EI_StructuredBufferRef sb);

        typedef EI_Resource* (*EI_CreateFP16RT_t)(EI_Device* pDevice,
            const size_t     width,
            const size_t     height,
            const size_t     channels,
            EI_StringHash    strHash,
            float            clearR,
            float            clearG,
            float            clearB,
            float            clearA);

        typedef EI_Resource* (*EI_CreateRW2D_t)(EI_Device* pDevice,
            const size_t     width,
            const size_t     height,
            const size_t     arraySize,
            EI_StringHash    strHash);


        // TODO get rid of clearValue argument?
        typedef void (*EI_ClearCounter_t)(
            EI_CommandContextRef    pContext,
            EI_StructuredBufferRef sb,
            AMD::uint32          clearValue);

        typedef void (*EI_Copy_t)(EI_CommandContextRef pContext, EI_StructuredBufferRef from, EI_StructuredBufferRef to);



        typedef void(*EI_ClearRW2D_t)(EI_CommandContext* pContext, EI_Resource* pResource, AMD::uint32 clearValue);



        typedef void(*EI_SubmitBarriers_t)(EI_CommandContextRef commands,
            int numBarriers,
            EI_Barrier* barriers);


        ///////////////////////////////////////////////////////////////
        // Compute Shaders
        //

        // TODO add layout array.
        typedef EI_PSO* (*EI_CreateComputeShaderPSO_t)(EI_Device* pDevice,
            EI_LayoutManagerRef     layoutManager,
            const EI_StringHash& shaderName);
        typedef void(*EI_DestroyPSO_t)(EI_Device* pDevice, EI_PSO* pso);

        // All our compute shaders have dimensions of (N,1,1)
        typedef void(*EI_Dispatch_t)(EI_CommandContextRef commandContext, EI_PSO& pso, int nThreadGroups);


        ///////////////////////////////////////////////////////////////
        // Indexed, instanced draw.
        //
        // Initialize and leave in state for use as index buffer.
        // Indices are assumed to be 32 bits / 4 bytes.
        // TODO upload should be seperate, and use command context.
        typedef EI_IndexBuffer* (*EI_CreateIndexBuffer_t)(EI_Device* pDevice,
            AMD::uint32      size,
            void*            pInitialData, EI_StringHash objectName);
        typedef void(*EI_DestroyIB_t)(EI_Device* pDevice, EI_IndexBuffer* pBuffer);


        // TODO Change this to TressFX definition.
        struct EI_IndexedDrawParams
        {
            EI_IndexBuffer* pIndexBuffer;
            AMD::uint32    numIndices;
            AMD::uint32    numInstances;
        };


        // TODO I don't think we are using the layouts and sets.  If we were, they would probaly be part of
        // the pso we are already passing in.
        // TODO This is probaly not the right place to have the PSO. It should actually be set before this.
        // But Sushi is using it (because it needs technique name) to make the draw.
        // TODO ...  so perhaps we need to define an "indexed draw" context.  Like in vulkan, it might
        // contain the pipeline layout info.
        typedef void(*EI_Draw_t)(EI_CommandContextRef     commandContext,
            EI_PSO&                pso,
            EI_IndexedDrawParams& drawParams);

        typedef struct TressFX_Callbacks_t
        {
            /////////////////////////////////////////////////////////////////////////////////
            // Report an error.
            // 
            EI_Error_t pfError;

            /////////////////////////////////////////////////////////////////////////////////
            // CPU Memory Allocation Callbacks.
            // 
            EI_Malloc_t pfMalloc;
            EI_Free_t pfFree;

            /////////////////////////////////////////////////////////////////////////////////
            // File IO Callbacks.
            // 
            EI_Read_t pfRead;
            EI_Seek_t pfSeek;

            /////////////////////////////////////////////////////////////////////////////////
            // Layouts
            //
            // Callbacks for the system that defines / assigns slots. 
            // Bind sets, in the next section, are the actual resources that are bound to these slots.
            EI_CreateLayout_t pfCreateLayout;
            EI_DestroyLayout_t pfDestroyLayout;


            ///////////////////////////////////////////////////////////////
            // Bind sets
            //
            // Collection of UAVs, SRVs, and constant buffer(s) that are bound together,
            // as well as updated values for the constant buffers.
            // order is always consistent with the associated layout.

            EI_CreateBindSet_t pfCreateBindSet;
            EI_DestroyBindSet_t pfDestroyBindSet;
            EI_Bind_t pfBind;


            ///////////////////////////////////////////////////////////////
            // Barriers
            //
            // Important for Vulkan/DX12 efficiency

            EI_SubmitBarriers_t pfSubmitBarriers;


            ///////////////////////////////////////////////////////////////
            // Callbacks for drawing strands.
            //
            // These are the minimum functions necessary to just draw hair strands.

            // Structured Buffers
            EI_CreateSB_t pfCreateReadOnlySB;
            EI_CreateSB_t pfCreateReadWriteSB;
            EI_DestroyResource_t pfDestroySB;

            // Initalizing structured buffer data.
            EI_Map_t pfMap;
            EI_Unmap_t pfUnmap;

            // Draw hair geometry.
            EI_CreateIndexBuffer_t pfCreateIndexBuffer;
            EI_DestroyIB_t pfDestroyIB;
            EI_Draw_t pfDraw;

            ///////////////////////////////////////////////////////////////
            // Callbacks for Simulation
            //
            // These are the additional functions necessary for simulation.
            // "Create" in this context, could just be a "get" using the string name.
            // Since this is really for compute, the "PSO" is equivalent to a shader.
            EI_CreateComputeShaderPSO_t pfCreateComputeShaderPSO;
            EI_DestroyPSO_t pfDestroyPSO;
            EI_Dispatch_t pfDispatch;


            ///////////////////////////////////////////////////////////////
            // Callbacks for OIT
            //
            // These are the additional functions necessary for OIT, 
            // common to both PPLL and Shortcut
            EI_CreateRW2D_t pfCreate2D;
            EI_ClearRW2D_t pfClear2D;


            ///////////////////////////////////////////////////////////////
            // Callbacks for PPLL
            //
            // These are the additional functions necessary for the PPLL method.
            EI_CreateSB_t pfCreateCountedSB;
            EI_ClearCounter_t pfClearCounter;

            ///////////////////////////////////////////////////////////////
            // Callbacks for PPLL
            //
            // These are the additional functions necessary for Shortcut.


            // used by shortcut.
            EI_CreateFP16RT_t pfCreateRT;


            ///////////////////////////////////////////////////////////////
            // Do we still need this?

            EI_Copy_t pfCopy;

        } TressFX_Callbacks;

        AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_GetCallbacks(TressFX_Callbacks** ppCallbacks);
        AMD_TRESSFX_DLL_API TRESSFX_RETURN_CODE TressFX_GetVersion(uint* major, uint* minor, uint* patch);

        // The C interface is not complete.
    }

} // namespace AMD




#endif
