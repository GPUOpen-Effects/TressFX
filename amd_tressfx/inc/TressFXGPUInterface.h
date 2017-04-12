// ----------------------------------------------------------------------------
// Engine Interface for graphics.
//
// TressFX calls to graphics come through this interface.  
// 
// To hook to your engine or low-level graphics API of choice,
// you essentially implement this interface.
//
// The basic parts are
// 1. Resoures - create, destroy, including views.
// 2. Layouts and Bind Sets - groups of resources and constants that need to be updated and bound together.
// 3. Barriers - These are only relevant for DX12 and Vulkan.
// 4. Graphics submission and pipeline state.
// 5. Compute submission.
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





#pragma once

#include "AMD_Types.h"
#include "TressFXCommon.h"

namespace AMD
{
    extern TressFX_Callbacks_t g_Callbacks;
}

///////////////////////////////////////////////////////////////
// 


///////////////////////////////////////////////////////////////
// Your implementation needs to define the following classes.

// The class                  // It's equivalent in DX11 (for example)
class EI_Device;              // ID3D11Device
class EI_CommandContext;      // ID3D11DeviceContext
class EI_Resource;            // ID3D11Texture2D + UAV + SRV + RTV   or    ID3D11Buffer + UAV + SRV  (see details below)
class EI_IndexBuffer;         // ID3D11Buffer to be used as index buffer.  element size is 32 bits.
class EI_LayoutManager;       // Anything you might use to assign shader slots.
struct EI_BindLayout;         // Array of resources to bind.
class EI_PSO;                 // Pipeline state (raster state, etc)




///////////////////////////////////////////////////////////////
// We use 4 kinds of resrouces.
//    2D RW buffer
//    Structured buffers.
//    Index buffer.
//    Constant buffers
//


// Definitions of types.



















// We distinguish between 2D and linear resources in a lot of places,
// but 
typedef EI_Resource EI_RWTexture2D;
typedef EI_Resource EI_StructuredBuffer;


// We get UAVs and SRVs from the resource object.
// This is your opportunity to change that.
typedef EI_Resource* EI_UAV;
typedef EI_Resource* EI_SRV;
inline EI_UAV EI_GetUAV(EI_Resource* resource) { return resource; }
inline EI_SRV EI_GetSRV(EI_Resource* resource) { return resource; }
inline EI_UAV EI_GetUAV(EI_Resource& resource) { return &resource; }
inline EI_SRV EI_GetSRV(EI_Resource& resource) { return &resource; }

typedef const char* EI_StringHash;  // how do you label a resource. by name? hashed string?
#define TRESSFX_STRING_HASH(a) a

typedef EI_LayoutManager& EI_LayoutManagerRef;

typedef EI_IndexBuffer& EI_IndexBufferRef;

typedef EI_StructuredBuffer& EI_StructuredBufferRef;

typedef EI_CommandContext& EI_CommandContextRef;

///////////////////////////////////////////////////////////////
//
// This contains a number of engine or API-specific typedefs.
//
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// Barriers / transitions
//
//  A UAV barrier is specified by setting both from and to as EI_STATE_UAV





///////////////////////////////////////////////////////////////
// 2D UAVs   (RWTexture2D)
//
// RW2D Specializations:
// GPU-side only.  No CPU upload.
// always bindable as SRV
// no mips, no mulitsampling.
// always bound in PS only.
// always a single SRV, RTV, and UAV, ever (meaning they can be created/destroyed at same time as
// resource)

#if 0

// Alwyas 32 bit uint type.
// GPU-side only. No CPU upload.
// Always bindable as SRV and UAV, and view matches resource.
// No mips, no multisampling.
// Always bound in PS only.
EI_Resource* EI_RW2D_Create(EI_Device* pDevice,
                                   const size_t     width,
                                   const size_t     height,
                                   const size_t     arraySize,
                                   EI_StringHash    strHash);

