//---------------------------------------------------------------------------------------
// Shader code related to per-pixel linked lists.
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

// This is code decoration for our sample.  
// Remove this and the EndHLSL at the end of this file 
// to turn this into valid HLSL

StartHLSL TressFXPPLL_Common

#if ERROR_CHECK
#	define ASSERT(x) if(!(x)) RETURN_ERROR
#else
#	define ASSERT(x) 
#endif


struct PPLL_STRUCT
{
    uint	depth;
    uint	data;
    uint    color;
    uint    uNext;
};

#ifndef FRAGMENT_LIST_NULL
#define FRAGMENT_LIST_NULL 0xffffffff
#endif

#define HAS_COLOR 1

EndHLSL



StartHLSL TressFXPPLL_Build


RWTexture2D<int>    tRWFragmentListHead;

RWStructuredBuffer<PPLL_STRUCT> LinkedListUAV;

int nNodePoolSize;

uint PackFloat4IntoUint(float4 vValue)
{
    return (((uint)(vValue.x * 255)) << 24) | (((uint)(vValue.y * 255)) << 16) | (((uint)(vValue.z * 255)) << 8) | (uint)(vValue.w * 255);
}

// Allocate a new fragment location in fragment color, depth, and link buffers
int AllocateFragment(int2 vScreenAddress)
{
    int newAddress = LinkedListUAV.IncrementCounter();
    if (newAddress <= 0 || newAddress > nNodePoolSize)
        newAddress = FRAGMENT_LIST_NULL;
    return newAddress;
}

// Insert a new fragment at the head of the list. The old list head becomes the
// the second fragment in the list and so on. Return the address of the *old* head.
int MakeFragmentLink(int2 vScreenAddress, int nNewHeadAddress)
{
    int nOldHeadAddress;

    InterlockedExchange(tRWFragmentListHead[vScreenAddress], nNewHeadAddress, nOldHeadAddress);

    return nOldHeadAddress;
}


// Write fragment attributes to list location. 
void WriteFragmentAttributes(int nAddress, int nPreviousLink, float4 vData, float3 vColor3, float fDepth)
{
    PPLL_STRUCT element;
    element.data = PackFloat4IntoUint(vData);
    element.color = PackFloat4IntoUint(float4(vColor3, 0));
    element.depth = asuint(saturate(fDepth));
    element.uNext = nPreviousLink;
    LinkedListUAV[nAddress] = element;
}

EndHLSL

StartHLSL TressFXPPLL_Resolve



#ifndef KBUFFER_SIZE
#define KBUFFER_SIZE 8
#endif


#ifndef TAIL_SHADING

float4 TressFXTailColor(float2 pixelCoord, float depth, float4 vTangentCoverage, float3 baseColor)
{

    return float4(baseColor, vTangentCoverage.w);
}
#define TAIL_SHADING TressFXTailColor

#endif


#ifndef HEAD_SHADING

float4 TressFXHeadColor(float2 pixelCoord, float depth, float4 vTangentCoverage, float3 baseColor)
{
    return float4(baseColor, vTangentCoverage.w);

}

#define HEAD_SHADING TressFXHeadColor

#endif


#define MAX_FRAGMENTS 512
#define TAIL_COMPRESS 0



#if (KBUFFER_SIZE <= 16)
#define ALU_INDEXING			// avoids using an indexed array for better performance
#endif



#ifdef ALU_INDEXING
//--------------------------------------------------------------------------------------
// 
// Helper functions for storing the k-buffer into non-indexed registers for better 
// performance. For this code to work, KBUFFER_SIZE must be <= 16.
//
//--------------------------------------------------------------------------------------

