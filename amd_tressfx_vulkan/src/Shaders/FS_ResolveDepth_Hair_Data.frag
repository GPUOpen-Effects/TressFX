#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"

layout(set = 0, binding = IDSRV_HAIR_FRAGMENT_DEPTHS, r32ui) uniform uimage2DArray FragmentDepthsTexture;

//--------------------------------------------------------------------------------------
// PS_ResolveDepth_Hair
//
//--------------------------------------------------------------------------------------
void main()
{
	ivec2 vScreenAddress = ivec2(gl_FragCoord.xy);

	uint uDepth = 0;

// NOTE: A bug in FXC in the Win 8.x SDKs prevents
// this shader from compiling if depth is read from a 
// structured buffer.  A texture array is used instead.

#if SHORTCUT_NUM_DEPTHS == 3
	uDepth = imageLoad(FragmentDepthsTexture, ivec3(vScreenAddress, 2)).x;
#else
	uDepth = imageLoad(FragmentDepthsTexture, ivec3(vScreenAddress, 1)).x;
#endif

	gl_FragDepth = uintBitsToFloat(uDepth);
}