// Needs RTV and SRV
EI_Resource* EI_FP16RT_Create(EI_Device* pDevice,
                                const size_t     width,
                                const size_t     height,
                                const size_t     channels,
                                EI_StringHash    strHash,
                                float            clearR,
                                float            clearG,
                                float            clearB,
                                float            clearA);

void EI_Destroy(EI_Device* pDevice, EI_Resource* pResource);
inline void EI_RW2D_Destroy(EI_Device* pDevice, EI_Resource* pResource) { EI_Destroy(pDevice, pResource); }


void EI_RW2D_Clear(EI_CommandContext* pContext, EI_Resource* pResource, AMD::uint32 clearValue);

#else



#endif

///////////////////////////////////////////////////////////////
// EI_StructuredBuffer  (SB for short)
//
// We have different create functions for each type of usage (there are 3)
// There is one SRV and 0 or 1 UAV for each, ever, so you can create and destory them along with
// the resource.
// Therefore, you could store the views inside your definition of "EI_StructuredBuffer".
// No buffer is ever updated from the CPU more than once.  That one time is done through an EI_Map
// / EI_Unmap.

// Creates a structured buffer and srv (StructuredBuffer). It necessarily needs data to start, so
// begin state should be EI_STATE_COPY_DEST.

#if 0
EI_StructuredBuffer* EI_CreateReadOnlySB(EI_Device* pDevice,
                                        const AMD::uint32 structSize,
                                        const AMD::uint32 structCount,
                                        EI_StringHash  resourceName,
                                        EI_StringHash  objectName);

// Creates a structured buffer and default UAV/SRV (StructuredBuffer and RWStructuredBuffer in
// HLSL).  It will be used as UAV, so begin state should be EI_STATE_UAV.
EI_StructuredBuffer* EI_CreateReadWriteSB(EI_Device* pDevice,
                                         const AMD::uint32 structSize,
                                         const AMD::uint32 structCount,
    EI_StringHash  resourceName,
    EI_StringHash  objectName);

// Same as EI_CreateReadWriteSB, but also includes an atomic counter.  Atomic counter is cleared to
// zero each frame (EI_SB_ClearCounter)
EI_StructuredBuffer* EI_CreateCountedSB(EI_Device* pDevice,
                                       const AMD::uint32 structSize,
                                       const AMD::uint32 structCount,
    EI_StringHash  resourceName,
    EI_StringHash  objectName);


// TODO get rid of clearValue argument?  It's always the same.
void EI_SB_ClearCounter(EI_CommandContextRef    pContext,
    EI_StructuredBuffer& sb,
    AMD::uint32          clearValue);


void EI_SB_Copy(EI_CommandContextRef pContext, EI_StructuredBuffer& from, EI_StructuredBuffer& to);


// Map gets a pointer to upload heap / mapped memory.
// Unmap issues the copy.
void* EI_SB_Map(EI_CommandContextRef pContext, EI_StructuredBuffer& sb);
bool EI_SB_Unmap(EI_CommandContextRef pContext, EI_StructuredBuffer& sb);

#else

#define EI_CreateLayout AMD::g_Callbacks.pfCreateLayout
#define EI_DestroyLayout AMD::g_Callbacks.pfDestroyLayout

#define EI_CreateReadOnlySB AMD::g_Callbacks.pfCreateReadOnlySB
#define EI_CreateReadWriteSB AMD::g_Callbacks.pfCreateReadWriteSB
#define EI_CreateCountedSB AMD::g_Callbacks.pfCreateCountedSB

#define EI_SB_ClearCounter AMD::g_Callbacks.pfClearCounter
#define EI_SB_Copy  AMD::g_Callbacks.pfCopy
#define EI_SB_Map  AMD::g_Callbacks.pfMap
#define EI_SB_Unmap  AMD::g_Callbacks.pfUnmap

#define EI_SB_Destroy AMD::g_Callbacks.pfDestroySB
#define EI_Destroy AMD::g_Callbacks.pfDestroySB
#define EI_RW2D_Destroy AMD::g_Callbacks.pfDestroySB

