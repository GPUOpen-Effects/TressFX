#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"
#include "PackUnpack.glsl"
#include "ComputeCoverage.glsl"

layout(set = 0, binding = IDSRV_HAIR_FRAGMENT_DEPTHS, r32ui) uniform uimage2DArray RWFragmentDepthsTexture;

layout(early_fragment_tests) in;

layout(location = 0) in float depth;
layout(location = 1) in vec4 p0p1;
layout(location = 2) in vec4 tangent;

layout(location = 0) out float FragColor;

//--------------------------------------------------------------------------------------
// PS_Depth_Hair
//
//--------------------------------------------------------------------------------------
void main()
{
    // Render AA Line, calculate pixel coverage
    vec4 proj_pos = vec4(2 * gl_FragCoord.x * g_WinSize.z - 1.0,  // g_WinSize.z = 1.0/g_WinSize.x
        1 - 2 * gl_FragCoord.y * g_WinSize.w,    // g_WinSize.w = 1.0/g_WinSize.y
        gl_FragCoord.z,
        1);

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
        FragColor = 1.0;
        return;
    }

    uvec2 vScreenAddress = uvec2(gl_FragCoord.xy);

    uint uDepth = floatBitsToUint(gl_FragCoord.z);
    uint uDepth0Prev, uDepth1Prev;

    // Min of depth 0 and input depth
    // Original value is uDepth0Prev
    uDepth0Prev = imageAtomicMin(RWFragmentDepthsTexture, ivec3(vScreenAddress, 0), uDepth);

    // Min of depth 1 and greater of the last compare
    // If fragment opaque, always use input depth (don't need greater depths)
    uDepth = (alpha > 0.98) ? uDepth : max(uDepth, uDepth0Prev);

    uDepth1Prev = imageAtomicMin(RWFragmentDepthsTexture, ivec3(vScreenAddress, 1), uDepth);

#if SHORTCUT_NUM_DEPTHS == 3
    uint uDepth2Prev;

    // Min of depth 2 and greater of the last compare
    // If fragment opaque, always use input depth (don't need greater depths)
    uDepth = (alpha > 0.98) ? uDepth : max(uDepth, uDepth1Prev);

    uDepth2Prev = imageAtomicMin(RWFragmentDepthsTexture, ivec3(vScreenAddress, 2), uDepth);
#endif

    FragColor = 1.0 - alpha;
}
