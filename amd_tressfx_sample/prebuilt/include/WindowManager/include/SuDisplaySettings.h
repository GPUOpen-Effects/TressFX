//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef _SUDISPLAYSETTINGS_H
#define _SUDISPLAYSETTINGS_H

#include "SuTypes.h"

/// \brief Structure containing window and 3D-API-specific settings
///
/// \ingroup WindowManager

// tolua_begin
class SuDisplaySettings
{
public:
	/// Constructor
	SU_DLL SuDisplaySettings();

	enum D3DPool
	{
		POOL_DEFAULT,
		POOL_MANAGED
	};

	enum
	{
		NO_POS_SET = INT_MAX
	};

	// just here for testing tolua++ binding generation
	//SuDisplaySettings* CreateDisplaySettings ();

	SuString windowTitle;
	bool    fullScreen;
	bool    borderless;
	uint32  videoAdapterIndex;
	uint32  windowWidth;
	uint32  windowHeight;
	int32   windowHPos;
	int32   windowVPos;
	uint32  multiSampleTaps;
	uint32  d3dMultiSampleQuality;
	uint32  backBufferColorBits;
	bool    srgb;
	uint32  depthBits;
	uint32  stencilBits;
	float32 screenAspectRatio;
	uint32  numBackBuffers;
	bool    stereo;                     ///< Create a stereo back buffer
	uint32  minGLVersion;               ///< The minimum OpenGL version required - format is 16.16, major, minor
	bool    waitForVSync;
	uint32  monitorRefreshRate;
	D3DPool d3dPool;
	bool    d3dUsePureDevice;
	bool    d3dRefRast;
	bool    d3dResourceShadowBuffers;
	bool    d3dUAVBackBuffer;          ///< The backbuffer should be created with the BIND_UNORDERED_ACCESS flag 
	bool    d3dSRVDepthBuffer;         ///<  Allow the depth buffer to be created with the BIND_SHADER_RESOURCE flag
	bool    d3d101Required;
	bool    d3d10Required;
	bool    d3d11Required;
	bool    d3d12Required;
    bool    d3d12_1Required;
	bool    hwTessellationRequired;    ///< This setting specifies whether HW tessellation extensions should be used for APIs prior to Direct3D 11. Note that the engine will only check support for the API used by the demo
	bool    d3dUseDebugLayer;
	bool    glUseDebugLayer;
	float   clearColorR;
	float   clearColorG;
	float   clearColorB;
	float   clearColorA;
	float   clearDepth;
	uint8   clearStencil;

	/// Using fixed-size allocations for CPU upload heap and for descriptor heaps of each type.
	uint32  d3d12UploadRingBufferMiB;
	uint32  d3d12MaxCBV_SRV_UAVRingBufferDescriptors;
	uint32  d3d12MaxCBV_SRV_UAVHeapDescriptors;
	uint32  d3d12MaxRTVHeapDescriptors;
	uint32  d3d12MaxDSVHeapDescriptors;
	uint32  d3d12MaxSamplerHeapDescriptors;

// tolua_end



	// Legacy support for d3d10FlushOnDraw;
	///< If true, and if the D3D10 debug layer is enabled, a flush will be inserted between draw calls
#if 0 
// tolua_begin
	bool d3d10FlushOnDraw;
	bool d3dFlushOnDraw;
// tolua_end
#else
	union
	{
		bool d3d10FlushOnDraw;
		bool d3dFlushOnDraw;
	};
#endif

// tolua_begin
};
// tolua_end
#endif
