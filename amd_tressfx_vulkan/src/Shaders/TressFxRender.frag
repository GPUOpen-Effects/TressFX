#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"
#include "PackUnpack.glsl"
#include "ComputeCoverage.glsl"

layout(set = 0, binding = IDSRV_ATOMIC_COUNTER_BUFFER) buffer AtomicBuffer
{
    uint PixelCount;
};
layout(set = 0, binding = IDSRV_HEAD_PPLL, r32ui) uniform restrict uimage2D PerPixelLinkedListHead;

struct PerPixelListBucket
{
     float depth;
     uint TangentAndCoverage;
     uint next;
};

layout(std430, set = 0, binding = IDSRV_PPLL) buffer PerPixelLinkedList
{
    PerPixelListBucket PPLL[];
};
layout(early_fragment_tests) in;

void StoreFragments_Hair(vec2 FragCoordXY, vec3 Tangent, float Coverage, float depth)
{
  uint pixel_id = atomicAdd(PixelCount, 1);
  int pxid = int(pixel_id);
  ivec2 iuv = ivec2(FragCoordXY);
  uint tmp = imageAtomicExchange(PerPixelLinkedListHead, iuv, pixel_id);
  PPLL[pxid].depth = depth;
  PPLL[pxid].TangentAndCoverage = PackTangentAndCoverage(Tangent.xyz, Coverage);
  PPLL[pxid].next = tmp;
}

layout(location = 0) in float depth;
layout(location = 1) in vec4 p0p1;
layout(location = 2) in vec4 tangent;


//--------------------------------------------------------------------------------------
// PS_ABuffer_Hair
//
// This is the first pass pixel shader for rendering hair geometry into an A-buffer.
// It keeps all of the layers of the transparent hair in a UAV per-pixel linked list.
//
// Coverage for antialiasing is calculated and stored in the linked list along with geometry
// and attributes necessary for lighting and shadows which are calculated in a subsequent
// pass by rendering a full screen quad.
//
//--------------------------------------------------------------------------------------
void main() {
    // Render AA Line, calculate pixel coverage
  vec4 proj_pos = vec4(2 * gl_FragCoord.x * g_WinSize.z - 1.,
	  1. - 2 * gl_FragCoord.y * g_WinSize.w,
	  1., 1.);
  vec4 original_pos = g_mInvViewProj * proj_pos; // Not used ??

  float coverage = (g_bUseCoverage != 0) ? ComputeCoverage(p0p1.xy, p0p1.zw, proj_pos.xy) : 1.;

  coverage *= g_FiberAlpha;
  if (coverage > g_alphaThreshold)
    StoreFragments_Hair(gl_FragCoord.xy, tangent.xyz, coverage, gl_FragCoord.z);
}
