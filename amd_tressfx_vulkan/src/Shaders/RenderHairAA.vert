#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"

layout(set = 1, binding = IDSRV_HAIR_VERTEX_POSITIONS) uniform samplerBuffer g_HairVertexPositions;
layout(set = 1, binding = IDSRV_HAIR_TANGENTS) uniform samplerBuffer g_HairVertexTangents;
layout(set = 1, binding = IDSRV_HAIR_THICKNESSES) uniform samplerBuffer g_HairThicknessCoeffs;

out gl_PerVertex {
  vec4 gl_Position;
};

vec2 Safe_normalize(vec2 vec)
{
    float len = length(vec);
    return len >= FLOAT_EPSILON ? (vec / len) : vec2(0,0);
}

vec3 Safe_normalize(vec3 vec)
{
    float len = length(vec);
    return len >= FLOAT_EPSILON ? (vec / len) : vec3(0,0,0);
}

// Need Depth, gl_FragCoord.z returns meaningless values...
layout(location = 0) out float depth;
layout(location = 1) out vec4 p0p1;
layout(location = 2) out vec4 tangent;
layout(location = 3) out vec4 strandColor;

//--------------------------------------------------------------------------------------
// VS_RenderHair_AA
//
// Vertex shader for rendering the hair strands with randomized copies and AA
//--------------------------------------------------------------------------------------
void main(void) {
  vec3 v = texelFetch(g_HairVertexPositions, gl_VertexIndex / 2).xyz;
  vec3 Tangent = texelFetch(g_HairVertexTangents, gl_VertexIndex / 2).xyz;
  float ratio = (g_bThinTip > 0) ? texelFetch(g_HairThicknessCoeffs, gl_VertexIndex / 2).x : 1.;

  // Calculate right and projected right vectors
  vec3 right = Safe_normalize( cross( Tangent, normalize(v - g_vEye)));
  vec2 proj_right = Safe_normalize( (g_mViewProj * vec4(right, 0)).xy );

  vec4 hairEdgePositions0, hairEdgePositions1; // 0 is negative, 1 is positive
  hairEdgePositions0 = vec4(v - 1.0 * right * ratio * g_FiberRadius, 1.0);
  hairEdgePositions1 = vec4(v + 1.0 * right * ratio * g_FiberRadius, 1.0);
  hairEdgePositions0 = g_mViewProj * hairEdgePositions0;
  hairEdgePositions1 = g_mViewProj * hairEdgePositions1;
  hairEdgePositions0 = hairEdgePositions0 / hairEdgePositions0.w;
  hairEdgePositions1 = hairEdgePositions1 / hairEdgePositions1.w;

  float expandPixels = (g_bExpandPixels < 0) ? 0.0 : 0.71;

  float fDirIndex = (gl_VertexIndex % 2 == 1) ? -1.0 : 1.0;

  gl_Position = (fDirIndex==-1.0 ? hairEdgePositions0 : hairEdgePositions1) + fDirIndex * vec4(proj_right * expandPixels / g_WinSize.y, 0.0, 0.0);
  depth = gl_Position.z;
  tangent = vec4(Tangent, ratio);
  p0p1 = vec4( hairEdgePositions0.xy, hairEdgePositions1.xy );
  // TODO: Strand color
}
