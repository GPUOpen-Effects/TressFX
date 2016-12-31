#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) out vec2 vTex;

out gl_PerVertex {
  vec4 gl_Position;
};

vec4 getPosition(uint index)
{
	switch (index)
	{
    case 0: return vec4(-1, 1, 0, 1);
    case 1: return vec4(3, 1, 0, 1);
    case 2: return vec4(-1, -3, 0, 1);
	default: return vec4(0.);
	}
}

vec2 getTC(uint index)
{
	switch (index)
	{
    case 0: return vec2(0., 0.);
    case 1: return vec2(0., 2.);
    case 2: return vec2(2., 0.);
	default: return vec2(0.);
	}
}

void main()
{
    gl_Position = getPosition(gl_VertexIndex);
    vTex = getTC(gl_VertexIndex);
}
