#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "RenderConfig.glsl"
#include "PackUnpack.glsl"

#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
layout(input_attachment_index = 2, set = 0, binding = IDSRV_HAIR_FRAGMENT_COLORS) uniform subpassInput FragmentColors;
#else
layout(set = 0, binding = IDSRV_HAIR_FRAGMENT_DEPTHS, r32ui) uniform uimage2DArray FragmentDepthsTexture;
layout(set = 0, binding = IDSRV_HAIR_FRAGMENT_COLORS, r32ui) uniform uimageBuffer FragmentColors;
#endif

layout(set = 0, binding = IDSRV_HAIRSM) uniform texture2D HairShadowMap;
layout(set = 0, binding = IDSRV_SHADOW_SAMPLER) uniform sampler g_samPointClamp;

layout(input_attachment_index = 1, set = 0, binding = IDSRV_HAIR_ACCUM_INV_ALPHA) uniform subpassInput tAccumInvAlpha;

layout(early_fragment_tests) in;

layout(location = 0) out vec4 FragColor;

// Convert 2D address to 1D address
int GetAddress(ivec2 vAddress)
{
	int nAddress = vAddress.y * int(g_WinSize.x) + vAddress.x;
	return nAddress;
}


//--------------------------------------------------------------------------------------
// PS_ResolveColor_Hair
//
//--------------------------------------------------------------------------------------
void main()
{
	ivec2 vScreenAddress = ivec2(gl_FragCoord.xy);
	int fragmentIndex = GetAddress(vScreenAddress);

	float fInvAlpha = subpassLoad(tAccumInvAlpha).x;
	float fAlpha = 1.0 - fInvAlpha;

	if (fAlpha < SHORTCUT_MIN_ALPHA)
	{
		FragColor = vec4(0, 0, 0, 1);
		return;
	}

#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
	vec4 fcolor = subpassLoad(FragmentColors);
	float colorSumX = fcolor.x;
	float colorSumY = fcolor.y;
	float colorSumZ = fcolor.z;
	float colorSumW = fcolor.w;
	fcolor.x = colorSumX / colorSumW;
	fcolor.y = colorSumY / colorSumW;
	fcolor.z = colorSumZ / colorSumW;
	fcolor.xyz *= fAlpha;
#else
	uint uDepth0 = imageLoad(FragmentDepthsTexture, ivec3(vScreenAddress, 0)).x;
	uint uDepth1 = imageLoad(FragmentDepthsTexture, ivec3(vScreenAddress, 1)).x;
	uint uColor0 = imageLoad(FragmentColors, fragmentIndex).x;
	uint uColor1 = imageLoad(FragmentColors, fragmentIndex).y;

	vec4 color0 = UnpackUintIntoFloat4(uColor0);
	float alpha0 = color0.w;
	float invAlpha0 = 1.0 - alpha0;

	vec4 color1 = (uDepth1 == SHORTCUT_INITIAL_DEPTH) ? vec4(0, 0, 0, 0) : UnpackUintIntoFloat4(uColor1);
	float alpha1 = color1.w;
	float invAlpha1 = 1.0 - alpha1;

#if SHORTCUT_NUM_DEPTHS == 3
	uint uDepth2 = FragmentDepthsTexture[uint3(vScreenAddress, 2)];
	uint uColor2 = FragmentColors[fragmentIndex].z;

	vec4 color2 = (uDepth2 == SHORTCUT_INITIAL_DEPTH) ? vec4(0, 0, 0, 0) : UnpackUintIntoFloat4(uColor2);
	float alpha2 = color2.w;
#endif

	vec4 fcolor;

#if SHORTCUT_WEIGHTED_AVERAGE
#if SHORTCUT_NUM_DEPTHS == 3
	fcolor.xyz = fAlpha * (color0.xyz * alpha0 + color1.xyz * alpha1 + color2.xyz * alpha2) / (alpha0 + alpha1 + alpha2);
#else
	fcolor.xyz = fAlpha * (color0.xyz * alpha0 + color1.xyz * alpha1) / (alpha0 + alpha1);
#endif
#else
#if SHORTCUT_NUM_DEPTHS == 3
	float invAlpha01 = invAlpha0 * invAlpha1;

	// Get cumulative alpha for all fragments after front two; apply to back fragment
	float fInvAlphaBack = invAlpha01 < 0.0001 ? fInvAlpha : fInvAlpha / invAlpha01;
	float fAlphaBack = 1.0 - fInvAlphaBack;

	vec3 backColor = (uColor2 == 0) ? ((uColor1 == 0) ? color0.xyz : color1.xyz) : color2.xyz;

	// Blend is:
	// alpha0 * color0 + (1 - alpha0) * (alpha1 * color1 + (1 - alpha1) * (fAlphaBack * backColor + fInvAlphaBack * dstcolor))
	fcolor.xyz = color0.xyz * alpha0 + color1.xyz * (1 - alpha0) * alpha1 + backColor.xyz * (1 - alpha0) * (1 - alpha1) * fAlphaBack;
#else
	// Get cumulative alpha for all fragments after front; apply to back fragment
	float fInvAlphaBack = invAlpha0 < 0.0001 ? fInvAlpha : fInvAlpha / invAlpha0;
	float fAlphaBack = 1.0 - fInvAlphaBack;

	vec3 backColor = (uColor1 == 0) ? color0.xyz : color1.xyz;

	// Blend is:
	// alpha0 * color0 + (1 - alpha0) * (fAlphaBack * backColor + fInvAlphaBack * dstcolor))
	fcolor.xyz = color0.xyz * alpha0 + backColor * (1 - alpha0) * fAlphaBack;
#endif
#endif
#endif
	fcolor.w = fInvAlpha;
	FragColor = fcolor;
}