#define EI_RW2D_Create AMD::g_Callbacks.pfCreate2D
#define EI_FP16RT_Create AMD::g_Callbacks.pfCreateRT

#define EI_RW2D_Clear AMD::g_Callbacks.pfClear2D

#define EI_SubmitBarriers AMD::g_Callbacks.pfSubmitBarriers

#define EI_CreateBindSet AMD::g_Callbacks.pfCreateBindSet
#define EI_DestroyBindSet AMD::g_Callbacks.pfDestroyBindSet
#define EI_Bind AMD::g_Callbacks.pfBind


#define EI_CreateComputeShaderPSO AMD::g_Callbacks.pfCreateComputeShaderPSO
#define EI_DestroyPSO AMD::g_Callbacks.pfDestroyPSO
#define EI_Dispatch AMD::g_Callbacks.pfDispatch

#define EI_CreateIndexBuffer AMD::g_Callbacks.pfCreateIndexBuffer
#define EI_IB_Destroy AMD::g_Callbacks.pfDestroyIB
#define EI_Draw AMD::g_Callbacks.pfDraw

#endif


//inline void EI_SB_Destroy(EI_Device* pDevice, EI_StructuredBuffer* sb) { EI_Destroy(pDevice, sb); }












///////////////////////////////////////////////////////////////
// Layouts
//
//

// The engine can assign SRV and UAV slots, as well as a reference to wehre the parameter is.
// The layout manager is whatever context the engine requires to do so.
// Input is the name of the resource in the shader. (EI_StringHash, which coudl be name of
// variable, for example.)
// the Slots themselves could be, for example 0 for t0 in HLSL.
// the "parameter" could be a reference to a constant buffer and an offset.  In UE4 it might be
// FShaderParameter.
//
// For example, the layout manager might be a shader, and the engine uses reflection to get what
// was assigned by the compiler.
// Or, with static assignments, layout manager might just be a stringhash -> number table.
//
// TODO specify frequency hint.







#if 0

EI_BindLayout* EI_CreateLayout(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager, const TressFXLayoutDescription& description);

void EI_DestroyLayout(EI_Device* pDevice, EI_BindLayout* pLayout);
#else


#endif









// The following are the predefined raster and depth states you need.
// TODO this needs to be passed into a "create PSO" call.

enum EI_RasterStateBits
{
    EI_OFF = 0,  // depth writes and testing off, blending off, stencil off, color writes off.
    EI_COLOR_WRITES_ON_BIT,
    EI_DEPTH_TEST_ON_BIT,
    EI_DEPTH_WRITE_ON_BIT,
    EI_STENCIL_WRITE_INCREMENT_BIT,
    EI_STENCIL_TEST_NONZERO_BIT,
    EI_BLEND_SRC_ALPHA_DEST_ONE_ADD_ALPHAS_BIT  // ocolor.rgb = src.a*(src.rgb) + dest.rgb;
                                                // ocolor.a = src.a + dest.a;
};

enum EI_RasterState
{
    EI_PPLL_BUILD = (1 << EI_DEPTH_TEST_ON_BIT) | (1 << EI_DEPTH_WRITE_ON_BIT) |
                    (1 << EI_STENCIL_WRITE_INCREMENT_BIT),
    EI_PPLL_RESOLVE = (1 << EI_COLOR_WRITES_ON_BIT) |
                      (1 << EI_BLEND_SRC_ALPHA_DEST_ONE_ADD_ALPHAS_BIT) |
                      (1 << EI_STENCIL_TEST_NONZERO_BIT),
    EI_SET_SIZE = 0xffffffff
};

#ifndef AMD_SAFE_RESOURCE_DELETE
#define AMD_SAFE_RESOURCE_DELETE(device, pResource)     \
    {                                                   \
        if( (pResource) != nullptr)                     \
        {                                               \
            EI_Destroy(device, pResource);              \
        }                                               \
        (pResource) = nullptr;                          \
    }
#endif

#define ENABLE_ROV_TEST 0
