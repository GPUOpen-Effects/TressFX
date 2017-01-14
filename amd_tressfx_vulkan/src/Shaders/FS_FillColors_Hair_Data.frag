#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"
#include "PackUnpack.glsl"
#include "ComputeCoverage.glsl"
#include "ComputeShadow.glsl"
#include "ComputeHairShading.glsl"

layout(set = 0, binding = IDSRV_HAIR_FRAGMENT_DEPTHS, r32ui) uniform uimage2DArray FragmentDepthsTexture;
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
#else
layout(set = 0, binding = IDSRV_HAIR_FRAGMENT_COLORS, r32ui) uniform uimageBuffer RWFragmentColors;
#endif

layout(early_fragment_tests) in;

// Convert 2D address to 1D address
int GetAddress(ivec2 vAddress)
{
    int nAddress = vAddress.y * int(g_WinSize.x) + vAddress.x;
    return nAddress;
}

layout(location = 0) in float depth;
layout(location = 1) in vec4 p0p1;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec4 strandColor;

#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
layout(location = 0) out vec4 FragColor;
#endif

void main()
{
    // Render AA Line, calculate pixel coverage
    vec4 proj_pos = vec4(2 * gl_FragCoord.x * g_WinSize.z - 1.0,  // g_WinSize.z = 1.0/g_WinSize.x
        1 - 2 * gl_FragCoord.y * g_WinSize.w,    // g_WinSize.w = 1.0/g_WinSize.y
        gl_FragCoord.z,
        1);

    vec4 world_pos = g_mInvViewProj * proj_pos;
    world_pos.xyz = world_pos.xyz / world_pos.w;
    world_pos.w = 1.0;

    float curve_scale = 1;
    if (g_bThinTip > 0)
        curve_scale = tangent.w;

    float fiber_radius = curve_scale * g_FiberRadius;

    float coverage = 1.f;
    if (g_bUseCoverage != 0)
    {
        coverage = ComputeCoverage(p0p1.xy, p0p1.zw, proj_pos.xy);
    }

    float alpha = coverage * g_FiberAlpha;

    if (alpha < g_alphaThreshold)
    {
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
        FragColor = vec4(0, 0, 0, 0);
#endif
        return;
    }

    ivec2 vScreenAddress = ivec2(gl_FragCoord.xy);
    int fragmentIndex = GetAddress(vScreenAddress);

    uint uDepth = floatBitsToUint(gl_FragCoord.z);

    uint uDepth0 = imageLoad(FragmentDepthsTexture, ivec3(vScreenAddress, 0)).x;
    uint uDepth1 = imageLoad(FragmentDepthsTexture, ivec3(vScreenAddress, 1)).x;
#if SHORTCUT_NUM_DEPTHS == 3
    uint uDepth2 = imageLoad(FragmentDepthsTexture, ivec3(vScreenAddress, 2)).x;
#endif

    // Shade regardless of depth, since ResolveDepth pass writes one of the near depths

    float amountLight = ComputeShadow(world_pos.xyz, g_HairShadowAlpha);
    vec3 color = ComputeHairShading(world_pos.xyz, tangent.xyz, vec4(0, 0, 0, 0), amountLight, strandColor.xyz);
    uint uColor = PackFloat4IntoUint(vec4(color, alpha));
    

#if SHORTCUT_DETERMINISTIC  // deterministic result when fragments match in depth, with added cost

#if SHORTCUT_WEIGHTED_AVERAGE
    FragColor = vec4(color * alpha, alpha);
    return;
#else
    // For fragments of equal depth, keep colors with greatest uint value
    if (uDepth == uDepth0)
    {
        uint uColorPrev = imageAtomicMax(RWFragmentColors, fragmentIndex, uColor);
        uColor = min(uColor, uColorPrev);
    }
    if (uDepth == uDepth1)
    {
        uint uColorPrev = imageAtomicMax(RWFragmentColors, fragmentIndex + 1, uColor);
        uColor = min(uColor, uColorPrev);
    }
#if SHORTCUT_NUM_DEPTHS == 3
    if (uDepth == uDepth2)
    {
        uint uColorPrev = imageAtomicMax(RWFragmentColors, fragmentIndex + 2, uColor);
        uColor = min(uColor, uColorPrev);
    }
#endif
#endif

#else // !SHORTCUT_DETERMINISTIC

    if (uDepth == uDepth0)
    {
        imageStore(RWFragmentColors, fragmentIndex, uvec4(uColor));
        uColor = 0;
    }
    if (uDepth == uDepth1)
    {
        imageStore(RWFragmentColors, fragmentIndex + 1, uvec4(uColor));
        uColor = 0;
    }
#if SHORTCUT_NUM_DEPTHS == 3
    if (uDepth == uDepth2)
    {
        imageStore(RWFragmentColors, fragmentIndex + 2, uvec4(uColor));
    }
#endif

#endif
}
