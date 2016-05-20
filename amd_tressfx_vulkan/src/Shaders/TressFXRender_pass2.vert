#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable


layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Texcoord;

layout(location = 0) out vec2 vTex;

out gl_PerVertex {
  vec4 gl_Position;
};


void main()
{
    gl_Position = vec4(Position.xyz, 1.0);
    vTex = Texcoord.xy;
}
