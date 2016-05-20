#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"

layout(set = 1, binding = IDSRV_HAIR_VERTEX_POSITIONS) uniform samplerBuffer g_HairVertexPositions;

out gl_PerVertex{
    vec4 gl_Position;
};

void main()
{
    gl_Position = g_mViewProjLight * vec4(texelFetch(g_HairVertexPositions, gl_VertexIndex).xyz, 1.);
}