uint GetUintFromIndex_Size16(uint4 V03, uint4 V47, uint4 V811, uint4 V1215, uint uIndex)
{
    uint u;
    u = uIndex == 1 ? V03.y : V03.x;
    u = uIndex == 2 ? V03.z : u;
    u = uIndex == 3 ? V03.w : u;
    u = uIndex == 4 ? V47.x : u;
    u = uIndex == 5 ? V47.y : u;
    u = uIndex == 6 ? V47.z : u;
    u = uIndex == 7 ? V47.w : u;
    u = uIndex == 8 ? V811.x : u;
    u = uIndex == 9 ? V811.y : u;
    u = uIndex == 10 ? V811.z : u;
    u = uIndex == 11 ? V811.w : u;
    u = uIndex == 12 ? V1215.x : u;
    u = uIndex == 13 ? V1215.y : u;
    u = uIndex == 14 ? V1215.z : u;
    u = uIndex == 15 ? V1215.w : u;
    return u;
}

void StoreUintAtIndex_Size16(inout uint4 V03, inout uint4 V47, inout uint4 V811, inout uint4 V1215, uint uIndex, uint uValue)
{
    V03.x = (uIndex == 0) ? uValue : V03.x;
    V03.y = (uIndex == 1) ? uValue : V03.y;
    V03.z = (uIndex == 2) ? uValue : V03.z;
    V03.w = (uIndex == 3) ? uValue : V03.w;
    V47.x = (uIndex == 4) ? uValue : V47.x;
    V47.y = (uIndex == 5) ? uValue : V47.y;
    V47.z = (uIndex == 6) ? uValue : V47.z;
    V47.w = (uIndex == 7) ? uValue : V47.w;
    V811.x = (uIndex == 8) ? uValue : V811.x;
    V811.y = (uIndex == 9) ? uValue : V811.y;
    V811.z = (uIndex == 10) ? uValue : V811.z;
    V811.w = (uIndex == 11) ? uValue : V811.w;
    V1215.x = (uIndex == 12) ? uValue : V1215.x;
    V1215.y = (uIndex == 13) ? uValue : V1215.y;
    V1215.z = (uIndex == 14) ? uValue : V1215.z;
    V1215.w = (uIndex == 15) ? uValue : V1215.w;
}


#define GET_DEPTH_AT_INDEX(uIndex) GetUintFromIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215, uIndex)
#define GET_DATA_AT_INDEX( uIndex) GetUintFromIndex_Size16(kBufferDataV03, kBufferDataV47, kBufferDataV811, kBufferDataV1215, uIndex)
#define GET_COLOR_AT_INDEX( uIndex) GetUintFromIndex_Size16(kBufferStrandColorV03, kBufferStrandColorV47, kBufferStrandColorV811, kBufferStrandColorV1215, uIndex)
#define STORE_DEPTH_AT_INDEX(uIndex, uValue) StoreUintAtIndex_Size16(kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215, uIndex, uValue)
#define STORE_DATA_AT_INDEX(uIndex, uValue) StoreUintAtIndex_Size16(kBufferDataV03, kBufferDataV47, kBufferDataV811, kBufferDataV1215, uIndex, uValue)
#define STORE_COLOR_AT_INDEX(uIndex, uValue) StoreUintAtIndex_Size16(kBufferStrandColorV03, kBufferStrandColorV47, kBufferStrandColorV811, kBufferStrandColorV1215, uIndex, uValue)

#else

//#define GET_DEPTH_AT_INDEX(uIndex) kBuffer[uIndex].depth
//#define GET_DATA_AT_INDEX(uIndex) kBuffer[uIndex].data
//#define STORE_DEPTH_AT_INDEX(uIndex, uValue) kBuffer[uIndex].depth = uValue
//#define STORE_DATA_AT_INDEX( uIndex, uValue) kBuffer[uIndex].data = uValue
#define GET_DEPTH_AT_INDEX(uIndex) kBuffer[uIndex].x
#define GET_DATA_AT_INDEX(uIndex) kBuffer[uIndex].y
#define STORE_DEPTH_AT_INDEX(uIndex, uValue) kBuffer[uIndex].x = uValue
#define STORE_DATA_AT_INDEX( uIndex, uValue) kBuffer[uIndex].y = uValue

#endif


#define NODE_DATA(x) LinkedListSRV[x].data
#define NODE_NEXT(x) LinkedListSRV[x].uNext
#define NODE_DEPTH(x) LinkedListSRV[x].depth
#define NODE_COLOR(x) LinkedListSRV[x].color



