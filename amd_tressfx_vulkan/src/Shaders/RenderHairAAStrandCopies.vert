#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"

layout(set = 1, binding = IDSRV_HAIR_VERTEX_POSITIONS) uniform samplerBuffer g_HairVertexPositions;
layout(set = 1, binding = IDSRV_HAIR_TANGENTS) uniform samplerBuffer g_HairVertexTangents;
layout(set = 1, binding = IDSRV_HAIR_THICKNESSES) uniform samplerBuffer g_HairThicknessCoeffs;
layout(set = 0, binding = IDSRV_NOISEMAP) uniform texture2D g_txNoise;
layout(set = 0, binding = IDSRV_NOISE_SAMPLER) uniform sampler g_samLinearWrap;

out gl_PerVertex{
    vec4 gl_Position;
};

vec2 Safe_normalize(vec2 vec)
{
    float len = length(vec);
    return len >= FLOAT_EPSILON ? (vec / len) : vec2(0, 0);
}

vec3 Safe_normalize(vec3 vec)
{
    float len = length(vec);
    return len >= FLOAT_EPSILON ? (vec / len) : vec3(0, 0, 0);
}

// Need Depth, gl_FragCoord.z returns meaningless values...
layout(location = 0) out float depth;
layout(location = 1) out vec4 p0p1;
layout(location = 2) out vec4 tangent;
layout(location = 3) out vec4 strandColor;

//--------------------------------------------------------------------------------------
// VS_RenderHair_AA_StrandCopies
//
// Vertex shader for rendering the hair strands with randomized copies and AA
//--------------------------------------------------------------------------------------
void main()
{
    // Access the current line segment
    int index = gl_VertexIndex / 2;  // vertexId is actually the indexed vertex id when indexed triangles are used

                                     // Identifies which copy of the strand this is (0 is the original, 1 is the first copy, etc.)
    int randOffsetIndex = gl_InstanceIndex;

    // Getting a random offset value
    uint seedRand = ((index / 64 + 1) * (randOffsetIndex + 1)) % 512;
    vec2 seedTexcorrd = vec2(float(seedRand % 512), float(seedRand / 512)) * (1.f / 512.f);
    vec3 randOffset = float(randOffsetIndex) * textureLod(sampler2D(g_txNoise, g_samLinearWrap), seedTexcorrd, 0).xyz;
    randOffset = 2.0f * randOffset - 1.0f; // so we have random offsets between -1 and 1


                                           // Get updated positions and tangents from simulation result
    vec3 Tangent = texelFetch(g_HairVertexTangents, gl_VertexIndex / 2).xyz;
    vec3 v = randOffset.xyz + texelFetch(g_HairVertexPositions, gl_VertexIndex / 2).xyz;  // We apply a random offset to each vertex when multiple strands are requested

                                                                                          // Get hair strand thickness
    float ratio = (g_bThinTip > 0) ? texelFetch(g_HairThicknessCoeffs, gl_VertexIndex / 2).x : 1.;

    // Calculate right and projected right vectors
    vec3 right = Safe_normalize(cross(Tangent, normalize(v - g_vEye)));
    vec2 proj_right = Safe_normalize((g_mViewProj * vec4(right, 0)).xy);

    // g_bExpandPixels should be set to 0 at minimum from the CPU side; this would avoid the below test
    float expandPixels = (g_bExpandPixels < 0) ? 0.0 : 0.71;

    // Calculate the negative and positive offset screenspace positions
    vec4 hairEdgePositions0, hairEdgePositions1; // 0 is negative, 1 is positive
    hairEdgePositions0 = vec4(v - 1.0 * right * ratio * g_FiberRadius, 1.0);
    hairEdgePositions1 = vec4(v + 1.0 * right * ratio * g_FiberRadius, 1.0);
    hairEdgePositions0 = g_mViewProj * hairEdgePositions0;
    hairEdgePositions1 = g_mViewProj * hairEdgePositions1;

    // Write output data

    float fDirIndex = (gl_VertexIndex % 2 == 1) ? -1.0 : 1.0;
    gl_Position = ((fDirIndex == -1.0) ? hairEdgePositions0 : hairEdgePositions1) + fDirIndex * vec4(proj_right * expandPixels / g_WinSize.y, 0.0f, 0.0f) * ((fDirIndex == -1.0) ? hairEdgePositions0.w : hairEdgePositions1.w);
    depth = gl_Position.z / gl_Position.w;
    tangent = vec4(Tangent, ratio);
    p0p1 = vec4(hairEdgePositions0.xy / max(hairEdgePositions0.w, FLOAT_EPSILON), hairEdgePositions1.xy / max(hairEdgePositions1.w, FLOAT_EPSILON));
    // TODO: Strand color
}