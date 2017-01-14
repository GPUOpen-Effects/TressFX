#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"
#include "PackUnpack.glsl"
#include "ComputeShadow.glsl"
#include "ComputeHairShading.glsl"

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

#define K_BUFFER 8

struct FragmentElement {
  float depth;
  uint TangentAndCoverage;
};

layout(location = 0) out vec4 FragColor;

//--------------------------------------------------------------------------------------
// PS_KBuffer_Hair
//
// Second pass pixel shader which selects the nearest k fragments (k-buffer) and renders
// then in depth order for correct transparent blending of the hair. It uses the per-pixel
// linked list to access all of the layers of hair fragment so that the nearest k fragments
// are rendered at a higher quality than the remaining fragments. The lighting and shadows
// are calculated in screen space using the tangent and depth stored in the per-pixel linked
// list, similar to deferred shading.
//
//--------------------------------------------------------------------------------------
void main() {
  ivec2 iuv = ivec2(gl_FragCoord.xy);
  uint ListBucketHead = imageLoad(PerPixelLinkedListHead, iuv).x;
  if (ListBucketHead == -1) discard;

  FragmentElement kbuf[K_BUFFER];
  // Load first K_BUFFER element in temp array
  int kbuf_size;
  for (kbuf_size = 0; kbuf_size < K_BUFFER; kbuf_size++)
  {
    if (ListBucketHead == -1)
      break;
    kbuf[kbuf_size].depth = PPLL[ListBucketHead].depth;
    kbuf[kbuf_size].TangentAndCoverage = PPLL[ListBucketHead].TangentAndCoverage;
    ListBucketHead = PPLL[ListBucketHead].next;
  }

  uint ListBucketId = ListBucketHead;
  vec4 result = vec4(0., 0., 0., 1.);

  uint numfrag = 0;
  while (ListBucketId != -1) {
    numfrag++;
    float max_depth = 0.;
    uint max_idx = 0;
    for (int i = 0; i < kbuf_size; i++)
    {
      float d = kbuf[i].depth;
      max_depth = max(max_depth, d);
      max_idx = (max_depth == d) ? i : max_idx;
    }

    if (PPLL[ListBucketId].depth < max_depth)
    {
      float tmp = PPLL[ListBucketId].depth;
      PPLL[ListBucketId].depth = kbuf[max_idx].depth;
      kbuf[max_idx].depth = tmp;
      uint tmpu = PPLL[ListBucketId].TangentAndCoverage;
      PPLL[ListBucketId].TangentAndCoverage = kbuf[max_idx].TangentAndCoverage;
      kbuf[max_idx].TangentAndCoverage = tmpu;
    }

    float d = PPLL[ListBucketId].depth;
    vec3 ndcPos = 2. * vec3(gl_FragCoord.x * g_WinSize.z, 1. - gl_FragCoord.y * g_WinSize.w, d) - 1.;
    ndcPos.z = d;
    vec4 Pos = g_mInvViewProj * vec4(ndcPos, 1.);
    Pos /= Pos.w;
    vec3 Tangent = GetTangent(PPLL[ListBucketId].TangentAndCoverage);
    float FragmentAlpha = GetCoverage(PPLL[ListBucketId].TangentAndCoverage);
    float amountOfLight = ComputeSimpleShadow(Pos.xyz, g_HairShadowAlpha);
    vec3 FragmentColor = SimpleHairShading(Pos.xyz, Tangent, vec4(0.), amountOfLight);
    result.xyz = result.xyz * (1. - FragmentAlpha) + FragmentAlpha * FragmentColor;
    result.w *= (1. - FragmentAlpha);

    ListBucketId = PPLL[ListBucketId].next;
  }

//#define DEBUG
#ifdef DEBUG
  result = vec4(0., 1., 0., 0.);
  if (numfrag > 32) result.xyz = vec3(1., 1., 0.);
  if (numfrag > 64) result.xyz = vec3(1., .5, 0.);
  if (numfrag > 128) result.xyz = vec3(1., 0., 0.);
  FragColor = result;
  return;
#endif

  uint reverse = 0;
  bool isSorted = false;
  while (!isSorted) {
    isSorted = true;
    for (int i = 0; i < kbuf_size - 1; i++)
    {
      if (kbuf[i].depth < kbuf[i + 1].depth) {
        isSorted = false;
        float tmp = kbuf[i].depth;
        kbuf[i].depth = kbuf[i + 1].depth;
        kbuf[i + 1].depth = tmp;
        uint ttmp = kbuf[i].TangentAndCoverage;
        kbuf[i].TangentAndCoverage = kbuf[i + 1].TangentAndCoverage;
        kbuf[i + 1].TangentAndCoverage = ttmp;
        reverse++;
      }
    }
  }

  for (int i = 0; i < kbuf_size; i++)
  {
    float d = kbuf[i].depth;
    vec3 ndcPos = 2. * vec3(gl_FragCoord.x * g_WinSize.z, 1. - gl_FragCoord.y * g_WinSize.w, d) - 1.;
    ndcPos.z = d;
    vec4 Pos = g_mInvViewProj * vec4(ndcPos, 1.);
    Pos /= Pos.w;

    vec3 Tangent = GetTangent(kbuf[i].TangentAndCoverage);
    float FragmentAlpha = GetCoverage(kbuf[i].TangentAndCoverage);
    float amountOfLight = ComputeShadow(Pos.xyz, g_HairShadowAlpha);
    vec3 FragmentColor = ComputeHairShading(Pos.xyz, Tangent, vec4(0.), amountOfLight, g_MatBaseColor.xyz);

    result.xyz = result.xyz * (1. - FragmentAlpha) + FragmentAlpha * FragmentColor;
    result.w *= (1. - FragmentAlpha);
  }
  FragColor = result;
}