Texture2D<int>    tFragmentListHead;
StructuredBuffer<PPLL_STRUCT> LinkedListSRV;



float4 UnpackUintIntoFloat4(uint uValue)
{
    return float4(((uValue & 0xFF000000) >> 24) / 255.0, ((uValue & 0x00FF0000) >> 16) / 255.0, ((uValue & 0x0000FF00) >> 8) / 255.0, ((uValue & 0x000000FF)) / 255.0);
}

float4 ExtractHairColor(float2 pixelCoord, float depth, float4 data)
{
    return data;
}

float4 ExtractHairColor(float2 pixelCoord, float depth, float4 data, float4 color)
{
    return data;
}





#define KBUFFER_TYPE uint2;





float4 GatherLinkedList(float2 vfScreenAddress)
{
    uint2 vScreenAddress = uint2(vfScreenAddress);
    uint pointer = tFragmentListHead[vScreenAddress];

    float4 outColor = float4(0, 0, 0, 1);


    if (pointer == FRAGMENT_LIST_NULL)
        discard;

    ASSERT(pointer >= 0 && pointer < FRAGMENT_LIST_NULL)


#ifdef ALU_INDEXING

        uint4 kBufferDepthV03, kBufferDepthV47, kBufferDepthV811, kBufferDepthV1215;
    uint4 kBufferDataV03, kBufferDataV47, kBufferDataV811, kBufferDataV1215;
    uint4 kBufferStrandColorV03, kBufferStrandColorV47, kBufferStrandColorV811, kBufferStrandColorV1215;
    kBufferDepthV03 = uint4(asuint(100000.0f), asuint(100000.0f), asuint(100000.0f), asuint(100000.0f));
    kBufferDepthV47 = uint4(asuint(100000.0f), asuint(100000.0f), asuint(100000.0f), asuint(100000.0f));
    kBufferDepthV811 = uint4(asuint(100000.0f), asuint(100000.0f), asuint(100000.0f), asuint(100000.0f));
    kBufferDepthV1215 = uint4(asuint(100000.0f), asuint(100000.0f), asuint(100000.0f), asuint(100000.0f));
    kBufferDataV03 = uint4(0, 0, 0, 0);
    kBufferDataV47 = uint4(0, 0, 0, 0);
    kBufferDataV811 = uint4(0, 0, 0, 0);
    kBufferDataV1215 = uint4(0, 0, 0, 0);
    kBufferStrandColorV03 = uint4(0, 0, 0, 0);
    kBufferStrandColorV47 = uint4(0, 0, 0, 0);
    kBufferStrandColorV811 = uint4(0, 0, 0, 0);
    kBufferStrandColorV1215 = uint4(0, 0, 0, 0);

#else

        KBUFFER_TYPE kBuffer[KBUFFER_SIZE];

    [unroll]
    for (int t = 0; t<KBUFFER_SIZE; ++t)
    {
        //kBuffer[t].y = 0;
        //kBuffer[t].x = asuint(100000.0);
        STORE_DEPTH_AT_INDEX(t, asuint(100000.0));
        STORE_DATA_AT_INDEX(t, 0);
        compile error
    }
#endif

    // Get first K elements.
    for (int p = 0; p<KBUFFER_SIZE; ++p)
    {
        if (pointer != FRAGMENT_LIST_NULL)
        {
            STORE_DEPTH_AT_INDEX(p, NODE_DEPTH(pointer));
            STORE_DATA_AT_INDEX(p, NODE_DATA(pointer));
            STORE_COLOR_AT_INDEX(p, NODE_COLOR(pointer));
            pointer = NODE_NEXT(pointer);
        }
    }

    float4 fcolor = float4(0, 0, 0, 1);

    float3 tailColor;
    [allow_uav_condition]
    for (int iFragment = 0; iFragment < MAX_FRAGMENTS && pointer != FRAGMENT_LIST_NULL; ++iFragment)
    {
        if (pointer == FRAGMENT_LIST_NULL) break;

        int id = 0;
        float max_depth = 0;

        // find the furthest node in array
        for (int i = 0; i<KBUFFER_SIZE; i++)
        {
            float fDepth = asfloat(GET_DEPTH_AT_INDEX(i));
            if (max_depth < fDepth)
            {
                max_depth = fDepth;
                id = i;
            }
        }


        uint data = NODE_DATA(pointer);
        uint color = NODE_COLOR(pointer);
        uint nodeDepth = NODE_DEPTH(pointer);
        float fNodeDepth = asfloat(nodeDepth);


        // If the node in the linked list is nearer than the furthest one in the local array, exchange the node 
        // in the local array for the one in the linked list.
        if (max_depth > fNodeDepth)
        {
            uint tmp = GET_DEPTH_AT_INDEX(id);
            STORE_DEPTH_AT_INDEX(id, nodeDepth);
            fNodeDepth = asfloat(tmp);

            tmp = GET_DATA_AT_INDEX(id);
            STORE_DATA_AT_INDEX(id, data);
            data = tmp;

            tmp = GET_COLOR_AT_INDEX(id);
            STORE_COLOR_AT_INDEX(id, color);
            color = tmp;

        }

        float4 vData = UnpackUintIntoFloat4(data);
#if TAIL_COMPRESS
        float4 vColor = UnpackUintIntoFloat4(color);
        fcolor.w = mad(-fcolor.w, vColor.w, fcolor.w);
#else
        float4 vColor = UnpackUintIntoFloat4(color);
        float4 fragmentColor = TAIL_SHADING(vfScreenAddress, fNodeDepth, vData, vColor);
        //fragmentColor = float4( max(float(iFragment)/255.0,255.0)/255.0, iFragment <= 255 ? float(iFragment%255) : 0, 0, 1); 
        fcolor.xyz = mad(-fcolor.xyz, fragmentColor.w, fcolor.xyz) + fragmentColor.xyz * fragmentColor.w;
        fcolor.w = mad(-fcolor.w, fragmentColor.w, fcolor.w);
#endif

        pointer = NODE_NEXT(pointer);
    }
#if TAIL_COMPRESS
    float fTailAlphaInv = fcolor.w;
    fcolor.xyzw = float4(0, 0, 0, 1);
#endif

    // Blend the k nearest layers of fragments from back to front, where k = MAX_TOP_LAYERS_EYE
    for (int j = 0; j<KBUFFER_SIZE; j++)
    {
        int id = 0;
        float max_depth = 0;


        // find the furthest node in the array
        for (int i = 0; i<KBUFFER_SIZE; i++)
        {
            float fDepth = asfloat(GET_DEPTH_AT_INDEX(i));
            if (max_depth < fDepth)
            {
                max_depth = fDepth;
                id = i;
            }
        }

        // take this node out of the next search
        uint nodeDepth = GET_DEPTH_AT_INDEX(id);
        uint data = GET_DATA_AT_INDEX(id);
        uint color = GET_COLOR_AT_INDEX(id);

        // take this node out of the next search
        STORE_DEPTH_AT_INDEX(id, 0);

        // Use high quality shading for the nearest k fragments
        float fDepth = asfloat(nodeDepth);
        float4 vData = UnpackUintIntoFloat4(data);
        float4 vColor = UnpackUintIntoFloat4(color);
        float4 fragmentColor = HEAD_SHADING(vfScreenAddress, fDepth, vData, vColor);
#if TAIL_COMPRESS
        fragmentColor.w = 1 - (1 - fragmentColor.w)*fTailAlphaInv;
        //fTailAlphaInv = 1;
#endif
        // Blend the fragment color
        fcolor.xyz = mad(-fcolor.xyz, fragmentColor.w, fcolor.xyz) + fragmentColor.xyz * fragmentColor.w;
        fcolor.w = fcolor.w* (1 - fragmentColor.w);//mad(-fcolor.w, fragmentColor.w, fcolor.w);
    }
    outColor = fcolor;
    return outColor;
}


EndHLSL


