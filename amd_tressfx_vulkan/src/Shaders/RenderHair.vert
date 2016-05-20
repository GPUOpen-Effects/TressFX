#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"

layout(set = 1, binding = IDSRV_HAIR_VERTEX_POSITIONS) uniform samplerBuffer g_HairVertexPositions;
layout(set = 1, binding = IDSRV_HAIR_TANGENTS) uniform samplerBuffer g_HairVertexTangents;
layout(set = 1, binding = IDSRV_HAIR_THICKNESSES) uniform samplerBuffer g_HairThicknessCoeffs;

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
// VS_RenderHair
//
// Vertex shader for rendering the hair strands without AA
//--------------------------------------------------------------------------------------
void main()
{
    vec3 v = texelFetch(g_HairVertexPositions, gl_VertexIndex / 2).xyz;
    vec3 Tangent = texelFetch(g_HairVertexTangents, gl_VertexIndex / 2).xyz;
    float ratio = (g_bThinTip > 0) ? texelFetch(g_HairThicknessCoeffs, gl_VertexIndex / 2).x : 1.;

    // Calculate right and projected right vectors
    vec3 right = Safe_normalize(cross(Tangent, normalize(v - g_vEye)));
    vec2 proj_right = Safe_normalize((g_mViewProj * vec4(right, 0)).xy);

    float expandPixels = (g_bExpandPixels < 0) ? 0.0 : 0.71;
    float fDirIndex = (gl_VertexIndex % 2 == 1) ? -1.0 : 1.0;

    vec3 temp = v + fDirIndex * right * ratio * g_FiberRadius;
    gl_Position = g_mViewProj * vec4(temp, 1.f);
    gl_Position = gl_Position + fDirIndex * vec4(proj_right * expandPixels / g_WinSize.y, 0.f, 0.f) * gl_Position.w;
    depth = gl_Position.z / gl_Position.w;
    tangent = vec4(Tangent, ratio);
    p0p1 = vec4(v.xy, texelFetch(g_HairVertexPositions, gl_VertexIndex / 2 + 1).xy);
    // TODO: Strand color
}